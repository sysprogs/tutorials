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

/* standard includes */
#include <stdlib.h>
#include <stdint.h>

/* Example/Board Header files */
#include "network_terminal.h"
#include "radio_tool.h"
#include "socket_cmd.h"
#include "transceiver_cmd.h"

/* Application defines */

#define MENU_CMD_SIZE               (2)
#define MIN_DURATION                (1UL)
#define MAX_DURATION                (UINT32_MAX)
#define MAX_AMOUNT                  (65535UL)
#define MAX_DELAY                   (65535UL)
#define MAX_FRAME_SIZE              (1500UL)
#define MIN_FRAME_SIZE              (14UL)
#define DEPRECATED_RATE             (5UL)

/****************************************************************************
                      LOCAL FUNCTION PROTOTYPES
****************************************************************************/
int32_t RxTask();
int32_t TxTask();
void showMenu();
void showRxMenu();
void showTxMenu();
void showPreambleMenu();
void showCCAThreshHoldMenu();
void showModCodeMenu();
void showPowMenu();
void showPHYConfMenu();

/*****************************************************************************
                  Callback Functions
*****************************************************************************/

/*!
    \brief          cmdTranceiverModecallback

    This routine shows how to tap into the CC3X20 Radio.
    It sets the device in transceiver mode which provides the user
    a direct access to the device's transmitter and receiver,
    overriding the MAC and Transport layers.

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
                                  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \note           Calling this API causes NWP reset, meaning any connection
                    or non-persistent settings, will be lost.

    \sa             RxTask, TxTask

 */
int32_t cmdTranceiverModecallback(void *arg)
{
    int32_t ret = 0;
    uint8_t menuDisp = TRUE;
    uint8_t TaskRunning = TRUE;
    char menuOpt[MENU_CMD_SIZE];

    /* This call, resets the NWP in STA role, Configure scan policy to none,
     * and puts the device in transceiver mode.  */
    ret = radioTool_Open(NULL);

    while(TaskRunning)
    {
        if(menuDisp)
        {
            /* Prints the Main radiotool menu */
            showMenu();
            UART_PRINT(radiotool_PromptStr);
        }

        GetCmd((char *)&menuOpt, MENU_CMD_SIZE);

        if(!strcmp((char *)&menuOpt, "1"))
        {
            /* Calling RX radio options */
            ret = RxTask();
            menuDisp = TRUE;
        }
        else if(!strcmp((char *)&menuOpt, "2"))
        {
            /* Calling TX radio options */
            ret = TxTask();
            menuDisp = TRUE;
        }
        else if(!strcmp((char *)&menuOpt, "3"))
        {
            /* Exit transceiver mode */
            TaskRunning = FALSE;

            /* This call, resets the NWP, configure the NWP to a default state,
             * and exists radiotool.*/
            ret = radioTool_Close(NULL);
        }
        else
        {
            menuDisp = FALSE;
            UART_PRINT("\r\nInvalid option\r\n");
            UART_PRINT(radiotool_PromptStr);
        }
    }

    return(ret);
}



int32_t printTranceiverModeUsage(void *arg)
{
    UART_PRINT(lineBreak);
    UART_PRINT(usageStr);
    UART_PRINT(radiotool_Str);
    UART_PRINT(radiotool_Str);
    UART_PRINT(descriptionStr);
    UART_PRINT(radiotoolDetailsStr);
    UART_PRINT(help_optaionDetails);
    UART_PRINT(lineBreak);

    return(0);
}

/*****************************************************************************
                  Local Functions
*****************************************************************************/

/*!
    \brief          RxTask

    This routine opens the CC3X20's receiver on a specific channel
    for a given period of time, provided by the user's input.
    If requested, this function also collects and display RX statistics.
    For more information about Transceiver mode's key features,
    please refer to: CC31xx/CC32xx NWP programmer's guide (SWRU455)

    \param          arg       -   Points to command line buffer.
                                  This container would be passed
                                  to the parser module.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function would return -1;

    \sa             RxTask, TxTask, radioTool_RxStart

 */
