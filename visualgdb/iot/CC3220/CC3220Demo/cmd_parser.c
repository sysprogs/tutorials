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

/* General includes */
#include <stdlib.h>
#include <ti/drivers/net/wifi/simplelink.h>

/* Application includes */
#include "str.h"
#include "cmd_parser.h"
#include "wlan_cmd.h"
#include "netapp_cmd.h"
#include "network_terminal.h"

/* Application defines */
#define MAX_SSID_ENTRIES            (30)
#define FRAME_TYPE_MASK             (0x0C)
#define DEVICE_YEAR                 (2016)
#define DEVICE_MONTH                (1)
#define DEVICE_DATE                 (6)
#define KEY_LEN_MAX                 (64)
#define MAX_PAYLOAD_SIZE            (1400)
#define FRAME_SUBTYPE_MASK          (0xF0)
#define MAX_PING_SIZE               (1452)
#define DEF_ADVERTISE_PORT          (2525)
#define DEF_ADVERTISE_TTL           (2000)

/******************************************************************************
                  Callback Functions
******************************************************************************/
/*!
    \brief          Parse Scan command.

    This routine takes a ScanCmd_t structure, and fill it's content with
    parameters taken from command line. 
    In case of a parsing error or invalid parameters,
    this function sets default values.

    \param          arg         -   Points to command line buffer.
                                    Contains the command line typed by user.

    \param          scanParams  -   Points to command structure provided 
                                    by the scan callback.
                                    This structure will later be read
                                    by the scan callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would print error, 
                    and show the Scan command help menu.

    \sa             cmdScanCallback
 */
int32_t ParseScanCmd(void *arg, ScanCmd_t *scanParams)
{
    char            *token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    uint8_t help = FALSE;

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(NULL == token)
    {
        help = TRUE;
    }

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, n_optionStr))
        {
            token = strtok(NULL, space_str);

            if(token)
            {
                scanParams->numOfentries = (uint8_t)atoi(token);
            }

            if(scanParams->numOfentries > MAX_SSID_ENTRIES ||
               (0 == scanParams->numOfentries))
            {
                scanParams->numOfentries = MAX_SSID_ENTRIES;
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, " ");
    }

    if(help)
    {
        printScanUsage(arg);
        return(-1);
    }

    return(0);
}

/*!
    \brief          Parse Set policy command.

    This routine takes a SetPolicyCmd_t structure, and fill it's content with
    parameters taken from command line.It sets some default values pre-parsing,
    in order to showcase 'default' parameters.
    In case of a parsing error or invalid parameters,
    this function prints help menu.

    \param          arg              -   Points to command line buffer.
                                         Contains the command line typed by user.

    \param          SetPolicyParams  -   Points to command structure provided 
                                         by the set policy callback.
                                         This structure will later be read by 
                                         the set policy callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would print error,
                    and show
                    the set scan policy command help menu.

    \sa             cmdSetPolicyCallback
 */
int32_t ParseSetPolicyCmd(void *arg, SetPolicyCmd_t *SetPolicyParams)
{
    char               *token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    uint8_t help = FALSE;

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(NULL == token)
    {
        help = TRUE;
    }

    /* Set channel mask and RSSI threshold to default */
    SetPolicyParams->ScanParamConfig.RssiThreshold = RSSI_TH_MAX;
    SetPolicyParams->ScanParamConfig.ChannelsMask = CHANNEL_MASK_ALL;
    SetPolicyParams->ScanIntervalinSec = 10;

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, h_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                if(!strcmp(token, "YES") || !strcmp(token, "yes"))
                {
                    SetPolicyParams->hiddenSsid = TRUE;
                }
                else if(!strcmp(token, "NO") || !strcmp(token, "no"))
                {
                    SetPolicyParams->hiddenSsid = FALSE;
                }
                else
                {
                    help = TRUE;
                }
            }
        }
        else if(!strcmp(token, i_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                SetPolicyParams->ScanIntervalinSec = (uint16_t)atoi(token);
            }
        }
        else if(!strcmp(token, o_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                if(!strcmp(token, "YES") || !strcmp(token, "yes"))
                {
                    SetPolicyParams->turnOff = TRUE;
                }
            }
        }
        else if(!strcmp(token, c_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                SetPolicyParams->ScanParamConfig.ChannelsMask =
                    (uint16_t)strtol(token, NULL, 16);
            }
        }
        else if(!strcmp(token, r_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                SetPolicyParams->ScanParamConfig.RssiThreshold =
                (int32_t)atoi(token);
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, " ");
    }

    if(help)
    {
        printSetPolicyUsage(arg);
        return(-1);
    }

    return(0);
}

/*!
    \brief          Parse Connect command.

    This routine takes a ConnectCmd_t structure, and fill it's content with
    parameters taken from command line.It sets some default values pre-parsing,
    and checks the parameters validity.
    In case of a parsing error or invalid parameters, 
    this function prints help menu.

    \param          arg            -   Points to command line buffer.
                                       Contains the command line typed by user.

    \param          ConnectParams  -   Points to command structure provided 
                                       by the connect callback.
                                       This structure will later be read by
                                       the connect callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure,this function would print error,
                    and show the set scan policy command help menu.

    \sa             cmdWlanConnectCallback
 */
int32_t ParseConnectCmd(void *arg, ConnectCmd_t *ConnectParams)
{
    char cmdStr[CMD_BUFFER_LEN + 1];
    char                 *token = NULL;
    char                 *ssid = NULL;
    char                 *password = NULL;
    char                 *security = NULL;
    char                 *ip = NULL;
    char                 *gw = NULL;
    char                 *dns = NULL;
    char                 *entUserName = NULL;
    uint8_t help = FALSE;

    strncpy(cmdStr, (char*) arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(token == NULL)
    {
        help = TRUE;
    }

    ConnectParams->ip = NULL;
    ConnectParams->dns = NULL;
    ConnectParams->gw = NULL;

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = 1;
        }
        else if(!strcmp(token, s_optionStr))
        {
            ssid = strtok(NULL, "\"");
        }
        else if(!strcmp(token, p_optionStr))
        {
            password = strtok(NULL, "\"");
        }
        else if(!strcmp(token, t_optionStr))
        {
            security = strtok(NULL, space_str);
        }
        else if(!strcmp(token, ip_optionStr))
        {
            ip = strtok(NULL, space_str);
        }
        else if(!strcmp(token, dns_optionStr))
        {
            dns = strtok(NULL, space_str);
        }
        else if(!strcmp(token, gw_optionStr))
        {
            gw = strtok(NULL, space_str);
        }
        else if(!strcmp(token, ent_optionStr))
        {
            entUserName = strtok(NULL,  "\" ");
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, space_str);
    }

    if(help)
    {
        printWlanConnectUsage(arg);
        return(-1);
    }

    if((ssid != NULL) && (strlen(ssid) <= SL_WLAN_SSID_MAX_LENGTH))
    {
        ConnectParams->ssid = (uint8_t *)calloc(sizeof(uint8_t),strlen(
                                                    ssid) + 1);
        strcpy((char *)ConnectParams->ssid, ssid);
    }
    else
    {
        UART_PRINT("\r\n[Cmd Parser] : Invalid SSID.\n\r");
        return(-1);
    }

    if(ip)
    {
        ConnectParams->ip = calloc(sizeof(uint8_t),strlen(ip));
        strcpy((char *)ConnectParams->ip, ip);
    }

    if(gw)
    {
        ConnectParams->gw = calloc(sizeof(uint8_t),strlen(gw));
        strcpy((char *)ConnectParams->gw, gw);
    }

    if(dns)
    {
        ConnectParams->dns = calloc(sizeof(uint8_t),strlen(dns));
        strcpy((char *)ConnectParams->dns, dns);
    }

    if(!security)
    {
        ConnectParams->secParams.Type = SL_WLAN_SEC_TYPE_OPEN;
    }
    else if(!strcmp(security, WPAWPA2_str))
    {
        ConnectParams->secParams.Type = SL_WLAN_SEC_TYPE_WPA_WPA2;
    }
    else if(!strcmp(security, WEP_str))
    {
        ConnectParams->secParams.Type = SL_WLAN_SEC_TYPE_WEP;
    }
    else if(!strcmp(security, WPS_str) && !password)
    {
        ConnectParams->secParams.Type = SL_WLAN_SEC_TYPE_WPS_PBC;
    }
    else if(!strcmp(security, WPS_str) && password)
    {
        ConnectParams->secParams.Type = SL_WLAN_SEC_TYPE_WPS_PIN;
    }
    else if(!strcmp(security, OPEN_str))
    {
        ConnectParams->secParams.Type = SL_WLAN_SEC_TYPE_OPEN;
    }
    else
    {
        UART_PRINT(
            "\r\n [Cmd Parser] : Parser expected password "
            "parameters [OPEN, WPA/WPA2, WEP, WPS].\n\r");
        return(-1);
    }

    if((password != NULL) && (strlen(password) <= PASSWD_LEN_MAX))
    {
        ConnectParams->secParams.KeyLen = strlen(password);
        ConnectParams->secParams.Key = (signed char *)calloc(
            sizeof(uint8_t), ConnectParams->secParams.KeyLen + 1);
        strncpy((char *)ConnectParams->secParams.Key, password,
                ConnectParams->secParams.KeyLen);
    }
    else if(ConnectParams->secParams.Type != SL_WLAN_SEC_TYPE_OPEN)
    {
        UART_PRINT("\r\n[Cmd Parser] : Invalid Password.\n\r");
        return(-1);
    }

    if(entUserName)
    {
        ConnectParams->secParamsEnt.UserLen = strlen(entUserName);
        ConnectParams->secParamsEnt.User = calloc(
            sizeof(uint8_t), ConnectParams->secParamsEnt.UserLen);
        ConnectParams->secParamsEnt.AnonUser = NULL;
        ConnectParams->secParamsEnt.EapMethod =
            SL_WLAN_ENT_EAP_METHOD_PEAP0_MSCHAPv2;
        ConnectParams->secParams.Type = SL_WLAN_SEC_TYPE_WPA_ENT;
        ConnectParams->dateTime.tm_year = DEVICE_YEAR;
        ConnectParams->dateTime.tm_mon = DEVICE_MONTH;
        ConnectParams->dateTime.tm_day = DEVICE_DATE;
    }

    return(0);
}

