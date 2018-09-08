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

#ifndef __CMD_PARSER_H__
#define __CMD_PARSER_H__

/* Application includes */
#include "socket_cmd.h"
#include "network_terminal.h"

/* Application defines */
#define MAX_SERVICE_NAME_LENGTH      (63)
#define CHANNEL_MASK_ALL            (0x1FFF)
#define RSSI_TH_MAX                 (-95)


/* Command structures */

typedef struct ScanCmd
{
    /* Number of Scan Entries to retrieve from the NWP */
    uint8_t         numOfentries;       
	/* The netEntries array position to start write results from */
    uint8_t         index;
	
}ScanCmd_t;


typedef struct SetPolicyCmd
{ 
    /* State of the scans - ON or OFF */
    uint8_t                  turnOff;                
    /* Default is set to 10 Seconds */
    uint32_t                 ScanIntervalinSec;       
    /* Displays hidden SSID's - Yes or No */
    uint8_t                  hiddenSsid;              
    /* Scan parameters configurations - RSSI threshold and channel mask */
    SlWlanScanParamCommand_t ScanParamConfig;         
}SetPolicyCmd_t;


typedef struct ConnectCmd
{
    /* Ap's SSID */
    uint8_t                 *ssid;              
    /* Static IP address (for static configuration) */
    uint8_t                 *ip;                
    /* Default gateway IP address (for static configuration) */
    uint8_t                 *gw;                
    /* Dns IP address (for static configuration) */
    uint8_t                 *dns;               
    /* Enterprise user name */
    uint8_t                 *entUserName;       
    /* Device Date and Time - IMPORTANT: Date and time 
       should match the certificate expiration date */
    SlDateTime_t dateTime;                      
    /* Security parameters - Security Type and Password */
    SlWlanSecParams_t secParams;                
    /* Enterprise parameters - Security Type and Password */
    SlWlanSecParamsExt_t secParamsEnt;          
    
}ConnectCmd_t;


typedef struct StartApCmd
{   /* Ap's SSID */
    uint8_t *ssid;           
    /* Determine if AP has hidden SSID */
    uint8_t hidden;                          
    /* 802.11 WLAN channel [1-11] */
    uint8_t channel;                         
    /* The AP's TX power */
    uint8_t tx_pow;                          
    /* Limits the number of stations that the AP's has */
    uint8_t sta_limit;                       
    /* Security parameters - Security Type and Password */
    SlWlanSecParams_t secParams;             
    
}StartApCmd_t;


typedef struct CreateFilterCmd
{/* Header or combination filter */
    SlWlanRxFilterRuleType_t ruleType;           
    /* Returned value for 'sl_WlanRxFilterAdd()' */
    SlWlanRxFilterID_t filterID;                 
    /* Dictates filter behavior */
    SlWlanRxFilterFlags_u flags;                 
    /* Match criteria */
    SlWlanRxFilterRule_u rule;                   
    /* What are the preconditions to trigger the filter */
    SlWlanRxFilterTrigger_t trigger;             
    /* Operation that execute upon a filter match */
    SlWlanRxFilterAction_t action;               
    
}CreateFilterCmd_t;


typedef struct WoWLANEnableCmd
{   /* Header or combination filter */
    SlWlanRxFilterRuleType_t ruleType;           
    /* Returned value for 'sl_WlanRxFilterAdd()' */
    SlWlanRxFilterID_t filterID;                 
    /* Dictates filter behavior */
    SlWlanRxFilterFlags_u flags;                 
    /* Match criteria */
    SlWlanRxFilterRule_u rule;                   
    /* What are the preconditions to trigger the filter */
    SlWlanRxFilterTrigger_t trigger;             
    /* Operation that execute upon a filter match */
    SlWlanRxFilterAction_t action;               
}WoWLANEnableCmd_t;

typedef struct PingCmd
{   /* Host name or address */
    uint8_t               *host;         
    /* SimpleLink Ping command stracture */
    SlNetAppPingCommand_t pingCmd;       
    
}PingCmd_t;

