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

/*****************************************************************************

 Application Name     - Network terminal

 Application Overview - Network Terminal is a command line interface
 (cli) based application,
 used to demonstrate the CC32xx/CC31xx networking capabilities.
 It does that by offering a list of commands, divided into four silos:

    Wlan        : Contains link layer functions like scan, connect, etc.
    NetApp      : Demonstrates the usage of networking applications.
    Socket      : Shows variety of socket API and responsible for sending
                  and receiving packets.
    Transceiver : Gives the user a direct interface to the NWP radio
                  for RF tests, raw sockets (L1) and more.

 Application Details  - Refer to 'Network Terminal' README.html

*****************************************************************************/

/* Standard includes */
#include <stdlib.h>
#include <stdint.h>

/* TI-DRIVERS Header files */
#include <ti/drivers/net/wifi/simplelink.h>

/* Example/Board Header files */
#include "network_terminal.h"
#include "cmd_parser.h"
#include "wlan_cmd.h"
#include "netapp_cmd.h"
#include "socket_cmd.h"
#include "transceiver_cmd.h"

/* Application defines */
#define SIX_BYTES_SIZE_MAC_ADDRESS  (17)

/****************************************************************************
                      LOCAL FUNCTION PROTOTYPES
****************************************************************************/
int32_t showAvailableCmd();
int32_t cmd_prompt(void *arg);
int32_t cmdClearcallback(void *arg);
int32_t printClearUsage(void *arg);
int32_t cmdHelpCallback(void *arg);
int32_t printHelpUsage(void *arg);
int32_t initAppVariables();
int32_t sem_wait_timeout(sem_t *sem, uint32_t Timeout);

/****************************************************************************
                      GLOBAL VARIABLES
****************************************************************************/

/*    Command List :
 *
 *    Upon calling 'cmd_prompt()', for every command line the user enters,
 *    This Table gets checked for the appropriate command column,
 *    If command was found, cmd_prompt would dispatch the command callback.
 */

cmdAction_t gCmdList[] =
{
/* command */         /* Command callback */   /* Print Usage */

/* Show help          */ 
{ helpStr,              cmdHelpCallback,         printHelpUsage             },
/* Scan               */ 
{ scanStr,             cmdScanCallback,          printScanUsage             },
/* Set Scan Policy    */ 
{ setPolicyStr,        cmdSetPolicyCallback,     printSetPolicyUsage        },
/* Connect            */ 
{ wlanConnectStr,      cmdWlanConnectCallback,   printWlanConnectUsage      },
/* Ap start           */  
{ ap_start_str,        cmdWlanStartApCallback,   printWlanStartApUsage      },
/* Ap stop            */ 
{ ap_stop_str,         cmdWlanStopApCallback,    printWlanStopApUsage       },
/* Disconnect         */
{ wlanDisconnectStr,   cmdWlanDisconnectCallback, printWlanDisconnectUsage  },
/* Connected Stations */ 
{ConnectedStationsStr,  cmdConnectedStationsCallback,
                                                 printConnectedStationsUsage},
/* Ping               */ 
{ pingStr,              cmdPingCallback,         printPingUsage             },
/* Send               */ 
{ sendStr,              cmdSendCallback,         printSendUsage             },
/* Recv               */ 
{ recvStr,              cmdRecvCallback,         printRecvUsage             },
/* Create Filter      */ 
{ createFilterStr,      cmdCreateFilterCallback, printCreateFilterUsage     },
/* Enable Filter      */
{ enableFilterStr,      cmdEnableFilterCallback, printEnableFilterUsage     },
/* Disable Filter     */
{ disableFilterStr,     cmdDisableFilterCallback,printDisableFilterUsage    },
/* Delete Filter      */
{ deleteFilterStr,     cmdDeleteFilterCallback, printDeleteFilterUsage      },
/* WoWlan Enable      */ 
{ enableWoWLANStr,      cmdEnableWoWLANCallback, printEnableWoWLANUsage     },
/* mDNS Advertise     */
{ mDNSAdvertiseStr,    mDNSAdvertiseCallback,   printmDNSAdvertiseUsage     },
/* mDNS Query         */
{ mDNSQueryStr,        mDNSQueryCallback,       printmDNSQueryUsage         },
/* Transceiver mode   */
{ radiotool_Str,       cmdTranceiverModecallback, printTranceiverModeUsage  },
/* P2P start          */
{ p2pStartcmdStr,       cmdP2PModecallback,     printP2PStartUsage          },
/* P2P stop             */  
{ p2pStopcmdStr,             cmdP2PStopcallback, printP2PStopUsage          },
/* Set Country Code     */  
{ SetCountryCodeStr,         cmdCountrycodecallback, printCountrycodeeUsage },
/* Clear term           */  
{ clearStr,                  cmdClearcallback,        printClearUsage       }
};

uint32_t            gMaxCmd = (sizeof(gCmdList)/sizeof(cmdAction_t));
pthread_t           gSpawn_thread = (pthread_t)NULL;
appControlBlock     app_CB;

/*****************************************************************************
                  Callback Functions
*****************************************************************************/