/*!
    \brief          Free Connect command.

    This routine takes a ConnectCmd_t structure, 
    and free all memory that was
    allocated in ParseConnectCmd.

    \param          ConnectParams  -   
    Points to command structure provided by the connect callback.

    \return         void

    \sa             cmdWlanConnectCallback
 */
void FreeConnectCmd(ConnectCmd_t *ConnectParams)
{
    if(ConnectParams->ssid != NULL)
    {
        free(ConnectParams->ssid);
        ConnectParams->ssid = NULL;
    }

    if(ConnectParams->ip != NULL)
    {
        free(ConnectParams->ip);
        ConnectParams->ip = NULL;
    }

    if(ConnectParams->gw != NULL)
    {
        free(ConnectParams->gw);
        ConnectParams->gw = NULL;
    }

    if(ConnectParams->dns != NULL)
    {
        free(ConnectParams->dns);
        ConnectParams->dns = NULL;
    }

    if(ConnectParams->secParams.Key != NULL)
    {
        free(ConnectParams->secParams.Key);
        ConnectParams->secParams.Key = NULL;
    }

    if(ConnectParams->secParamsEnt.User != NULL)
    {
        free(ConnectParams->secParamsEnt.User);
        ConnectParams->secParamsEnt.User = NULL;
    }

    return;
}

/*!
    \brief          Parse Start AP command.

    This routine takes a StartApCmd_t structure, and fill it's content with
    parameters taken from command line. It sets some default values pre-parsing,
    and checks the parameters validity.
    In case of a parsing error or invalid parameters,
    this function prints help menu.

    \param          arg            -   Points to command line buffer.
                                       Contains the command line typed by user.

    \param          StartApParams  -   Points to command structure provided
                                       by the connect callback.
                                       This structure will later be read by 
                                       the Start AP callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would print error,
                    and show the set Start AP command help menu.

    \sa             cmdWlanStartApCallback
 */
int32_t ParseStartApCmd(void *arg, StartApCmd_t *StartApParams)
{
    char cmdStr[CMD_BUFFER_LEN + 1];
    char                *token;
    char                *ssid = NULL;
    char                *password = NULL;
    char                *security = NULL;
    uint8_t help = FALSE;

    strncpy(cmdStr, (char*) arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(token == NULL)
    {
        help = TRUE;
    }

    /* Set default parameters */
    StartApParams->sta_limit = 4;
    StartApParams->hidden = FALSE;
    StartApParams->tx_pow = 0;
    StartApParams->channel = 1;

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, s_optionStr))
        {
            ssid = strtok(NULL, "\"");
        }
        else if(!strcmp(token, p_optionStr))
        {
            password = strtok(NULL, "\"");
        }
        else if(!strcmp(token, t_optionStr))
        {
            security = strtok(NULL, space_str);
        }
        else if(!strcmp(token, h_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                if(!strcmp(token, "YES") || !strcmp(token, "yes"))
                {
                    StartApParams->hidden = TRUE;
                }
                else if(!strcmp(token, "NO") || !strcmp(token, "no"))
                {
                    StartApParams->hidden = FALSE;
                }
                else
                {
                    UART_PRINT(
                        "\r\n [Cmd Parser] : invalid Parameter for hidden "
                        "AP option. Using default (Not hidden).\n\r");
                    StartApParams->hidden = FALSE;
                }
            }
        }
        else if(!strcmp(token, c_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                StartApParams->channel = atoi(token);
            }

            if(!((StartApParams->channel < 13) && (StartApParams->channel > 0)))
            {
                UART_PRINT(
                    "\r\n [Cmd Parser] : invalid Parameter for channel option."
                    " Using default (1).\n\r");
                StartApParams->channel = 1;
            }
        }
        else if(!strcmp(token, txpow_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                StartApParams->tx_pow = atoi(token);
            }

            if(StartApParams->tx_pow > 15)
            {
                UART_PRINT(
                   "\r\n [Cmd Parser] : invalid Parameter for tx power option."
                   " Using default (0).\n\r");
                StartApParams->tx_pow = 0;
            }
        }
        else if(!strcmp(token, l_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                StartApParams->sta_limit = atoi(token);
            }

            if((StartApParams->sta_limit < 1) || 
            (StartApParams->sta_limit > 4))
            {
                UART_PRINT(
                    "\r\n [Cmd Parser] : invalid Parameter for station limit "
                    "option. Using default (4).\n\r");
                StartApParams->sta_limit = 4;
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, space_str);
    }

    if(help)
    {
        printWlanStartApUsage(arg);
        return(-1);
    }

    if((NULL == ssid) || (strlen(ssid) >= SL_WLAN_SSID_MAX_LENGTH))
    {
        UART_PRINT(
            "\r\n [Cmd Parser] : invalid Parameter for SSID - Should be max"
            " 31 characters.\n\r");
        printWlanStartApUsage(arg);
        return(-1);
    }
    else
    {
        StartApParams->ssid = (uint8_t *)calloc(sizeof(uint8_t), strlen(ssid));
        strcpy((char *)StartApParams->ssid, ssid);
    }

    if(password != NULL)
    {
        StartApParams->secParams.KeyLen = strlen(password);

        if((StartApParams->secParams.KeyLen >= KEY_LEN_MAX) ||
           (StartApParams->secParams.KeyLen < PASSWD_LEN_MIN))
        {
            UART_PRINT(
                "\r\n [Cmd Parser] : Key length invalid - "
                "Should be in range: [8,63].\n\r");
            return(-1);
        }

        StartApParams->secParams.Key =
            (signed char *)(uint8_t *)calloc(sizeof(uint8_t),
                                             StartApParams->secParams.KeyLen);
        strcpy((char *)StartApParams->secParams.Key, password);
    }

    if(!security)
    {
        StartApParams->secParams.Type = SL_WLAN_SEC_TYPE_OPEN;
    }
    else if(!strcmp(security, WPAWPA2_str))
    {
        StartApParams->secParams.Type = SL_WLAN_SEC_TYPE_WPA_WPA2;
    }
    else if(!strcmp(security, WEP_str))
    {
        StartApParams->secParams.Type = SL_WLAN_SEC_TYPE_WEP;
    }
    else if(!strcmp(security, OPEN_str))
    {
        StartApParams->secParams.Type = SL_WLAN_SEC_TYPE_OPEN;
    }
    else
    {
        UART_PRINT(
            "\r\n [Cmd Parser] : Parser expected password parameters"
            " [OPEN, WPA/WPA2, WEP, WPS].\n\r");
        return(-1);
    }

    return(0);
}