typedef struct mDnsAdvertiseCmd
{/* Port on which to accept service requests */
    uint16_t service_port;                                       
    /* Service time to leave; time to stop advertising. */
    uint32_t service_ttl;                                        
    /* Service name */
    uint8_t service_name[MAX_SERVICE_NAME_LENGTH];               
    /* Advertised text. */
    uint8_t        *adv_text;                                    
    /* device part of the advertise address */
    uint8_t        *dev_name;                                    
    
}mDnsAdvertiseCmd_t;

typedef struct mDnsQueryCmd
{   /* The service name to query */
    uint8_t ServiceName[MAX_SERVICE_NAME_LENGTH];              
    /* Sets the query type: One shot or continuous */
    uint8_t OneShotFlag;                                       
    
}mDnsQueryCmd_t;

typedef struct SendCmd
{   /* Number of packets to send */
    int32_t numberOfPackets;                
    /* IP address */
    uint8_t *ip;                   
    /* Server's port address*/
    int16_t portNumber;                     
    /* Decides the type of transport protocol */
    uint8_t udpFlag;                        
    /* Send as server or client flag */
    uint8_t server;                         
    /* Blocking or non-blocking on socket */
    uint8_t nb;                             
    /* IPV4 or IPv6 flag. By default IPV4 enable */
    uint8_t ipv6;                           
    /* IP in binary format */
    ip_t ipAddr;                            
    
}SendCmd_t;

typedef struct RecvCmd
{/* Number of packets to receive */
    int32_t numberOfPackets;                             
    /* IP address */
    uint8_t                   *ip;                       
    /* Server's port address*/
    int16_t portNumber;                                  
    /* Decides the type of transport protocol */
    uint8_t udpFlag;                                     
    /* Send as server or client flag */
    uint8_t server;                                      
    /* Blocking or non-blocking on socket */
    uint8_t nb;                                          
    /* IPV4 or IPv6 flag. By default IPV4 enable */
    uint8_t ipv6;                                        
    /* IP in binary format */
    ip_t ipAddr;                                         
}RecvCmd_t;


/* Function prototypes */
int32_t ParseScanCmd
	(void *arg, ScanCmd_t *scanParams);
int32_t ParseSetPolicyCmd
	(void *arg, SetPolicyCmd_t *SetPolicyParams);
int32_t ParseConnectCmd
	(void *arg, ConnectCmd_t *ConnectParams);
int32_t ParseStartApCmd
	(void *arg, StartApCmd_t *StartApParams);
int32_t ParseCreateFilterCmd
	(void *arg,  CreateFilterCmd_t *CreateFilterParams);
int32_t ParseEnableWoWLANCmd
	(void *arg, WoWLANEnableCmd_t *WoWLANEnableParams);
int32_t ParsePingCmd
	(void *arg, PingCmd_t *pingParams);
int32_t ParsemDNSAdvertiseCmd
	(void *arg, mDnsAdvertiseCmd_t *mDNSAdvertiseParams);
int32_t ParsemDNSQueryCmd
	(void *arg,  mDnsQueryCmd_t *QueryCmdParams);
int32_t mDNScreateServiceName
	(char *p_sevName, char *p_servOver ,
	 char *p_servType, char *p_adv_service_name);
int32_t ParseSendCmd
	(void *arg,  SendCmd_t *SendCmdParams);
int32_t ParseRecvCmd
	(void *arg,  RecvCmd_t *RecvCmdParams);
int32_t ParseCountycodeCmd
	(void *arg, uint8_t *country);
int32_t ParseCmd(void *arg);
void FreeConnectCmd(ConnectCmd_t *ConnectParams);
void FreeStartApCmd(StartApCmd_t *StartApParams);
void FreePingCmd(PingCmd_t *pingParams);
void FreemDNSAdvertiseCmd(mDnsAdvertiseCmd_t *mDNSAdvertiseParams);
void FreeSendCmd(SendCmd_t *SendCmdParams);
void FreeRecvCmd(RecvCmd_t *RecvCmdParams);

#endif /* __CMD_PARSER_H__ */