/*!
    \brief          SimpleLinkWlanEventHandler

    This handler gets called whenever a WLAN event is reported
    by the host driver / NWP. Here user can implement he's own logic
    for any of these events. This handler is used by 'network_terminal'
    application to show case the following scenarios:

    1. Handling connection / Disconnection.
    2. Handling Addition of station / removal.
    3. RX filter match handler.
    4. P2P connection establishment.

    \param         pWlanEvent       -   pointer to Wlan event data.

    \return        void

    \note          For more information, please refer to: user.h in the porting
                   folder of the host driver and the 
                   CC31xx/CC32xx NWP programmer's guide (SWRU455)

    \sa            cmdWlanConnectCallback, cmdEnableFilterCallback,
                   cmdWlanDisconnectCallback, cmdP2PModecallback.

*/
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    if(!pWlanEvent)
    {
        return;
    }

    switch(pWlanEvent->Id)
    {
        case SL_WLAN_EVENT_CONNECT:
        {
            SET_STATUS_BIT(app_CB.Status, STATUS_BIT_CONNECTION);

        /* Copy new connection SSID and BSSID to global parameters */
        memcpy(app_CB.CON_CB.ConnectionSSID, pWlanEvent->Data.Connect.SsidName,
               pWlanEvent->Data.Connect.SsidLen);
        memcpy(app_CB.CON_CB.ConnectionBSSID, pWlanEvent->Data.Connect.Bssid,
               SL_WLAN_BSSID_LENGTH);

        UART_PRINT(
            "\n\r[WLAN EVENT] STA Connected to the AP: %s , "
            "BSSID: %x:%x:%x:%x:%x:%x\n\r",
            app_CB.CON_CB.ConnectionSSID,
            app_CB.CON_CB.ConnectionBSSID[0],
            app_CB.CON_CB.ConnectionBSSID[1],
            app_CB.CON_CB.ConnectionBSSID[2],
            app_CB.CON_CB.ConnectionBSSID[3],
            app_CB.CON_CB.ConnectionBSSID[4],
            app_CB.CON_CB.ConnectionBSSID[5]);

        sem_post(&app_CB.CON_CB.connectEventSyncObj);
    }
    break;

    case SL_WLAN_EVENT_DISCONNECT:
    {
        SlWlanEventDisconnect_t  *pEventData = NULL;

        CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_CONNECTION);
        CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_IP_ACQUIRED);
        CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_IPV6_ACQUIRED);

        /* If ping operation is running, release it. */
        if(IS_PING_RUNNING(app_CB.Status))
        {
            sem_post(&app_CB.CON_CB.eventCompletedSyncObj);
            UART_PRINT(
                "\n\rPing failed, since device is no longer connected.\n\r");
        }

        pEventData = &pWlanEvent->Data.Disconnect;

        /* If the user has initiated 'Disconnect' request,
           'reason_code' is SL_WLAN_DISCONNECT_USER_INITIATED */
        if(SL_WLAN_DISCONNECT_USER_INITIATED == pEventData->ReasonCode)
        {
            UART_PRINT(
                "\n\r[WLAN EVENT] Device disconnected from the AP: %s,\n\r"
                "BSSID: %x:%x:%x:%x:%x:%x on application's request \n\r",
                app_CB.CON_CB.ConnectionSSID,
                app_CB.CON_CB.ConnectionBSSID[0],
                app_CB.CON_CB.ConnectionBSSID[1],
                app_CB.CON_CB.ConnectionBSSID[2],
                app_CB.CON_CB.ConnectionBSSID[3],
                app_CB.CON_CB.ConnectionBSSID[4],
                app_CB.CON_CB.ConnectionBSSID[5]);
        }
        else
        {
            UART_PRINT(
                "\n\r[WLAN ERROR] Device disconnected from the AP: %s,\n\r"
                "BSSID: %x:%x:%x:%x:%x:%x\n\r",
                app_CB.CON_CB.ConnectionSSID,
                app_CB.CON_CB.ConnectionBSSID[0],
                app_CB.CON_CB.ConnectionBSSID[1],
                app_CB.CON_CB.ConnectionBSSID[2],
                app_CB.CON_CB.ConnectionBSSID[3],
                app_CB.CON_CB.ConnectionBSSID[4],
                app_CB.CON_CB.ConnectionBSSID[5]);
        }
        memset(&(app_CB.CON_CB.ConnectionSSID), 0x0,
               sizeof(app_CB.CON_CB.ConnectionSSID));
        memset(&(app_CB.CON_CB.ConnectionBSSID), 0x0,
               sizeof(app_CB.CON_CB.ConnectionBSSID));
    }
    break;

    case SL_WLAN_EVENT_PROVISIONING_STATUS:
    {
        /* Do nothing, this suppress provisioning event is because
            simplelink is configured to default state. */
    }
    break;

    case SL_WLAN_EVENT_STA_ADDED:
    {
        memcpy(&(app_CB.CON_CB.ConnectionBSSID), pWlanEvent->Data.STAAdded.Mac,
               SL_WLAN_BSSID_LENGTH);
        UART_PRINT(
            "\n\r[WLAN EVENT] STA was added to AP:"
            " BSSID: %x:%x:%x:%x:%x:%x\n\r",
            app_CB.CON_CB.ConnectionBSSID[0],
            app_CB.CON_CB.ConnectionBSSID[1],
            app_CB.CON_CB.ConnectionBSSID[2],
            app_CB.CON_CB.ConnectionBSSID[3],
            app_CB.CON_CB.ConnectionBSSID[4],
            app_CB.CON_CB.ConnectionBSSID[5]);
    }
    break;

    case SL_WLAN_EVENT_STA_REMOVED:
    {
        memcpy(&(app_CB.CON_CB.ConnectionBSSID), pWlanEvent->Data.STAAdded.Mac,
               SL_WLAN_BSSID_LENGTH);
        UART_PRINT(
            "\n\r[WLAN EVENT] STA was removed "
            "from AP: BSSID: %x:%x:%x:%x:%x:%x\n\r",
            app_CB.CON_CB.ConnectionBSSID[0],
            app_CB.CON_CB.ConnectionBSSID[1],
            app_CB.CON_CB.ConnectionBSSID[2],
            app_CB.CON_CB.ConnectionBSSID[3],
            app_CB.CON_CB.ConnectionBSSID[4],
            app_CB.CON_CB.ConnectionBSSID[5]);

        memset(&(app_CB.CON_CB.ConnectionBSSID), 0x0,
               sizeof(app_CB.CON_CB.ConnectionBSSID));
    }
    break;

    case SL_WLAN_EVENT_RXFILTER:
    {
        SlWlanEventRxFilterInfo_t  *triggred_filter = NULL;

        triggred_filter = &(pWlanEvent->Data.RxFilterInfo);

        UART_PRINT(
            "\n\r[WLAN EVENT] Rx filter match triggered. Set filters in"
            "filter bitmap :0x%x.\n\r",
            triggred_filter->UserActionIdBitmap);

        /*
         *  User can write he's / her's rx filter match handler here.
         *  Be advised, you can use the 'triggred_filter'
         *    structure info to determine which filter
         *  has received a match.
         * (Bit X is set if user action id X was
         *  passed to a filter that matched a packet.)
         */
    }
    break;

        case SL_WLAN_EVENT_P2P_DEVFOUND:
        {
            UART_PRINT("\n\r[WLAN EVENT] P2P Remote device found\n\r");
            sem_post(&(app_CB.P2P_CB.DeviceFound));
        }
        break;

        case SL_WLAN_EVENT_P2P_REQUEST:
        {
            UART_PRINT("\n\r[WLAN EVENT] P2P Negotiation request received\n\r");

        /* This information is needed to create connection*/
        memset(&(app_CB.P2P_CB.p2pPeerDeviceName), '\0',
               sizeof(app_CB.P2P_CB.p2pPeerDeviceName));
        memcpy(&app_CB.P2P_CB.p2pPeerDeviceName,
               pWlanEvent->Data.P2PRequest.GoDeviceName,
               pWlanEvent->Data.P2PRequest.GoDeviceNameLen);

        sem_post(&app_CB.P2P_CB.RcvNegReq);
    }
    break;

    case SL_WLAN_EVENT_P2P_CONNECT:
    {
        UART_PRINT(
            "n\r[WLAN EVENT] P2P connection was successfully completed as "
            "CLIENT\n\r");
        UART_PRINT("n\rBSSID is %02x:%02x:%02x:%02x:%02x:%02x\n\r",
                   pWlanEvent->Data.STAAdded.Mac[0],
                   pWlanEvent->Data.STAAdded.Mac[1],
                   pWlanEvent->Data.STAAdded.Mac[2],
                   pWlanEvent->Data.STAAdded.Mac[3],
                   pWlanEvent->Data.STAAdded.Mac[4],
                   pWlanEvent->Data.STAAdded.Mac[5]);

        sem_post(&app_CB.P2P_CB.RcvConReq);
    }
    break;

    case SL_WLAN_EVENT_P2P_CLIENT_ADDED:
    {
        UART_PRINT(
            "n\r[WLAN EVENT] P2P connection was successfully completed "
            "as GO\n\r");
        UART_PRINT("n\rBSSID is %02x:%02x:%02x:%02x:%02x:%02x\n\r",
                   pWlanEvent->Data.P2PClientAdded.Mac[0],
                   pWlanEvent->Data.P2PClientAdded.Mac[1],
                   pWlanEvent->Data.P2PClientAdded.Mac[2],
                   pWlanEvent->Data.P2PClientAdded.Mac[3],
                   pWlanEvent->Data.P2PClientAdded.Mac[4],
                   pWlanEvent->Data.P2PClientAdded.Mac[5]);

            sem_post(&app_CB.P2P_CB.RcvConReq);
        }
        break;

    case SL_WLAN_EVENT_P2P_DISCONNECT:
    {
        UART_PRINT("\n\r[WLAN EVENT] STA disconnected from device.\n\r");
        CLR_STATUS_BIT(app_CB.Status,STATUS_BIT_CONNECTION);
    }
    break;

    default:
    {
        UART_PRINT("\n\r[WLAN EVENT] Unexpected event [0x%x]\n\r",
                   pWlanEvent->Id);
    }
    break;
    }
    UART_PRINT(cmdPromptStr);
}

