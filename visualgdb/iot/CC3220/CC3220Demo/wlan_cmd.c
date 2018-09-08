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
#ifdef CC32XX
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC32XX.h>
#endif
/* Example Header files */
#include "cmd_parser.h"
#include "network_terminal.h"
#include "wlan_cmd.h"
#include "transceiver_cmd.h"

/* Application defines */
#define WLAN_EVENT_TOUT             (6000)
#define MAX_SCAN_TRAILS             (10)
#define P2P_CONNECT_PRIORITY        (SPAWN_TASK_PRIORITY - 1)
#define P2P_STACK_SIZE              (2048)
#define P2P_REMOTE_DEVICE           ("StartScan")
#define P2P_DEVICE_TYPE             ("1-0050F204-1")
#define P2P_DEVICE_NAME             ("cc32xx_p2p_device")
#define LISTEN_CHANNEL              (11)
#define LISTEN_REGULATORY_CLASS     (81)
#define OPRA_CHANNEL                (6)
#define OPRA_REGULATORY_CLASS       (81)
#define TIMEOUT_SEM                 (-1)
#define LPDS_WAKEUP_SW              (1)
#define MGMT                        (0)
#define CTRL                        (1)
#define DATA                        (2)

/******************************************************************************
                      LOCAL FUNCTION PROTOTYPES
******************************************************************************/
int32_t setStaticIPConfig(uint8_t* pIP,
                          uint8_t* pGw,
                          uint8_t* pDns);
int32_t sendConnectionReq(void);
int32_t setP2Pparams(void);
void printScanResults(uint32_t res_num);
void printExtScanResults(uint32_t res_num);
void *p2pConnect(void *arg);

/******************************************************************************
                      GLOBAL VARIABLES
******************************************************************************/
uint8_t DataFrames[] = { 0x08, 0x18, 0x28, 0x38, 0x48, 0x58, 0x68, 0x78,
                         0x88, 0x98, 0xA8, 0xB8, 0xC8, 0xD8, 0xE8, 0xF8 };

uint8_t CtrlFrames[] = { 0x84, 0x94, 0xA4, 0xB4, 0xC4, 0xD4, 0xE4, 0xF4 };

uint8_t MgmtFrames[] = { 0x00, 0x01, 0x20, 0x30, 0x40, 0x50,
                         0x80, 0x90, 0xA0, 0xB0, 0xC0, 0xD0 };

/******************************************************************************
                  Callback Functions
******************************************************************************/

/*!
    \brief          WLAN connect callback.

    This routine shows how to Connect to an AP.
    As a part of the connection process,
    we also demonstrate how to set a static IP,
    Static default gateway address and static DNS.
    Enterprise credentials are also shown for users who wish to connect
    to an enterprise network.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
                                  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           For more information about Security types, Enterprise networks,
                    And driver asynchronous events related to connection process,
                    please refer to CC31xx/CC32xx NWP programmer's guide (SWRU455)

    \sa             setStaticIPConfig

 */
int32_t cmdWlanConnectCallback(void *arg)
{
    int32_t ret = 0;
    ConnectCmd_t ConnectParams;

    /* Call the command parser */
    memset(&ConnectParams, 0x0, sizeof(ConnectCmd_t));
    ret = ParseConnectCmd(arg, &ConnectParams);

    if(ret < 0)
    {
        FreeConnectCmd(&ConnectParams);
        return(-1);
    }

    /*
     *  Check to see if the NWP is in STA role,
     *  since it has to be in STA role in order to connect to an AP.
     *  If it isn't - set role and reset the NWP.
     */
    if(app_CB.Role != ROLE_STA)
    {
        ret = sl_WlanSetMode(ROLE_STA);
        ASSERT_AND_CLEAN_CONNECT(ret, WLAN_ERROR, &ConnectParams);

        ret = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_AND_CLEAN_CONNECT(ret, DEVICE_ERROR, &ConnectParams);

        ret = sl_Start(0, 0, 0);
        ASSERT_AND_CLEAN_CONNECT(ret, DEVICE_ERROR, &ConnectParams);
    }
    /* Here wer'e in STA mode */
    app_CB.Role = ROLE_STA;

    /* Set static IP configurations, if needed - otherwise,
       IP would be allocated by your network's DHCP server. */
    if(ConnectParams.ip)
    {
        ret =
            setStaticIPConfig((uint8_t*)(ConnectParams.ip),
                              (uint8_t*)(ConnectParams.gw),
                              (uint8_t*)(ConnectParams.dns));
        ASSERT_AND_CLEAN_CONNECT(ret, CMD_ERROR, &ConnectParams);
    }

    /* Set device time and date before connection to ENT network, if needed */
    if(ConnectParams.secParamsEnt.User != NULL)
    {
        ret =
            sl_DeviceSet(SL_DEVICE_GENERAL, SL_DEVICE_GENERAL_DATE_TIME,
                         sizeof(SlDateTime_t),
                         (uint8_t *)(&ConnectParams.dateTime));
        ASSERT_AND_CLEAN_CONNECT(ret, DEVICE_ERROR, &ConnectParams);

        /* Connect to ENT AP */
        ret =
            sl_WlanConnect((const signed char *)(ConnectParams.ssid),
                           strlen(
                               (const char *)(ConnectParams.ssid)), 0,
                           &(ConnectParams.secParams),
                           &(ConnectParams.secParamsEnt));
        ASSERT_AND_CLEAN_CONNECT(ret, WLAN_ERROR, &ConnectParams);
    }
    else
    {
        /* Connect to AP */
        ret =
            sl_WlanConnect((const signed char *)(ConnectParams.ssid),
                           strlen(
                               (const char *)(ConnectParams.ssid)), 0,
                           &ConnectParams.secParams, 0);
        ASSERT_AND_CLEAN_CONNECT(ret, WLAN_ERROR, &ConnectParams);
    }

    /* Wait for connection events:
     * In order to verify that connection was successful,
     * we pend on two incoming events: Connected and Ip acquired.
     * The semaphores below are pend by this (Main) context.
     * They will be signaled once an asynchronous event
     * Indicating that the NWP has connected and acquired IP address is raised.
     * For further information, see this application read me file.
     */
    if(!IS_CONNECTED(app_CB.Status))
    {
        ret = sem_wait_timeout(&app_CB.CON_CB.connectEventSyncObj,
                               WLAN_EVENT_TOUT);
        if(ret == TIMEOUT_SEM)
        {
            UART_PRINT("\n\r[wlanconnect] : Failed to connect to AP: %s\n\r",
                       ConnectParams.ssid);
            FreeConnectCmd(&ConnectParams);
            return(-1);
        }
    }

    if(!IS_IP_ACQUIRED(app_CB.Status))
    {
        ret = sem_wait_timeout(&app_CB.CON_CB.ip4acquireEventSyncObj,
                               WLAN_EVENT_TOUT);
        if(ret == TIMEOUT_SEM)
        {
            /* In next step try to get IPv6,
              may be router/AP doesn't support IPv4 */
            UART_PRINT(
                "\n\r[wlanconnect] : Failed to acquire IPv4 address.\n\r");
        }
    }

    if(!IS_IPV6G_ACQUIRED(app_CB.Status))
    {
        ret = sem_wait_timeout(&app_CB.CON_CB.ip6acquireEventSyncObj,
                               WLAN_EVENT_TOUT);
        if(ret == TIMEOUT_SEM)
        {
            UART_PRINT(
                "\n\r[wlanconnect] : Failed to acquire IPv6 address.\n\r");
        }
    }

    if(!IS_IPV6G_ACQUIRED(app_CB.Status) &&
       !IS_IPV6L_ACQUIRED(app_CB.Status) && !IS_IP_ACQUIRED(app_CB.Status))
    {
        UART_PRINT("\n\r[line:%d, error:%d] %s\n\r", __LINE__, -1,
                   "Network Error");
    }

    FreeConnectCmd(&ConnectParams);
    return(0);
}

/*!
    \brief          Prints WLAN connect command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdWlanConnectCallback
 */