/*!
    \brief          Free Start AP command.

    This routine takes a StartApCmd_t structure, and free all memory that was
    allocated in ParseStartApCmd.

    \param          StartApParams  -   
                Points to command structure provided by the Start AP callback.

    \return         void

    \sa             cmdWlanStartApCallback
 */
void FreeStartApCmd(StartApCmd_t *StartApParams)
{
    if(StartApParams->ssid != NULL)
    {
        free(StartApParams->ssid);
        StartApParams->ssid = NULL;
    }

    if(StartApParams->secParams.Key != NULL)
    {
        free(StartApParams->secParams.Key);
        StartApParams->secParams.Key = NULL;
    }

    return;
}

/*!
    \brief          Parse Cmd.

    This routine is a general command parser, 
    used with commands that has no parameters to parse.
    such an example is 'wlandisconnect'.

    \param        arg         -   Points to command line buffer.
                                  Contains the command line typed by user.

    \return       Upon successful completion, the function shall return 0.
                  In case of failure, this function would return -1.
                  It's the callback's responsibility to check the return value,
                  and print the help for this command.

    \sa           cmdConnectCallback, cmdDisableFilterCallback,
                  cmdP2PModecallback, cmdP2PStopcallback
 */
int32_t ParseCmd(void *arg)
{
    char             *token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    uint8_t help = FALSE;

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, " ");
    }

    if(help)
    {
        return(-1);
    }

    return(0);
}

/*!
    \brief          Parse Create filter command.

   This routine takes a CreateFilterCmd_t structure, and fill it's content with
   parameters taken from command line. It checks the parameters validity.
   In case of a parsing error or invalid parameters, 
   this function prints help menu.

   \param          arg               - Points to command line buffer.
                                       Contains the command line typed by user.

   \param          CreateFilterParams - Points to command structure
                                        provided by the create filter callback.
                                        This structure will later be read by 
                                        the create filter callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would print error, and show
                    the set createfilter command help menu.

    \sa             cmdCreateFilterCallback
 */