/*!
    \brief          SimpleLinkNetAppEventHandler

    This handler gets called whenever a Netapp event is reported
    by the host driver / NWP. Here user can implement he's own logic
    for any of these events. This handler is used by 'network_terminal'
    application to show case the following scenarios:

    1. Handling IPv4 / IPv6 IP address acquisition.
    2. Handling IPv4 / IPv6 IP address Dropping.

    \param          pNetAppEvent     -   pointer to Netapp event data.

    \return         void

    \note           For more information, please refer to:
                    user.h in the porting
                    folder of the host driver and the 
                    CC31xx/CC32xx NWP programmer's
                    guide (SWRU455).

*/
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    if(!pNetAppEvent)
    {
        return;
    }

    switch(pNetAppEvent->Id)
    {
        case SL_NETAPP_EVENT_IPV4_ACQUIRED:
        {
            SlIpV4AcquiredAsync_t *pEventData = NULL;

            SET_STATUS_BIT(app_CB.Status, STATUS_BIT_IP_ACQUIRED);

            /* Ip Acquired Event Data */
            pEventData = &pNetAppEvent->Data.IpAcquiredV4;
            app_CB.CON_CB.IpAddr = pEventData->Ip ;

            /* Gateway IP address */
            app_CB.CON_CB.GatewayIP = pEventData->Gateway;

            UART_PRINT("\n\r[NETAPP EVENT] IP set to: IPv4=%d.%d.%d.%d , "
                    "Gateway=%d.%d.%d.%d\n\r",

                    SL_IPV4_BYTE(app_CB.CON_CB.IpAddr,3),
                    SL_IPV4_BYTE(app_CB.CON_CB.IpAddr,2),
                    SL_IPV4_BYTE(app_CB.CON_CB.IpAddr,1),
                    SL_IPV4_BYTE(app_CB.CON_CB.IpAddr,0),

                    SL_IPV4_BYTE(app_CB.CON_CB.GatewayIP,3),
                    SL_IPV4_BYTE(app_CB.CON_CB.GatewayIP,2),
                    SL_IPV4_BYTE(app_CB.CON_CB.GatewayIP,1),
                    SL_IPV4_BYTE(app_CB.CON_CB.GatewayIP,0));

            sem_post(&(app_CB.CON_CB.ip4acquireEventSyncObj));
        }
        break;

        case SL_NETAPP_EVENT_IPV6_ACQUIRED:
        {
            uint32_t i = 0;

            SET_STATUS_BIT(app_CB.Status, STATUS_BIT_IPV6_ACQUIRED);

            for(i = 0 ; i < 4 ; i++)
            {
                app_CB.CON_CB.Ipv6Addr[i] = 
                pNetAppEvent->Data.IpAcquiredV6.Ip[i];
            }

            UART_PRINT("\n\r[NETAPP EVENT] IP Acquired: IPv6=");

        for(i = 0; i < 3; i++)
        {
            UART_PRINT("%04x:%04x:",
                       ((app_CB.CON_CB.Ipv6Addr[i] >> 16) & 0xffff),
                       app_CB.CON_CB.Ipv6Addr[i] & 0xffff);
        }

        UART_PRINT("%04x:%04x", ((app_CB.CON_CB.Ipv6Addr[3] >> 16) & 0xffff),
                   app_CB.CON_CB.Ipv6Addr[3] & 0xffff);
        UART_PRINT(lineBreak);
        sem_post(&app_CB.CON_CB.ip6acquireEventSyncObj);
    }
    break;

        case SL_NETAPP_EVENT_DHCPV4_LEASED:
        {
            SET_STATUS_BIT(app_CB.Status, STATUS_BIT_IP_LEASED);
            SET_STATUS_BIT(app_CB.Status, STATUS_BIT_IP_ACQUIRED);

        app_CB.CON_CB.StaIp = pNetAppEvent->Data.IpLeased.IpAddress;
        UART_PRINT(
            "\n\r[NETAPP EVENT] IP Leased to Client: IP=%d.%d.%d.%d \n\r",
            SL_IPV4_BYTE(app_CB.CON_CB.StaIp,
                         3), SL_IPV4_BYTE(app_CB.CON_CB.StaIp,2),
            SL_IPV4_BYTE(app_CB.CON_CB.StaIp,
                         1), SL_IPV4_BYTE(app_CB.CON_CB.StaIp,0));

        sem_post(&(app_CB.CON_CB.ip4acquireEventSyncObj));
    }
    break;

    case SL_NETAPP_EVENT_DHCPV4_RELEASED:
    {
        UART_PRINT("\n\r[NETAPP EVENT] IP is released.\n\r");
    }
    break;

    default:
    {
        UART_PRINT("\n\r[NETAPP EVENT] Unexpected event [0x%x] \n\r",
                   pNetAppEvent->Id);
    }
    break;
    }
    UART_PRINT(cmdPromptStr);
}