int32_t printWlanConnectUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(wlanConnectStr);
    UART_PRINT(wlanConnectUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(wlanConnectDetailsStr);
    UART_PRINT(wlanConnect_s_optionDetailsStr);
    UART_PRINT(wlanConnect_t_optionDetailsStr);
    UART_PRINT(wlanConnect_p_optionDetailsStr);
    UART_PRINT(wlanConnect_ip_optionDetailsStr);
    UART_PRINT(wlanConnect_gw_optionDetailsStr);
    UART_PRINT(wlanConnect_dns_optionDetailsStr);
    UART_PRINT(wlanConnect_ent_optionDetailStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Scan callback.

    This routine shows how to retrieve scan results form the NWP.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed 
                                  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           If scans aren't active, this function triggers one scan
                    and later prints the results.

    \sa             ParseScanCmd

*/
int32_t cmdScanCallback(void *arg)
{

    int32_t         ret = -1;
    uint8_t         triggeredScanTrials = 0;
    ScanCmd_t       ScanParams;

    /* Call the command parser */
    memset(&ScanParams, 0x0, sizeof(ScanParams));
    ret = ParseScanCmd(arg , &ScanParams);

    if(ret < 0)
    {
        return(-1);
    }

    /* Clear the results buffer */
    memset(&app_CB.gDataBuffer, 0x0, sizeof(app_CB.gDataBuffer));


    /* Get scan results from NWP - 
    results would be placed inside the provided buffer */
    ret = sl_WlanGetNetworkList(ScanParams.index, ScanParams.numOfentries,
		 &app_CB.gDataBuffer.netEntries[ScanParams.index]);
    

    /* If scan policy isn't set, invoking 'sl_WlanGetNetworkList()'
     * for the first time triggers 'one shot' scan.
     * The scan parameters would be according to the system persistent 
     * settings on enabled channels.
     * For more information, see: <simplelink user guide, page: pr.>
     */
    if(SL_ERROR_WLAN_GET_NETWORK_LIST_EAGAIN == ret)
    {
        while(triggeredScanTrials < MAX_SCAN_TRAILS)
        {
            /* We wait for one second for the NWP to complete
             the initiated scan and collect results */
            sleep(1);

            /* Collect results form one-shot scans.*/           
	    /* Get scan results from NWP - 
				results would be placed inside the provided buffer */
                ret = sl_WlanGetNetworkList(ScanParams.index, 
				ScanParams.numOfentries,
				 &app_CB.gDataBuffer.netEntries[ScanParams.index]);
            
            if(ret > 0)
            {
                break;
            }
            else
            {
                /* If NWP results aren't ready,
				 try 'MAX_SCAN_TRAILS' attempts to get results */
                triggeredScanTrials++ ;
            }
        }
    }

    if(ret <= 0)
    {
        UART_PRINT("\n\r[scan] : Unable to retrieve the network list\n\r");
        return(-1);
    }
    /* Print the result table */
    printScanResults(ret);

    return(ret);
}

/*!
    \brief          Prints scan command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdScanCallback
*/
int32_t printScanUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(scanStr);
    UART_PRINT(scanUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(scanDetailsStr);
    UART_PRINT(scan_n_optionDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    UART_PRINT(scan_Note_optionDetailsStr);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Set scan policy callback.

    This routine shows how to set the scan parameters.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
                                  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           Calling this API automatically enables
                    scans in the background,
                    such that calling 'Scan' command simply returns
                    the results form the NWP.

    \sa             cmdScanCallback, ParseSetPolicyCmd

 */
int32_t cmdSetPolicyCallback(void *arg)
{
    uint8_t policyOpt;
    int32_t ret;
    SetPolicyCmd_t SetPolicyParams;

    /* Call the command parser */
    memset(&SetPolicyParams, 0x0, sizeof(SetPolicyParams));
    ret = ParseSetPolicyCmd(arg , &SetPolicyParams);

    if(ret < 0)
    {
        return(-1);
    }

    if(SetPolicyParams.turnOff)
    {
        /* Disable scans */
        policyOpt = SL_WLAN_SCAN_POLICY(0, 0);
        ret =
            sl_WlanPolicySet(SL_WLAN_POLICY_SCAN, policyOpt,
                             (uint8_t*)(&(SetPolicyParams.ScanIntervalinSec)),
                             sizeof(SetPolicyParams.ScanIntervalinSec));

        if(ret != 0)
        {
            UART_PRINT("\n\r[setpolicy] : "
            "Unable to Clear the Scan Policy.\n\r");
            return(ret);
        }

        return(0);
    }

    /* In case NWP is in AP Mode, setting the scan interval to a value less
     * Than 10 seconds, can seriously harm the AP's functioning, since it
     * has scheduling limitations.
     */
    if((app_CB.Role == ROLE_AP) && (SetPolicyParams.ScanIntervalinSec < 10))
    {
        UART_PRINT(
            "\n\r[setpolicy] : In AP role the minimum scan interval"
            " is 10 seconds. Scan Interval is set to 10 seconds.\n\r");
        SetPolicyParams.ScanIntervalinSec = 10;
    }

    /* Make sure no connection policy is not set 
      (so no scan is run in the background) */
    policyOpt = SL_WLAN_CONNECTION_POLICY(0, 0, 0, 0);

    ret = sl_WlanPolicySet(SL_WLAN_POLICY_CONNECTION ,policyOpt , NULL, 0);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    /* Set scan parameters */
    ret =
        sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
                   SL_WLAN_GENERAL_PARAM_OPT_SCAN_PARAMS,
                   sizeof(SetPolicyParams.ScanParamConfig),
                   (uint8_t *)(&SetPolicyParams.ScanParamConfig));
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    /* Enable scan */
    policyOpt = SL_WLAN_SCAN_POLICY(1, SetPolicyParams.hiddenSsid);

    /* Set scan policy - this API starts the scans */
    ret =
        sl_WlanPolicySet(SL_WLAN_POLICY_SCAN, policyOpt,
                         (uint8_t*)(&SetPolicyParams.ScanIntervalinSec),
                         sizeof(SetPolicyParams.ScanIntervalinSec));
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    return(0);
}

/*!
    \brief          Prints set policy command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdSetPolicyCallback
 */
int32_t printSetPolicyUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(setPolicyStr);
    UART_PRINT(setPolicyUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(setPolicyDetailsStr);
    UART_PRINT(setPolicy_i_optionDetailsStr);
    UART_PRINT(setPolicy_h_optionDetailsStr);
    UART_PRINT(setPolicy_c_optionDetailsStr);
    UART_PRINT(setPolicy_r_optionDetailsStr);
    UART_PRINT(setPolicy_o_optionDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Start AP callback.

    This routine shows how to start the device in AP role.
    After starting the device in AP role, in order to go back to station role,
    a device reset is needed.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed 
                                  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           For more information about Security types,
                    Enterprise network,
                    And Other AP parameters, please refer to:
                    CC31xx/CC32xx NWP programmer's guide (SWRU455)

    \sa             ParseStartApCmd

*/
int32_t cmdWlanStartApCallback(void *arg)
{

    int32_t      ret = 0;
    int32_t      mode = 0;
    StartApCmd_t StartApParams;

    /* Call the command parser */
    memset(&StartApParams, 0x0, sizeof(StartApParams));
    ret = ParseStartApCmd(arg , &StartApParams);

    if(ret < 0)
    {
        FreeStartApCmd(&StartApParams);
        return(-1);
    }

    /* set SSID name */
    ret =
        sl_WlanSet(SL_WLAN_CFG_AP_ID, SL_WLAN_AP_OPT_SSID,
                   strlen((const char *)(StartApParams.ssid)),
                   (unsigned char*)(StartApParams.ssid));
    ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);

    /* Set security type */
    ret =
        sl_WlanSet(SL_WLAN_CFG_AP_ID, SL_WLAN_AP_OPT_SECURITY_TYPE, 1,
                   (unsigned char *)(&StartApParams.secParams.Type));
    ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);

    /* Set password (if needed) */
    if(StartApParams.secParams.Type != SL_WLAN_SEC_TYPE_OPEN)
    {
        ret =
            sl_WlanSet(SL_WLAN_CFG_AP_ID, SL_WLAN_AP_OPT_PASSWORD,
                       StartApParams.secParams.KeyLen,
                       (unsigned char *)(StartApParams.secParams.Key));
    }

    ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);

    /* Set AP's SSID as hidden (if needed) */
    if(StartApParams.hidden)
    {
        ret =
            sl_WlanSet(SL_WLAN_CFG_AP_ID, SL_WLAN_AP_OPT_HIDDEN_SSID,
                       sizeof(StartApParams.hidden),
                       (unsigned char *)(&StartApParams.hidden));
    }

    ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);

    /* Set channel number */
    if(StartApParams.channel)
    {
        ret =
            sl_WlanSet(SL_WLAN_CFG_AP_ID, SL_WLAN_AP_OPT_CHANNEL,
                       sizeof(StartApParams.channel),
                       (unsigned char *)(&StartApParams.channel));
    }

    ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);

    /* Set STA connection limit */
    ret =
        sl_WlanSet(SL_WLAN_CFG_AP_ID, SL_WLAN_AP_OPT_MAX_STATIONS,
                   sizeof(StartApParams.sta_limit),
                   (unsigned char *)(&StartApParams.sta_limit));
    ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);

    /* Set TX power */
    ret =
        sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
                   SL_WLAN_GENERAL_PARAM_OPT_AP_TX_POWER,
                   1,
                   (unsigned char *)(&StartApParams.tx_pow));
    ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);

    /* If wer'e not in AP role, disconnect from any existing AP's or Peer */
    if(app_CB.Role != ROLE_AP)
    {
        if(IS_CONNECTED(app_CB.Status)) 
        {
            ret = sl_WlanDisconnect();
            ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);
            CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_IP_ACQUIRED);
        }

        ret = sl_WlanSetMode(ROLE_AP);        /* Set device role as AP */
        ASSERT_AND_CLEAN_STARTAP(ret, WLAN_ERROR, &StartApParams);
    }

    /* Restart the NWP so the new configuration will take affect */
    ret = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_AND_CLEAN_STARTAP(ret, DEVICE_ERROR, &StartApParams);

    mode = sl_Start(0, 0, 0);
    ASSERT_AND_CLEAN_STARTAP(ret, DEVICE_ERROR, &StartApParams);

    if(mode != ROLE_AP)
    {
        UART_PRINT("\n\r[wlan ap start] : Unable to configure AP role.\n\r");
        FreeStartApCmd(&StartApParams);
        return(-1);
    }
    else
    {
        app_CB.Role = ROLE_AP;
    }

    FreeStartApCmd(&StartApParams);
    return(ret);
}