int32_t ParseCreateFilterCmd(void *arg,  CreateFilterCmd_t *CreateFilterParams)
{
    int32_t ret = -1;
    uint8_t help = FALSE;
    int32_t offset = -1;
    uint32_t patternLen = 0;
    uint8_t Frametype = 0;
    uint32_t ip4add = 0;
    uint8_t hostEvent = FALSE;
    char                    *filterType = NULL;
    char                    *cValue = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    char                    *token = NULL;

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(token == NULL)
    {
        help = TRUE;
    }

    while(token)
    {
        if(!strcmp(token, (char*)&help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, f_optionStr))
        {
            filterType = strtok(NULL, space_str);
        }
        else if(!strcmp(token, v_optionStr))
        {
            cValue = strtok(NULL, space_str);
        }
        else if(!strcmp(token, e_optionStr))
        {
            token = strtok(NULL, space_str);

            if(token)
            {
                if(!strcmp(token, "equals") || !strcmp(token, "EQUALS"))
                {
                    CreateFilterParams->rule.Header.CompareFunc =
                        SL_WLAN_RX_FILTER_CMP_FUNC_EQUAL;
                }
                else if(!strcmp(token,
                                "not_equals") || !strcmp(token, "NOT_EQUALS"))
                {
                    CreateFilterParams->rule.Header.CompareFunc =
                        SL_WLAN_RX_FILTER_CMP_FUNC_NOT_EQUAL_TO;
                }
                else
                {
                    UART_PRINT(
                        "\r\n [Cmd Parser] : Invalid option for"
                        " Comparison type.\n\r");
                    help = TRUE;
                    break;
                }
            }
        }
        else if(!strcmp(token, a_optionStr))
        {
            token = strtok(NULL, (char*)&space_str);
            if(token)
            {
                if(!strcmp(token, "drop") || !strcmp(token, "DROP"))
                {
                    CreateFilterParams->action.Type =
                        SL_WLAN_RX_FILTER_ACTION_DROP;
                }
                else if(!strcmp(token, "event") || !strcmp(token, "EVENT"))
                {
                    CreateFilterParams->action.Type =
                        SL_WLAN_RX_FILTER_ACTION_EVENT_TO_HOST;
                    hostEvent = TRUE;
                }
                else if(!strcmp(token, "pass") || !strcmp(token, "PASS"))
                {
                    CreateFilterParams->action.Type =
                        SL_WLAN_RX_FILTER_ACTION_NULL;
                }
                else
                {
                    UART_PRINT(
                        "\r\n [Cmd Parser] : Invalid option for action type."
                        " Using default: (pass).\n\r");
                    CreateFilterParams->action.Type =
                        SL_WLAN_RX_FILTER_ACTION_NULL;
                }
            }
            else
            {
                UART_PRINT(
                    "\r\n [Cmd Parser] : Invalid option for action type.\n\r");
                help = TRUE;
                break;
            }
        }
        else if(!strcmp(token, (char*)&m_optionStr))
        {
            token = strtok(NULL, (char*)space_str);

            if(token)
            {
                if(!strcmp(token, "L1"))
                {
                    CreateFilterParams->rule.Header.Field =
                        SL_WLAN_RX_FILTER_HFIELD_L1_PAYLOAD_PATTERN;
                    CreateFilterParams->trigger.ConnectionState =
                        SL_WLAN_RX_FILTER_STATE_STA_NOT_CONNECTED;
                    CreateFilterParams->trigger.Role =
                        SL_WLAN_RX_FILTER_ROLE_TRANCIEVER;
                }
                else if(!strcmp(token, "L4_CON"))
                {
                    CreateFilterParams->rule.Header.Field =
                        SL_WLAN_RX_FILTER_HFIELD_L4_PAYLOAD_PATTERN;
                    CreateFilterParams->trigger.ConnectionState =
                        SL_WLAN_RX_FILTER_STATE_STA_CONNECTED;
                    CreateFilterParams->trigger.Role =
                        SL_WLAN_RX_FILTER_ROLE_STA;
                }
                else if(!strcmp(token, "L4"))
                {
                    CreateFilterParams->rule.Header.Field =
                        SL_WLAN_RX_FILTER_HFIELD_L4_PAYLOAD_PATTERN;
                    CreateFilterParams->trigger.ConnectionState =
                        SL_WLAN_RX_FILTER_STATE_STA_NOT_CONNECTED;
                    CreateFilterParams->trigger.Role =
                        SL_WLAN_RX_FILTER_ROLE_STA;
                }
                else
                {
                    UART_PRINT(
                        "\r\n [Cmd Parser] : Invalid option for 'Mode'"
                        " argument. Using default (L4_CON).\n\r");
                    CreateFilterParams->rule.Header.Field =
                        SL_WLAN_RX_FILTER_HFIELD_L4_PAYLOAD_PATTERN;
                    CreateFilterParams->trigger.ConnectionState =
                        SL_WLAN_RX_FILTER_STATE_STA_CONNECTED;
                    CreateFilterParams->trigger.Role =
                        SL_WLAN_RX_FILTER_ROLE_STA;
                }
            }
            else
            {
                UART_PRINT(
                    "\r\n [Cmd Parser] : Invalid option for "
                    "'Mode' argument.\n\r");
                help = TRUE;
                break;
            }
        }
        else if(!strcmp(token, (char*)&o_optionStr))
        {
            token = strtok(NULL, (char*)space_str);

            if(token)
            {
                offset = (int32_t)atol((const char*)token);
            }
            else
            {
                offset = 0;
                UART_PRINT(
                    "\r\n [Cmd Parser] : Invalid option for 'Offset' "
                    "argument. Using default: (0)\n\r");
            }
        }
        /* USER event ID goes here */
        else if(!strcmp(token, (char*)&i_optionStr))
        {
            token = strtok(NULL, space_str);

            if(token)
            {
                CreateFilterParams->trigger.ParentFilterID = (char)atol(token);
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, " ");
    }

    if(help)
    {
        printCreateFilterUsage(arg);
        return(-1);
    }

    if((filterType == NULL) || (cValue == NULL))
    {
        UART_PRINT(
            "\r\n [Cmd Parser] : Please enter valid filter type"
            " and compare value.\n\r");
        printCreateFilterUsage(arg);
        return(-1);
    }

    /* Set filter rule type as basic header filter. */
    CreateFilterParams->ruleType = SL_WLAN_RX_FILTER_HEADER;

    /* Comparison is done as binary numbers rather than by ASCII characters */
    CreateFilterParams->flags = SL_WLAN_RX_FILTER_BINARY;

    /* Set Header field according to filter type */
    if(!strcmp((char*)filterType, (char*)SOURCE_MAC_str))
    {
        CreateFilterParams->rule.Header.Field =
            SL_WLAN_RX_FILTER_HFIELD_MAC_SRC_ADDR;

        memset(CreateFilterParams->rule.Header.Args.Mask, 0xFF,
               SL_WLAN_BSSID_LENGTH);

        ret = macAddressParse(cValue,
                              CreateFilterParams->rule.Header.Args.Value.Mac[0]);
        ASSERT_ON_ERROR(ret, CMD_ERROR);
    }
    else if(!strcmp((char*)filterType, (char*)DESTINATION_MAC_str))
    {
        CreateFilterParams->rule.Header.Field =
            SL_WLAN_RX_FILTER_HFIELD_MAC_DST_ADDR;

        memset(CreateFilterParams->rule.Header.Args.Mask, 0xFF,
               SL_WLAN_BSSID_LENGTH);

        ret = macAddressParse(cValue,
                              CreateFilterParams->rule.Header.Args.Value.Mac[0]);
        ASSERT_ON_ERROR(ret, CMD_ERROR);
    }
    else if(!strcmp((char*)filterType, (char*)BSSID_str))
    {
        CreateFilterParams->rule.Header.Field = SL_WLAN_RX_FILTER_HFIELD_BSSID;

        memset(CreateFilterParams->rule.Header.Args.Mask, 0xFF,
               SL_WLAN_BSSID_LENGTH);

        ret = macAddressParse(
            cValue, CreateFilterParams->rule.Header.Args.Value.Bssid[0]);
        ASSERT_ON_ERROR(ret, CMD_ERROR);
    }
    else if(!strcmp((char*)filterType, (char*)S_IP_str))
    {
        CreateFilterParams->rule.Header.Field =
            SL_WLAN_RX_FILTER_HFIELD_IPV4_SRC_ADDR;

        token = strchr((char *)cValue, ':');
        if(token)
        {
            /* Ipv6 */
            ret = ipv6AddressParse(
                cValue, CreateFilterParams->rule.Header.Args.Value.Ipv6[0]);
            memset(CreateFilterParams->rule.Header.Args.Mask, 0xFF,
                   IPV6_ADDR_LEN);
        }
        else
        {
            /* Ipv4 */
            ret = ipv4AddressParse(cValue, &ip4add);
            CreateFilterParams->rule.Header.Args.Value.Ipv4[0][0] =
                (uint8_t)(ip4add >> 24);
            CreateFilterParams->rule.Header.Args.Value.Ipv4[0][1] =
                (uint8_t)(ip4add >> 16);
            CreateFilterParams->rule.Header.Args.Value.Ipv4[0][2] =
                (uint8_t)(ip4add >> 8);
            CreateFilterParams->rule.Header.Args.Value.Ipv4[0][3] =
                (uint8_t)(ip4add);

            memset(CreateFilterParams->rule.Header.Args.Mask, 0xFF,
                   IPV4_ADDR_LEN);
        }
        ASSERT_ON_ERROR(ret, CMD_ERROR);
    }
    else if(!strcmp((char*)filterType, (char*)D_IP_str))
    {
        CreateFilterParams->rule.Header.Field =
            SL_WLAN_RX_FILTER_HFIELD_IPV4_DST_ADDR;
        token = strchr((char*)cValue, ':');
        if(token)
        {
            /* Ipv6 */
            ret = ipv6AddressParse(
                cValue, CreateFilterParams->rule.Header.Args.Value.Ipv6[0]);
            memset(CreateFilterParams->rule.Header.Args.Mask, 0xFF,
                   IPV6_ADDR_LEN);
        }
        else
        {
            /* Ipv4 */
            ip4add = 0;

            ret = ipv4AddressParse(cValue, &ip4add);
            CreateFilterParams->rule.Header.Args.Value.Ipv4[0][0] =
                (uint8_t)(ip4add >> 24);
            CreateFilterParams->rule.Header.Args.Value.Ipv4[0][1] =
                (uint8_t)(ip4add >> 16);
            CreateFilterParams->rule.Header.Args.Value.Ipv4[0][2] =
                (uint8_t)(ip4add >> 8);
            CreateFilterParams->rule.Header.Args.Value.Ipv4[0][3] =
                (uint8_t)(ip4add);

            memset(CreateFilterParams->rule.Header.Args.Mask, 0xFF,
                   IPV4_ADDR_LEN);
        }
        ASSERT_ON_ERROR(ret, CMD_ERROR);
    }
    else if(!strcmp((char*)filterType, (char*)FRAME_TYPE_str))
    {
        CreateFilterParams->rule.Header.Field =
            SL_WLAN_RX_FILTER_HFIELD_FRAME_TYPE;

        /* check the frame type validity */
        /* There are 3 frame types: (0 - management, 1 - Control, 2 - Data) */
        if(!strcmp(cValue, "management"))
        {
            ret = 0;
            CreateFilterParams->rule.Header.Args.Value.Frametype[0] = 0;
        }
        else if(!strcmp(cValue, "control"))
        {
            ret = 0;
            CreateFilterParams->rule.Header.Args.Value.Frametype[0] = 1;
        }
        else if(!strcmp(cValue, "data"))
        {
            ret = 0;
            CreateFilterParams->rule.Header.Args.Value.Frametype[0] = 2;
        }
        ASSERT_ON_ERROR(ret, CMD_ERROR);

        CreateFilterParams->rule.Header.Args.Value.Frametype[0] |=
            FRAME_TYPE_MASK;
    }
    else if(!strcmp((char*)filterType, (char*)FRAME_SUBTYPE_str))
    {
        if(CreateFilterParams->trigger.ParentFilterID == 0)
        {
            UART_PRINT(
                "\r\n [Cmd Parser] : Parent frame type filter ID "
                "is required for frame sub-type filter.\n\r");
            return(-1);
        }
        else
        {
            /* check frame sub type validity */
            ret = hexbyteStrtoASCII(cValue, &Frametype);
            ret = CheckSubFrameType(&Frametype);

            if(0 != ret)
            {
                UART_PRINT(
                    "\r\n [Cmd Parser] : frame sub-type argument is invalid."
                    " Supported sub type codes:\n\r");
                printFrameSubTyps();
                return(-1);
            }
        }

        CreateFilterParams->rule.Header.Field =
            SL_WLAN_RX_FILTER_HFIELD_FRAME_SUBTYPE;
        memset(&CreateFilterParams->rule.Header.Args.Mask, FRAME_SUBTYPE_MASK,
               1);

        ret = hexbyteStrtoASCII(
            cValue, CreateFilterParams->rule.Header.Args.Value.FrameSubtype);
        ASSERT_ON_ERROR(ret, CMD_ERROR);
        CreateFilterParams->rule.Header.Args.Value.FrameSubtype[0] &=
            FRAME_SUBTYPE_MASK;
    }
    else if(!strcmp((char*)filterType, (char*)PATTERN_str))
    {
        patternLen = strlen(cValue);

        /* Check offset validity */
        if((offset < 0) || (offset > (MAX_PAYLOAD_SIZE - patternLen)))
        {
            UART_PRINT(
                "\r\n [Cmd Parser] : For pattern matching filter, you must"
                " enter valid offset in packet:"
                "[0, max payload length - pattern length]\n\r");
            return(-1);
        }
        else
        {
            CreateFilterParams->rule.Header.Args.Value.Pattern.Offset = offset;

            /* Check pattern validity */
            if((patternLen <= 16) && (patternLen != 0))
            {
                memcpy(
                    &CreateFilterParams->rule.Header.Args.Value.Pattern.Value,
                    cValue,
                    CreateFilterParams->rule.Header.Args.Value.Pattern.Length);
                CreateFilterParams->rule.Header.Args.Value.Pattern.Length =
                    (patternLen - 1);
            }
            else
            {
                UART_PRINT(
                    "\r\n [Cmd Parser] : pattern length argument is invalid."
                    " should be in range: [1, 16]\n\r");
                return(-1);
            }
            CreateFilterParams->rule.Header.Args.Value.Pattern.Reserved = 0;

            /* This mask determines which part of 
            the 16 byte pattern would be checked, 
            to enable partial comparison */
            memset(&CreateFilterParams->rule.Header.Args.Mask, 0xFF, 16);
        }
    }
    else
    {
        UART_PRINT(
            "\r\n [Cmd Parser] : Invalid filter type.\n\r Filter types are:"
            " [S_MAC, D_MAC, BSSID, S_IP, D_IP, "
            "FRAME_TYPE, FRAME_SUBTYPE].\n\r");
        return(-1);
    }

    CreateFilterParams->trigger.Counter = SL_WLAN_RX_FILTER_NO_TRIGGER_COUNTER;

    /* If host event was selected, take the event ID: */
    if(hostEvent)
    {
        UART_PRINT(
            "\r\n [Cmd Parser] : Please enter user ID, in range: [0,63]"
            " (needed for Host event action).\n\r");
        UART_PRINT(cmdPromptStr);
        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, " ");
        if(token)
        {
            CreateFilterParams->action.UserId = (uint8_t)atol(token);
        }
        else
        {
            UART_PRINT(
                "\n\r [Cmd Parser]: Invalid range.\n\rrange: [0,63]"
                " (needed for Host event action).\n\r");
            return(-1);
        }
    }

    return(0);
}