int32_t RxTask()
{
    int32_t ret = 0;
    char                         *token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    Channel RxChannel = 1;
    uint32_t duration = 0;
    uint8_t Stats = TRUE;
    SlWlanGetRxStatResponse_t StatResp = {0};

    /* This code is responsible to Parse user's parameters */

    showRxMenu();

    UART_PRINT("\n\r\tChoose Rx channel: 2.4Ghz-[1,13] 5Ghz-[36,169]\n\r");
    UART_PRINT(lineBreak);
    UART_PRINT(radiotool_PromptStr);
    ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
    token = strtok(cmdStr, " ");
    if(token)
    {
        RxChannel = (Channel)atoi(token);
    }

    if(!( RxChannel >= 1 && RxChannel < 14))
    {
        UART_PRINT(
            "\n\r[Transceiver Mode Error]:"
            " Channel argument is out of range.\n\r");
        return(-1);
    }

    UART_PRINT("\n\r\tEnter Rx duration (in mSec units): [1,UINT32_MAX]\n\r");
    UART_PRINT(lineBreak);
    UART_PRINT(radiotool_PromptStr);
    ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
    token = strtok(cmdStr, " ");
    if(token)
    {
        duration = atoi(token);
    }

    if(duration < MIN_DURATION)
    {
        UART_PRINT(
            "\n\r[Transceiver Mode Error]:"
            " duration argument is out of range.\n\r");
        return(-1);
    }

    UART_PRINT("\n\r\tShow Rx statistics?: \n\r");
    UART_PRINT("\n\r\t1. Yes \n\r");
    UART_PRINT("\n\r\t2. No \n\r");
    UART_PRINT(lineBreak);
    UART_PRINT(radiotool_PromptStr);
    ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
    token = strtok(cmdStr, " ");

    if(token)
    {
        if(!strcmp(token, "1"))
        {
            Stats = TRUE;
        }
        else if(!strcmp(token, "2"))
        {
            Stats = FALSE;
        }
        else
        {
            UART_PRINT("\n\r[Transceiver Mode Error] : invalid argument.\n\r");
            return(-1);
        }
    }

    /* After parsing user's input,
     * Wait for user GO indication - in order to start the RX task. */
    UART_PRINT(
        "\n\r[Transceiver Mode] :"
        " Ready to start RX, press any key to start...\n\r");
    getch();

    UART_PRINT(
        "\n\r\n\r[Transceiver Mode] :"
        " Open Receiver on channel %d, for ~ %d mSec period...\n\r",
        RxChannel, duration);

    /* Now, we can start RX */
    ret = radioTool_RxStart(RxChannel, duration);
    ASSERT_ON_ERROR(ret, BSD_SOCKET_ERROR);

    /* Sleep for the duration of the RX period. */
    MSEC_SLEEP(duration);

    /* Now, we can stop RX */
    ret = radioTool_RxStop();
    ASSERT_ON_ERROR(ret, SL_SOCKET_ERROR);

    UART_PRINT("\n\r[Transceiver Mode] : Rx complete.\n\r");

    if(Stats)
    {
        /* Collect RX statistics */
        ret = radioTool_GetStats(&StatResp);

        /* Draw RX histogram */
        drawRxHist(&StatResp);

        UART_PRINT("\n\r[Transceiver Mode] : Press any key to continue...\n\r");
        Stats = (uint8_t)getch();
        if(Stats != FALSE)
        {
            return(ret);
        }
    }

    return(0);
}

/*!
    \brief          TxTask

    This routine shows how to transmit data / Continuous
    wave using the CC3X20's transmitter.
    The transmission parameters are defined by the user.
    This function offers three transmission modes:

        1. Continues: Where packets are generated using the internal packet
                      generator of the NWP. For more information, please
                      refer to:
                      CC31xx/CC32xx NWP programmer's guide (SWRU455)

        2. Packetized: where packets are generated in the APPS processor
                       and sent to the NWP over the SPI/UART interface.

        3. Carrier Wave : This mode doesn't transmit data,
                          but rather continues wave,
                          with center frequency and offset defined by the user.
                          For more information, please refer to:
                          CC31xx/CC32xx NWP programmer's guide (SWRU455)

    \param          arg       -   Points to command line buffer.
                                  This container would be 
                                  passed to the parser module.
                                  
    \return         Upon successful completion, this function shall return 0.
                    In case of failure, this function would return -1;

    \sa             RxTask, TxTask, radioTool_RxStart

 */