/*!
    \brief          Prints Start Ap command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdWlanStartApCallback
 */
int32_t printWlanStartApUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(ap_start_str);
    UART_PRINT(ap_start_UsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(ap_start_DetailsStr);
    UART_PRINT(wlanConnect_s_optionDetailsStr);
    UART_PRINT(wlanConnect_t_optionDetailsStr);
    UART_PRINT(wlanConnect_p_optionDetailsStr);
    UART_PRINT(ap_start_h_optionDetailsStr);
    UART_PRINT(ap_start_txp_optionDetailsStr);
    UART_PRINT(ap_start_channel_optionDetailsStr);
    UART_PRINT(ap_start_l_optionDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    return(0);
}


/*!
    \brief          Stop AP callback.

    This routine shows how to start the device in Station role.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             ParseCmd

*/
int32_t cmdWlanStopApCallback(void *arg)
{
    int32_t           ret = 0;
    int32_t           role = 0;
    uint16_t          sta_info_len;
    uint16_t          start_sta_index = 0;
    uint16_t          actual_num_sta;
    uint16_t          i;
   /* No more than 4 stations can be connected to the CC31xx/CC32xx */
    SlNetCfgStaInfo_t ApStaList[4]; 

    ret = ParseCmd(arg);

    if(ret < 0)
    {
        printWlanStopApUsage(arg);
        return(-1);
    }

    if(app_CB.Role != ROLE_STA)
    {
        /* Before switching to Station role,
        send deauth request to all connected stations */

        /* Receive information about the connected stations */
        start_sta_index = 0;
        sta_info_len = sizeof(ApStaList);
        ret =
            sl_NetCfgGet(SL_NETCFG_AP_STATIONS_INFO_LIST, &start_sta_index,
                         &sta_info_len,
                         (uint8_t *)ApStaList);
        ASSERT_ON_ERROR(ret, WLAN_ERROR);
        actual_num_sta = sta_info_len / sizeof(SlNetCfgStaInfo_t);
        UART_PRINT("\r\n[AP Stop]: disconnecting from %d stations\r\n",
                   actual_num_sta);

        /* Iterate over the connected stations and send deauth request */
        for(i = 0; i < actual_num_sta; i++)
        {
            SlNetCfgStaInfo_t *staInfo = &ApStaList[i];
            ret =
               sl_NetCfgSet(SL_NETCFG_AP_STATION_DISCONNECT,1,SL_MAC_ADDR_LEN,
                             (const uint8_t *)&(staInfo->MacAddr));
            if(ret)
            {
                UART_PRINT("\r\n[AP Stop]: failed to disconnect from %s\r\n",
                           staInfo->Name);
            }
        }

        /* Set the device in Station role */
        ret = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(ret, WLAN_ERROR);

        /* Reset the device */
        ret = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_ON_ERROR(ret, DEVICE_ERROR);

        role = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(role, DEVICE_ERROR);

        if(ROLE_STA != role)
        {
            UART_PRINT(
                "\n\r[wlan ap stop] : Unable to configure Station role.\n\r");
            return(-1);
        }
        else
        {
            UART_PRINT(
                "\n\r[wlan ap stop] : Device configured to Station role.\n\r");
            app_CB.Role = ROLE_STA;
        }
    }
    else
    {
        UART_PRINT(
            "\n\r[wlan ap stop] : Device is already in Station role.\n\r");
        return(-1);
    }

    return(ret);
}

/*!
    \brief          Prints Stop Ap command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdWlanStopApCallback
*/
int32_t printWlanStopApUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(ap_stop_str);
    UART_PRINT(ap_stop_UsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(ap_stop_DetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Disconnect callback.

    This routine shows how to disconnect a device from an AP.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the 
                                  parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             ParseCmd

 */
int32_t cmdWlanDisconnectCallback(void *arg)
{
    int32_t          ret;

    ret = ParseCmd(arg);

    /* Since we call the general parser,
     * we print help menu here if it fails. */
    if(ret < 0)
    {
        printWlanDisconnectUsage(arg);
        return(0);
    }

    /* After calling sl_WlanDisconnect(),
     *    we expect WLAN disconnect asynchronous event.
     * Cleaning the former connection information from 
     * the application control block
     * is handled in that event handler,
     * as well as getting the disconnect reason.
     */
    ret = sl_WlanDisconnect();
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    return(ret);
}

/*!
    \brief          Prints Disconnect command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdWlanDisconnectCallback
*/
int32_t printWlanDisconnectUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(wlanDisconnectStr);
    UART_PRINT(wlanDisconnectUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(wlanDisConnectDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Connected Stations callback.

    This routine shows the stations that are connected to the AP.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             ParseCmd

*/
int32_t cmdConnectedStationsCallback(void *arg)
{
    int32_t ret;
    uint16_t sta_info_len;
    uint16_t start_sta_index = 0;
    uint16_t actual_num_sta;
    uint16_t i;
    /* No more than 4 stations can be connected to the CC3X20 */
    SlNetCfgStaInfo_t ApStaList[4]; 

    ret = ParseCmd(arg);

    if(ret < 0)
    {
        printConnectedStationsUsage(arg);
        return(0);
    }

    if (ROLE_AP == app_CB.Role)
    {
        start_sta_index = 0;
        sta_info_len = sizeof(ApStaList);
        ret =
            sl_NetCfgGet(SL_NETCFG_AP_STATIONS_INFO_LIST, &start_sta_index,
                         &sta_info_len,
                         (uint8_t *)ApStaList);

        ASSERT_ON_ERROR(ret, WLAN_ERROR);

        actual_num_sta = sta_info_len / sizeof(SlNetCfgStaInfo_t);
        UART_PRINT(
            "\r\n[Connected Stations]: number of connected stations is %d\r\n",
            actual_num_sta);

        for(i = 0; i < actual_num_sta; i++)
        {
            SlNetCfgStaInfo_t *staInfo = &ApStaList[i];
            UART_PRINT("\r\n AP Station %d is connected\r\n", i + 1);
            UART_PRINT(" NAME: %s\r\n", staInfo->Name);
            UART_PRINT(" MAC:  %02x:%02x:%02x:%02x:%02x:%02x\r\n",
                       staInfo->MacAddr[0], staInfo->MacAddr[1],
                       staInfo->MacAddr[2],
                       staInfo->MacAddr[3], staInfo->MacAddr[4],
                       staInfo->MacAddr[5]);
            UART_PRINT(" IP:   %d.%d.%d.%d\r\n", SL_IPV4_BYTE(staInfo->Ip,
                                                              3),
                       SL_IPV4_BYTE(staInfo->Ip,2), SL_IPV4_BYTE(staInfo->Ip,
                                                                 1),
                       SL_IPV4_BYTE(staInfo->Ip,0));
        }
    }
    else
    {
        UART_PRINT(
            "\r\n[Connected Stations]:"
            " This command is relevant to AP role only.\n");
        return(-1);
    }

    return(ret);
}

/*!
    \brief          Prints Disconnect command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdWlanDisconnectCallback
 */
int32_t printConnectedStationsUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(wlanDisconnectStr);
    UART_PRINT(ConnectedStationsUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(ConnectedStationsDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Create RX filter callback.

    This routine shows how to create an RX filter.
    These filter are created and added here, yet in order to get them to work
    calling 'enablefilter' command is necessary.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the
                                  parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           For more information about RX filters please refer to:
                    please refer to CC31xx/CC32xx NWP
					programmer's guide (SWRU455)

    \sa             ParseCreateFilterCmd

*/
int32_t cmdCreateFilterCallback(void *arg)
{
    int32_t           ret;
    CreateFilterCmd_t CreateFilterParams;

    /* Call the command parser */
    memset(&CreateFilterParams, 0x0, sizeof(CreateFilterParams));
    ret = ParseCreateFilterCmd(arg , &CreateFilterParams);

    if(ret < 0)
    {
        return(-1);
    }

    /*   These fields are required in order to configure 
                                            and successfully add an RX filter:
     *
     *   SlWlanRxFilterRuleType_t     ruleType  : What kind of filter to 
                                                  create: filter packets by
     *                                            Header fields or define a 
                                                  combination filter.
     *
     *   SlWlanRxFilterFlags_u        flags     : Dictates the following filter
                                                  settings and properties:
     *                                            Persistent - namely if filter
                                                  is retained after reset
     *                                            filter status -
                                                 (enabled/disabled).
     *                                            Comparison rule - 
                                                  Binary values are
                                                    compared when looking
                                                  for match,
     *                                            or strings are compared
                                                  when checking for 
                                                  a criteria match.
     *
     *   SlWlanRxFilterRule_u         rule      : Defines match criteria -
                                                   decides which compare 
                                                  criteria to use
     *                                            when looking for match. 
                                                  (Fields, arguments or 
                                                  compare function).
     *
     *   SlWlanRxFilterTrigger_t      trigger   : Sets preconditions to
                                                    trigger the filter.
                                                  Given a filter and 
                                                  trigger conditions,
     *                                            filter action would 
                                                  triggered if and only 
                                                  if a filter match has 
                                                  occurred and these
                                                  pre-conditions are met.
     *
     *   SlWlanRxFilterAction_t       action    : Operation to execute upon
                                                  a filter match - Could be
                                                  Host async event from NWP,
     *                                            or dropping the packet.
     *
     *   SlWlanRxFilterID_t           filterID  : Unique ID Returned by 
                                                  'sl_WlanRxFilterAdd()'
     *                                            So user could access and 
                                                  enable the created filter.
     *
     *   It is highly advised to see how these components 
          of the RX filter are filled.
     *   For more info. please refer to: ParseCreateFilterCmd.
     */

    /* Add filter. Note: Filters are not enabled yet! */
    ret = sl_WlanRxFilterAdd(CreateFilterParams.ruleType,
                             CreateFilterParams.flags,
                             &(CreateFilterParams.rule),
                             &(CreateFilterParams.trigger),
                             &(CreateFilterParams.action),
                             &(CreateFilterParams.filterID));

    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    UART_PRINT(
      "\r\n[Create filter] : Filter Created successfully, filters ID = %d\r\n",
      CreateFilterParams.filterID);

    return(0);
}

/*!
    \brief          Prints create filter command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdCreateFilterCallback
*/
int32_t printCreateFilterUsage(void *arg)
{
    uint8_t ch;

    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)usageStr);
    UART_PRINT((char*)createFilterStr);
    UART_PRINT((char*)createFilterUsageStr);
    UART_PRINT((char*)descriptionStr);
    UART_PRINT((char*)creteFilterDetailsStr);
    UART_PRINT((char*)createFilter_f_optionDetailsStr);
    UART_PRINT((char*)createFilter_v_optionDetailsStr);
    UART_PRINT((char*)createFilter_e_optionDetailsStr);
    UART_PRINT((char*)createFilter_a_optionDetailsStr);
    UART_PRINT((char*)createFilter_m_optionDetailsStr);
    UART_PRINT((char*)createFilter_o_optionDetailsStr);
    UART_PRINT((char*)createFilter_i_optionDetailsStr);
    UART_PRINT((char*)help_optaionDetails);
    UART_PRINT((char*)lineBreak);

    UART_PRINT("\n\rPrint frame sub-types ?\n\r");
    UART_PRINT((char*)lineBreak);
    UART_PRINT("\n\r 1. Yes.\n\r");
    UART_PRINT("\n\r 2. No.\n\r");

    ch = getch();

    if(ch == '1')
        printFrameSubTyps();

    return(0);
}

/*!
    \brief          Enable filter callback.

    This routine shows how to enable RX filters.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed 
                                  to the parser module.

    \return         Upon successful completion, the function
                    shall return 0.
                    In case of failure, this function would return -1;

    \note           This function enables *ALL* the added filters.

    \sa             ParseCmd

*/
int32_t cmdEnableFilterCallback(void *arg)
{
    SlWlanRxFilterOperationCommandBuff_t filterBitmap = {
        {0}
    };
    int32_t ret;

    ret = ParseCmd(arg);

    if(ret < 0)
    {
        printEnableFilterUsage(arg);
        return(0);
    }

    memset(&filterBitmap.FilterBitmap, 0xFF, 16);
    ret = sl_WlanSet(SL_WLAN_RX_FILTERS_ID, SL_WLAN_RX_FILTER_STATE,
                     sizeof(SlWlanRxFilterOperationCommandBuff_t),
                     (uint8_t*)&filterBitmap);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    return(0);
}

/*!
    \brief          Prints enable filter command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdEnableFilterCallback
*/
int32_t printEnableFilterUsage(void *arg)
{
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)usageStr);
    UART_PRINT((char*)enableFilterStr);
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)descriptionStr);
    UART_PRINT((char*)enableFilterDetailsStr);
    UART_PRINT((char*)help_optaionDetails);
    UART_PRINT((char*)lineBreak);
    return(0);
}

/*!
    \brief          Disable filter callback.

    This routine shows how to Disable RX filters.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to the 
                                  parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           This function Disables *ALL* the enabled filters.

    \sa             ParseCmd

*/
int32_t cmdDisableFilterCallback(void *arg)
{
    SlWlanRxFilterOperationCommandBuff_t filterBitmap = {
        {0}
    };
    int32_t ret;

    ret = ParseCmd(arg);

    if(ret < 0)
    {
        printDisableFilterUsage(arg);
        return(0);
    }

    memset(&filterBitmap.FilterBitmap, 0, 16);

    ret = sl_WlanSet(SL_WLAN_RX_FILTERS_ID, SL_WLAN_RX_FILTER_STATE,
                     sizeof(SlWlanRxFilterOperationCommandBuff_t),
                     (uint8_t *)&filterBitmap);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    return(0);
}

/*!
    \brief          Prints Disable filter command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdDisableFilterCallback
 */
int32_t printDisableFilterUsage(void *arg)
{
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)usageStr);
    UART_PRINT((char*)disableFilterStr);
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)descriptionStr);
    UART_PRINT((char*)disableFilterDetailsStr);
    UART_PRINT((char*)help_optaionDetails);
    UART_PRINT((char*)lineBreak);
    return(0);
}

