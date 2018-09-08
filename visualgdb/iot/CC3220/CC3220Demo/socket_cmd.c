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

/* Example Header files */
#include "socket_cmd.h"
#include "network_terminal.h"
#include "cmd_parser.h"

//#define SECURE_SOCKET
//#define CLIENT_AUTHENTICATION

#ifdef SECURE_SOCKET
#define TCP_PROTOCOL_FLAGS    SL_SEC_SOCKET
#define ROOT_CA_CERT_FILE     "dummy-root-ca-cert"
#define PRIVATE_KEY_FILE      "dummy-trusted-cert-key"
#define TRUSTED_CERT_FILE     "dummy-trusted-cert"
#define TRUSTED_CERT_CHAIN    "trusted-chain.pem"

#define DEVICE_YEAR                 (2017)
#define DEVICE_MONTH                (4)
#define DEVICE_DATE                 (5)

#define BUF_LEN                (MAX_BUF_SIZE - 20)
#else
#define TCP_PROTOCOL_FLAGS      0
#define BUF_LEN                (MAX_BUF_SIZE)
#endif

typedef union
{
    SlSockAddrIn6_t in6;       /* Socket info for Ipv6 */
    SlSockAddrIn_t in4;        /* Socket info for Ipv4 */
}sockAddr_t;

/****************************************************************************
                      LOCAL FUNCTION PROTOTYPES
****************************************************************************/
int32_t UDPServer(uint8_t nb,
                  uint16_t portNumber,
                  uint8_t ipv6,
                  uint32_t numberOfPackets,
                  uint8_t tx);
int32_t UDPClient(uint8_t nb,
                  uint16_t portNumber,
                  ip_t ipAddress,
                  uint8_t ipv6,
                  uint32_t numberOfPackets,
                  uint8_t tx);
int32_t TCPServer(uint8_t nb,
                  uint16_t portNumber,
                  uint8_t ipv6,
                  uint32_t numberOfPackets,
                  uint8_t tx);
int32_t TCPClient(uint8_t nb,
                  uint16_t portNumber,
                  ip_t ipAddress,
                  uint8_t ipv6,
                  uint32_t numberOfPackets,
                  uint8_t tx);

/*****************************************************************************
                  Callback Functions
*****************************************************************************/

/*!
    \brief          Send callback.

    This routine shows how to send data in several configurations,
    As server, client, over TCP or UDP and also with various
    blocking or non-blocking socket option.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
								  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1.

    \sa             UDPServer, TCPServer, UDPClient, TCPClient

 */
int32_t cmdSendCallback(void *arg)
{
    int32_t ret = 0;
    SendCmd_t SendCmdParams;

    /* Call the command parser */
    memset(&SendCmdParams, 0x0, sizeof(SendCmd_t));
    ret = ParseSendCmd(arg, &SendCmdParams);

    if(ret < 0)
    {
        FreeSendCmd(&SendCmdParams);
        return(-1);
    }

    /*    After Parsing, call the corresponding function,
     *    Based on protocol and role (Server/Client).
     */
    if(SendCmdParams.udpFlag && SendCmdParams.server)
    {
        ret =
            UDPServer(SendCmdParams.nb, SendCmdParams.portNumber,
                      SendCmdParams.ipv6,
                      SendCmdParams.numberOfPackets,
                      TRUE);
    }
    else if(SendCmdParams.udpFlag && !SendCmdParams.server)
    {
        ret =
            UDPClient(SendCmdParams.nb, SendCmdParams.portNumber,
                      SendCmdParams.ipAddr, SendCmdParams.ipv6,
                      SendCmdParams.numberOfPackets,
                      TRUE);
    }
    else if(!SendCmdParams.udpFlag && SendCmdParams.server)
    {
        ret =
            TCPServer(SendCmdParams.nb, SendCmdParams.portNumber,
                      SendCmdParams.ipv6,
                      SendCmdParams.numberOfPackets,
                      TRUE);
    }
    else if(!SendCmdParams.udpFlag && !SendCmdParams.server)
    {
        ret =
            TCPClient(SendCmdParams.nb, SendCmdParams.portNumber,
                      SendCmdParams.ipAddr, SendCmdParams.ipv6,
                      SendCmdParams.numberOfPackets,
                      TRUE);
    }

    FreeSendCmd(&SendCmdParams);
    return(ret);
}

/*!
    \brief          Prints Send command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion,
					the function shall return 0.

    \sa             cmdSendCallback

 */