/*!
    \brief          Parse enable WowLan command.

    This routine takes a WoWLANEnableCmd_t structure,
    and fill it's content with
    parameters taken from command line. 
    It checks the parameters validity.
    In case of a parsing error or invalid parameters,
    this function prints help menu.

    \param          arg                 -   Points to command line buffer.
                                            Contains the command line 
                                            typed by user.

    \param          WoWLANEnableParams  -   Points to command 
                                            structure provided 
                                            by the WowLan callback.
                                            This structure will later 
                                            be read by the
                                            the WowLan callback.

    \return         Upon successful completion, 
                    the function shall return 0.
                    In case of failure,
                    this function would print error, and show
                    the set enablewowlan command help menu.

    \sa             cmdEnableWoWLANCallback
 */
int32_t ParseEnableWoWLANCmd(void *arg,
                             WoWLANEnableCmd_t *WoWLANEnableParams)
{
    char cmdStr[CMD_BUFFER_LEN + 1];
    char                          *token = NULL;
    uint16_t offset = 0;
    uint8_t actionID = 0;
    uint8_t help = FALSE;
    uint8_t wowStr = FALSE;

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(token == NULL)
    {
        help = TRUE;
    }

    while(token)
    {
        if(!strcmp(token, (char*)help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, (char*)v_optionStr))
        {
            token = strtok(NULL, (char*)space_str);
            if(token)
            {
                if(*token != '"')
                {
                    UART_PRINT(
                        "\n\r[cmd parser] : enablewowlan expects the pattern"
                        " value in quotation marks (/"
                        ").\r\n");
                    help = TRUE;
                    break;
                }

                token++;

                WoWLANEnableParams->rule.Header.Args.Value.Pattern.Length =
                    (strlen(token) - 1);
                if(WoWLANEnableParams->rule.Header.Args.Value.Pattern.Length <=
                   16)
                {
                    memcpy(
                        &WoWLANEnableParams->rule.Header.Args.Value.Pattern.
                        Value,
                        token,
                        WoWLANEnableParams->rule.Header.Args.Value.Pattern.
                        Length);
                }
                else
                {
                    SHOW_WARNING(-1, CMD_ERROR);
                    help = TRUE;
                    break;
                }
                /* This mask determines which filters would be triggered */
                memset(&WoWLANEnableParams->rule.Header.Args.Mask, 0xFF, 16);
                wowStr = TRUE;
            }
        }
        else if(!strcmp(token, (char*)i_optionStr))
        {
            token = strtok(NULL, (char*)space_str);

            if(token)
            {
                offset = (uint16_t)atol((const char*)token);
                WoWLANEnableParams->rule.Header.Args.Value.Pattern.Offset =
                    offset;
            }
            else
            {
                WoWLANEnableParams->rule.Header.Args.Value.Pattern.Offset = 0;
            }

            WoWLANEnableParams->rule.Header.Args.Value.Pattern.Reserved = 0;
        }
        else if(!strcmp(token, (char*)u_optionStr))
        {
            token = strtok(NULL, (char*)space_str);

            if(token)
            {
                /* this sets the action as Host event,
                and UserId sets the bit corresponding to the filter */
                WoWLANEnableParams->action.UserId = (uint8_t)atol(token);
                actionID = TRUE;
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, " ");
    }

    if(!wowStr || !actionID)
    {
        help = TRUE;
    }

    if(help)
    {
        printEnableWoWLANUsage(arg);
        return(-1);
    }

    return(0);
}

/*!
    \brief          Parse Ping command.

    This routine takes a PingCmd_t structure,
    and fill it's content with
    parameters taken from command line.
    It checks the parameters validity.
    In case of a parsing error or invalid parameters,
    this function prints help menu.

    \param          arg         -   Points to command line buffer.
                                    Contains the command line typed by user.

    \param          pingParams  -   Points to command structure provided 
                                    by the Ping callback.
                                    This structure will later be read
                                    by the the Ping callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure,
                    this function would print error, and show
                    the set Ping command help menu.

    \sa             cmdPingCallback
 */
int32_t ParsePingCmd(void *arg,
                     PingCmd_t *pingParams)
{
    int32_t n;
    char            *token = NULL;
    char            *host = NULL;
    char            *r_name = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    uint8_t help = FALSE;

    /* Fill ping parameters with default values */
    pingParams->pingCmd.PingIntervalTime = 1000;
    pingParams->pingCmd.PingSize = 56;
    pingParams->pingCmd.PingRequestTimeout = 3000;
    pingParams->pingCmd.TotalNumberOfAttempts = 10;
    /* Report after each ping packet is successful */   
    pingParams->pingCmd.Flags = 1;               

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(NULL == token)
    {
        help = TRUE;
    }

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, h_optionStr))
        {
            host = strtok(NULL, space_str);
        }
        else if(!strcmp(token, c_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token != NULL)
            {
                n = atol(token);
                if((n > 0) && (n < MAX_PING_SIZE))
                {
                    pingParams->pingCmd.TotalNumberOfAttempts = n;
                }
                else
                {
                    UART_PRINT(
                        "\n\r[Cmd Parser]: 'Num of packets' argument is"
                        " invalid. Using default: (10).\n\r");
                }
            }
        }
        else if(!strcmp(token, i_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token != NULL)
            {
                n = atol(token);
                if(n > 0)
                {
                    pingParams->pingCmd.PingIntervalTime = 1000 * n;
                }
                else
                {
                    UART_PRINT(
                        "\n\r[Cmd Parser]: 'delay interval' argument is"
                        " invalid. Using default: (1 sec).\n\r");
                }
            }
        }
        else if(!strcmp(token, s_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token != NULL)
            {
                n = atol(token);
                if(n > 0)
                {
                    pingParams->pingCmd.PingSize = n;
                }
                else
                {
                    UART_PRINT(
                        "\n\r[Cmd Parser]: 'data size' argument is"
                        " invalid. Using default: (56).\n\r");
                }
            }
        }
        else if(!strcmp(token, t_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token != NULL)
            {
                n = atol(token);
                if(n > 0)
                {
                    pingParams->pingCmd.PingRequestTimeout = n * 1000;
                }
                else
                {
                    UART_PRINT(
                        "\n\r[Cmd Parser]: 'timeout' argument is invalid."
                        " Using default: (3 seconds per ping packet).\n\r");
                }
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, " ");
    }

    if(!host)
    {
        UART_PRINT(
            "\n\r[Cmd Parser]: Parser is expecting host name / IP address\n\r");
        help = TRUE;
    }
    else
    {
        r_name = strstr((const char *)host, "www.");

        if(!r_name)
        {
            r_name = host;
        }

        pingParams->host = calloc(sizeof(uint8_t), strlen(r_name));
        strcpy((char *)pingParams->host, r_name);
    }

    if(help)
    {
        printPingUsage(arg);
        return(-1);
    }

    return(0);
}