/*!
    \brief          Delete filter callback.

    This routine shows how to Delete RX filters.
    This function deletes all the added filters.
    In order to delete an enabled filters, first disable it
    and only than call this function.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to 
                                  the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           This function Delete *ALL* the Added filters.

    \sa             ParseCmd

*/
int32_t cmdDeleteFilterCallback(void *arg)
{
    SlWlanRxFilterOperationCommandBuff_t filterBitmap = {
        {0}
    };
    int32_t ret;

    ret = ParseCmd(arg);

    if(ret < 0)
    {
        printDeleteFilterUsage(arg);
        return(0);
    }

    memset(&filterBitmap.FilterBitmap, 0xFF, 16);

    ret = sl_WlanSet(SL_WLAN_RX_FILTERS_ID, SL_WLAN_RX_FILTER_REMOVE,
                     sizeof(SlWlanRxFilterOperationCommandBuff_t),
                     (uint8_t *)&filterBitmap);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    return(ret);
}

/*!
    \brief          Prints Delete filter command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdDeleteFilterCallback
 */
int32_t printDeleteFilterUsage(void *arg)
{
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)usageStr);
    UART_PRINT((char*)deleteFilterStr);
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)descriptionStr);
    UART_PRINT((char*)deleteFilterDetailsStr);
    UART_PRINT((char*)help_optaionDetails);
    UART_PRINT((char*)lineBreak);

    return(0);
}