int32_t printSendUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(sendStr);
    UART_PRINT(sendUsage1Str);
    UART_PRINT("\t");
    UART_PRINT(sendStr);
    UART_PRINT(sendUsage2Str);
    UART_PRINT(descriptionStr);
    UART_PRINT(sendDetailsStr);
    UART_PRINT(send_c_optionDetailsStr);
    UART_PRINT(send_s_optionDetailsStr);
    UART_PRINT(send_u_optionDetailsStr);
    UART_PRINT(send_p_optionDetailsStr);
    UART_PRINT(send_nb_optionDetailsStr);
    UART_PRINT(send_n_optionDetailsStr);
    UART_PRINT(send_6_optionDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Receive callback.

    This routine shows how to Receive data in several configurations,
    As server, client, over TCP or UDP and also with various
    blocking or non-blocking socket option.

    \param          arg       -   Points to command line buffer.
                                  This container would be
								  passed to the parser module.

    \return         Upon successful completion,
					the function shall return 0.
                    In case of failure, this function would return -1.

    \sa             UDPServer, TCPServer, UDPClient, TCPClient

 */
int32_t cmdRecvCallback(void *arg)
{
    int32_t ret = 0;
    int16_t rx_stat;
    RecvCmd_t RecvCmdParams;
    SlWlanGetRxStatResponse_t RxStat;

    /* Call the command parser */
    memset(&RecvCmdParams, 0x0, sizeof(RecvCmd_t));
    ret = ParseRecvCmd(arg, &RecvCmdParams);

    if(ret < 0)
    {
        FreeRecvCmd(&RecvCmdParams);
        return(-1);
    }

    /* In order to have RX statistics, we invoke sl_WlanRxStatStart().
     * the Variable 'rx_stat' Signify the status of the statistics request.
     */
    rx_stat = sl_WlanRxStatStart();
    ASSERT_AND_CLEAN_RECV(rx_stat, WLAN_ERROR, &RecvCmdParams);

    if(RecvCmdParams.udpFlag && RecvCmdParams.server)
    {
        ret =
            UDPServer(RecvCmdParams.nb, RecvCmdParams.portNumber,
                      RecvCmdParams.ipv6,
                      RecvCmdParams.numberOfPackets,
                      FALSE);
    }
    else if(RecvCmdParams.udpFlag && !RecvCmdParams.server)
    {
        ret =
            UDPClient(RecvCmdParams.nb, RecvCmdParams.portNumber,
                      RecvCmdParams.ipAddr, RecvCmdParams.ipv6,
                      RecvCmdParams.numberOfPackets,
                      FALSE);
    }
    else if(!RecvCmdParams.udpFlag && RecvCmdParams.server)
    {
        ret =
            TCPServer(RecvCmdParams.nb, RecvCmdParams.portNumber,
                      RecvCmdParams.ipv6,
                      RecvCmdParams.numberOfPackets,
                      FALSE);
    }
    else if(!RecvCmdParams.udpFlag && !RecvCmdParams.server)
    {
        ret =
            TCPClient(RecvCmdParams.nb, RecvCmdParams.portNumber,
                      RecvCmdParams.ipAddr, RecvCmdParams.ipv6,
                      RecvCmdParams.numberOfPackets,
                      FALSE);
    }

    if(ret < 0)
    {
        FreeRecvCmd(&RecvCmdParams);
        return(-1);
    }

    if(!rx_stat)
    {
        sl_WlanRxStatGet(&RxStat,0);
        drawRxHist(&RxStat);
    }

    FreeRecvCmd(&RecvCmdParams);
    return(0);
}

/*!
    \brief          Prints Receive command help menu.

    \param          arg       -   Points to command line buffer.

    \return         Upon successful completion, the function shall return 0.

    \sa             cmdRecvCallback

 */
int32_t printRecvUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(recvStr);
    UART_PRINT(recvUsage1Str);
    UART_PRINT("\t");
    UART_PRINT(recvStr);
    UART_PRINT(recvUsage2Str);
    UART_PRINT(descriptionStr);
    UART_PRINT(recvDetailsStr);
    UART_PRINT(recv_c_optionDetailsStr);
    UART_PRINT(recv_s_optionDetailsStr);
    UART_PRINT(recv_u_optionDetailsStr);
    UART_PRINT(recv_p_optionDetailsStr);
    UART_PRINT(recv_nb_optionDetailsStr);
    UART_PRINT(send_n_r_optionDetailsStr);
    UART_PRINT(recv_6_optionDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(recv_Note_optionDetailsStr);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Plot RX histogram.

    This routine prints the rate and RSSI histogram,
    which were sampled by 'sl_WlanRxStatStart'.
        For each received packet over the sampling period.

    \param          rxStatResp       -   Points to the RX statistics structure,
                                         returned by sl_WlanRxStatGet().

    \sa             sl_WlanRxStatGet, sl_WlanRxStatStart

 */
void drawRxHist(SlWlanGetRxStatResponse_t *rxStatResp)
{
    float percent;
    int32_t i,j;

    UART_PRINT(
        "\r\n*********************************Rx Statistics"
		"**********************************\r\n");
    UART_PRINT("Received Packets: %d \r\n",
               rxStatResp->ReceivedValidPacketsNumber);
    UART_PRINT(
        "Average RSSI for management: %d Average"
		" RSSI for other packets: %d\r\n\r\n",
        rxStatResp->AvarageMgMntRssi,rxStatResp->AvarageDataCtrlRssi);

    if(rxStatResp->ReceivedValidPacketsNumber == 0)
    {
        UART_PRINT(
            "    -----------------------   no data collected"
			"   -----------------------\r\n");
    }
    else
    {
        UART_PRINT(
            "    -----------------------   RSSI Histogram"
			"   -----------------------\r\n\r\n");
        UART_PRINT(
            "-40dBm to -87dBm (below and above RSSI will"
			" appear in the first and last cells]\r\n\r\n");

        UART_PRINT(
            "           10   20   30   40   50   60   70   80   90   100\r\n");
        UART_PRINT(
            "         |----+----+----+----+----+----+----+----+----+----|\r\n");

        //RSSI histogram from -40 until -87 
		//(all below and above RSSI will appear in the first and last cells
        for(i = 0; i < SL_WLAN_SIZE_OF_RSSI_HISTOGRAM; i++)
        {
            percent =
                ((float)rxStatResp->RssiHistogram[i] /
                 (float)rxStatResp->ReceivedValidPacketsNumber * (float)100);
            if(0 == i)
            {
                UART_PRINT(">-40dBm  ");
            }
            else
            {
                if((SL_WLAN_SIZE_OF_RSSI_HISTOGRAM - 1) == i)
                {
                    UART_PRINT("<-87dBm");
                }
                else
                {
                    UART_PRINT("  -%-2ddBm",40 +
                               (i * 47 / (SL_WLAN_SIZE_OF_RSSI_HISTOGRAM - 1)));
                }
            }
            for(j = 0; j < (int)percent / 2; j++)
            {
                UART_PRINT("*");
            }
            for(j = 0; j < 50 - (int)percent / 2; j++)
            {
                UART_PRINT(" ");
            }
            UART_PRINT("(%.2f%%)\r\n",percent);
            UART_PRINT(
                "         |----+----+----+----+----+----+----+----+----+----|"
				"\r\n");
        }

        UART_PRINT(
            "              10   20   30   40   50 "
			"  60   70   80   90   100\r\n\r\n");

        /* Print Rate Histogram */

        UART_PRINT(
            "    -----------------------   Rate Histogram"
			"   -----------------------\r\n\r\n");
        UART_PRINT(
            "           10   20   30   40   50   60   70   80   90   100\r\n");
        UART_PRINT(
            "         |----+----+----+----+----+----+----+----+----+----|\r\n");

        for(i = 0; i < SL_WLAN_NUM_OF_RATE_INDEXES; i++)
        {
            percent =
                ((float)rxStatResp->RateHistogram[i] /
                 (float)rxStatResp->ReceivedValidPacketsNumber * (float)100);
            UART_PRINT("%-2d         ",i);
            for(j = 0; j < (int)percent / 2; j++)
            {
                UART_PRINT("*");
            }
            for(j = 0; j < 50 - (int)percent / 2; j++)
            {
                UART_PRINT(" ");
            }
            UART_PRINT("(%.2f%%)\r\n",percent);
            UART_PRINT(
                "         |----+----+----+----+----+----+----+----+----+----|"
				"\r\n");
        }

        UART_PRINT(
            "              10   20   30   40   50   60   70   80   90   100\r\n");
    }

    UART_PRINT(
        "\r\n                     The data was sampled during %umSec\r\n",
        (((unsigned int)rxStatResp->GetTimeStamp -
          rxStatResp->StartTimeStamp)) / 1000);

    UART_PRINT(
        "*******************************End Rx Statistics"
		"********************************");
}

/*****************************************************************************
                  Local Functions
*****************************************************************************/

/*!
    \brief          UDP server.

    This routine shows how to set up a simple UDP server.
    It shows sending and receiving packets as well.

    \param          nb              -   Sets the socket type:
										blocking or non-blocking.

    \param          portNumber      -   Decides which port is affiliated 
										with the server's socket.

    \param          ipv6            -   Sets the version of the L3 IP protocol,
										IPv4 or IPv6.

    \param          numberOfPackets -   Sets the Number of 
										packets to send \ receive.

    \param          tx              -   Decides if the function would 
										transmit data.
										If this flag is set to false,
										this function would receive.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             cmdSendCallback, cmdRecvCallback

 */
int32_t UDPServer(uint8_t nb,
                  uint16_t portNumber,
                  uint8_t ipv6,
                  uint32_t numberOfPackets,
                  uint8_t tx)
{
    int32_t sock;
    int32_t status;
    uint32_t i = 0;
    int32_t nonBlocking;
	 /* Contains the local ip address and port */
    int32_t buflen;
    SlSockAddr_t    *sa;           
    /* Contains the ip address and port of the connected peer. */
	SlSockAddr_t    *csa;           
    sockAddr_t sAddr;
    int32_t addrSize;
    uint16_t nb_counter = 0;
    struct SlTimeval_t TimeVal;

    /* clear the global data buffer */
    memset(&app_CB.gDataBuffer.nwData, 0x0, sizeof(app_CB.gDataBuffer.nwData));

    /* filling the buffer with data */
    for(i = 0; i < MAX_BUF_SIZE; i++)
    {
        app_CB.gDataBuffer.nwData[i] = (char)(i % 10);
    }

    buflen = MAX_BUF_SIZE;

    /* filling the addresses and ports for the server's side.
     * Note: Since this is the server's side, we do not require to
     * fill the IP address field of the SlSockAddr_t structure */
    if(ipv6)
    {
		/* Set socket family according to L3 Protocol: Ipv4 or Ipv6 */
        sAddr.in6.sin6_family = SL_AF_INET6;             
        /* Change the port number byte ordering from Host order (little endian)
		* to network order (Big endian) */
		sAddr.in6.sin6_port = sl_Htons(portNumber);      
                                                          

        sAddr.in6.sin6_flowinfo = 0;
        memset(&sAddr.in6.sin6_addr, 0, 16);

        sa = (SlSockAddr_t*)&sAddr.in6;
        csa = (SlSockAddr_t*)&sAddr.in6;
        addrSize = sizeof(SlSockAddrIn6_t);
    }
    else
    {
        sAddr.in4.sin_family = SL_AF_INET;
        sAddr.in4.sin_port = sl_Htons((uint16_t)portNumber);
        sAddr.in4.sin_addr.s_addr = SL_INADDR_ANY;

        sa = (SlSockAddr_t*)&sAddr.in4;
        csa = (SlSockAddr_t*)&sAddr.in4;
        addrSize = sizeof(SlSockAddrIn_t);
    }

    /* Get UDP sock descriptor - This call opens the socket. */
    sock = sl_Socket(sa->sa_family, SL_SOCK_DGRAM, 0);
    ASSERT_ON_ERROR(sock, SL_SOCKET_ERROR);

    /* Bind socket to port -
     * Since this is the server's side, we bind the socket descriptor we just
     * Opened, to a specific port, on which we'll get traffic from. */
    status = sl_Bind(sock, sa, addrSize);

    if(status < 0)
    {
        UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                   SL_SOCKET_ERROR);
        sl_Close(sock);
        return(-1);
    }

    /* Set socket as non-blocking socket (if needed):
     * Non-blocking sockets allows user to handle other tasks rather than block
     * on socket API calls. If an API call using the Non-blocking socket descriptor
     * returns 'SL_ERROR_BSD_EAGAIN' - 
	 * this indicate that the user should try the API again later.
     */
    if(TRUE == nb)
    {
        nonBlocking = TRUE;
        status =
            sl_SetSockOpt(sock, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking,
                          sizeof(nonBlocking));
        if(status < 0)
        {
            UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                       SL_SOCKET_ERROR);
            sl_Close(sock);
            return(-1);
        }
    }
    else
    {
        /* In case of blocking, a timeout for sl_RecvFrom will be set to TimeVal
        * When timeout is expired sl_RecvFrom will return SL_ERROR_BSD_EAGAIN */
        TimeVal.tv_sec = RECEIVE_TIMEOUT;
        TimeVal.tv_usec = 0;
        status =
            sl_SetSockOpt(sock,SL_SOL_SOCKET,SL_SO_RCVTIMEO,
                          (uint8_t *)&TimeVal,
                          sizeof(TimeVal));
        if(status < 0)
        {
            UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                       SL_SOCKET_ERROR);
            sl_Close(sock);
            return(-1);
        }
    }

    i = 0;

    if(tx)
    {
        while(i < numberOfPackets)
        {
            /* Even though this part of the function runs as UDP transmitter(tx),
			 * 'Sl_RecvFrom' is invoked here
             * in order to fetch the peer's IP address. This information is 
			 * copied to 'csa'.
             */
            status =
                sl_RecvFrom(sock, &app_CB.gDataBuffer.nwData, buflen, 0, csa,
                            (SlSocklen_t*)&addrSize);

            if((status == SL_ERROR_BSD_EAGAIN) && (TRUE == nb))
            {
                sleep(1);
                continue;
            }
            else if(status <= 0)
            {
                UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                           SL_SOCKET_ERROR);
                sl_Close(sock);
                return(-1);
            }

            UART_PRINT("Connected to client: ");
            if(ipv6)
            {
                PrintIPAddress(ipv6,(void*)&sAddr.in6.sin6_addr);
            }
            else
            {
                PrintIPAddress(ipv6,(void*)&sAddr.in4.sin_addr);
            }

            UART_PRINT(lineBreak);

            while(TRUE)
            {
                /*  Calling 'sl_SendTo' until we send all the packets
                 *  we requested to send.
                 */
                status =
                    sl_SendTo(sock, &app_CB.gDataBuffer.nwData, buflen, 0, csa,
                              addrSize);

                if((status == SL_ERROR_BSD_EAGAIN) && (TRUE == nb))
                {
                    sleep(1);
                    continue;
                }
                else if(status < 0)
                {
                    UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                               SL_SOCKET_ERROR);
                    sl_Close(sock);
                    return(-1);
                }
                break;
            }

            i++;
        }
        UART_PRINT("Sent %u packets successfully\n\r",numberOfPackets);
    }
    else
    {
        while(i < numberOfPackets)
        {
            /* If user doesn't with to transmit data,
			 * sl_recvFrom is invoked in order to
             * receive UDP datagrams here.
             */
            status =
                sl_RecvFrom(sock, &app_CB.gDataBuffer.nwData, buflen, 0, csa,
                            (SlSocklen_t*)&addrSize);
            if(status == SL_ERROR_BSD_EAGAIN)
            {
                if(TRUE == nb)
                {
                    sleep(1);
                    nb_counter++;
                    if(nb_counter < RECEIVE_TIMEOUT)
                    {
                        continue;
                    }
                    UART_PRINT(
                        "Timeout expired before receiving all packets\n\r");
                    break;
                }
                else
                {
                    UART_PRINT(
                        "Timeout expired before receiving all packets\n\r");
                    break;
                }
            }
            else if(status < 0)
            {
                UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                           SL_SOCKET_ERROR);
                sl_Close(sock);
                return(-1);
            }

            i++;
        }
        UART_PRINT("Received %u packets successfully\n\r",i);
    }

    /* Calling 'close' with the socket descriptor,
     * once operation is finished.
     */
    status = sl_Close(sock);
    ASSERT_ON_ERROR(status, SL_SOCKET_ERROR);

    return(0);
}

