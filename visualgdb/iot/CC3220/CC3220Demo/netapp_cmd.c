/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Standard includes */
#include <stdlib.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/net/wifi/simplelink.h>

/* Example Header files */
#include "netapp_cmd.h"
#include "network_terminal.h"
#include "cmd_parser.h"

/* Application defines */
#define QUERY_CONTINUOUS_MAX_TEXT_LENGTH        (120)
#define QUERY_CONTINUOUS_MAX_SERVICES           (3)
#define QUERY_CONTINUOUS_MAX_CHARACTERS         (1480)

/****************************************************************************
                   LOCAL FUNCTION PROTOTYPES
****************************************************************************/
void SimpleLinkPingReport(SlNetAppPingReport_t *pPingReport);

/*****************************************************************************
                  Callback Functions
*****************************************************************************/

/*!
    \brief          Ping command callback.

    This routine shows how to send ping to a remote host name / address.
    It sends ping command to the NWP and reports the ping status inside
    the designated report function (SimpleLinkPingReport).

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
                                  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           This function is blocked until ping has completed,
                    or an error occured.

    \sa             SimpleLinkPingReport, ParsePingCmd

 */
int32_t cmdPingCallback(void *arg)
{
    int32_t                   ret = -1;
    SlNetAppPingReport_t      pingReport = {0};
    PingCmd_t                 pingParams;
    int32_t                   ulIpAddr = 0;

    /* Call the command parser */
    memset(&pingParams, 0x0, sizeof(PingCmd_t));
    ret = ParsePingCmd(arg, &pingParams);

    if(ret < 0)
    {
        FreePingCmd(&pingParams);
        return(-1);
    }

    /* Check if the device has IP and is connected. */
    if((!IS_IP_ACQUIRED(app_CB.Status)) && (!IS_CONNECTED(app_CB.Status)))
    {
        UART_PRINT(
            "\n\r[Ping error]: Cannot ping host when IP "
            "address isn't leased.\n\r");
        FreePingCmd(&pingParams);
        return(-1);
    }

    /* Clear the Sync object - This semaphore along with the Ping report event,
     * signals The main context if sending ping packets has been completed.
     * For further info, please refer to: SimpleLinkPingReport()  */
    sem_wait_timeout(&app_CB.CON_CB.eventCompletedSyncObj, 0);
    SET_STATUS_BIT(app_CB.Status, STATUS_BIT_PING_STARTED);

    /* If the user inserted a name rather than IP address,
     * we get host address via DNS query, assuming Host address is IPV4.
     * In case the user provided an address rather than host name,
     * this API returns the IP address.
     */
    ret =
        sl_NetAppDnsGetHostByName((signed char*)pingParams.host,
                                  strlen((const char*)pingParams.host),
                                  (unsigned long*)&ulIpAddr, SL_AF_INET);

    ASSERT_AND_CLEAN_PING(ret, NETAPP_ERROR, &pingParams);

    /* Replace the ping address to match host name's IP address */
    pingParams.pingCmd.Ip = ulIpAddr;

    UART_PRINT("\n\r[Ping cmd] : Pinging  %s (", pingParams.host);
    PrintIPAddress(0,(void *)&ulIpAddr);
    UART_PRINT(") with %d bytes of data, for %d attempts.\n\r",
               pingParams.pingCmd.PingSize,
               pingParams.pingCmd.TotalNumberOfAttempts);

    /* Switch to network byte ordering*/
    ulIpAddr = sl_Htonl(ulIpAddr);

    /* Update Ping attempts global */
    app_CB.PingAttempts = pingParams.pingCmd.TotalNumberOfAttempts;

    /* Try to ping HOST_NAME */
    ret = sl_NetAppPing((SlNetAppPingCommand_t*)&pingParams.pingCmd,SL_AF_INET,
                        (SlNetAppPingReport_t*)&pingReport,
                        SimpleLinkPingReport);
    ASSERT_AND_CLEAN_PING(ret, NETAPP_ERROR, &pingParams);

    /* Wait for ping to complete - SimpleLinkPingReport would
     * post this semaphore when Ping would complete or in case of error.*/
    sem_wait(&app_CB.CON_CB.eventCompletedSyncObj);

    /* Calling ping API with all zeros, stops any ongoing ping command */
    ret = sl_NetAppPing(0, 0, 0, 0);
    ASSERT_AND_CLEAN_PING(ret, NETAPP_ERROR, &pingParams);

    return(0);
}