/*!
    \brief          WowLAN callback.

    This routine shows how to set Wake on wireless LAN packet.
    WoWLAN is basically an RX filter, which triggers an event upon
    receiving a packet containing "wake-up" pattern in it's header 
    or payload section.
    this allows the host to work in low power mode,
    and exit low power deep sleep (LPDS)
    when receiving this specific packet.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
                                  to the parser module.

    \return         Upon successful completion,
                    the function shall return 0.
                    In case of failure,
                    this function would return -1;

    \note           If the user wishes not to wait for a WLAN packet, 
	                he/she can press SW3
                    on the CC32xx board or S2 for the MSP-432 in
					 order to wake up the host processor.
                    Also, if the RX filter triggers,
					 the user can write his\her
                    handler in the asynchronous event handler,
					 SimpleLinkWlanEventHandler().

                    LPDS_WAKEUP_SW has the value 1 since this 
					is the same index for both
                    SW3 GPIO button in CC3220_LAUNCHXL.h and S2 button
                    in MSP_EXP432P401R.h.

    \sa             ParseEnableWoWLANCmd

*/
int32_t cmdEnableWoWLANCallback(void *arg)
{
    SlWlanRxFilterOperationCommandBuff_t filterBitmap = {{0}};
    WoWLANEnableCmd_t WoWLANEnableParams;
    uint16_t Opt;
    int32_t ret;
    uint16_t Size;

    /* Call the command parser */
    memset(&WoWLANEnableParams, 0x0, sizeof(WoWLANEnableParams));
    ret = ParseEnableWoWLANCmd(arg, &WoWLANEnableParams);

    if(ret < 0)
    {
        return(-1);
    }

    /* Sets the filter to be binary comparison filter */
    WoWLANEnableParams.ruleType = SL_WLAN_RX_FILTER_HEADER;
    WoWLANEnableParams.flags = SL_WLAN_RX_FILTER_BINARY;

    /* Sets the compare function to be simple comparison,
        and the action upon match to be
     * user event sent from the NWP.
     */
    WoWLANEnableParams.rule.Header.CompareFunc =
        SL_WLAN_RX_FILTER_CMP_FUNC_EQUAL;
    WoWLANEnableParams.action.Type = SL_WLAN_RX_FILTER_ACTION_EVENT_TO_HOST;

    /* Sets the filter rule as a header field filter rule */
    WoWLANEnableParams.rule.Header.Field =
        SL_WLAN_RX_FILTER_HFIELD_L4_PAYLOAD_PATTERN;

    /* Set the pre-conditions to trigger the filter : 
       if it's connected and in STA mode */
    WoWLANEnableParams.trigger.ConnectionState =
        SL_WLAN_RX_FILTER_STATE_STA_CONNECTED;
    WoWLANEnableParams.trigger.Role = SL_WLAN_RX_FILTER_ROLE_STA;

    /* Check if device is connected to network */
    if(!IS_CONNECTED(app_CB.Status))
    {
        UART_PRINT(
            "\n\r[wowlanenable] : Connect device to network in order to"
            " test wake on pattern\r\n");
        return(-1);
    }

    /* This filter has no parent filter */
    WoWLANEnableParams.trigger.ParentFilterID = 0;

    /* Configure this filter to *not* count
       how many times it's been triggered */
    WoWLANEnableParams.trigger.Counter = SL_WLAN_RX_FILTER_NO_TRIGGER_COUNTER;

    /* Remove all 64 RX filters (8*8) */
    memset(filterBitmap.FilterBitmap, 0xFF, 8);

    ret =
        sl_WlanSet(SL_WLAN_RX_FILTERS_ID, SL_WLAN_RX_FILTER_REMOVE,
                   sizeof(SlWlanRxFilterOperationCommandBuff_t),
                   (uint8_t *)&filterBitmap);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    /* Add this filter */
    ret = sl_WlanRxFilterAdd(WoWLANEnableParams.ruleType,
                             WoWLANEnableParams.flags,
                             &(WoWLANEnableParams.rule),
                             &(WoWLANEnableParams.trigger),
                             &(WoWLANEnableParams.action),
                             &(WoWLANEnableParams.filterID));

    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    /* Retrieve all enabled filters and enable the current
        (read modify write operation).*/
    Opt = SL_WLAN_RX_FILTER_STATE;
    Size = sizeof(SlWlanRxFilterRetrieveStateBuff_t);

    ret = sl_WlanGet(SL_WLAN_RX_FILTERS_ID, &Opt, &Size,
                     (uint8_t*)&filterBitmap);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    SL_WLAN_SETBIT8(filterBitmap.FilterBitmap, WoWLANEnableParams.filterID);

    /* Enable the added filter, without changing the existing ones */
    ret = sl_WlanSet(SL_WLAN_RX_FILTERS_ID, SL_WLAN_RX_FILTER_STATE,
                     sizeof(SlWlanRxFilterOperationCommandBuff_t),
                     (uint8_t*)&filterBitmap);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    UART_PRINT(
        "\n\r[wowlanenable] : MCU going to LPDS - wake up by sending packet "
        "containing the pattern:'%s'\n\r in offset %d in packet header,\r\n Or"
        " Press SW3 to exit LPDS.\r\n",
        WoWLANEnableParams.rule.Header.Args.Value.Pattern.Value,
        WoWLANEnableParams.rule.Header.Args.Value.Pattern.Offset);

    /*  Configure GPIO button 3 (SW3) 
	    as a wake-up source from LPDS for the CC32xx.
     *  For the MSP-432 WiFi plugin, we configure GPIO button 2 
	    (S2) as a wake-up source from LPDS.
     *  This is made in order to let the user wake it's MCU,
	    to abort waiting for WOWLAN (wake on wireless LAN) packet.
     */

#ifdef CC32XX
    PowerCC32XX_Wakeup wakeup;

    PowerCC32XX_getWakeup(&wakeup);
    wakeup.wakeupGPIOFxnLPDS = gpioButtonFxn1;
    PowerCC32XX_configureWakeup(&wakeup);
#endif

    GPIO_setCallback(LPDS_WAKEUP_SW, (GPIO_CallbackFxn)(&gpioButtonFxn1));

    /* Enable wake-up interrupt for the GPIO */
    GPIO_enableInt(LPDS_WAKEUP_SW);

    /* This call would enable the sleep policy,
	 as it defined in Board file. */
    Power_enablePolicy();

    /* Here we are pending on a semaphore, this context is blocked, 
       such that the idle function is called.
     * Once in the idle function runs, and the Power policy is enabled, 
	   if all peripherals are idle,
     * device would go into LPDS.
     */
    sem_wait(&app_CB.WowlanSleepSem);

    /* Clear the interrupt */
    GPIO_clearInt(LPDS_WAKEUP_SW);

    /* Disable the interrupt */
    GPIO_disableInt(LPDS_WAKEUP_SW);

    /* Return power policy to NORMAL */
    Power_disablePolicy();

    return(0);
}