/*!
    \brief          UDP Client.

    This routine shows how to set up a simple UDP Client.
    It shows sending and receiving packets as well.

    \param          nb              -   Sets the socket type:
										blocking or non-blocking.

    \param          portNumber      -   Decides which port is 
										affiliated with the server's socket.

    \param          ipv6            -   Sets the version of
										the L3 IP protocol, IPv4 or IPv6.

    \param          numberOfPackets -   Sets the Number of
										packets to send \ receive.

    \param          tx              -   Decides if the function 
										would transmit data. If this flag
                                        is set to false,
										this function would receive.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             cmdSendCallback, cmdRecvCallback

 */
int32_t UDPClient(uint8_t nb,
                  uint16_t portNumber,
                  ip_t ipAddress,
                  uint8_t ipv6,
                  uint32_t numberOfPackets,
                  uint8_t tx)
{
    int32_t sock;
    int32_t status;
    uint32_t i = 0;
    int32_t nonBlocking;
    int32_t buflen;
    SlSockAddr_t        *sa;
    int32_t addrSize;
    sockAddr_t sAddr;
    uint16_t nb_counter = 0;
    struct SlTimeval_t TimeVal;

    /* clear the global data buffer */
    memset(&app_CB.gDataBuffer.nwData, 0x0, sizeof(app_CB.gDataBuffer));

    /* filling the buffer with data */
    for(i = 0; i < MAX_BUF_SIZE; i++)
    {
        app_CB.gDataBuffer.nwData[i] = (char)(i % 10);
    }

    buflen = MAX_BUF_SIZE;

    if(ipv6)
    {
        sAddr.in6.sin6_family = SL_AF_INET6;
        sAddr.in6.sin6_port = sl_Htons(portNumber);
        sAddr.in6.sin6_flowinfo = 0;

        sAddr.in6.sin6_addr._S6_un._S6_u32[0] =
            ((unsigned long *)ipAddress.ipv6)[0];
        sAddr.in6.sin6_addr._S6_un._S6_u32[1] =
            ((unsigned long *)ipAddress.ipv6)[1];
        sAddr.in6.sin6_addr._S6_un._S6_u32[2] =
            ((unsigned long *)ipAddress.ipv6)[2];
        sAddr.in6.sin6_addr._S6_un._S6_u32[3] =
            ((unsigned long *)ipAddress.ipv6)[3];

        sa = (SlSockAddr_t*)&sAddr.in6;
        addrSize = sizeof(SlSockAddrIn6_t);
    }
    else
    {
        /* Set socket family according to L3 Protocol:
			Ipv4 or Ipv6 - this is IPv4 case */
        sAddr.in4.sin_family = SL_AF_INET;
        /* Change the port number and IP 
		   address byte ordering from Host order (little endian)
         * to network order (Big endian) */
        sAddr.in4.sin_port = sl_Htons((unsigned short)portNumber);
        sAddr.in4.sin_addr.s_addr = sl_Htonl((unsigned int)ipAddress.ipv4);

        /* Since this is the client's side, 
		 * we must know beforehand the IP address
         * and the port of the server wer'e trying to send/receive UDP data to.
         */
        sa = (SlSockAddr_t*)&sAddr.in4;
        addrSize = sizeof(SlSockAddrIn_t);
    }

    /* Get UDP sock descriptor - This call opens the socket. */
    sock = sl_Socket(sa->sa_family,SL_SOCK_DGRAM, 0);
    ASSERT_ON_ERROR(sock, SL_SOCKET_ERROR);

    /* Set socket as non-blocking socket (if needed):
     * Non-blocking sockets allows user to handle other tasks rather than block
     * on socket API calls. 
	 * If an API call using the Non-blocking socket descriptor
     * returns 'SL_ERROR_BSD_EAGAIN' - 
	 * this indicate that the user should try the API again later.
     */
    if(TRUE == nb)
    {
        nonBlocking = TRUE;
        status =
            sl_SetSockOpt(sock, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking,
                          sizeof(nonBlocking));
        if(status < 0)
        {
            UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                       SL_SOCKET_ERROR);
            sl_Close(sock);
            return(-1);
        }
    }
    else
    {
       /* In case of blocking, a timeout for sl_RecvFrom will be set to TimeVal
       * When timeout is expired sl_RecvFrom will return SL_ERROR_BSD_EAGAIN */
        TimeVal.tv_sec = RECEIVE_TIMEOUT;
        TimeVal.tv_usec = 0;
        status =
            sl_SetSockOpt(sock,SL_SOL_SOCKET,SL_SO_RCVTIMEO,
                          (uint8_t *)&TimeVal,
                          sizeof(TimeVal));
        if(status < 0)
        {
            UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                       SL_SOCKET_ERROR);
            sl_Close(sock);
            return(-1);
        }
    }

    i = 0;

    if(tx)
    {
        while(i < numberOfPackets)
        {
            /* Send packets to server */
            status =
                sl_SendTo(sock, &app_CB.gDataBuffer.nwData, buflen, 0, sa,
                          (SlSocklen_t)addrSize);
            if((status == SL_ERROR_BSD_EAGAIN) && (TRUE == nb))
            {
                sleep(1);
                continue;
            }
            else if(status < 0)
            {
                UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                           SL_SOCKET_ERROR);
                sl_Close(sock);
                return(-1);
            }
            i++;
        }
        UART_PRINT("Sent %u packets successfully\n\r", numberOfPackets);
    }
    else
    {
        while(i < numberOfPackets)
        {
            /* Receive packets from Server */
            status =
                sl_RecvFrom(sock, &app_CB.gDataBuffer.nwData, buflen, 0, sa,
                            (SlSocklen_t*)&addrSize);
            if(status == SL_ERROR_BSD_EAGAIN)
            {
                if(TRUE == nb)
                {
                    sleep(1);
                    nb_counter++;
                    if(nb_counter < RECEIVE_TIMEOUT)
                    {
                        continue;
                    }
                    UART_PRINT(
                        "Timeout expired before receiving all packets\n\r");
                    break;
                }
                else
                {
                    UART_PRINT(
                        "Timeout expired before receiving all packets\n\r");
                    break;
                }
            }
            else if(status < 0)
            {
                UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                           SL_SOCKET_ERROR);
                sl_Close(sock);
                return(-1);
            }
            i++;
        }
        UART_PRINT("Received %u packets successfully\n\r",i);
    }
    /* Calling 'close' with the socket descriptor,
     * once operation is finished.
     */
    status = sl_Close(sock);
    ASSERT_ON_ERROR(status, SL_SOCKET_ERROR);

    return(0);
}