/*!
    \brief          Free Ping command.

    This routine takes a PingCmd_t structure, and free all memory that was
    allocated in ParsePingCmd.

    \param          pingParams  -   Points to command structure provided by the
                                    ping callback.

    \return         void

    \sa             cmdPingCallback
 */
void FreePingCmd(PingCmd_t *pingParams)
{
    if(pingParams->host != NULL)
    {
        free(pingParams->host);
        pingParams->host = NULL;
    }

    return;
}

/*!
    \brief          Parse mDNS advertise command.

    This routine takes a mDnsAdvertiseCmd_t structure,
    and fill it's content with
    parameters taken from command line. It checks the
    parameters validity.
    In case of a parsing error or invalid parameters,

    this function prints help menu.
    \param          arg                  -   Points to 
                                             command line buffer.
                                             Contains the command
                                             line typed by user.

    \param          mDNSAdvertiseParams  -   Points to command structure 
                                             provided by the mDNS 
                                             advertise callback.
                                             This structure will later be
                                             read by the the mDNS 
                                             advertise callback.

    \return         Upon successful completion,
                    the function shall return 0.
                    In case of failure, this function 
                    would print error, and show
                    the set mDNS advertise command help menu.

    \sa             mDNSAdvertiseCallback
 */
int32_t ParsemDNSAdvertiseCmd(void *arg,
                              mDnsAdvertiseCmd_t *mDNSAdvertiseParams)
{
    int32_t length = -1;
    uint32_t help = FALSE;

    /* Fill default parameters */
    mDNSAdvertiseParams->service_port = DEF_ADVERTISE_PORT;
    mDNSAdvertiseParams->service_ttl = DEF_ADVERTISE_TTL;

    char            *service_name = NULL;
    char            *adv_text = NULL;
    char            *p_adv_service_over = NULL;
    char            *adv_service_type = NULL;
    char            *token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, n_optionStr))
        {
            service_name = strtok(NULL, space_str);
        }
        else if(!strcmp(token, st_optionStr))
        {
            adv_service_type = strtok(NULL,space_str);
        }
        else if(!strcmp(token, so_optionStr))
        {
            p_adv_service_over = strtok(NULL, space_str);
        }
        else if(!strcmp(token, t_optionStr))
        {
            adv_text = strtok(NULL, "\"");
        }
        else if(!strcmp(token, p_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                mDNSAdvertiseParams->service_port = (uint16_t)atoi(token);
            }
            else
            {
                UART_PRINT(
                    "\n\r[Cmd Parser]: Service port was not set,"
                    " using default parameter: %d\n\r",
                    mDNSAdvertiseParams->service_port);
            }
        }
        else if(!strcmp(token, ttl_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                mDNSAdvertiseParams->service_ttl = (uint32_t)atoi(token);
            }
            else
            {
                UART_PRINT(
                    "\n\r[Cmd Parser]: Service ttl was not set, "
                    "using default parameter: %d\n\r",
                    mDNSAdvertiseParams->service_ttl);
            }
            if(!mDNSAdvertiseParams->service_ttl)
            {
                mDNSAdvertiseParams->service_ttl = DEF_ADVERTISE_TTL;
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, " ");
    }

    /* Sanity check the parameters */
    if(help || (!p_adv_service_over)  || (!service_name) || (!adv_service_type))
    {
        printmDNSAdvertiseUsage(arg);
        return(-1);
    }

    /* Copy the text */
    if(adv_text)
    {
        mDNSAdvertiseParams->adv_text =
            (uint8_t *)calloc(sizeof(uint8_t), strlen(
                                  adv_text));
        strcpy((char *)mDNSAdvertiseParams->adv_text, adv_text);
    }
    if(service_name)
    {
        mDNSAdvertiseParams->dev_name =
            (uint8_t *)calloc(sizeof(uint8_t), strlen(
                                  service_name));
        strcpy((char *)mDNSAdvertiseParams->dev_name, service_name);
    }

    /* Compose the advertise address from it's parts: 
    name, service type and protocol */
    length =
        mDNScreateServiceName(service_name, p_adv_service_over,
                              adv_service_type,
                              (char *)&mDNSAdvertiseParams->service_name);

    if(length < 0)
    {
        return(-1);
    }

    return(0);
}

/*!
    \brief          Free mDNS advertise command.

    This routine takes a mDnsAdvertiseCmd_t structure,
    and free all memory that was
    allocated in ParsemDNSAdvertiseCmd.

    \param          mDNSAdvertiseParams  -   Points to command structure 
                                provided by the mDNSAdvertise callback.

    \return         void

    \sa             mDNSAdvertiseCallback
 */
void FreemDNSAdvertiseCmd(mDnsAdvertiseCmd_t *mDNSAdvertiseParams)
{
    if(mDNSAdvertiseParams->adv_text != NULL)
    {
        free(mDNSAdvertiseParams->adv_text);
        mDNSAdvertiseParams->adv_text = NULL;
    }

    if(mDNSAdvertiseParams->dev_name != NULL)
    {
        free(mDNSAdvertiseParams->dev_name);
        mDNSAdvertiseParams->dev_name = NULL;
    }

    return;
}

/*!
    \brief          Parse mDNS query command.

    This routine takes a mDnsQueryCmd_t structure, and fill it's content with
    parameters taken from command line. It checks the parameters validity.
    In case of a parsing error or invalid parameters,
    this function prints help menu.

    \param          arg             -   Points to command line buffer.
                                        Contains the command line 
                                        typed by user.

    \param          QueryCmdParams  -   Points to command structure provided 
                                        by the mDNS query callback.
                                        This structure will later be read by
                                        the the mDNS query callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would print error,
                    and show the mDNS query command help menu.

    \sa             mDNSQueryCallback
 */
int32_t ParsemDNSQueryCmd(void *arg,  mDnsQueryCmd_t *QueryCmdParams)
{
    char            *service_name[3] = {0};
    char            *token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    int8_t help = FALSE;
    int32_t ret;

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, n_optionStr))
        {
            service_name[0] = strtok(NULL, space_str);
        }
        else if(!strcmp(token, st_optionStr))
        {
            service_name[1] = strtok(NULL, space_str);
        }
        else if(!strcmp(token, so_optionStr))
        {
            service_name[2] = strtok(NULL, space_str);
        }
        else if(!strcmp(token, o_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                if(!strcmp(token, "YES") || !strcmp(token, "yes"))
                {
                    QueryCmdParams->OneShotFlag = TRUE;
                }
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }

        token = strtok(NULL, " ");
    }

    /* Perform input validation */
    if((help) || (service_name[1] == NULL) || (service_name[2] == NULL) ||
       (!IS_CONNECTED(app_CB.Status)))
    {
        printmDNSQueryUsage(arg);
        return(-1);
    }

    /* Compose the mDNS service name */
    ret =
        mDNScreateServiceName(
        service_name[0],
        service_name[2],
        service_name[1],
        (char *)&QueryCmdParams->ServiceName);

    if(ret <= 0)
    {
        printmDNSQueryUsage(arg);
        return(-1);
    }

    return(0);
}