/*!
    \brief          SimpleLinkHttpServerEventHandler

    This handler gets called whenever a HTTP event is reported
    by the NWP internal HTTP server.

    \param          pHttpEvent       -   pointer to http event data.

    \param          pHttpEvent       -   pointer to http response.

    \return         void

    \note           For more information, please refer to: user.h in the porting
                    folder of the host driver and the  CC31xx/CC32xx NWP 
                    programmer's guide (SWRU455).

 */
void SimpleLinkHttpServerEventHandler(
    SlNetAppHttpServerEvent_t *pHttpEvent,
    SlNetAppHttpServerResponse_t *
    pHttpResponse)
{
    /* Unused in this application */
}

/*!
    \brief          SimpleLinkGeneralEventHandler

    This handler gets called whenever a general error is reported
    by the NWP / Host driver. Since these errors are not fatal,
    application can handle them.

    \param          pDevEvent    -   pointer to device error event.

    \return         void

    \note           For more information, please refer to:
                    user.h in the porting
                    folder of the host driver and the 
                    CC31xx/CC32xx NWP programmer's
                    guide (SWRU455).

*/
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    if(!pDevEvent)
    {
        return;
    }
    /*
      Most of the general errors are not FATAL are are to be handled
      appropriately by the application
    */
    UART_PRINT("\n\r[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
               pDevEvent->Data.Error.Code,
               pDevEvent->Data.Error.Source);
}

/*!
    \brief          SimpleLinkSockEventHandler

    This handler gets called whenever a socket event is reported
    by the NWP / Host driver.

    \param          SlSockEvent_t    -   pointer to socket event data.

    \return         void

    \note           For more information, please refer to:
                    user.h in the porting
                    folder of the host driver and the 
                    CC31xx/CC32xx NWP programmer's
                    guide (SWRU455).

*/
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    /* Unused in this application */
}

/*!
    \brief          SimpleLinkFatalErrorEventHandler

    This handler gets called whenever a socket event is reported
    by the NWP / Host driver. After this routine is called, the user's
    application must restart the device in order to recover.

    \param          slFatalErrorEvent    -   pointer to fatal error event.

    \return         void

    \note           For more information, please refer to:
                    user.h in the porting
                    folder of the host driver and the
                    CC31xx/CC32xx NWP programmer's
                    guide (SWRU455).

*/
void SimpleLinkFatalErrorEventHandler(SlDeviceFatal_t *slFatalErrorEvent)
{

    switch (slFatalErrorEvent->Id)
    {
        case SL_DEVICE_EVENT_FATAL_DEVICE_ABORT:
        {
            UART_PRINT("\n\r[ERROR] - FATAL ERROR: Abort NWP event detected: "
                        "AbortType=%d, AbortData=0x%x\n\r",
                        slFatalErrorEvent->Data.DeviceAssert.Code,
                        slFatalErrorEvent->Data.DeviceAssert.Value);
        }
        break;

        case SL_DEVICE_EVENT_FATAL_DRIVER_ABORT:
        {
            UART_PRINT("\n\r[ERROR] - FATAL ERROR: Driver Abort detected. \n\r");
        }
        break;

        case SL_DEVICE_EVENT_FATAL_NO_CMD_ACK:
        {
            UART_PRINT("\n\r[ERROR] - FATAL ERROR: No Cmd Ack detected "
                        "[cmd opcode = 0x%x] \n\r",
                                        slFatalErrorEvent->Data.NoCmdAck.Code);
        }
        break;

        case SL_DEVICE_EVENT_FATAL_SYNC_LOSS:
        {
            UART_PRINT("\n\r[ERROR] - FATAL ERROR: Sync loss detected n\r");
        }
        break;

        case SL_DEVICE_EVENT_FATAL_CMD_TIMEOUT:
        {
            UART_PRINT("\n\r[ERROR] - FATAL ERROR: Async event timeout detected "
                        "[event opcode =0x%x]  \n\r",
                                    slFatalErrorEvent->Data.CmdTimeout.Code);
        }
        break;

    default:
        UART_PRINT("\n\r[ERROR] - FATAL ERROR:"
        " Unspecified error detected \n\r");
        break;
    }
}

/*!
    \brief          SimpleLinkNetAppRequestEventHandler

    This handler gets called whenever a NetApp event is reported
    by the NWP / Host driver. User can write he's logic to handle
    the event here.

    \param          pNetAppRequest     -   Pointer to NetApp request structure.

    \param          pNetAppResponse    -   Pointer to NetApp request Response.

    \note           For more information, please refer to: 
                    user.h in the porting
                    folder of the host driver and the
                    CC31xx/CC32xx NWP programmer's
                    guide (SWRU455).

    \return         void

 */
void SimpleLinkNetAppRequestEventHandler(SlNetAppRequest_t *pNetAppRequest,
                                         SlNetAppResponse_t *pNetAppResponse)
{
    /* Unused in this application */
}

/*!
    \brief          SimpleLinkNetAppRequestMemFreeEventHandler

    This handler gets called whenever the NWP is done handling with
    the buffer used in a NetApp request. This allows the use of
    dynamic memory with these requests.

    \param          pNetAppRequest     -   Pointer to NetApp request structure.

    \param          pNetAppResponse    -   Pointer to NetApp request Response.

    \note           For more information, please refer to: 
                    user.h in the porting
                    folder of the host driver and the
                    CC31xx/CC32xx NWP programmer's
                    guide (SWRU455).

    \return         void

*/
void SimpleLinkNetAppRequestMemFreeEventHandler(uint8_t *buffer)
{
    /* Unused in this application */
}


/*****************************************************************************
                  Local Functions
*****************************************************************************/