int32_t TxTask()
{
    int32_t ret = 0;
    char*                   token = NULL;
    char cmdStr[CMD_BUFFER_LEN + 1];
    uint32_t duration = 1000;
    Channel txchannel = 1;
    TxMode txType;
    uint32_t amount = 1000;
    uint32_t delay = 1;
    uint32_t packetSize = FRAME_SIZE;
    int8_t cwTone = 0;
    DataPattern pattern = DataPattern_all_0;
    uint8_t pDestMac[6] = {0};
    uint8_t cca_override = TRUE;
    SlWlanRateIndex_e Mod = SL_WLAN_RATE_1M;
    Preamble preamble_type = Preamble_longMode;
    PowerLevel txPower = PowerLevel_HighGainStep_7;
    SlTxInhibitThreshold_e cca_threadhold = SL_TX_INHIBIT_THRESHOLD_DEFAULT;

    /* This code is responsible to Parse user's parameters */
    showTxMenu();

    UART_PRINT(radiotool_PromptStr);
    ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
    token = strtok(cmdStr, " ");

    if(token)
    {
        if(!strcmp(token, "1"))
        {
            txType = TxMode_Continues;
        }
        else if(!strcmp(token, "2"))
        {
            txType = TxMode_Packetized;
        }
        else if(!strcmp(token, "3"))
        {
            txType = TxMode_CW;
        }
        else
        {
            UART_PRINT(
                "\n\r[Transceiver Mode Error]: Tx mode is out of range.\n\r");
            return(-1);
        }
    }
    else
    {
        UART_PRINT("\n\r[Transceiver Mode Error]: invalid argument.\n\r");
        return(-1);
    }

    if(txType == TxMode_CW)
    {
        UART_PRINT(Txtask_tone_optionDetailsStr);
        UART_PRINT(radiotool_PromptStr);
        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, " ");
        if(token)
        {
            cwTone = (uint8_t)atoi(token);
            if(cwTone > CW_HIGH_TONE || cwTone < CW_LOW_TONE)
            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]: "
                    "CW tone argument is out of range.\n\r");
                return(-1);
            }
        }
        else
        {
            UART_PRINT(
                "\n\r[Transceiver Mode Error]: CW tone argument is invalid.\n\r");
            return(-1);
        }

        UART_PRINT(radiotool_PromptStr);
        UART_PRINT("\n\rEnter TX duration: [1,UINT_MAX](mSec)\n\r");
        UART_PRINT(lineBreak);

        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, " ");

        if(token)
        {
            duration = atoi(token);
            if(duration < MIN_DURATION)
            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]:"
                    " duration argument is out of range.\n\r");
                return(-1);
            }
        }
        else
        {
            UART_PRINT(
             "\n\r[Transceiver Mode Error]: duration argument is invalid.\n\r");
            return(-1);
        }

        UART_PRINT("\n\rChoose Tx channel: 2.4Ghz-[1,13] 5Ghz-[36,169]\n\r");
        UART_PRINT(lineBreak);
        UART_PRINT(radiotool_PromptStr);
        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, " ");

        if(token)
        {
            txchannel = (Channel)atoi(token);
            if(!(txchannel >= 1 && txchannel < 14))
            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]:"
                    " Channel argument is out of range.\n\r");
                return(-1);
            }
        }
        else
        {
            UART_PRINT(
                "\n\r[Transceiver Mode Error]:"
                " channel argument is invalid.\n\r");
            return(-1);
        }
        /* Maybe add power here? */
    }
    else
    {
        /* These settings are common for Packetized & continues */
        UART_PRINT(
            "\n\rEnter destination MAC address:"
            " [six bytes, seperated by ':' (colon sign)]\n\r");
        UART_PRINT(lineBreak);
        UART_PRINT(radiotool_PromptStr);
        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, " ");

        if(token)
        {
            if(macAddressParse(token, (uint8_t*)&pDestMac))
            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]:"
                    " Invalid MAC address format.\n\r");
                return(-1);
            }
        }
        else
        {
            UART_PRINT("\n\r[Transceiver Mode Error]:"
            " Invalid MAC address.\n\r");
            return(-1);
        }

        UART_PRINT("\n\rEnter packet size: [14,1500](Bytes)\n\r");
        UART_PRINT(lineBreak);
        UART_PRINT(radiotool_PromptStr);
        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, space_str);

        if(token)
        {
            packetSize = atoi(token);
        }

        if((packetSize < MIN_FRAME_SIZE) || (packetSize > MAX_FRAME_SIZE))
        {
            UART_PRINT(
                "[Transceiver Mode Error]:"
                " packet size argument is out of range.\n\r");
            return(-1);
        }

        UART_PRINT("\n\rChoose Tx channel: 2.4Ghz-[1,13] 5Ghz-[36,169]\n\r");
        UART_PRINT(lineBreak);
        UART_PRINT(radiotool_PromptStr);
        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, " ");

        if(token)
        {
            txchannel = (Channel)atoi(token);
            if(!(txchannel >= 1 && txchannel < 14))

            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]:"
                    " Channel argument is out of range.\n\r");
                return(-1);
            }
        }
        else
        {
            UART_PRINT(
                "[Transceiver Mode Error]: Channel argument is invalid.\n\r");
        }

        UART_PRINT("\n\rChoose Packet Data pattern:\n\r");
        UART_PRINT("\n\r1. All zeros. \n\r");
        UART_PRINT("\n\r2. All ones. \n\r");
        UART_PRINT("\n\r3. Incremental.\n\r");
        UART_PRINT("\n\r4. Decremental.\n\r");
        UART_PRINT(lineBreak);
        UART_PRINT(radiotool_PromptStr);
        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, " ");

        if(token)
        {
            if(!strcmp(token, "1"))
            {
                pattern = DataPattern_all_0;
            }
            else if(!strcmp(token, "2"))
            {
                pattern = DataPattern_all_1;
            }
            else if(!strcmp(token, "3"))
            {
                pattern = DataPattern_incemental;
            }
            else if(!strcmp(token, "4"))
            {
                pattern = DataPattern_decremental;
            }
            else
            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]:"
                    " data pattern arguments are out of range.\n\r");
            }
        }

        if(txType == TxMode_Packetized)
        {
            UART_PRINT("\n\rChoose Number of packets to transmit: [1,%d]\n\r",
                       MAX_AMOUNT);
            UART_PRINT(lineBreak);
            UART_PRINT(radiotool_PromptStr);
            ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
            token = strtok(cmdStr, " ");
            if(token)
            {
                amount = atoi(token);
            }
            else
            {
                UART_PRINT(
                    "\n\rPacket amount was not set,"
                    " using default parameter: %d\n\r",
                    amount);
            }
            if(amount > MAX_AMOUNT)
            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]:"
                    " Number of packets argument is out of range.\n\r");
                return(-1);
            }

            UART_PRINT(
                "\n\rChoose delay time between each packet: [1,%d](mSec)\n\r",
                MAX_DELAY);
            UART_PRINT(lineBreak);
            UART_PRINT(radiotool_PromptStr);
            ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
            token = strtok(cmdStr, " ");
            if(token)
            {
                delay = atoi(token);
            }
            else
            {
                UART_PRINT(
                    "\n\rDelay was not set, using default parameter: %d\n\r",
                    delay);
            }
            if(delay > MAX_DELAY)
            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]:"
                    " delay time argument is out of range.\n\r");
                return(-1);
            }
        }

        if(txType == TxMode_Continues)
        {
            UART_PRINT("\n\rChoose TX burst duration: [1,UINT_MAX](mSec)\n\r");
            UART_PRINT(lineBreak);

            ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
            token = strtok(cmdStr, " ");
            if(token)
            {
                duration = atoi(token);
            }
            else
            {
                UART_PRINT(
                    "\n\rDuration was not set, using default parameter: %d\n\r",
                    duration);
            }
            UART_PRINT(radiotool_PromptStr);
            if(duration < MIN_DURATION)
            {
                UART_PRINT(
                    "\n\r[Transceiver Mode Error]:"
                    " duration argument is out of range.\n\r");
                return(-1);
            }
        }

        showPHYConfMenu();
        UART_PRINT(radiotool_PromptStr);
        ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
        token = strtok(cmdStr, " ");

        if(token)
        {
            if(!strcmp(token, "1"))
            {
                /* Get preamble type */
                showPreambleMenu();
                UART_PRINT(radiotool_PromptStr);
                ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
                token = strtok(cmdStr, " ");

                if(token)
                {
                    preamble_type = (Preamble)atoi(token);
                    if(preamble_type >= Preamble_MaxNumMode)
                    {
                        UART_PRINT(
                            "\n\r[Transceiver Mode Error]:"
                            " Preamble arguments are out of range.\n\r");
                        return(-1);
                    }
                }
                else
                {
                    UART_PRINT(
                        "\n\r[Transceiver Mode Error]:"
                        " data pattern arguments are out of range.\n\r");
                }

                /* Get Rate\Mod-Code index */
                showModCodeMenu();
                UART_PRINT(radiotool_PromptStr);
                ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
                token = strtok(cmdStr, " ");

                if(token)
                {
                    Mod = (SlWlanRateIndex_e)atoi(token);
                    if((Mod > SL_WLAN_RATE_MCS_7) || (Mod < SL_WLAN_RATE_1M) ||
                       (Mod == DEPRECATED_RATE))
                    {
                        UART_PRINT(
                            "\n\r[Transceiver Mode Error]:"
                            " Modulation arguments are out of range.\n\r");
                        return(-1);
                    }
                }
                else
                {
                    UART_PRINT(
                        "\n\r[Transceiver Mode Error]:"
                        " Modulation arguments are invalid.\n\r");
                }

                /* Get Power */
                showPowMenu();
                UART_PRINT(radiotool_PromptStr);
                ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
                token = strtok(cmdStr, " ");

                if(token)
                {
                    txPower = (PowerLevel)atoi(token);
                    if(txPower >= PowerLevel_MaxPowerLevel)
                    {
                        UART_PRINT(
                            "\n\r[Transceiver Mode Error]:"
                            " Power level arguments are out of range.\n\r");
                        return(-1);
                    }
                }
                else
                {
                    UART_PRINT(
                        "\n\r[Transceiver Mode Error]:"
                        " Power level arguments are invalid.\n\r");
                }

                /* CCA threshold and setup */

                UART_PRINT("\n\rOverride CCA mechanism: [Yes/No]\n\r");
                UART_PRINT(lineBreak);
                UART_PRINT(radiotool_PromptStr);
                ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
                token = strtok(cmdStr, " ");

                if(token)
                {
                    if(!strcmp(token, "YES") || !strcmp(token, "yes"))
                    {
                        cca_override = TRUE;
                    }
                    else if(!strcmp(token, "NO") || !strcmp(token, "no"))
                    {
                        cca_override = FALSE;
                    }
                    else
                    {
                        UART_PRINT(
                            "\n\r[Transceiver Mode Error]:"
                            " CCA override arguments are out of range."
							" Using default: (Yes).\n\r");
                    }
                }
                else
                {
                    UART_PRINT(
                        "\n\r[Transceiver Mode Error]:"
                        " CCA override arguments are invalid.\n\r");
                }

                /*  Override Clear Channel Assessment - (CCA)
                 *  Whether or not to sense the air and decides if
                 *  wer'e clear to TX or not.
                 */
                if(!cca_override)
                {
                    showCCAThreshHoldMenu();
                    UART_PRINT(radiotool_PromptStr);
                    ret = GetCmd((char *)cmdStr, CMD_BUFFER_LEN);
                    token = strtok(cmdStr, " ");

                    if(token)
                    {
                        cca_threadhold = (SlTxInhibitThreshold_e)atoi(token);
                        if((cca_threadhold > SL_TX_INHIBIT_THRESHOLD_MAX) ||
                           (cca_threadhold < SL_TX_INHIBIT_THRESHOLD_MIN))
                        {
                            UART_PRINT(
                                "\n\r[Transceiver Mode Error]:"
                                " Power level arguments are out of range.\n\r");
                            return(-1);
                        }
                    }
                    else
                    {
                        UART_PRINT(
                            "\n\r[Transceiver Mode Error]:"
                            " CCA Threshold arguments are invalid.\n\r");
                    }
                }
            }
            else if(!strcmp(token, "2"))
            {
                UART_PRINT("\n\r[Transceiver Mode]: Using default values.\n\r");
            }
            else
            {
                UART_PRINT("\n\r[Transceiver Mode Error]: Invalid option.\n\r");
                return(-1);
            }
        }
    }

    /* Once Parsing is done, Wait for user GO indication */
    UART_PRINT(
        "\n\r[Transceiver Mode] :"
        " Ready to start TX, press any key to continue...\n\r");
    getch();

    /* Now, we can start TX */
    ret = radioTool_TxStart(txType,
                            txchannel,
                            Mod,
                            preamble_type,
                            pattern,
                            txPower,
                            packetSize,
                            amount,
                            delay,
                            cca_override,
                            cca_threadhold,
                            cwTone,
                            (uint8_t*)&pDestMac);

    RET_ON_ERROR(ret);

    switch(txType)
    {
    case TxMode_CW:
    {
        UART_PRINT(
            "\n\r\n\r[Transceiver Mode] :"
            " Start Transmitting CW for %d mSec period ...\n\r",
            duration);
        MSEC_SLEEP(duration);
        break;
    }
    case TxMode_Packetized:
    {
        UART_PRINT(
            "\n\r\n\r[Transceiver Mode] :"
            " Start Transmitting %d packets...\n\r",
            amount);
        break;
    }
    case TxMode_Continues:
    {
        UART_PRINT(
            "\n\r\n\r[Transceiver Mode] :"
            " Start Transmitting packets for %d mSec period ...\n\r",
            duration);
        MSEC_SLEEP(duration);
        break;
    }
    default:
    {
        break;
    }
    }

    /* Stop the transmission */
    ret = radioTool_TxStop(txType);

    UART_PRINT("\n\r[Transceiver Mode] : Tx Complete.\n\r");

    return(ret);
}