/*!
    \brief          Prints enable wowlan command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion,
                    the function shall return 0.

    \sa             cmdEnableWoWLANCallback
 */
int32_t printEnableWoWLANUsage(void *arg)
{
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)usageStr);
    UART_PRINT((char*)enableWoWLANStr);
    UART_PRINT((char*)enableWoWLANUsageStr);
    UART_PRINT((char*)descriptionStr);
    UART_PRINT((char*)enableWoWLANDetailsStr);
    UART_PRINT((char*)enableWoWLAN_v_optionDetailsStr);
    UART_PRINT((char*)enableWoWLAN_u_optionDetailsStr);
    UART_PRINT((char*)enableWoWLAN_i_optionDetailsStr);
    UART_PRINT((char*)help_optaionDetails);
    UART_PRINT((char*)enableWoWLAN_Note_optionDetailsStr);
    UART_PRINT((char*)lineBreak);
    return(0);
}

/*!
    \brief          P2P start callback.

    This routine shows how to set device role to P2P mode.
    It consists of two staged process:
    Configure the P2P operational
    parameters, and later dispatch a worker 
    thread which responsible
    to establish a P2P connection with a remote device.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
                                  to the parser module.

    \return         Upon successful completion,
                    the function shall return 0.
                    In case of failure, 
                    this function would return -1;

    \note           During the connection process,
                    the cmd window can
                    Only accept "p2pstop" command 
                    in order to abort the P2P connection.
                    Any other command is ignored.

    \sa             setP2Pparams, p2pConnect

*/
int32_t cmdP2PModecallback(void *arg)
{

    int32_t             ret;
    uint8_t             abortflag = FALSE;
    pthread_t           p2p_connectThread = (pthread_t)NULL;
    pthread_attr_t      pAttrs_p2p_connect;
    struct sched_param  priParam;
    char                cmdStr[CMD_BUFFER_LEN+1];

    ret = ParseCmd(arg);

    if(ret < 0)
    {
        printP2PStartUsage(arg);
        return(0);
    }

    /* Configure P2P parameters before restarting the NWP in P2P role.*/
    ret = setP2Pparams();

    if(ret < 0)
    {
        UART_PRINT("\n\r[p2pstart] : Failed to configure P2P parameters.\n\r");
        return(-1);
    }

    /*  Create the p2p connection thread - 
        This thread is responsible to go over
     *  the p2p connection stages, and establish 
        connection as client or Group owner.
     *  User can stop the process at any time, 
        by typing p2pstop command.
     */
    pthread_attr_init(&pAttrs_p2p_connect);
    priParam.sched_priority = P2P_CONNECT_PRIORITY;
    ret = pthread_attr_setschedparam(&pAttrs_p2p_connect, &priParam);
    ret |= pthread_attr_setstacksize(&pAttrs_p2p_connect, P2P_STACK_SIZE);

    ASSERT_ON_ERROR(ret, OS_ERROR);

    ret = pthread_create(&p2p_connectThread, &pAttrs_p2p_connect, p2pConnect,
                         NULL);
    ASSERT_ON_ERROR(ret, OS_ERROR);

    memset(&cmdStr[0], 0x0, CMD_BUFFER_LEN+1);
    UART_PRINT(cmdPromptStr);

    while(!IS_CONNECTED(app_CB.Status))
    {
        ret = GetCmd(&cmdStr[0], CMD_BUFFER_LEN+1);
        if(ret > 0)
        {
            if(!strcmp(&cmdStr[0], p2pStopcmdStr))
            {
                /*signal the Connection thread to stop. */
                pthread_cancel(p2p_connectThread);
                /* Set the thread abort flag, in order to call P2P stop when this
                 * function exists. */
                abortflag = TRUE;
                break;
            }
            else
            {
                /* Clear line .. */
                UART_PRINT(
                    "\n\r[p2pstart] : Type \"p2pstop\" in order to stop p2p"
                    " connection process..\n\r");
                UART_PRINT(lineBreak);
                UART_PRINT(cmdPromptStr);
            }
        }
    }

    /* join with connection thread */
    ret = pthread_join(p2p_connectThread, NULL);
    ASSERT_ON_ERROR(ret, OS_ERROR);

    if(abortflag)
    {
        ret = cmdP2PStopcallback(arg);
    }

    return(ret);
}

/*!
    \brief          P2P connect thread.

    This routine connects with a P2P device. It has a similar 
	structure to wlanconnect;
    in the sense that is sends commands to the NWP, 
	and pend on several incoming events.
    The semaphores below are pend by this (p2pConnect thread) context.
	 They will be signaled once an asynchronous event
    Indicating that the NWP has gone through all the 
	stages of establishing a P2P connection.
    For further information, see CC31xx/CC32xx NWP 
	programmer's guide (SWRU455)

    \param          arg       -   Points to command line buffer.
                                  This container would be passed 
								  to the parser module.

    \return         Upon completion the function shall return NULL.

    \sa             setP2Pparams, cmdP2PModecallback, sendConnectionReq

*/
void *p2pConnect(void *arg)
{
    int32_t    ret;

    UART_PRINT(
        "\n\r[p2pstart] : User can stop the P2P connection process by typing"
        " 'p2pstop' command.\n\r");

    /* This would cause the NWP to send P2P probes, and listen on channel 6 */
    ret = sl_WlanConnect((int8_t*)P2P_REMOTE_DEVICE, strlen(
                             P2P_REMOTE_DEVICE), 0,&app_CB.P2P_CB.P2PsecParams,
                         0);

    if(ret < 0)
    {
        SHOW_WARNING(ret, WLAN_ERROR);
        return (NULL);
    }

    UART_PRINT("\n\r[p2pstart] : Waiting for device to be found ...\n\r");
    UART_PRINT(cmdPromptStr);
    ret = sem_wait(&app_CB.P2P_CB.DeviceFound);

    if(ret < 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return (NULL);
    }

    UART_PRINT("\n\r[p2pstart] : Waiting for Negotiation request ...\n\r");
    UART_PRINT(cmdPromptStr);
    ret = sem_wait(&app_CB.P2P_CB.RcvNegReq);

    if(ret < 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return (NULL);
    }

    /* Replay with connection request */
    UART_PRINT("\n\r[p2pstart] : Connecting ...\n\r");
    ret = sendConnectionReq();

    if(ret < 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return (NULL);
    }

    /* Wait for connect event */
    ret = sem_wait(&app_CB.P2P_CB.RcvConReq);

    if(ret < 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return (NULL);
    }

    /* Wait for IP to be leased */
    ret = sem_wait(&app_CB.CON_CB.ip4acquireEventSyncObj);

    if(ret < 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return (NULL);
    }

    SET_STATUS_BIT(app_CB.Status, STATUS_BIT_CONNECTION);

    /* After successful connection, this thread exists */
    pthread_exit(0);
    return (NULL);
}