/*!
    \brief          Command Prompt.

    This routine reads characters from UART interface,
    and serves as command line for application's user input.
    it reads a string representing the command followed by parameters.
    First - it searches for the command in the global command table
    (gCmdList) and if found, dispatches the handler.
    If no command is found, appropriate error would be printed to screen.
    This function isn't expected to return.

    \param          arg       -   Points to command line buffer.

    \return

    \sa             GetCmd

*/
int32_t cmd_prompt(void *arg)
{
    int32_t     lRetVal;
    uint32_t    i = 0;
    char        cmdBuffer[(MAX_CMD_NAME_LEN+5)];
    char        *token = NULL;

    while(!app_CB.Exit)
    {
        UART_PRINT(cmdPromptStr);

        /* Poll UART terminal to receive user command terminated by '/r' */
        lRetVal = GetCmd((char *)app_CB.CmdBuffer, CMD_BUFFER_LEN);
        if(0 == lRetVal)
        {
            UART_PRINT(lineBreak);
            continue;
        }
        else
        {
            memcpy(cmdBuffer, &app_CB.CmdBuffer, (MAX_CMD_NAME_LEN+4));
            cmdBuffer[MAX_CMD_NAME_LEN+4] = '\0';
            token = strtok(cmdBuffer, " ");
            if(token)
            {
                for(i = 0; i < gMaxCmd; i++)
                {
                    /* Search for typed command in 
                       the application command list. */
                    if(!strcmp((char*)token, gCmdList[i].cmd))
                    {
                        /* Dispatch command callback */
                        lRetVal =
                            gCmdList[i].callback((void *)(app_CB.CmdBuffer +
                                                          strlen(token)));
                        UART_PRINT(lineBreak);
                        break;
                    }
                }
                if(i >= gMaxCmd)
                {
                    UART_PRINT(lineBreak);
                    UART_PRINT("No such command\n\r");
                }
            }
        }
    }
    return(0);
}

/*!
    \brief          Show help callback.

    This routine searches for the command in the global command table
    (gCmdList) and if found, dispatches the appropriate callback, to
    display it's menu options. If no command is found, appropriate error
    would be printed to screen and list of available commands.

    \param          arg       -   Points to command name.

    \return         This function shall return 0.

    \sa             GetCmd

*/
int32_t cmdHelpCallback(void *arg)
{
    uint32_t    i = 0;
    char        *param[2] = {NULL, NULL};
    char        *token = NULL;
    char        cmdStr[CMD_BUFFER_LEN+1];

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    /* Get command options/params */
    token = strtok(cmdStr, space_str);
    while(token && (i < 2))
    {
        param[i] = token;
        token = strtok(NULL, space_str);
        i++;
    }

    /* Lookup for help display callback for command */
    if(param[0])
    {
        for(i = 0; i < gMaxCmd; i++)
        {
            if(!strcmp(param[0], gCmdList[i].cmd))
            {
                gCmdList[i].printusagecallback((void *) arg);
                break;
            }
        }
        if(i >= gMaxCmd)
        {
            UART_PRINT("Help doesn't exist for command %s\n\r", param[0]);
        }
    }
    else
    {
        printHelpUsage(arg);
        showAvailableCmd();
    }

    return(0);
}

/*!
    \brief          Prints 'help' command help menu.

    \param          arg       -   Points to command line buffer.

    \return         This function shall return 0.

    \sa             cmdHelpCallback

*/
int32_t printHelpUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(helpStr);
    UART_PRINT(helpUsageStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(helpDetailsStr);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          clear command

    This routine clears the command line console screen.

    \param          arg       -   Points to command line buffer.

    \return         This function shall return 0.

    \sa             printClearUsage

 */
int32_t cmdClearcallback(void *arg)
{
    ClearTerm();
    return(0);
}

/*!
    \brief          Prints 'clear' command help menu.

    \param          arg       -   Points to command line buffer.

    \return         This function shall return 0.

    \sa             cmdClearcallback

 */
int32_t printClearUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(clearStr);
    UART_PRINT(descriptionStr);
    UART_PRINT(clearDetailsStr);
    UART_PRINT(lineBreak);
    return(0);
}

/*!
    \brief          Configure SimpleLink to default state.

    This routine configures the device to a default state.
    It's important to note that this is one example for a 
    'restore to default state'
    function, which meet the needs of this application, 
    'Network Terminal'. User who
    wish to incorporate this function into he's app,
    must adjust the implementation
    and make sure it meets he's needs.

    \return         Upon successful completion,
                    the function shall return 0.
                    In case of failure, this function would return -1.

*/
int32_t ConfigureSimpleLinkToDefaultState()
{
     uint8_t                              ucConfigOpt;
     uint8_t                              ucPower;
     int32_t                              RetVal = -1;
     int32_t                              Mode = -1;
     uint32_t                             IfBitmap = 0;
     SlWlanScanParamCommand_t             ScanDefault = {0};
     SlWlanRxFilterOperationCommandBuff_t RxFilterIdMask = {{0}};

     /* Turn NWP on */
     Mode = sl_Start(0, 0, 0);
     ASSERT_ON_ERROR(Mode, DEVICE_ERROR);

    if(Mode != ROLE_STA)
    {
        /* Set NWP role as STA */
        Mode = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(Mode, WLAN_ERROR);

         /* For changes to take affect, we restart the NWP */
         RetVal = sl_Stop(SL_STOP_TIMEOUT);
         ASSERT_ON_ERROR(RetVal, DEVICE_ERROR);

         Mode = sl_Start(0, 0, 0);
         ASSERT_ON_ERROR(Mode, DEVICE_ERROR);
     }

    if(Mode != ROLE_STA)
    {
        UART_PRINT("Failed to configure device to it's default state");
        return(-1);
    }

    /* Set policy to auto only */
    RetVal =
        sl_WlanPolicySet(SL_WLAN_POLICY_CONNECTION,
                         SL_WLAN_CONNECTION_POLICY(1,0,0,
                                                   0),
                         NULL,0);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Disable Auto Provisioning */
    RetVal = sl_WlanProvisioning(SL_WLAN_PROVISIONING_CMD_STOP,
                                 0xFF,
                                 0,
                                 NULL,
                                 0x0);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

     /* Delete existing profiles */

     /* RetVal = sl_WlanProfileDel(0xFF);
        ASSERT_ON_ERROR(RetVal, WLAN_ERROR); */


     /* enable DHCP client */
     RetVal = sl_NetCfgSet(SL_NETCFG_IPV4_STA_ADDR_MODE, 
     SL_NETCFG_ADDR_DHCP, 0, 0);
     ASSERT_ON_ERROR(RetVal, NETAPP_ERROR);

     /* Disable ipv6 */
     IfBitmap = !(SL_NETCFG_IF_IPV6_STA_LOCAL | SL_NETCFG_IF_IPV6_STA_GLOBAL);
     RetVal = sl_NetCfgSet(SL_NETCFG_IF, SL_NETCFG_IF_STATE, sizeof(IfBitmap),
                          (const unsigned char *)&IfBitmap);
     ASSERT_ON_ERROR(RetVal, NETAPP_ERROR);

     /* Configure scan parameters to default */
     ScanDefault.ChannelsMask = CHANNEL_MASK_ALL;
     ScanDefault.RssiThreshold = RSSI_TH_MAX;

    RetVal =
        sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
                   SL_WLAN_GENERAL_PARAM_OPT_SCAN_PARAMS,
                   sizeof(ScanDefault),
                   (uint8_t *)&ScanDefault);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Disable scans */
    ucConfigOpt = SL_WLAN_SCAN_POLICY(0, 0);
    RetVal = sl_WlanPolicySet(SL_WLAN_POLICY_SCAN, ucConfigOpt, NULL, 0);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Set TX power lvl to max */
    ucPower = 0;
    RetVal =
        sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
                   SL_WLAN_GENERAL_PARAM_OPT_STA_TX_POWER,
                   1,
                   (uint8_t *)&ucPower);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Set NWP Power policy to 'normal' */
    RetVal =
         sl_WlanPolicySet(SL_WLAN_POLICY_PM, SL_WLAN_NORMAL_POLICY, NULL,0);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Unregister mDNS services */
    RetVal = sl_NetAppMDNSUnRegisterService(0, 0, 0);
    ASSERT_ON_ERROR(RetVal, NETAPP_ERROR);

    /* Remove all 64 RX filters (8*8) */
    memset(RxFilterIdMask.FilterBitmap, 0xFF, 8);

    RetVal =
        sl_WlanSet(SL_WLAN_RX_FILTERS_ID, SL_WLAN_RX_FILTER_REMOVE,
                   sizeof(SlWlanRxFilterOperationCommandBuff_t),
                   (uint8_t *)&RxFilterIdMask);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* Set NWP role as STA */
    RetVal = sl_WlanSetMode(ROLE_STA);
    ASSERT_ON_ERROR(RetVal, WLAN_ERROR);

    /* For changes to take affect, we restart the NWP */
    RetVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(RetVal, DEVICE_ERROR);

    Mode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(Mode, DEVICE_ERROR);

    if(ROLE_STA != Mode)
    {
        UART_PRINT("Failed to configure device to it's default state");
        return(-1);
    }
    else
    {
        app_CB.Role = ROLE_STA;
        SET_STATUS_BIT(app_CB.Status, STATUS_BIT_NWP_INIT);
    }

    return(0);
}