/*!
    \brief          Prints radiotool option menu.

    \sa              RxTask, TxTask
 */
void showMenu()
{
    UART_PRINT(lineBreak);
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('-', 48);
    UART_PRINT(lineBreak);
    UART_PRINT("\t             RadioTool Option Menu        \n\r");
    UART_PRINT("\t");
    printBorder('-', 48);
    UART_PRINT(lineBreak);
    UART_PRINT("    1. |            Rx task options.            |\n\r");
    UART_PRINT("    2. |            Tx task options.            |\n\r");
    UART_PRINT("    3. |            Exit radio tool.            |\n\r");
    UART_PRINT("\t");
    printBorder('-', 48);
    UART_PRINT(lineBreak);

    return;
}

/*!
    \brief          Prints RX task menu

    \sa             RxTask
 */
void showRxMenu()
{
    ClearTerm();
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('-', 48);
    UART_PRINT(lineBreak);
    UART_PRINT("\t             RX Task Option Menu        \n\r");
    UART_PRINT("\t");
    printBorder('-', 48);
    UART_PRINT(lineBreak);
    return;
}

/*!
    \brief          Prints the available preamble types.

    \sa             TxTask
 */
void showPreambleMenu()
{
    UART_PRINT(lineBreak);
    UART_PRINT(Txtask_p_optionDetailsStr);
    UART_PRINT(Txtask_p0_optionDetailsStr);
    UART_PRINT(Txtask_p1_optionDetailsStr);
    UART_PRINT(Txtask_p2_optionDetailsStr);
    UART_PRINT(Txtask_p3_optionDetailsStr);
    UART_PRINT(Txtask_p4_optionDetailsStr);
    UART_PRINT(lineBreak);
}