/*!
    \brief          TCP Server.

    This routine shows how to set up a simple TCP server.
    It shows sending and receiving packets as well.

    \param          nb              -   Sets the socket type:
										blocking or non-blocking.

    \param          portNumber      -   Decides which port is
										affiliated with the server's socket.

    \param          ipv6            -   Sets the version of the
										L3 IP protocol, IPv4 or IPv6.

    \param          numberOfPackets -   Sets the Number of 
										packets to send \ receive.

    \param          tx              -   Decides if the function would 
										transmit data. If this flag
                                        is set to false, 
										this function would receive.

    \return         Upon successful completion,
					the function shall return 0.
                    In case of failure, 
					this function would return -1;

    \sa             cmdSendCallback, cmdRecvCallback

 */

int32_t TCPServer(uint8_t nb,
                  uint16_t portNumber,
                  uint8_t ipv6,
                  uint32_t numberOfPackets,
                  uint8_t tx)
{
    int32_t sock;
    int32_t status;
    int32_t newsock = -1;
    uint32_t i = 0;
    int32_t nonBlocking = FALSE;
	/* Contains the local ip address and port */
    SlSockAddr_t    *sa;    
	/* Contains the ip address and port of the connected peer. */	
    SlSockAddr_t    *csa;           
    sockAddr_t sAddr;
    int32_t addrSize;

    /* clear the global data buffer */
    memset(&app_CB.gDataBuffer.nwData, 0x0, sizeof(app_CB.gDataBuffer));

    /* clear the global data buffer */
    for(i = 0; i < MAX_BUF_SIZE; i++)
    {
        app_CB.gDataBuffer.nwData[i] = (char)(i % 10);
    }

    if(ipv6)
    {
        sAddr.in6.sin6_family = SL_AF_INET6;
        sAddr.in6.sin6_port = sl_Htons(portNumber);
        memset(sAddr.in6.sin6_addr._S6_un._S6_u32, SL_INADDR_ANY,
               sizeof(sAddr.in6.sin6_addr._S6_un._S6_u32));

        sa = (SlSockAddr_t*)&sAddr.in6;
        csa = (SlSockAddr_t*)&sAddr.in6;
        addrSize = sizeof(SlSockAddrIn6_t);
    }
    else
    {
        /* filling the TCP server socket address */
        sAddr.in4.sin_family = SL_AF_INET;

        /* Set the server's port: 
		   We'll receive connection requests on this port */
        sAddr.in4.sin_port = sl_Htons((unsigned short)portNumber);
        sAddr.in4.sin_addr.s_addr = SL_INADDR_ANY;

        sa = (SlSockAddr_t*)&sAddr.in4;
        csa = (SlSockAddr_t*)&sAddr.in4;
        addrSize = sizeof(SlSockAddrIn_t);
    }

    /*
     *  Open a TCP socket:
     *  Since TCP is a connection oriented channel,
     *  the opened socket would serve as 'welcome' socket,
     *  on which we'll receive connection requests from clients.
     */
    sock = sl_Socket(sa->sa_family, SL_SOCK_STREAM, TCP_PROTOCOL_FLAGS);
    ASSERT_ON_ERROR(sock, SL_SOCKET_ERROR);

#ifdef SECURE_SOCKET

#ifdef CLIENT_AUTHENTICATION
    SlDateTime_t dateTime;
    dateTime.tm_day = DEVICE_DATE;
    dateTime.tm_mon = DEVICE_MONTH;
    dateTime.tm_year = DEVICE_YEAR;

    sl_DeviceSet(SL_DEVICE_GENERAL, SL_DEVICE_GENERAL_DATE_TIME,
                 sizeof(SlDateTime_t), (uint8_t *)(&dateTime));

    /* Set the following to enable Client Authentication */
    sl_SetSockOpt(sock,SL_SOL_SOCKET,SL_SO_SECURE_FILES_CA_FILE_NAME,
                  ROOT_CA_CERT_FILE, strlen(
                      ROOT_CA_CERT_FILE));
#endif
    /* Set the following to pass Server Authentication */
    sl_SetSockOpt(sock,SL_SOL_SOCKET,SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME,
                  PRIVATE_KEY_FILE, strlen(
                      PRIVATE_KEY_FILE));
    sl_SetSockOpt(sock,SL_SOL_SOCKET,SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME,
                  TRUSTED_CERT_CHAIN, strlen(
                      TRUSTED_CERT_CHAIN));
#endif

    /* Bind socket to server's port */
    status = sl_Bind(sock, sa, addrSize);
    if(status < 0)
    {
        UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                   SL_SOCKET_ERROR);
        sl_Close(sock);
        return(-1);
    }

   /* 'Listen' signify that wer'e ready to receive connection's from clients */
    status = sl_Listen(sock, 0);
    if(status < 0)
    {
        UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                   SL_SOCKET_ERROR);
        sl_Close(sock);
        return(-1);
    }

    /* Set socket as non-blocking socket (if needed):
     * Non-blocking sockets allows user to handle other tasks rather than block
     * on socket API calls. 
	 * If an API call using the Non-blocking socket descriptor
     * returns 'SL_ERROR_BSD_EAGAIN' - 
	 * this indicate that the user should try the API again later.
     */
    if(TRUE == nb)
    {
        nonBlocking = TRUE;
        status =
            sl_SetSockOpt(sock, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking,
                          sizeof(nonBlocking));
        if(status < 0)
        {
            UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                       SL_SOCKET_ERROR);
            return(-1);
        }
        newsock = SL_ERROR_BSD_EAGAIN;
    }

    while(newsock < 0)
    {
        /* This call accepts incoming client's connection requests.
         * Accept returns a new socket descriptor, which is dedicated for
         * the accepted client's session. Accept takes the 'welcome' socket
         * descriptor as socket descriptor.
         */
        newsock = sl_Accept(sock, csa, (SlSocklen_t*)&addrSize);

        if((newsock == SL_ERROR_BSD_EAGAIN) && (TRUE == nb))
        {
            sleep(1);
        }
        else if(newsock < 0)
        {
            UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                       SL_SOCKET_ERROR);
            sl_Close(newsock);
            sl_Close(sock);
            return(-1);
        }
    }

    UART_PRINT("Connected to client: ");

    if(ipv6)
    {
        PrintIPAddress(ipv6,(void*)&sAddr.in6.sin6_addr);
    }
    else
    {
        sAddr.in4.sin_addr.s_addr = sl_Htonl(sAddr.in4.sin_addr.s_addr);
        PrintIPAddress(ipv6,(void*)&sAddr.in4.sin_addr);
    }

    UART_PRINT(lineBreak);

    i = 0;

    if(tx)
    {
        int32_t buflen;
        uint32_t sent_bytes = 0;
        uint32_t bytes_to_send = (numberOfPackets * BUF_LEN);

        while(sent_bytes < bytes_to_send)
        {
            if(bytes_to_send - sent_bytes >= BUF_LEN)
            {
                buflen = BUF_LEN;
            }
            else
            {
                buflen = bytes_to_send - sent_bytes;
            }

            /* Send packets to server */
            status = sl_Send(newsock, &app_CB.gDataBuffer.nwData, buflen, 0);
            if((status == SL_ERROR_BSD_EAGAIN) && (TRUE == nb))
            {
                sleep(1);
                continue;
            }
            else if(status < 0)
            {
                UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                           SL_SOCKET_ERROR);
                sl_Close(newsock);
                sl_Close(sock);
                return(-1);
            }
            i++;
            sent_bytes += status;
        }

        UART_PRINT("Sent %u packets (%u bytes) successfully\n\r",
                   i,
                   sent_bytes);
    }
    else
    {
        uint32_t rcvd_bytes = 0;

        while(rcvd_bytes < (numberOfPackets * BUF_LEN))
        {
            status = sl_Recv(newsock, &app_CB.gDataBuffer.nwData, MAX_BUF_SIZE,
                             0);
            if((status == SL_ERROR_BSD_EAGAIN) && (TRUE == nb))
            {
                sleep(1);
                continue;
            }
            else if(status < 0)
            {
                UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                           BSD_SOCKET_ERROR);
                sl_Close(sock);
                return(-1);
            }
            else if(status == 0)
            {
                UART_PRINT("TCP Client closed the connection \n\r");
                break;
            }
            rcvd_bytes += status;
        }

        UART_PRINT("Received %u packets (%u bytes) successfully\n\r",
                   (rcvd_bytes / BUF_LEN), rcvd_bytes);
    }

    /* Calling 'close' with the both socket descriptors,
     * once operation is finished.
     */
    status = sl_Close(newsock);
    ASSERT_ON_ERROR(status, SL_SOCKET_ERROR);

    status = sl_Close(sock);
    ASSERT_ON_ERROR(status, SL_SOCKET_ERROR);

    return(0);
}