/*!
    \brief          Prints IP address.

    This routine prints IP addresses in a dotted decimal
    notation (IPv4) or colon : notation (IPv6)

    \param          ip         -   Points to command line buffer.

    \param          ipv6       -   Flag that sets 
                                   if the address is IPv4 or IPv6.

    \return         void

 */
void PrintIPAddress(unsigned char ipv6,
                    void *ip)
{
    uint32_t        *pIPv4;
    uint8_t         *pIPv6;
    int32_t          i=0;

    if(!ip)
    {
        return;
    }

    if(ipv6)
    {
        pIPv6 = (uint8_t*) ip;

        for(i = 0; i < 14; i+=2)
        {
            UART_PRINT("%02x%02x:", pIPv6[i], pIPv6[i+1]);
        }

        UART_PRINT("%02x%02x", pIPv6[i], pIPv6[i+1]);
    }
    else
    {
        pIPv4 = (uint32_t*)ip;
        UART_PRINT("%d.%d.%d.%d", 
                    SL_IPV4_BYTE(*pIPv4,3), 
                    SL_IPV4_BYTE(*pIPv4,2),
                    SL_IPV4_BYTE(*pIPv4,1),
                    SL_IPV4_BYTE(*pIPv4,0));
    }
    return;
}

/*!
    \brief          ipv6 Address Parse

    This routine converts string representing IPv6 address
    in a colon notation, into an array of bytes,
    representing each IP address octate.

    \param          str         -   Points to string address.

    \param          ipv6ip      -   Points to a 16 byte long array.

    \return         Upon successful completion,
                    the function shall return 0.
                    In case of failure, this function would return -1.

 */
int32_t ipv6AddressParse(char *str,
                         uint8_t *ipv6ip)
{
    int32_t         i;
    int32_t         l;
    int32_t         zeroCompressPos;
    uint8_t        *t;
    uint8_t         tmp[16];
    uint16_t        value;
    uint8_t         hexDigit;

    i = 0;
    t = (uint8_t*)str;
    value = 0;
    hexDigit=0;
    zeroCompressPos=-1;
    memset(tmp, 0, sizeof(tmp));

    if(*t==':')
    {
        if(*++t!=':')
        {
            return(-1);
        }
    }

    while(*t && (i < 16))
    {
        if(*t >= '0' && *t <= '9')
        {
            value = (value << 4) | (*t - '0');
            hexDigit = 1;
        }
        else if(*t >= 'a' && *t <= 'f')
        {
            value = (value << 4) | ((*t - 'a') + 10);
            hexDigit = 1;
        }
        else if(*t >= 'A' && *t <= 'F')
        {
            value = (value << 4) | ((*t - 'A') + 10);
            hexDigit = 1;
        }
        else if((*t == ':') && (i < 14))
        {
            if(hexDigit)
            {
                tmp[i++] = (value >> 8) & 0xFF;
                tmp[i++] = (value) & 0xFF;
                hexDigit = 0;
                value = 0;
            }
            else
            {
                if(zeroCompressPos < 0)
                {
                    zeroCompressPos = i;
                }
                else
                {
                    return(-1);
                }
            }
        }
        t++;
    }

    if(i > 15)
    {
        return(-1);
    }
    else if(hexDigit && (zeroCompressPos < 0) && (i < 14))
    {
        return(-1);
    }
    else if((!hexDigit) && (zeroCompressPos < 0))
    {
        return(-1);
    }
    else if((!hexDigit) && (zeroCompressPos != i))
    {
        return(-1);
    }
    else if((zeroCompressPos >= 0) && i >= 14)
    {
        return(-1);
    }

    if((hexDigit) && (i < 15))
    {
        tmp[i++] = (value >> 8) & 0xFF;
        tmp[i++] = (value) & 0xFF;
        hexDigit = 0;
        value = 0;
    }

    if(zeroCompressPos>=0)
    {
        i--;
        l = 15;
        while(i >= zeroCompressPos)
        {
            if((l >= 0) && (i >= 0))
            {
                tmp[l] = tmp[i];
                tmp[i] = 0;
                l--;
                i--;
            }
        }
    }

    memcpy(ipv6ip, tmp, sizeof(tmp));

    return(0);
}

/*!
    \brief          ipv4 Address Parse

    This routine converts string representing IPv4 address
    in a dotted decimal notation, into an array of bytes,
    representing each IP address octate.

    \param          str         -   Points to string address.

    \param          ipv4ip      -   Points to a 4 byte long array.

    \return         Upon successful completion,
                    the function shall return 0.
                    In case of failure, this function would return -1.

 */