/*!
    \brief          Prints MCS (modulation - coding) option menu.

    \sa             TxTask
 */
void showModCodeMenu()
{
    UART_PRINT(lineBreak);
    UART_PRINT(Txtask_m_optionDetailsStr);
    UART_PRINT(Txtask_m1_optionDetailsStr);
    UART_PRINT(Txtask_m2_optionDetailsStr);
    UART_PRINT(Txtask_m3_optionDetailsStr);
    UART_PRINT(Txtask_m4_optionDetailsStr);
    UART_PRINT(Txtask_m5_optionDetailsStr);
    UART_PRINT(Txtask_m6_optionDetailsStr);
    UART_PRINT(Txtask_m7_optionDetailsStr);
    UART_PRINT(Txtask_m8_optionDetailsStr);
    UART_PRINT(Txtask_m9_optionDetailsStr);
    UART_PRINT(Txtask_m10_optionDetailsStr);
    UART_PRINT(Txtask_m11_optionDetailsStr);
    UART_PRINT(Txtask_m12_optionDetailsStr);
    UART_PRINT(Txtask_m13_optionDetailsStr);
    UART_PRINT(Txtask_m14_optionDetailsStr);
    UART_PRINT(Txtask_m15_optionDetailsStr);
    UART_PRINT(Txtask_m16_optionDetailsStr);
    UART_PRINT(Txtask_m17_optionDetailsStr);
    UART_PRINT(Txtask_m18_optionDetailsStr);
    UART_PRINT(Txtask_m19_optionDetailsStr);
    UART_PRINT(Txtask_m20_optionDetailsStr);
    UART_PRINT(Txtask_m21_optionDetailsStr);
    UART_PRINT(lineBreak);
}