/*!
    \brief          Prints Ping command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdPingCallback
 */
int32_t printPingUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(pingStr);
    UART_PRINT(pingUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(pingDetailsStr);
    UART_PRINT(ping_h_optionDetailsStr);
    UART_PRINT(ping_c_optionDetailsStr);
    UART_PRINT(ping_s_optionDetailsStr);
    UART_PRINT(ping_i_optionDetailsStr);
    UART_PRINT(ping_t_optionDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          mDNS advertise callback.

    This routine shows how to Advertise a service over mDNS.

    \param          arg       -   Points to command line buffer.
                                  This container would 
                                  be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             mDNScreateServiceName, ParsemDNSAdvertiseCmd

 */
int32_t mDNSAdvertiseCallback(void *arg)
{
    int32_t             ret = 0;
    mDnsAdvertiseCmd_t  mDNSAdvertiseParams;

    /* Call the command parser */
    memset(&mDNSAdvertiseParams, 0x0, sizeof(mDnsAdvertiseCmd_t));
    ret = ParsemDNSAdvertiseCmd(arg, &mDNSAdvertiseParams);

    if(ret < 0)
    {
        FreemDNSAdvertiseCmd(&mDNSAdvertiseParams);
        return(-1);
    }

    if(!IS_CONNECTED(app_CB.Status))
    {
        UART_PRINT(
            "\n\r[mDNS advertise] : mDNS failed to start"
            " : Not connected to AP.\n\r");
        FreemDNSAdvertiseCmd(&mDNSAdvertiseParams);
        return(-1);
    }

    /* Set device name.
     * Device name is part of the advertised address:
     * Device_name.Service_type.Service_over.local
     */
    UART_PRINT("\n\r[mDNS advertise] : Setting device name: %s\n\r",
               mDNSAdvertiseParams.dev_name);
    ret =
        sl_NetAppSet(SL_NETAPP_DEVICE_ID, SL_NETAPP_DEVICE_URN,
                     strlen(
                         (const char *)mDNSAdvertiseParams.service_name),
                     (uint8_t *)mDNSAdvertiseParams.dev_name);
    ASSERT_AND_CLEAN_MDNS_ADV(ret, NETAPP_ERROR, &mDNSAdvertiseParams);

    /* Start mDNS -
     * This API enables the NWP internal mDNS. */
    ret = sl_NetAppStart(SL_NETAPP_MDNS_ID);

    if((ret < 0) && (ret != SL_ERROR_NET_APP_MDNS_ALREADY_STARTED))
    {
        UART_PRINT(
            "\n\r[mDNS advertise] : mDNS failed to start"
            " with ERROR code: %d\n\r",
            ret);
        FreemDNSAdvertiseCmd(&mDNSAdvertiseParams);
        return(ret);
    }

    UART_PRINT("\n\r[mDNS advertise] : mDNS netapp started successfully.\n\r");

    /*
     *  Register a mDNS service. - service would be advertise as:
     *  service_name.service_name.service_over.local
     *  for example: 'PC_1.ipp.tcp.local' for a printer server over tcp,
     *  on a server named 'PC_1'.
     * (For more information, read mDNS RFC).
     *  Users can register up to 5 mDNS advertise services.
     *  If calling sl_NetAppMDNSUnRegisterService()
     *    when there's no such service,
     *  the function will return SL_ERROR_NET_APP_MDNS_EXISTED_SERVICE_ERROR
     */

    ret = sl_NetAppMDNSRegisterService(
        (const signed char*)&mDNSAdvertiseParams.service_name,
        strlen((const char *)mDNSAdvertiseParams
               .service_name),
        (const signed char*)(mDNSAdvertiseParams
                             .adv_text),
        (uint8_t)strlen((const char *)
                        mDNSAdvertiseParams.adv_text),
        mDNSAdvertiseParams.service_port,
        mDNSAdvertiseParams.service_ttl,
        (SL_NETAPP_MDNS_OPTIONS_IS_UNIQUE_BIT |
         SL_NETAPP_MDNS_OPTIONS_IS_NOT_PERSISTENT));
    ASSERT_AND_CLEAN_MDNS_ADV(ret, NETAPP_ERROR, &mDNSAdvertiseParams);

    UART_PRINT(
        "\n\r[mDNS advertise] : service name %s registered successfully.\n\r",
        (const char*)(&mDNSAdvertiseParams.service_name));

    FreemDNSAdvertiseCmd(&mDNSAdvertiseParams);
    return(0);
}

/*!
    \brief          Prints mDNS advertise command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             mDNSAdvertiseCallback
 */
int32_t printmDNSAdvertiseUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(mDNSAdvertiseStr);
    UART_PRINT(mDNSAdvertiseUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(mDNSAdvertiseDetailsStr);
    UART_PRINT(mDNSAdvertise_n_optionDetailsStr);
    UART_PRINT(mDNSAdvertise_st_optionDetailsStr);
    UART_PRINT(mDNSAdvertise_so_optionDetailsStr);
    UART_PRINT(mDNSAdvertise_ttl_optionDetailsStr);
    UART_PRINT(mDNSAdvertise_t_optionDetailsStr);
    UART_PRINT(mDNSAdvertise_p_optionDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);

    return(0);
}

/*!
    \brief          mDNS Query callback.

    This routine shows how to Query for services over mDNS.
    Query can be partial, or full and can be 'one-shot' namely,
    return the first result or continues, which return several
    results from the NWP cache.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
                                  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             mDNScreateServiceName, ParsemDNSQueryCmd

 */
int32_t mDNSQueryCallback(void *arg)
{
    mDnsQueryCmd_t mDNSQueryParams;
    int32_t ret;

    /* Call the command parser */
    memset(&mDNSQueryParams, 0x0, sizeof(mDnsQueryCmd_t));
    ret = ParsemDNSQueryCmd(arg, &mDNSQueryParams);

    if(ret < 0)
    {
        return(-1);
    }

    int32_t i;
    uint32_t ipAddr = 0;
    uint32_t portNumber = 0;
    uint16_t textLen = QUERY_CONTINUOUS_MAX_TEXT_LENGTH;
    char rcvText[QUERY_CONTINUOUS_MAX_TEXT_LENGTH];
    SlNetAppGetFullServiceWithTextIpv4List_t serviceList[
        QUERY_CONTINUOUS_MAX_SERVICES] = {
        {0}
    };

    /* Start mDNS -
     * This API enables the NWP internal mDNS. */
    ret =  sl_NetAppStart(SL_NETAPP_MDNS_ID);
    if((ret < 0) && (ret != SL_ERROR_NET_APP_MDNS_ALREADY_STARTED))
    {
        UART_PRINT(
            "\n\r[mDNS Query] : mDNS failed to start with ERROR code: %d\n\r",
            ret);
        return(ret);
    }

    UART_PRINT("\n\r[mDNS Query] : mDNS netapp started successfully\n\r");

    /* Configure mDNS query to continues :
     * This means that we retrieve mDNS results from the NWP's cache,
     * Rather than the first answer we pick up (one-shot).
     */
    if(!mDNSQueryParams.OneShotFlag)
    {
        ret = sl_NetAppSet(SL_NETAPP_MDNS_ID,
                           SL_NETAPP_MDNS_CONT_QUERY_OPT,
                           0,
                           0);

        if(ret < 0)
        {
            UART_PRINT(
                "\n\r[mDNS Query] : Failed configure mDNS to perform"
                " continuous query, ERROR code: %d\n\r",
                ret);
            return(ret);
        }
        else
        {
            ret = 1;
        }
    }

    /* Start performing query - wait until first response. */
    while(ret)
    {
        /* Issue an mDNS query - 
        get the host address and name by service, assuming IPv4 */
        ret = sl_NetAppDnsGetHostByService(
            (signed char*)mDNSQueryParams.ServiceName,
            (uint8_t)strlen((const char *)
                            mDNSQueryParams.ServiceName),
            SL_AF_INET,
            (unsigned long*)&ipAddr,
            (unsigned long*)&portNumber,
            &textLen,
            (signed char*)&rcvText);

        if(ret == SL_ERROR_NET_APP_MDNS_NO_ANSWER)
        {
            UART_PRINT("\\n\r[mDNS Query] : mDNS No Answer\n\r");
            return(0);
        }
        else if(ret < 0)
        {
            UART_PRINT(
                "\n\r[mDNS Query] : mDNS failed to query, ERROR code: %d\n\r",
                ret);
            return(ret);
        }
    }

    /* Print the First response */
    rcvText[textLen] = '\0';

    UART_PRINT(
        "\n\r[mDNS Query] : Host: %s\n\rIP:%d.%d.%d.%d\n\rPort:"
        " %d\n\rReceived text:%s",
        mDNSQueryParams.ServiceName,
        SL_IPV4_BYTE(ipAddr, 3),
        SL_IPV4_BYTE(ipAddr, 2),
        SL_IPV4_BYTE(ipAddr, 1),
        SL_IPV4_BYTE(ipAddr, 0),
        (uint16_t)portNumber,
        rcvText);

    if(!mDNSQueryParams.OneShotFlag)
    {
        /* Since this query was continues,
         * we have more results in the NWP cache,
         * this loop prints them.
         */
        ret = sl_NetAppGetServiceList(
            0, QUERY_CONTINUOUS_MAX_SERVICES,
            SL_NETAPP_FULL_SERVICE_WITH_TEXT_IPV4_TYPE,
            (signed char *) &serviceList[0],
            QUERY_CONTINUOUS_MAX_CHARACTERS);

        if(ret < 0)
        {
            UART_PRINT(
                "\n\r[mDNS Query] : mDNS failed to get service list"
                " from query, ERROR code: %d\n\r",
                ret);
            return(ret);
        }

        /* delay one second in order to give 
        the NWP enough time to fill it's cache */
        sleep(1);

        for(i = 0; i < ret; i++)
        {
            UART_PRINT(
                "\n\r[mDNS Query] : Host: %s\n\rIP:   %d.%d.%d.%d\n\rName:"
                " %s\n\rPort: %d\n\r",
                serviceList[i].service_host,
                SL_IPV4_BYTE(serviceList[i].service_ipv4, 3),
                SL_IPV4_BYTE(serviceList[i].service_ipv4, 2),
                SL_IPV4_BYTE(serviceList[i].service_ipv4, 1),
                SL_IPV4_BYTE(serviceList[i].service_ipv4, 0),
                serviceList[i].service_name,
                serviceList[i].service_port);
        }
    }

    return(0);
}

/*!
    \brief          Prints mDNS Query command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             mDNSQueryCallback
 */
int32_t printmDNSQueryUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(mDNSQueryStr);
    UART_PRINT(mDNSdQueryUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(mDNSdQueryDetailsStr);
    UART_PRINT(mDNSdQuery_n_optionDetailsStr);
    UART_PRINT(mDNSdQuery_st_optionDetailsStr);
    UART_PRINT(mDNSdQuery_so_optionDetailsStr);
    UART_PRINT(mDNSdQuery_o_optionDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);

    return(0);
}


/*****************************************************************************
                  Local Functions
*****************************************************************************/

/*!
    \brief          SimpleLinkPingReport callback.

    This routine prints ping status for each received 'echo reply'.
    (This feature can be determined by Flags field in the PingCmd_t structure).

    \param          pPingReport       -   Points to a 
                                          SlNetAppPingReport_t structure,
                                          which contains the ping 
                                          statistics raised by the NWP.

    \return         None.

    \note           In case of an error or completion,
                    this function releases the main context.

    \sa             cmdPingCallback, ParsePingCmd

 */
void SimpleLinkPingReport(SlNetAppPingReport_t *pPingReport)
{
    if(IS_PING_RUNNING(app_CB.Status) && (pPingReport->PacketsSent > 0))
    {
        UART_PRINT(lineBreak);
        UART_PRINT("[Ping report]: Packet Sent: %d, Packet Received: %d \n\r",
                   pPingReport->PacketsSent,
                   pPingReport->PacketsReceived);
        UART_PRINT(
            "Max Round Time: %dms , Min Round Time: %dms,"
            " Average Round Time: %dms\n\r",
            pPingReport->MaxRoundTime, pPingReport->MinRoundTime,
            pPingReport->AvgRoundTime);

        /* Release the ping semaphore */
        if(app_CB.PingAttempts == pPingReport->PacketsSent)
        {
            CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_PING_STARTED);
            app_CB.PingAttempts = 0;
            UART_PRINT(lineBreak);
            /* This post releases the Ping Callback */
            sem_post(&app_CB.CON_CB.eventCompletedSyncObj);
        }
    }
}