int32_t ipv4AddressParse(char *str,
                         uint32_t *ipv4ip)
{
    volatile int32_t i = 0;
    uint32_t n;
    uint32_t ipv4Address = 0;
    char             *token;

    token = strtok(str, ".");
    if(token)
    {
        n = (int)strtoul(token, 0, 10);
    }
    else
    {
        return(-1);
    }

    while(i < 4)
    {
       /* Check Whether IP is valid */
       if((token != NULL) && (n < 256))
       {
           ipv4Address |= n;
           if(i < 3)
           {
               ipv4Address = ipv4Address << 8;
           }
           token=strtok(NULL,".");
           if (token)
           {
               n = (int)strtoul(token, 0, 10);
           }
           i++;
       }
       else
       {
           return -1;
       }
    }

    *ipv4ip = ipv4Address;

    return(0);
}

/*!
    \brief          hex byte string to ASCII

    This routine converts a hexadecimal base byte represented
    as a string, into the equivalent ASCII character.

    \param          str         -   Points to string Hex.

    \param          ascii       -   Points to a buffer 
                                    containing the converted ASCII char.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1.

 */
int32_t hexbyteStrtoASCII(char *str,
                          uint8_t *ascii)
{
    char        *t = NULL;
    uint8_t hexchar = 0;

    /* Skip 0x if present */
    if(str[1] == 'x')
    {
        t = str + 2;
    }
    else
    {
        t = str;
    }

    while((hexchar < 2) && ((*t) != '\0'))
    {
        if(*t >= '0' && *t <= '9')
        {
            *ascii = (*ascii << (hexchar * 4)) | (*t - '0');
        }
        else if(*t >= 'a' && *t <= 'f')
        {
            *ascii = (*ascii << (hexchar * 4)) | ((*t - 'a') + 10);
        }
        else if(*t >= 'A' && *t <= 'F')
        {
            *ascii = (*ascii << (hexchar * 4)) | ((*t - 'A') + 10);
        }
        else
        {
            /* invalid entry */
            return(-1);
        }
        hexchar++;
        t++;
    }

    if(hexchar == 0)
    {
        return(-1);
    }

    return(0);
}

/*!
    \brief          Parse MAC address.

    This routine converts a MAC address given in a colon separated
    format in a string form, and converts it to six byte number format,
    representing the MAC address.

    \param          str       -   Points to string Hex.

    \param          mac       -   Points to a buffer containing
                                  the converted ASCII char.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1.

 */
int32_t macAddressParse(char *str,
                        uint8_t *mac)
{

    int32_t        count = 0;
    char           *t = NULL;
    uint8_t        tmp[3];
    uint8_t        byte = 0;
    size_t         MAC_length;

    t = (char*)str;

    MAC_length = strlen(t);

    if (MAC_length > SIX_BYTES_SIZE_MAC_ADDRESS)
    {
        /* invalid MAC size */
        return(-1);
    }

    memset(tmp, 0, sizeof(tmp));

    while(byte < 6)
    {
        count  = 0;
        while(*t != ':' && count < 2)
        {
            tmp[count] = *t;
            count++;
            t++;
        }

        tmp[count] = 0;

        if(hexbyteStrtoASCII((char*)&tmp[0], &mac[byte]) < 0)
        {
            return(-1);
        }

        byte++;

        if(*t != ':' && byte < 6)
        {
            /* invalid entry */
            return(-1);
        }

        /* Skip ':' */
        t++;
    }

    return(0);
}

/*!
    \brief          GPIO button function callback

    This routine gets called whenever board button (SW3 for CC32xx
    or S2 for MSP-432) gets pressed. It posts the sleep semaphore,
    in order to wake the device from LPDS.

    \param          index    -   Contains the board GPIO index who
                                 triggered this function.

    \return         void

    \sa             cmdEnableWoWLANCallback

*/
void gpioButtonFxn1(uint8_t index)
{
    UART_PRINT("\n\rWoke up from GPIO button .. \n\r");
    sem_post(&app_CB.WowlanSleepSem);
    return;
}

/*!
    \brief          Prints list of available commands.

    \return         This function shall return 0.

    \sa             cmd_prompt

 */
int32_t showAvailableCmd()
{
    uint8_t i = 0;

    printBorder('=', 80);
    UART_PRINT("\n\rAvailable commands:\n\r");

    for(i = 0; i < gMaxCmd; i++)
    {
        if(!(i % 4))
        {
            UART_PRINT(lineBreak);
        }
        UART_PRINT("%-20s", gCmdList[i].cmd);
    }

    UART_PRINT("\n\r\n\r");
    printBorder('=', 80);
    UART_PRINT("\n\r\n\r");

    return(0);
}

/*!
    \brief          Display application banner

    This routine shows how to get device information form the NWP.
    Also, it prints the PHY, MAC, NWP and Driver versions.

    \param          appName    -   points to a string representing 
                                   application name.

    \param          appVersion -   points to a string representing 
                                   application version number.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure,
                    this function would return negative value.

    \sa             sl_DeviceGet, sl_NetCfgGet

*/
int32_t DisplayAppBanner(char* appName, char* appVersion)
{
     int32_t            ret = 0;
     uint8_t            macAddress[SL_MAC_ADDR_LEN];
     uint16_t           macAddressLen = SL_MAC_ADDR_LEN;
     uint16_t           ConfigSize = 0;
     uint8_t            ConfigOpt = SL_DEVICE_GENERAL_VERSION;
     SlDeviceVersion_t ver = {0};

     ConfigSize = sizeof(SlDeviceVersion_t);

    /* Print device version info. */
    ret =
        sl_DeviceGet(SL_DEVICE_GENERAL, &ConfigOpt, &ConfigSize,
                     (uint8_t*)(&ver));
    ASSERT_ON_ERROR(ret, DEVICE_ERROR);

    /* Print device Mac address */
    ret = sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, 0, &macAddressLen,
                       &macAddress[0]);
    ASSERT_ON_ERROR(ret, WLAN_ERROR);

    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 44);
    UART_PRINT(lineBreak);
    UART_PRINT("\t   %s Example Ver: %s",appName, appVersion);
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 44);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);
    UART_PRINT("\t CHIP: 0x%x",ver.ChipId);
    UART_PRINT(lineBreak);
    UART_PRINT("\t MAC:  %d.%d.%d.%d",ver.FwVersion[0],ver.FwVersion[1],
               ver.FwVersion[2],
               ver.FwVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t PHY:  %d.%d.%d.%d",ver.PhyVersion[0],ver.PhyVersion[1],
               ver.PhyVersion[2],
               ver.PhyVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t NWP:  %d.%d.%d.%d",ver.NwpVersion[0],ver.NwpVersion[1],
               ver.NwpVersion[2],
               ver.NwpVersion[3]);
    UART_PRINT(lineBreak);
    UART_PRINT("\t ROM:  %d",ver.RomVersion);
    UART_PRINT(lineBreak);
    UART_PRINT("\t HOST: %s", SL_DRIVER_VERSION);
    UART_PRINT(lineBreak);
    UART_PRINT("\t MAC address: %02x:%02x:%02x:%02x:%02x:%02x",
               macAddress[0],
               macAddress[1],
               macAddress[2],
               macAddress[3],
               macAddress[4],
               macAddress[5]);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('=', 44);
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);

    return(ret);
}

