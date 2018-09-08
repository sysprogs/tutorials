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

#ifndef __NETWORK_TERMINAL_H__
#define __NETWORK_TERMINAL_H__

#include "str.h"
#include <ti/drivers/net/wifi/simplelink.h>

#ifndef WIN32
#include "semaphore.h"
#include "pthread.h"
#include "uart_term.h"
#include "unistd.h"
#include "time.h"

#include <Board.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/Power.h>

#else

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include "sl_studioP.h"

#endif

#define CMD_BUFFER_LEN          (256)
#define MAX_CMD_NAME_LEN        (32)
#define APPLICATION_NAME        ("Network Terminal")
#define APPLICATION_VERSION     ("1.0.1.0")
#define TASK_STACK_SIZE         (2048)
#define SPAWN_TASK_PRIORITY     (9)
#define PASSWD_LEN_MAX          (63)
#define PASSWD_LEN_MIN          (8)
#define WLAN_SCAN_COUNT         (30)
#define MAX_FILE_NAME_LEN       (32)
#define MAX_TEXT_PAD_SIZE       (256)
#define MAX_FILE_LIST           (20)
#define MAX_BUF_SIZE            (1400)
#define SL_STOP_TIMEOUT         (200)
#define DEV_TYPE_LEN            (17)
#define IPV6_ADDR_LEN           (16)
#define IPV4_ADDR_LEN           (4)
#define DEVICE_ERROR            ( \
        "Device error, please refer \"DEVICE ERRORS CODES\" section in errors.h")
#define WLAN_ERROR              ( \
        "WLAN error, please refer \"WLAN ERRORS CODES\" section in errors.h")
#define BSD_SOCKET_ERROR        ( \
        "BSD Socket error, please refer \"BSD SOCKET ERRORS CODES\" section in errors.h")
#define SL_SOCKET_ERROR         ( \
        "Socket error, please refer \"SOCKET ERRORS CODES\" section in errors.h")
#define NETAPP_ERROR            ( \
        "Netapp error, please refer \"NETAPP ERRORS CODES\" section in errors.h")
#define OS_ERROR                ( \
        "OS error, please refer \"NETAPP ERRORS CODES\" section in errno.h")
#define CMD_ERROR               ("Invalid option/command.")
#define RECEIVE_TIMEOUT         (60)

typedef union
{
    uint8_t                    nwData[MAX_BUF_SIZE];
    int8_t                     textPad[MAX_TEXT_PAD_SIZE];
    SlWlanNetworkEntry_t       netEntries[WLAN_SCAN_COUNT];
}gDataBuffer_t;

typedef struct cmdAction
{
    char        *cmd;
    int32_t    (*callback)(void *);
    int32_t    (*printusagecallback)(void *);
}cmdAction_t;

typedef struct p2p_ControlBlock_t
{
    uint8_t           p2pPeerDeviceName[32];
    uint8_t           p2pDeviceName[32];
    sem_t             DeviceFound;
    sem_t             RcvNegReq;
    sem_t             RcvConReq;
    SlWlanSecParams_t P2PsecParams;
}p2p_CB;

typedef struct connectionControlBlock_t
{
    sem_t    connectEventSyncObj;
    sem_t    ip4acquireEventSyncObj;
    sem_t    ip6acquireEventSyncObj;
    sem_t    eventCompletedSyncObj;
    uint32_t GatewayIP;
    uint8_t  ConnectionSSID[SL_WLAN_SSID_MAX_LENGTH +1];
    uint8_t  ConnectionBSSID[SL_WLAN_BSSID_LENGTH];
    uint32_t DestinationIp;
    uint32_t IpAddr;
    uint32_t StaIp;
    uint32_t Ipv6Addr[4];
}connection_CB;

typedef struct appControlBlock_t
{
    /* Status Variables */
    /* This bit-wise status variable shows the state of the NWP */
    uint32_t Status;                    
     /* This field keeps the device's role (STA, P2P or AP) */
    uint32_t Role;                     
    /* This flag lets the application to exit */
    uint32_t Exit;                      
    /* Sets the number of Ping attempts to send */
    uint32_t PingAttempts;                  
    /* Data & Network entry Union */
    gDataBuffer_t gDataBuffer;

    /* P2P mode CB */
    p2p_CB P2P_CB;

    /* STA/AP mode CB */
    connection_CB CON_CB;

    /* Cmd Prompt buffer */
    uint8_t CmdBuffer[CMD_BUFFER_LEN];

    /* WoWLAN semaphore */
    sem_t WowlanSleepSem;

}appControlBlock;

extern appControlBlock app_CB;