/*!
    \brief          TCP Client.

    This routine shows how to set up a simple TCP client.
    It shows sending and receiving packets as well.

    \param          nb              -   Sets the socket type: blocking or
										non-blocking.

    \param          portNumber      -   Decides which port is affiliated
										with the server's socket.

    \param          ipv6            -   Sets the version of the L3 IP
										protocol, IPv4 or IPv6.

    \param          numberOfPackets -   Sets the Number of packets
										to send \ receive.

    \param          tx              -   Decides if the function would
										transmit data. If this flag
                                        is set to false, 
										this function would receive.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             cmdSendCallback, cmdRecvCallback

 */
int32_t TCPClient(uint8_t nb,
                  uint16_t portNumber,
                  ip_t ipAddress,
                  uint8_t ipv6,
                  uint32_t numberOfPackets,
                  uint8_t tx)
{
    int32_t sock;
    int32_t status;
    uint32_t i = 0;
    int32_t nonBlocking;
    SlSockAddr_t        *sa;
    int32_t addrSize;
    sockAddr_t sAddr;

    /* clear the global data buffer */
    memset(&app_CB.gDataBuffer.nwData, 0x0, sizeof(app_CB.gDataBuffer));

    /* filling the buffer with data */
    for(i = 0; i < MAX_BUF_SIZE; i++)
    {
        app_CB.gDataBuffer.nwData[i] = (char)(i % 10);
    }

    if(ipv6)
    {
        sAddr.in6.sin6_family = SL_AF_INET6;
        sAddr.in6.sin6_port = sl_Htons(portNumber);
        sAddr.in6.sin6_flowinfo = 0;

        sAddr.in6.sin6_addr._S6_un._S6_u32[0] =
            ((unsigned long*)ipAddress.ipv6)[0];
        sAddr.in6.sin6_addr._S6_un._S6_u32[1] =
            ((unsigned long*)ipAddress.ipv6)[1];
        sAddr.in6.sin6_addr._S6_un._S6_u32[2] =
            ((unsigned long*)ipAddress.ipv6)[2];
        sAddr.in6.sin6_addr._S6_un._S6_u32[3] =
            ((unsigned long*)ipAddress.ipv6)[3];
        sa = (SlSockAddr_t*)&sAddr.in6;
        addrSize = sizeof(SlSockAddrIn6_t);
    }
    else
    {
        /* filling the TCP server socket address */
        sAddr.in4.sin_family = SL_AF_INET;

        /* Since this is the client's side, 
		 * we must know beforehand the IP address
         * and the port of the server wer'e trying to connect.
         */
        sAddr.in4.sin_port = sl_Htons((unsigned short)portNumber);
        sAddr.in4.sin_addr.s_addr = sl_Htonl((unsigned int)ipAddress.ipv4);

        sa = (SlSockAddr_t*)&sAddr.in4;
        addrSize = sizeof(SlSockAddrIn6_t);
    }

    /* Get socket descriptor - this would be the
     * socket descriptor for the TCP session.
     */
    sock = sl_Socket(sa->sa_family, SL_SOCK_STREAM, TCP_PROTOCOL_FLAGS);
    ASSERT_ON_ERROR(sock, SL_SOCKET_ERROR);

#ifdef SECURE_SOCKET

    SlDateTime_t dateTime;
    dateTime.tm_day = DEVICE_DATE;
    dateTime.tm_mon = DEVICE_MONTH;
    dateTime.tm_year = DEVICE_YEAR;

    sl_DeviceSet(SL_DEVICE_GENERAL, SL_DEVICE_GENERAL_DATE_TIME,
                 sizeof(SlDateTime_t), (uint8_t *)(&dateTime));

    /* Set the following to enable Server Authentication */
    sl_SetSockOpt(sock,SL_SOL_SOCKET,SL_SO_SECURE_FILES_CA_FILE_NAME,
                  ROOT_CA_CERT_FILE, strlen(
                      ROOT_CA_CERT_FILE));

#ifdef CLIENT_AUTHENTICATION
    /* Set the following to pass Client Authentication */
    sl_SetSockOpt(sock,SL_SOL_SOCKET,SL_SO_SECURE_FILES_PRIVATE_KEY_FILE_NAME,
                  PRIVATE_KEY_FILE, strlen(
                      PRIVATE_KEY_FILE));
    sl_SetSockOpt(sock,SL_SOL_SOCKET,SL_SO_SECURE_FILES_CERTIFICATE_FILE_NAME,
                  TRUSTED_CERT_CHAIN, strlen(
                      TRUSTED_CERT_CHAIN));
#endif
#endif

    /* Set socket as non-blocking socket (if needed):
     * Non-blocking sockets allows user to handle 
	 * other tasks rather than block
     * on socket API calls. 
	 * If an API call using the Non-blocking socket descriptor
     * returns 'SL_ERROR_BSD_EAGAIN' - 
	 * this indicate that the user should try the API again later.
     */
    if(TRUE == nb)
    {
        nonBlocking = TRUE;
        status =
            sl_SetSockOpt(sock, SL_SOL_SOCKET, SL_SO_NONBLOCKING, &nonBlocking,
                          sizeof(nonBlocking));

        if(status < 0)
        {
            UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                       SL_SOCKET_ERROR);
            sl_Close(sock);
            return(-1);
        }
    }

    status = -1;

    while(status < 0)
    {
        /* Calling 'sl_Connect' followed by server's
         * 'sl_Accept' would start session with
         * the TCP server. */
        status = sl_Connect(sock, sa, addrSize);
        if((status == SL_ERROR_BSD_EALREADY)&& (TRUE == nb))
        {
            sleep(1);
            continue;
        }
        else if(status < 0)
        {
            UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                       SL_SOCKET_ERROR);
            sl_Close(sock);
            return(-1);
        }
        break;
    }

    i = 0;

    if(tx)
    {
        int32_t buflen;
        uint32_t sent_bytes = 0;
        uint32_t bytes_to_send = (numberOfPackets * BUF_LEN);

        while(sent_bytes < bytes_to_send)
        {
            if(bytes_to_send - sent_bytes >= BUF_LEN)
            {
                buflen = BUF_LEN;
            }
            else
            {
                buflen = bytes_to_send - sent_bytes;
            }

            /* Send packets to the server */
            status = sl_Send(sock, &app_CB.gDataBuffer.nwData, buflen, 0);
            if((status == SL_ERROR_BSD_EAGAIN) && (TRUE == nb))
            {
                sleep(1);
                continue;
            }
            else if(status < 0)
            {
                UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                           SL_SOCKET_ERROR);
                sl_Close(sock);
                return(-1);
            }
            i++;
            sent_bytes += status;
        }

        UART_PRINT("Sent %u packets (%u bytes) successfully\n\r",
                   i,
                   sent_bytes);
    }
    else
    {
        uint32_t rcvd_bytes = 0;

        while(rcvd_bytes < (numberOfPackets * BUF_LEN))
        {
            status = sl_Recv(sock, &app_CB.gDataBuffer.nwData, MAX_BUF_SIZE, 0);
            if((status == SL_ERROR_BSD_EAGAIN) && (TRUE == nb))
            {
                sleep(1);
                continue;
            }
            else if(status < 0)
            {
                UART_PRINT("[line:%d, error:%d] %s\n\r", __LINE__, status,
                           BSD_SOCKET_ERROR);
                sl_Close(sock);
                return(-1);
            }
            else if(status == 0)
            {
                UART_PRINT("TCP Server closed the connection\n\r");
                break;
            }
            rcvd_bytes += status;
        }

        UART_PRINT("Received %u packets (%u bytes) successfully\n\r",
                   (rcvd_bytes / BUF_LEN), rcvd_bytes);
    }

    /* Calling 'close' with the socket descriptor,
     * once operation is finished. */
    status = sl_Close(sock);
    ASSERT_ON_ERROR(status, SL_SOCKET_ERROR);

    return(0);
}