/*!
    \brief          Compose mDNS advertise address.

    This routine takes individual strings, which represent
    each part of a mDNS advertise address
    and put them together in order to create one mDNS address.
    The Address is null terminated and allocated
    by the caller.

    \param          p_sevName           -   Points to a string
                                            containing device name. ex:'PC1'

    \param          p_servOver          -   Points to a string containing
                                            protocol for this service. ex:'tcp'

    \param          p_servType          -   Points to a string containing
                                            service type. ex: 'http'

    \param          p_adv_service_name  -   Container to store the 
                                            advertise service name.
                                            mDNS service names
                                            are of the following form:
                                            'PC1._http._tcp.local'.

    \return         Upon successful completion, the function shall return
                    the length of the composed name,
                    and place it in the provided container. The caller is 
                    responsible to maintain this container.

    \note           mDNS advertise length, is max MAX_SERVICE_NAME_LENGTH
                    (63) 64 including null termination character.
                    Also, this function doesn't check for input validity,
                    since all caller functions provide valid parameters.

    \sa             ParsemDNSAdvertiseCmd, ParsemDNSQueryCmd
 */
int32_t mDNScreateServiceName(char *p_sevName,
                              char *p_servOver,
                              char *p_servType,
                              char *p_adv_service_name)
{
    int32_t str_len = -1;
    char        *point = "._";
    char        *space = "_";
    char        *spoint = ".";
    char        *local = "local";

    /* query can be partial - without name.
     *  for example: _http._tcp.local -
     *  find available http servers over tcp
     */
    if(p_sevName != NULL)
    {
        str_len = strlen(p_sevName) + strlen(spoint);
    }

    str_len += (strlen(p_servOver) + strlen(p_servType) + strlen(local) + 5);

    memset(p_adv_service_name, 0x0, str_len);

    if(p_sevName != NULL)
    {
        strcat(p_adv_service_name, p_sevName);
        strcat(p_adv_service_name, (const char*)spoint);
    }

    strcat(p_adv_service_name, (const char*)space);
    strcat(p_adv_service_name, (const char*)p_servType);
    strcat(p_adv_service_name, (const char*)point);
    strcat(p_adv_service_name, (const char*)p_servOver);
    strcat(p_adv_service_name, (const char*)spoint);
    strcat(p_adv_service_name, (const char*)local);
    strcat(p_adv_service_name, (const char*)"\0");

    return(str_len);
}

/*!
    \brief          Parse Send command.

    This routine takes a SendCmd_t structure, 
    and fill it's content with
    parameters taken from command line.
    It checks the parameters validity.
    In case of a parsing error or invalid parameters, 
    this function prints help menu.

    \param          arg             -   Points to command line buffer.
                                        Contains the command 
                                        line typed by user.

    \param          SendCmdParams   -   Points to command structure
                                        provided by the Send callback.
                                        This structure will later be 
                                        read by the the Send callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, 
                    this function would print error, and show
                    the Send command help menu.

    \sa             cmdSendCallback
 */
int32_t ParseSendCmd(void *arg,  SendCmd_t *SendCmdParams)
{
    int32_t ret = 0;
    int8_t help = FALSE;
    char                    *ip = NULL;
    char                    *token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    /* Fill default to structures */
    SendCmdParams->numberOfPackets = 1000;
    SendCmdParams->udpFlag = FALSE;
    SendCmdParams->server = TRUE;
    SendCmdParams->nb = FALSE;
    SendCmdParams->ipv6 = FALSE;
    SendCmdParams->ip = NULL;

    if(token == NULL)
    {
        help = TRUE;
    }

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, u_optionStr))
        {
            SendCmdParams->udpFlag = TRUE;
        }
        else if(!strcmp(token, s_optionStr))
        {
            SendCmdParams->server = TRUE;
        }
        else if(!strcmp(token, V_optionStr))
        {
            SendCmdParams->ipv6 = TRUE;
        }
        else if(!strcmp(token, nb_optionStr))
        {
            SendCmdParams->nb = TRUE;
        }
        else if(!strcmp(token, p_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                SendCmdParams->portNumber = (unsigned short)atol(token);
            }
            else
            {
                UART_PRINT(
                    "\n\r[cmd Parser] : Port number was not set, "
                    "using default parameter: %d\n\r",
                    SendCmdParams->portNumber);
            }
        }
        else if(!strcmp(token, c_optionStr))
        {
            ip = (char*)strtok(NULL, space_str);
            if((ip != NULL) && (SendCmdParams->ip == NULL))
            {
                SendCmdParams->ip =
                    (uint8_t *)calloc(sizeof(uint8_t), strlen((const char*)ip));
                strcpy((char *)SendCmdParams->ip, ip);
            }
            else
            {
                UART_PRINT("\n\r[cmd Parser] : Invalid IP\n\r");
                return(-1);
            }

            SendCmdParams->server = FALSE;
        }
        else if(!strcmp(token, n_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                SendCmdParams->numberOfPackets = atol(token);
            }
            else
            {
                UART_PRINT(
                    "\n\r[cmd Parser] : Number Of Packets was not set, "
                    "using default parameter: %d\n\r",
                    SendCmdParams->numberOfPackets);
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, space_str);
    }

    if(help)
    {
        ret = printSendUsage(arg);
        return(-1);
    }

    if(!SendCmdParams->server)
    {
        token = NULL;
        token = strchr((const char*)SendCmdParams->ip, ':');
        if(token)
        {
            ret = ipv6AddressParse((char*)SendCmdParams->ip,
                                   (uint8_t*)&SendCmdParams->ipAddr.ipv6);
            SendCmdParams->ipv6 = TRUE;
        }
        else
        {
            ret = ipv4AddressParse((char*)SendCmdParams->ip,
                                   (uint32_t*)&SendCmdParams->ipAddr.ipv4);
            SendCmdParams->ipv6 = FALSE;
        }
    }

    if(ret < 0)
    {
        UART_PRINT("\n\r[cmd Parser] : Invalid IP\n\r");
        return(-1);
    }

    /* Perform a sanity check, since the device has 
    to be connected in order to send packets. */
    if(SendCmdParams->ipv6)
    {
        if(!IS_IPV6G_ACQUIRED(app_CB.Status) &&
           !IS_IPV6L_ACQUIRED(app_CB.Status) && !IS_CONNECTED(app_CB.Status))
        {
            UART_PRINT(
                "\n\r[cmd Parser] : (error) Cannot send data if device is "
                "disconnected from network.\n\r");
            return(-1);
        }
    }
    else
    {
        if(!IS_IP_ACQUIRED(app_CB.Status) && !IS_CONNECTED(app_CB.Status))
        {
            UART_PRINT(
                "\n\r[cmd Parser] : (error) Cannot send data if device is "
                "disconnected from network.\n\r");
            return(-1);
        }
    }

    if(SendCmdParams->numberOfPackets <= 0)
    {
        SendCmdParams->numberOfPackets = 1000;
    }

    if(SendCmdParams->portNumber <= 0)
    {
        SendCmdParams->portNumber = 5001;
    }

    UART_PRINT("\n\r");
    UART_PRINT("Protocol: %s\n\r", SendCmdParams->udpFlag ? "udp" : "tcp");
    UART_PRINT("Traffic: Tx\n\r");
    UART_PRINT("Client\\Server: ");

    if(SendCmdParams->server)
    {
        UART_PRINT("Server\n\r");
    }
    else
    {
        UART_PRINT("Client\n\r");

        if(SendCmdParams->ipv6)
        {
            PrintIPAddress(SendCmdParams->ipv6,
                           (void*)&SendCmdParams->ipAddr.ipv6);
            UART_PRINT(lineBreak);
        }
        else
        {
            PrintIPAddress(SendCmdParams->ipv6,
                           (void*)&SendCmdParams->ipAddr.ipv4);
            UART_PRINT(lineBreak);
        }
    }

    UART_PRINT("Port: %d\n\r", SendCmdParams->portNumber);
    UART_PRINT("Number of Packets to send: %d\n\r",
               SendCmdParams->numberOfPackets);
    UART_PRINT("\n\r");

    return(0);
}

/*!
    \brief          Free Send command.

    This routine takes a SendCmd_t structure, and free all memory that was
    allocated in ParseSendCmd.

    \param          SendCmdParams  -   Points to command
                                       structure provided by the send callback.

    \return         void

    \sa             cmdSendCallback
 */
void FreeSendCmd(SendCmd_t *SendCmdParams)
{
    if(SendCmdParams->ip != NULL)
    {
        free(SendCmdParams->ip);
        SendCmdParams->ip = NULL;
    }

    return;
}