/*!
    \brief          initialize Application's Variables

    This routine initialize the application control block.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1.

    \sa             MainThread

*/
int32_t    initAppVariables(void)
{
    int32_t ret = 0;

    app_CB.Status = 0 ;
    app_CB.Role = ROLE_RESERVED;
    app_CB.Exit = FALSE;

    memset(&app_CB.CmdBuffer, 0x0, CMD_BUFFER_LEN);
    memset(&app_CB.gDataBuffer, 0x0, sizeof(app_CB.gDataBuffer));
    memset(&app_CB.CON_CB, 0x0, sizeof(app_CB.CON_CB));

    ret = sem_init(&app_CB.CON_CB.connectEventSyncObj,    0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.CON_CB.eventCompletedSyncObj,  0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.CON_CB.ip4acquireEventSyncObj, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.CON_CB.ip6acquireEventSyncObj, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    memset(&app_CB.P2P_CB, 0x0, sizeof(app_CB.P2P_CB));

    ret = sem_init(&app_CB.P2P_CB.DeviceFound, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.P2P_CB.RcvConReq, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.P2P_CB.RcvNegReq, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    ret = sem_init(&app_CB.WowlanSleepSem, 0, 0);
    if(ret != 0)
    {
        SHOW_WARNING(ret, OS_ERROR);
        return(-1);
    }

    return(ret);
}

/*!
    \brief          Semaphore wait Timeout

    This routine shows how to to set a timeout while waiting on
    a semaphore for POSIX based OS API.

    \param          sem     -   points to a semaphore object.

    \param          Timeout -   Timeout in mSec value.

    \return         Upon successful completion,
                    the function shall return 0.
                    In case of failure,
                    this function would return negative value.

    \sa             sem_timedwait, cmdWlanConnectCallback

 */
int32_t sem_wait_timeout(sem_t *sem,
                         uint32_t Timeout)
{
    struct timespec abstime;
    abstime.tv_nsec = 0;
    abstime.tv_sec = 0;

    /* Since POSIX timeout are relative and not absolute,
     * take the current timestamp. */
    clock_gettime(CLOCK_REALTIME, &abstime);
    if(abstime.tv_nsec < 0)
    {
        abstime.tv_sec = Timeout;
        return (sem_timedwait(sem, &abstime));
    }

    /* Add the amount of time to wait */
    abstime.tv_sec += Timeout / 1000;
    abstime.tv_nsec += (Timeout % 1000) * 1000000;

    abstime.tv_sec += (abstime.tv_nsec / 1000000000);
    abstime.tv_nsec = abstime.tv_nsec % 1000000000;

    /* Call the semaphore wait API */
    return(sem_timedwait(sem, &abstime));
}

/*!
    \brief          Main thread

    This routine starts the application:
    It initializes the SPI interface with the NWP.
    Starts the GPIO drivers and UART console.
    Sets the Start time for the realtime clock.
    It starts the driver's internal Spawn thread 
    (to receive asynchronous events).
    and launch the command prompt handler.

    \param          arg

    \return         This function doesn't return.

    \sa             sl_Task, Board_initSPI, InitTerm, cmd_prompt.

 */
void * mainThread(void *arg)
{
    int32_t             RetVal ;
    pthread_attr_t      pAttrs_spawn;
    struct sched_param  priParam;
    struct timespec     ts = {0};

    /* Initializes the SPI interface to the Network 
       Processor and peripheral SPI (if defined in the board file) */
    SPI_init();
    GPIO_init();

    /* Init Application variables */
    RetVal = initAppVariables();

    /* Init Terminal UART */
    InitTerm();

    /* initialize the realtime clock */
    clock_settime(CLOCK_REALTIME, &ts);

    /* Switch off all LEDs on boards */
    GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);
    GPIO_write(Board_GPIO_LED1, Board_GPIO_LED_OFF);
    GPIO_write(Board_GPIO_LED2, Board_GPIO_LED_OFF);

    /* Create the sl_Task internal spawn thread */
    pthread_attr_init(&pAttrs_spawn);
    priParam.sched_priority = SPAWN_TASK_PRIORITY;
    RetVal = pthread_attr_setschedparam(&pAttrs_spawn, &priParam);
    RetVal |= pthread_attr_setstacksize(&pAttrs_spawn, TASK_STACK_SIZE);

    /* The SimpleLink host driver architecture mandate spawn 
       thread to be created prior to calling Sl_start (turning the NWP on). */
    /* The purpose of this thread is to handle
       asynchronous events sent from the NWP.
     * Every event is classified and later handled 
       by the Host driver event handlers. */
    RetVal = pthread_create(&gSpawn_thread, &pAttrs_spawn, sl_Task, NULL);
    if(RetVal < 0)
    {
        /* Handle Error */
        UART_PRINT("Network Terminal - Unable to create spawn thread \n");
        return(NULL);
    }

    /* Before turning on the NWP on,
       reset any previously configured parameters */
    /*
         IMPORTANT NOTE - This is an example reset function,
         user must update this function to match the application settings.
     */
    RetVal = ConfigureSimpleLinkToDefaultState();
    if(RetVal < 0)
    {
        /* Handle Error */
        UART_PRINT("Network Terminal - Couldn't configure Network Processor\n");
        return(NULL);
    }

    /* Output device information to the UART terminal */
    RetVal = DisplayAppBanner(APPLICATION_NAME, APPLICATION_VERSION);

    if(RetVal < 0)
    {
        /* Handle Error */
        UART_PRINT(
            "Network Terminal - Unable to retrieve device information \n");
        return(NULL);
    }

    /* Display Network Terminal API commands */
    showAvailableCmd();

    /*
     * Calling UART handling method which serves as the application main loop.
     * Note that this function doesn't return.
     */
    RetVal = cmd_prompt(NULL);

    if(RetVal)
    {
        while(1)
        {
            ;
        }
    }

    return(0);
}