/*!
    \brief          Prints CCA threshold option menu.

    \sa             RxTask, TxTask
 */
void showCCAThreshHoldMenu()
{
    UART_PRINT(lineBreak);
    UART_PRINT(cca_th_optionDetailStr);
    UART_PRINT(lineBreak);
    UART_PRINT(cca_th1_optionDetailStr);
    UART_PRINT(cca_th2_optionDetailStr);
    UART_PRINT(cca_th3_optionDetailStr);
    UART_PRINT(cca_th4_optionDetailStr);
    UART_PRINT(cca_th5_optionDetailStr);
    UART_PRINT(cca_th6_optionDetailStr);
    UART_PRINT(lineBreak);
}

/*!
    \brief          Prints TX power option menu.

    \sa             RxTask, TxTask
 */
void showPowMenu()
{
    UART_PRINT(lineBreak);
    UART_PRINT(Txtask_txp_optionDetailsStr);
    UART_PRINT(lineBreak);
}

/*!
    \brief          Prints Tx Task option menu.

    \sa             TxTask
 */
void showTxMenu()
{
    ClearTerm();
    UART_PRINT(lineBreak);
    UART_PRINT("\t");
    printBorder('-', 48);
    UART_PRINT(lineBreak);
    UART_PRINT("\t             TX Task Option Menu        \n\r");
    UART_PRINT("\t");
    printBorder('-', 48);
    UART_PRINT(lineBreak);

    UART_PRINT("\n\r\tChoose Tx task Mode: \n\r");
    UART_PRINT(
        "\n\r\t1. Continuous:  This mode TX frames "
        "generated internally by NWP, for a given amount time. \n\r");
    UART_PRINT(
        "\n\r\t2. Packetized:  This mode sends each TX"
        " frame from host processor to the NWP TX queue. \n\r");
    UART_PRINT(
        "\n\r\t3. CW Mode:     TX carrier wave signal."
        " User can set CW tone offset. \n\r");
    UART_PRINT(lineBreak);
}

/*!
    \brief          Prints default PHY option menu.

    \sa             TxTask
 */
void showPHYConfMenu()
{
    UART_PRINT("\n\rConfigure PHY parameters? \n\r");
    UART_PRINT("\n\r1. Yes. \n\r");
    UART_PRINT("\n\r2. Use Defaults. \n\r");
    UART_PRINT(lineBreak);
    UART_PRINT("\n\rDefault Values: \n\r");
    UART_PRINT("\n\r\tPreamble type:  Long. \n\r");
    UART_PRINT("\n\r\tRate:  1Mbps. \n\r");
    UART_PRINT("\n\r\tTx Power:  Max Power. \n\r");
    UART_PRINT("\n\r\tCCA override:  Yes. \n\r");
    UART_PRINT("\n\r\tCCA thrashold:  Not used. \n\r");
    UART_PRINT(lineBreak);
}