/*!
    \brief          Parse Receive command.

    This routine takes a RecvCmd_t structure, and fill it's content with
    parameters taken from command line. It checks the parameters validity.
    In case of a parsing error or invalid parameters, 
    this function prints help menu.

    \param          arg             -   Points to command line buffer.
                                        Contains the command line
                                        typed by user.

    \param          RecvCmdParams   -   Points to command structure provided
                                        by the Recv callback.
                                        This structure will later be read 
                                        by the the Recv callback.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would print error,
                    and show the Send command help menu.

    \sa             cmdRecvCallback
 */
int32_t ParseRecvCmd(void *arg,  RecvCmd_t *RecvCmdParams)
{
    int32_t ret = 0;
    int8_t help = FALSE;
    char                    *ip = NULL;
    char                    *token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    /* Fill default to structures */
    RecvCmdParams->numberOfPackets = 1000;
    RecvCmdParams->udpFlag = FALSE;
    RecvCmdParams->server = TRUE;
    RecvCmdParams->nb = FALSE;
    RecvCmdParams->ipv6 = FALSE;
    RecvCmdParams->portNumber = 5001;
    RecvCmdParams->ip = NULL;

    strncpy(cmdStr, (char*) arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(token == NULL)
    {
        help = TRUE;
    }

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, u_optionStr))
        {
            RecvCmdParams->udpFlag = TRUE;
        }
        else if(!strcmp(token, s_optionStr))
        {
            RecvCmdParams->server = TRUE;
        }
        else if(!strcmp(token, V_optionStr))
        {
            RecvCmdParams->ipv6 = TRUE;
        }
        else if(!strcmp(token, nb_optionStr))
        {
            RecvCmdParams->nb = TRUE;
        }
        else if(!strcmp(token, p_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                RecvCmdParams->portNumber = (unsigned short)atol(token);
            }
            else
            {
                UART_PRINT(
                    "\n\r[cmd Parser] : Port number was not set, using default"
                    " parameter: %d\n\r",
                    RecvCmdParams->portNumber);
            }
        }
        else if(!strcmp(token, c_optionStr))
        {
            ip = (char*)strtok(NULL, space_str);
            if((ip != NULL) && (RecvCmdParams->ip == NULL))
            {
                RecvCmdParams->ip =
                    (uint8_t *)calloc(sizeof(uint8_t), strlen((const char*)ip));
                strcpy((char *)RecvCmdParams->ip, ip);
            }
            else
            {
                UART_PRINT("\n\r[cmd Parser] : Invalid IP\n\r");
                return(-1);
            }

            RecvCmdParams->server = FALSE;
        }
        else if(!strcmp(token, n_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                RecvCmdParams->numberOfPackets = atol(token);
            }
            else
            {
                UART_PRINT(
                    "\n\r[cmd Parser] : Number Of Packets was not set, using"
                    " default parameter: %d\n\r",
                    RecvCmdParams->numberOfPackets);
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, space_str);
    }

    if(help)
    {
        printRecvUsage(arg);
        return(-1);
    }

    if(!RecvCmdParams->server)
    {
        token = NULL;
        token = strchr((const char*)RecvCmdParams->ip, ':');
        if(token)
        {
            ret = ipv6AddressParse((char*)RecvCmdParams->ip,
                                   (uint8_t*)&RecvCmdParams->ipAddr.ipv6);
            RecvCmdParams->ipv6 = TRUE;
        }
        else
        {
            ret = ipv4AddressParse((char*)RecvCmdParams->ip,
                                   (uint32_t *)&RecvCmdParams->ipAddr.ipv4);
            RecvCmdParams->ipv6 = FALSE;
        }
    }

    /* Perform a sanity check,
       since the device has to be connected in order to send packets. */
    if(RecvCmdParams->ipv6)
    {
        if(!IS_IPV6G_ACQUIRED(app_CB.Status) &&
           !IS_IPV6L_ACQUIRED(app_CB.Status) && !IS_CONNECTED(app_CB.Status))
        {
            UART_PRINT(
                "\n\r[cmd Parser] : (error) Cannot receive data if device"
                " is disconnected from network.\n\r");
            return(-1);
        }
    }
    else
    {
        if(!IS_IP_ACQUIRED(app_CB.Status) && !IS_CONNECTED(app_CB.Status))
        {
            UART_PRINT(
                "\n\r[cmd Parser] : (error)Cannot send data if device"
                " is disconnected from network.\n\r");
            return(-1);
        }
    }

    if(ret < 0)
    {
        UART_PRINT("\n\r[cmd Parser] : Invalid IP\n\r");
        return(-1);
    }

    if(!RecvCmdParams->numberOfPackets)
    {
        RecvCmdParams->numberOfPackets = 1000;
    }

    if(RecvCmdParams->portNumber <= 0)
    {
        RecvCmdParams->portNumber = 5001;
    }

    UART_PRINT("\n\r");
    UART_PRINT("Protocol: %s\n\r", RecvCmdParams->udpFlag ? "udp" : "tcp");
    UART_PRINT("Traffic: Rx\n\r");
    UART_PRINT("Client\\Server: ");

    if(RecvCmdParams->server)
    {
        UART_PRINT("Server\n\r");
    }
    else
    {
        UART_PRINT("Client\n\r");
        UART_PRINT("Client IP: ");

        if(RecvCmdParams->ipv6)
        {
            PrintIPAddress(RecvCmdParams->ipv6,
                           (void*)&RecvCmdParams->ipAddr.ipv6);
        }
        else
        {
            PrintIPAddress(RecvCmdParams->ipv6,
                           (void*)&RecvCmdParams->ipAddr.ipv4);
        }

        UART_PRINT(lineBreak);
    }

    UART_PRINT("Port: %d\n\r", RecvCmdParams->portNumber);
    UART_PRINT("Number of Packets to receive: %d\n\r",
               RecvCmdParams->numberOfPackets);
    UART_PRINT("\n\r");

    return(0);
}





/*!
    \brief          Parse Set Country Code command.

    This routine parse set country code parameters.
    In case of a parsing error or invalid parameters,
	this function prints help menu.

    \param          arg             -   Points to command line buffer.
                                        Contains the command line typed by 
										user.
                    InputPad        -   Pad number to assign for BLE input
                    OutputPad       -   Pad number to assign for RF switch 
						                control

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would print error.

    \sa             ParseCountycodeCmd
 */

int32_t ParseCountycodeCmd(void *arg, uint8_t *country)
{
    char               *token = NULL;
    uint8_t            *country_input = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    uint8_t help = FALSE;

    strncpy(cmdStr, (char*)arg, CMD_BUFFER_LEN);
    cmdStr[CMD_BUFFER_LEN] = '\0';
    token = strtok(cmdStr, space_str);

    if(NULL == token)
    {
        help = TRUE;
    }

    while(token)
    {
        if(!strcmp(token, help_optionStr))
        {
            help = TRUE;
        }
        else if(!strcmp(token, g_optionStr))
        {
            token = strtok(NULL, space_str);
            if(token)
            {
                country_input = (uint8_t *)token;
            }
        }
        else
        {
            SHOW_WARNING(-1, CMD_ERROR);
            help = TRUE;
            break;
        }
        token = strtok(NULL, " ");
    }

    if(!((strlen((char *)country_input) == 2) &&
         (country_input[0] >= 'A') && (country_input[0] <= 'Z') &&
         (country_input[1] >= 'A') && (country_input[1] <= 'Z')))
    {
        UART_PRINT("\n\rInvalid input \n\r");
        help = TRUE;
    }

    if(help)
    {
        printCountrycodeeUsage(arg);
        return(-1);
    }

    memcpy(country, country_input, 2);
    return(0);
}

/*!
    \brief          Free Recv command.

    This routine takes a RecvCmd_t structure, and free all memory that was
    allocated in RecvCmdParams.

    \param          RecvCmdParams  -   Points to command 
                                       structure provided by the send callback.

    \return         void

    \sa             cmdRecvCallback
 */
void FreeRecvCmd(RecvCmd_t *RecvCmdParams)
{
    if(RecvCmdParams->ip != NULL)
    {
        free(RecvCmdParams->ip);
        RecvCmdParams->ip = NULL;
    }

    return;
}