/*!
    \brief          Prints P2P start command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function 
                    shall return 0.

    \sa             cmdP2PModecallback
*/
int32_t printP2PStartUsage(void *arg)
{
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)usageStr);
    UART_PRINT((char*)p2pStartcmdStr);
    UART_PRINT((char*)helpUsageStr);
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)descriptionStr);
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)p2pStartDetailsStr);
    UART_PRINT((char*)help_optaionDetails);
    UART_PRINT((char*)lineBreak);

    return(0);
}

/*!
    \brief          P2P stop callback.

    This routine shows how to stop P2P role 
    and set device role back to Station.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed to 
                                  the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           During this function we perform an NWP reset.
                    Also, any connection to P2P device would be lost.

    \sa             setP2Pparams, p2pConnect, cmdP2PModecallback

 */
int32_t cmdP2PStopcallback(void *arg)
{
    int32_t        ret = 0;

    ret = ParseCmd(arg);

    if(ret < 0)
    {
        printP2PStopUsage(arg);
        return(0);
    }

    /* Check to see if the current role is p2p */
    if(app_CB.Role == ROLE_P2P)
    {
        /* If we're connected to a peer, gracefully disconnect */
        if(IS_CONNECTED(app_CB.Status))
        {
            ret = sl_WlanDisconnect();
        }

        UART_PRINT("\n\r[p2pstop] : Stopping P2P process..\n\r");

        /* Stop the NWP in order to be able to configure it to default. */
        ret = sl_Stop(SL_STOP_TIMEOUT);
        ASSERT_ON_ERROR(ret, DEVICE_ERROR);
        /* return to default settings, and stop P2P mode */
        /*
            IMPORTANT NOTE - This is an example of a 'restore to default'
            function, it changes various
            parameters, which suit this application. User must update
            and adjust this function for
            he's / her needs, before using it.
        */
        ret = ConfigureSimpleLinkToDefaultState();
        memset(&app_CB.P2P_CB.p2pPeerDeviceName, 0x0,
               sizeof(app_CB.P2P_CB.p2pPeerDeviceName));
    }
    else
    {
        UART_PRINT(
            "\n\r[p2pstop] :  Put device in P2P mode first, see: "
            "p2pstart command\n\r");
    }

    UART_PRINT("\n\r[p2pstop] : P2P Mode Stopped.\n\r");
    return(ret);
}

/*!
    \brief          Prints P2P Stop command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function 
                    shall return 0.

    \sa             cmdP2PStopcallback
*/
int32_t printP2PStopUsage(void *arg)
{
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)usageStr);
    UART_PRINT((char*)p2pStopcmdStr);
    UART_PRINT((char*)helpUsageStr);
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)descriptionStr);
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)p2pStopDetailsStr);
    UART_PRINT((char*)help_optaionDetails);
    UART_PRINT((char*)lineBreak);

    return(0);
}



/*!
    \brief          Prints set country code.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             printCountrycodeeUsage
*/
int32_t printCountrycodeeUsage(void *arg)
{
    UART_PRINT((char*)lineBreak);
    UART_PRINT((char*)usageStr);
    UART_PRINT((char*)SetCountryCodeStr);
    UART_PRINT((char*)SetCountryCodeUsageStr);
    UART_PRINT((char*)descriptionStr);
    UART_PRINT((char*)CountryCodeDetailsStr);
    UART_PRINT(CountryCode_g_optionDetailsStr);
    UART_PRINT((char*)help_optaionDetails);
    UART_PRINT((char*)CountryCode_Note_optionDetailsStr);
    UART_PRINT((char*)lineBreak);
    return 0;
}

/*!
    \brief          Prints country code setting usage.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa
*/
int32_t cmdCountrycodecallback(void *arg)
{
    int32_t        ret = 0;
    uint8_t  country[2];

    ret = ParseCountycodeCmd(arg, country);
    if (ret < 0)
    {
        return 0;
    }
    sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, 
	SL_WLAN_GENERAL_PARAM_OPT_COUNTRY_CODE, 2, country);
    if (ret == 0)
    {
        /* reset is required to activate the configuration */
        sl_Stop(SL_STOP_TIMEOUT);
        sl_Start(NULL, NULL, NULL);
    }
    return (ret);
}
/*****************************************************************************
                  Local Functions
*****************************************************************************/

/*!
    \brief          Static IP setter.

    This routine shows how to stop set static configuration
    for Device IP, default gateway and static DNS address.

    \param          pIP       -   Points to static IP address in dotted decimal
                                  format (255.255.255.255)

    \param          pGw       -   Points to static gateway IP address 
                                  in dotted decimal format (255.255.255.255)

    \param          pDns       -  Points to static DNS server IP address
                                  in dotted decimal format (255.255.255.255)

    \return         Upon successful completion, the function 
                    shall return 0.
                    In case of failure, this function would return -1;

    \note           During this function we perform an NWP reset.
                    Also, if only static IP is provided, the default
                    gateway and the DNS server
                    would be the static IP with
                    netmask of 24 and the DNS would.

    \sa             cmdWlanConnectCallback

 */
int32_t setStaticIPConfig(uint8_t* pIP,
                          uint8_t* pGw,
                          uint8_t* pDns)
{

    int32_t                ret = 0;
    SlNetCfgIpV4Args_t     ipV4 = {0};

    if(pIP != NULL)
    {
        ret = ipv4AddressParse((char*)pIP, (uint32_t*)&ipV4.Ip);
        ASSERT_ON_ERROR(ret, CMD_ERROR);
    }

    if(pGw != NULL)
    {
        ret = ipv4AddressParse((char*)pGw, (uint32_t*)&ipV4.IpGateway);
        ASSERT_ON_ERROR(ret, CMD_ERROR);
    }
    else
    {
        /* Use the first address in subnet mask 24, as default GW */
        ipV4.IpGateway = ((ipV4.Ip & 0xFFFFFF00)|0x00000001);
    }

    ipV4.IpMask = (unsigned long)(SL_IPV4_VAL(255,255,255,0));

    if(pDns != NULL)
    {
        ret = ipv4AddressParse((char*)pDns, (uint32_t*)&ipV4.IpDnsServer);
        ASSERT_ON_ERROR(ret, CMD_ERROR);
    }
    else
    {
        ipV4.IpDnsServer = ipV4.IpGateway ;
    }

    ret =
        sl_NetCfgSet(SL_NETCFG_IPV4_STA_ADDR_MODE, SL_NETCFG_ADDR_STATIC,
                     sizeof(SlNetCfgIpV4Args_t),
                     (uint8_t *)&ipV4);
    ASSERT_ON_ERROR(ret, NETAPP_ERROR);

    /* Restart NWP to have Static IP configurations */
    ret = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(ret, DEVICE_ERROR);

    ret = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(ret, DEVICE_ERROR);

    return(0);
}

/*!
    \brief          Send p2p connection request.

    After P2P device was found, this function would let the NWP to sends probe
    requests for the P2P remote and continue the P2P connection process.
    For more info, please refer to: 
	CC31xx/CC32xx NWP programmer's guide (SWRU455)

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             cmdP2PModecallback

*/
int32_t sendConnectionReq(void)
{

    int32_t ret = 0;

    /* Remote's SSID known - Establish connection */
    ret = sl_WlanConnect((signed char*)&app_CB.P2P_CB.p2pPeerDeviceName[0],
                                strlen(
								(char*)&app_CB.P2P_CB.p2pPeerDeviceName[0]),
                                0,
                                &(app_CB.P2P_CB.P2PsecParams),
                                0);

    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    return(ret);
}