typedef enum
{
/* This bit is set: Network Processor is powered up */
    STATUS_BIT_NWP_INIT = 0,          
/* This bit is set: the device is connected to the AP or 
   client is connected to device (AP) */
    STATUS_BIT_CONNECTION,
/* This bit is set: the device has leased IP to  any connected client */
    STATUS_BIT_IP_LEASED,
/* This bit is set: the device has acquired an IP */
    STATUS_BIT_IP_ACQUIRED,               
/* If this bit is set: the device (P2P mode)
   found any p2p-device in scan */
    STATUS_BIT_P2P_DEV_FOUND,
/* If this bit is set: the device (P2P mode) 
found any p2p-negotiation request */
    STATUS_BIT_P2P_REQ_RECEIVED,
/* If this bit is set: the device(P2P mode)
connection to client(or reverse way) is failed */
    STATUS_BIT_CONNECTION_FAILED,
/* This bit is set: device is undergoing ping operation */
    STATUS_BIT_PING_STARTED,              
/* This bit is set: Scan is running is background */
    STATUS_BIT_SCAN_RUNNING,              
/* If this bit is set: the device 
has acquired an IPv6 address */
    STATUS_BIT_IPV6_ACQUIRED,             
/* If this bit is set: the device has acquired 
   an IPv6 address */
    STATUS_BIT_IPV6_GLOBAL_ACQUIRED,
/* If this bit is set: the device has acquired
an IPv6 address */
    STATUS_BIT_IPV6_LOCAL_ACQUIRED,   
                                         
/* If this bit is set: Authentication with ENT AP failed. */
    STATUS_BIT_AUTHENTICATION_FAILED, 
    

    STATUS_BIT_RESET_REQUIRED,

    STATUS_BIT_TX_STARED

}e_StatusBits;


/* Status keeping MACROS */

#define SET_STATUS_BIT(status_variable, bit) status_variable |= (1<<(bit))

#define CLR_STATUS_BIT(status_variable, bit) status_variable &= ~(1<<(bit))

#define GET_STATUS_BIT(status_variable, bit) \
                                (0 != (status_variable & (1<<(bit))))

#define IS_NW_PROCSR_ON(status_variable)     \
                GET_STATUS_BIT(status_variable, STATUS_BIT_NWP_INIT)

#define IS_CONNECTED(status_variable)        \
                GET_STATUS_BIT(status_variable, STATUS_BIT_CONNECTION)

#define IS_IP_LEASED(status_variable)        \
                GET_STATUS_BIT(status_variable, STATUS_BIT_IP_LEASED)

#define IS_IP_ACQUIRED(status_variable)      \
                GET_STATUS_BIT(status_variable, STATUS_BIT_IP_ACQUIRED)

#define IS_IP6_ACQUIRED(status_variable)     \
    GET_STATUS_BIT(status_variable, \
                   (STATUS_BIT_IPV6_LOCAL_ACQUIRED | \
                    STATUS_BIT_IPV6_GLOBAL_ACQUIRED))

#define IS_IPV6L_ACQUIRED(status_variable)   \
                GET_STATUS_BIT(status_variable, STATUS_BIT_IPV6_LOCAL_ACQUIRED)

#define IS_IPV6G_ACQUIRED(status_variable)   \
                GET_STATUS_BIT(status_variable, STATUS_BIT_IPV6_GLOBAL_ACQUIRED)

#define IS_PING_RUNNING(status_variable)     \
                GET_STATUS_BIT(status_variable, STATUS_BIT_PING_STARTED)

#define IS_TX_ON(status_variable)            \
                GET_STATUS_BIT(status_variable, STATUS_BIT_TX_STARED)

#define ASSERT_ON_ERROR(ret, errortype)\
        {\
            if(ret < 0)\
            {\
                SHOW_WARNING(ret, errortype);\
                return -1;\
            }\
        }

#define ASSERT_AND_CLEAN_CONNECT(ret, errortype, ConnectParams)\
        {\
            if(ret < 0)\
            {\
                SHOW_WARNING(ret, errortype);\
                FreeConnectCmd(ConnectParams);\
                return -1;\
            }\
        }

#define ASSERT_AND_CLEAN_STARTAP(ret, errortype, StartApParams)\
        {\
            if(ret < 0)\
            {\
                SHOW_WARNING(ret, errortype);\
                FreeStartApCmd(StartApParams);\
                return -1;\
            }\
        }

#define ASSERT_AND_CLEAN_PING(ret, errortype, pingParams)\
        {\
            if(ret < 0)\
            {\
                SHOW_WARNING(ret, errortype);\
                FreePingCmd(pingParams);\
                return -1;\
            }\
        }

#define ASSERT_AND_CLEAN_MDNS_ADV(ret, errortype, mDNSAdvertiseParams)\
        {\
            if(ret < 0)\
            {\
                SHOW_WARNING(ret, errortype);\
                FreemDNSAdvertiseCmd(mDNSAdvertiseParams);\
                return -1;\
            }\
        }

#define ASSERT_AND_CLEAN_RECV(ret, errortype, RecvCmdParams)\
        {\
            if(ret < 0)\
            {\
                SHOW_WARNING(ret, errortype);\
                FreeRecvCmd(RecvCmdParams);\
                return -1;\
            }\
        }

#define SHOW_WARNING(ret, errortype)        UART_PRINT( \
        "\n\r[line:%d, error code:%d] %s\n\r", __LINE__, ret, errortype);

void gpioButtonFxn1(uint8_t index);

void PrintIPAddress(unsigned char ipv6,
                    void *ip);

int32_t sem_wait_timeout(sem_t *sem,
                         uint32_t Timeout);

int32_t ConfigureSimpleLinkToDefaultState(void);

int32_t ipv6AddressParse(char *str,
                         uint8_t *ipv6ip);

int32_t ipv4AddressParse(char *str,
                         uint32_t *ipv4ip);

int32_t hexbyteStrtoASCII(char *str,
                          uint8_t *ascii);

int32_t macAddressParse(char *str,
                        uint8_t *mac);

#endif /* __NETWORK_TERMINAL_H__ */