/*!
    \brief          Print scan results.

    This function print the scan results neatly in a table.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdScancallback

*/
void printScanResults(uint32_t res_num)
{

    uint32_t    index;
    uint32_t    sub_index;
    uint32_t    i;
    uint8_t     ssid_len;


    /* Print table column headers */
    UART_PRINT(lineBreak);
    printBorder('-', 93);
    UART_PRINT(lineBreak);
    UART_PRINT(
        "    |               SSID               |       BSSID       | RSSI  |"
        " Ch | Hidden | Security |\n\r");
    printBorder('-', 93);
    UART_PRINT(lineBreak);

    /* Print the table */
    for(index = 0; index < res_num; index++)
    {
        UART_PRINT(" %-2d ", index+1);

        /* In case the SSID length is 32 characters (the maximum valid size),
         * the NWP sends the SSID field without NULL terminating character.
         * In order to avoid printing a string which has no NULL terminated
         character,
         * print each character individually.
         */
        ssid_len = app_CB.gDataBuffer.netEntries[index].SsidLen;

        if (ssid_len < SL_WLAN_SSID_MAX_LENGTH)
        {
            UART_PRINT("| %-32s | ", app_CB.gDataBuffer.netEntries[index].Ssid);
        }
        else
        {
            UART_PRINT("| ");
            for(i = 0; i < ssid_len; i++)
            {
                UART_PRINT("%c", app_CB.gDataBuffer.netEntries[index].Ssid[i]);
            }
            UART_PRINT(" | ");
        }

        for(sub_index = 0; sub_index < 5 ; sub_index++)
        {
            UART_PRINT
			("%02x:", app_CB.gDataBuffer.netEntries[index].Bssid[sub_index]);
        }

        UART_PRINT("%02x |", app_CB.gDataBuffer.netEntries[index].Bssid[5]);

        UART_PRINT(" %-5d |", app_CB.gDataBuffer.netEntries[index].Rssi);

        UART_PRINT(" %-2d |", app_CB.gDataBuffer.netEntries[index].Channel);

        sub_index = app_CB.gDataBuffer.netEntries[index].SecurityInfo;

        UART_PRINT(" %s |", SL_WLAN_SCAN_RESULT_HIDDEN_SSID(
                       sub_index) == 0 ? "NO    " : "YES   ");

        if(SL_WLAN_SCAN_RESULT_SEC_TYPE_BITMAP(sub_index) == 0x6)
        {
            UART_PRINT(" %-4s |", WPAWPA2_str);
        }
        else
        {
            UART_PRINT(" %-4s     |",
                       SL_WLAN_SCAN_RESULT_SEC_TYPE_BITMAP(
                           sub_index) ==
                       0 ? OPEN_str : SL_WLAN_SCAN_RESULT_SEC_TYPE_BITMAP(
                           sub_index) == 4 ? WPA2_str :
                       SL_WLAN_SCAN_RESULT_SEC_TYPE_BITMAP(
                           sub_index) == 2 ? WPA_str : WEP_str);
        }

        UART_PRINT("\n\r");
    }

    printBorder('-', 93);
    UART_PRINT(lineBreak);

    return;
}

/*!
    \brief          Set P2P connection parameters.

    After P2P device was found, this function would let the NWP to sends probe
    requests for the P2P remote and continue the P2P connection process.
    For more info, please refer to: CC31xx/CC32xx NWP programmer's guide (SWRU455)

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             cmdP2PModecallback

*/
int32_t setP2Pparams(void)
{
    int32_t    ret ;
    int32_t    Mode;
    uint8_t    channels[4] = {0};
    uint8_t    nameLen = 0;

    /* Sets role negotiations: P2P intent and negotiation initiator */
    ret =
        sl_WlanPolicySet(SL_WLAN_POLICY_P2P,
                         SL_WLAN_P2P_POLICY(
                             SL_WLAN_P2P_ROLE_NEGOTIATE,
                             SL_WLAN_P2P_NEG_INITIATOR_RAND_BACKOFF),
                         NULL, 0);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    /* Set the P2P device type,
     * which is a part of the discovery process. Default is 1-0050F204-1 */
    ret =
        sl_WlanSet(SL_WLAN_CFG_P2P_PARAM_ID, SL_WLAN_P2P_OPT_DEV_TYPE,
                   strlen(P2P_DEVICE_TYPE),
                   (const  uint8_t *)P2P_DEVICE_TYPE);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    /* Configure device listen and operation channels */
    channels[0] = LISTEN_CHANNEL;
    channels[1] = LISTEN_REGULATORY_CLASS;
    channels[2] = OPRA_CHANNEL;
    channels[3] = OPRA_REGULATORY_CLASS;

    ret =
        sl_WlanSet(SL_WLAN_CFG_P2P_PARAM_ID, SL_WLAN_P2P_OPT_CHANNEL_N_REGS,
                   sizeof(channels),
                   channels);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    nameLen = sizeof(app_CB.P2P_CB.p2pDeviceName);

    /* Print the device's name */
    ret =
        sl_NetAppGet(SL_NETAPP_DEVICE_ID, SL_NETAPP_DEVICE_URN, &nameLen,
                     (uint8_t*)&app_CB.P2P_CB.p2pDeviceName[0]);
    ASSERT_ON_ERROR(ret, NETAPP_ERROR);

    UART_PRINT("\n\r[p2pstart] : Starting NWP in P2P role..\n\r");

    UART_PRINT("\n\rDevice name: %s\n\r", &app_CB.P2P_CB.p2pDeviceName);

    /* Set role to P2P and perform NWP reset */
    ret = sl_WlanSetMode(ROLE_P2P);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    ret = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(ret, DEVICE_ERROR);

    Mode = sl_Start(0, 0, 0);

    if(Mode != ROLE_P2P)
    {
        UART_PRINT(
            "\r\n[p2pstart] : Failed to configure device to P2P mode...\r\n");
        ASSERT_ON_ERROR(Mode, DEVICE_ERROR);
        return(-1);
    }
    else
    {
        app_CB.P2P_CB.P2PsecParams.Key = (signed char *)"";
        app_CB.P2P_CB.P2PsecParams.KeyLen = strlen(
            (const char*)app_CB.P2P_CB.P2PsecParams.Key);
        app_CB.P2P_CB.P2PsecParams.Type = SL_WLAN_SEC_TYPE_P2P_PBC;
        app_CB.Role = ROLE_P2P;
    }

    return(ret);
}

/*!
    \brief          Check sub-frame type validity.

    This function takes a byte representing a sub-frame type defined by 
    802.11 MAC and checks to make sure it's a valid frame type.

    \param          pSubFrameType   -   Points to byte representing frame type.

    \return         Upon successful completion (valid frame), the function
                    shall return 0.
                    In case of failure (invalid frame), this function 
                    would return -1;

    \sa             cmdCreateFilterCallback

*/
int32_t CheckSubFrameType(uint8_t *pSubFrameType)
{
    int32_t  ret = -1;
    uint8_t i ;

    for(i = 0; i < sizeof(MgmtFrames) ; i++)
    {
        if (MgmtFrames[i] == *(pSubFrameType))
        {
            return(0);
        }
    }

    for(i = 0; i < sizeof(DataFrames); i++)
    {
        if(DataFrames[i] == *(pSubFrameType))
        {
            return(0);
        }
    }

    for(i = 0; i < sizeof(CtrlFrames) ; i++)
    {
        if (CtrlFrames[i] == *(pSubFrameType))
        {
            return(0);
        }
    }

    return(ret);
}

/*!
    \brief          Print sub-frames type.

    This function print the various bytes represnting sub-frame types
    from a table.

    \return         Upon successful completion, the function shall return 0.

    \sa             CheckSubFrameType, cmdCreateFilterCallback

*/
void printFrameSubTyps(void)
{
    uint8_t i = 0;

    UART_PRINT("\n\rManagement sub frame types \n\r");
    for(i = 0; i < sizeof(MgmtFrames) ; i++)
    {
        UART_PRINT("0x%02x : %s \n\r", MgmtFrames[i], MangmentFrames_str[i] );
    }

    UART_PRINT("\n\rControl sub frame types \n\r");
    for(i = 0; i < sizeof(CtrlFrames) ; i++)
    {
        UART_PRINT("0x%02x: %s \n\r", CtrlFrames[i], CtrlFrames_str[i]);
    }

    UART_PRINT("\n\rData sub frame types \n\r");
    for(i = 0; i < sizeof(DataFrames) ; i++)
    {
        UART_PRINT("0x%02x : %s \n\r", DataFrames[i], DataFrames_str[i]);
    }

    return;
}
