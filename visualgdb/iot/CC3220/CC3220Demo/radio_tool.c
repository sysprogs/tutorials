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

#include "network_terminal.h"
#include "radio_tool.h"

int16_t radioTool_rawSocketdesc;        /* RAW socket descriptor        */
int8_t Tx_frame[FRAME_SIZE];                    /* Assembled Tx frame   */
int8_t Rx_frame[FRAME_SIZE];                    /* Assembled RX frame   */
uint8_t Template_Hdr[] =                /* 802.11 frame header template */
{
    FRAME_TYPE,
    FRAME_CONTROL,
    DURATION,
    RECEIVE_ADDR,
    TRANSMITTER_ADDR,
    DESTINATION_ADDR,
    FRAME_NUMBER,
    QOS_CONTROL
};

/*!
    \brief          Radio tool Open.

    This routine sets the NWP in station role and
    sets the connection policy to none, since it prepare
    the application to use the device's radio in promiscuous mode.

    \param          arg       -   Not in use at the moment.
                                  Reserved for future use.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function shall 
                    return negative number.

    \note           This API perform NWP reset - any non-persistent
                    settings would be lost.

    \sa             Tx_Task, Rx_Task

 */
int32_t radioTool_Open(void *arg)
{
    uint8_t Policy;
    int32_t ret = 0;

    if(!IS_NW_PROCSR_ON(app_CB.Status))
    {
        ret = sl_Start(0, 0,0);
        RET_ON_ERROR(ret);
        SET_STATUS_BIT(app_CB.Status, STATUS_BIT_NWP_INIT);
    }

    /* Configure Role, an set connection Policy to None. */
    ret = sl_WlanSetMode(ROLE_STA);
    RET_ON_ERROR(ret);

    ret =
        sl_WlanPolicySet(SL_WLAN_POLICY_CONNECTION,
                         SL_WLAN_CONNECTION_POLICY(0, 0, 0, 0), &Policy, 1);
    RET_ON_ERROR(ret);

    ret = sl_Stop(SL_STOP_TIMEOUT);
    RET_ON_ERROR(ret);
    CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_NWP_INIT);

    /* Restart the NWP */
    ret = sl_Start(0, 0,0);
    if(ret != ROLE_STA)
    {
        return(-1);
    }
    else
    {
        SET_STATUS_BIT(app_CB.Status, STATUS_BIT_NWP_INIT);
        app_CB.Role = ROLE_STA;
    }

    return(0);
}

/*!
    \brief          Radio tool Close.

    This routine Configure the NWP back
    to it's default state and exits promiscuous mode.

    \param          arg       -   Not in use at the moment.
                                  Reserved for future use.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure,
                    this function shall return negative number.

    \note           This API perform NWP reset - any non-persistent
                                settings would be lost.

    \sa             Tx_Task, Rx_Task

 */
int32_t radioTool_Close(void *arg)
{
    int32_t ret = 0;
    ret = sl_Stop(SL_STOP_TIMEOUT);

    if(ret < 0)
    {
        RET_ON_ERROR(RADIO_TOOL_ERROR_STOPPING_RADIO_TOOL);
    }

    /* Clear the Radio On bit*/
    CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_TX_STARED);
    CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_NWP_INIT);

    /* Return SimpleLink to default state */
    /*
        IMPORTANT NOTE - This is an example of a 
        'restore to default' function, it changes various
        parameters, which suit this application.
        User must update and adjust this function for
        he's / her needs, before using it.
     */
    return (ConfigureSimpleLinkToDefaultState());
}

/*!
    \brief          Radio tool Tx start

    This routine essentially configures the NWP radio,
    with the desired parameters and showcase how to open
    a RAW (L1) socket.

    \param          Mode            -   Continuous /
                                        Packetized /
                                        Carrier Wave (CW)

    \param          TxChannel       -  802.11 Wireless LAN channel number.

    \param          Rate            -  Modulation / Coding index (MCS).

    \param          preambleSeq     -  Preamble type.

    \param          Data            -  Frame's payload pattern.

    \param          powerLevel      -  Tx power. [in dBm units]

    \param          size            -  Frame's size excluding header- 
                                       [in Byte units].

    \param          packet_amount   -  Amount of frames to transmit.

    \param          delayPeriod     -  For Continuous mode,
                                       user can set the delay between
                                       the sending of two adjacent frames.
                                       [in mSec units].

    \param          overrideCCA     -  Flag which decides whether
                                       or not to check for CCA before
                                       transmission.

    \param          thold           -  In case 'overrideCCA' isn't set,
                                       user can choose it's threshold
                                       of assessing if channel is clear or not.
                                       [in dBm units].

    \param            tone            -  For CW mode only.
                                       0 means tone at center frequency.
                                       A value N between the range [-25, 25] 
                                       means offset
                                       of N*312.5kHz from center frequency.

    \param          pDestMac        -  Points to the MAC address 
                                       which is then put in the 
                                       'destination MAC' field
                                       of the transmitted frames.
                                       In colon notation [XX:XX:XX:XX:XX:XX].


    \return         Upon successful completion, the function shall return 0.
                    In case of failure,
                    this function shall return negative number.

    \sa             Tx_Task, radioTool_TxStop

 */
int32_t radioTool_TxStart(TxMode Mode,
                          Channel TxChannel,
                          SlWlanRateIndex_e Rate,
                          Preamble preambleSeq,
                          DataPattern Data,
                          uint8_t powerLevel,
                          uint16_t size,
                          uint16_t packet_amount,
                          uint32_t delayPeriod,
                          uint8_t overrideCCA,
                          SlTxInhibitThreshold_e thold,
                          int8_t tone,
                          uint8_t *pDestMac)
{
    int32_t ret = 0;
    int32_t i = 0;
    uint16_t pConfigLen = SL_WLAN_BSSID_LENGTH;
    uint32_t length = 0;

    /* Check is Tx is already running */
    if(IS_TX_ON(app_CB.Status))
    {
        return(-1);
    }

    /* If the size parameter is invalid, use the default size: 1500 bytes. */
    if(!size)
    {
        size = FRAME_SIZE;
    }

    /* Construct frame */
    if((Mode == TxMode_Continues) || (Mode == TxMode_Packetized))
    {
        switch(Data)
        {
        case DataPattern_all_0:

            memset(&Tx_frame, 0x0, size);
            break;

        case DataPattern_all_1:

            memset(&Tx_frame, 0x1, size);
            break;

        case DataPattern_incemental:

            for(i = 0; i < size; i++)
            {
                Tx_frame[i] = (uint8_t)(i);
            }
            break;

        case DataPattern_decremental:

            for(i = 0; i < size; i++)
            {
                Tx_frame[i] = (uint8_t)(size - 1 - i);
            }
            break;

        default:
            memset(&Tx_frame, 0x0, size);
        }

        /* Update header fields, and than make Tx Frame */
        sl_NetCfgGet(SL_NETCFG_MAC_ADDRESS_GET, NULL, &pConfigLen,
                     &Template_Hdr[TA_OFFSET]);
        memcpy(&Template_Hdr[RA_OFFSET], pDestMac, SL_WLAN_BSSID_LENGTH);
        memcpy(&Template_Hdr[DA_OFFSET], pDestMac, SL_WLAN_BSSID_LENGTH);
        memcpy(Tx_frame, Template_Hdr, sizeof(Template_Hdr));

        /* Open RAW/DGRAM(UDP) Socket based on the CCA override parameter */
        if(overrideCCA == TRUE)
        {
            radioTool_rawSocketdesc = sl_Socket(SL_AF_RF, SL_SOCK_RAW,
                                                TxChannel);
        }
        else
        {
            radioTool_rawSocketdesc = sl_Socket(SL_AF_RF, SL_SOCK_DGRAM,
                                                TxChannel);
            ret =
                sl_SetSockOpt(radioTool_rawSocketdesc, SL_SOL_PHY_OPT,
                              SL_SO_PHY_TX_INHIBIT_THRESHOLD,
                              &thold,
                              sizeof(thold));
            if(ret)
            {
                return(RADIO_TOOL_ERROR_RX_CREATING_RAW_SOCKET);
            }
        }

        if(radioTool_rawSocketdesc < 0)
        {
            return(RADIO_TOOL_ERROR_TX_CREATING_RAW_SOCKET);
        }
    }

    /* Transmit the frame according to TxMode cases, for the given time */
    switch(Mode)
    {
    case TxMode_Packetized:
    {
        /* Transmit frames */
        for(i = 0; i < packet_amount; i++)
        {
            length =
                sl_Send(radioTool_rawSocketdesc, &Tx_frame, size,
                        SL_WLAN_RAW_RF_TX_PARAMS(TxChannel, Rate, powerLevel,
                                                 preambleSeq));

            if(length != size)
            {
                ret = radioTool_TxStop(Mode);
                return(RADIO_TOOL_ERROR_TX_FULL_SIZE_DATA);
            }

            MSEC_SLEEP(delayPeriod);
        }

        break;
    }

    case TxMode_Continues:
    {
        i = 0;

        /* This call activates the internal packet generator. If packetNum = 0 ,
        it generate infinite packets, until stopped */
        ret =
            sl_SetSockOpt(radioTool_rawSocketdesc, SL_SOL_PHY_OPT,
                          SL_SO_PHY_NUM_FRAMES_TO_TX,
                          &i,
                          sizeof(i));

        length =
            sl_Send(radioTool_rawSocketdesc, &Tx_frame, size,
                    SL_WLAN_RAW_RF_TX_PARAMS(TxChannel, Rate, powerLevel,
                                             preambleSeq));

        if(length != size)
        {
            ret = radioTool_TxStop(Mode);
            return(RADIO_TOOL_ERROR_TX_FULL_SIZE_DATA);
        }

        break;
    }

    case TxMode_CW:
    {
        /* Start transmitting a carrier wave */
        radioTool_rawSocketdesc = sl_Socket(SL_AF_RF, SL_SOCK_RAW, TxChannel);
        sl_Send(radioTool_rawSocketdesc, NULL, 0, tone);
        break;
    }

    default:
    {
        ret = RADIO_TOOL_ERROR_TX_TYPE_UNKNOWN;
    }
    }

    return(ret);
}

/*!
    \brief          Radio tool Tx Stop

    This routine stops the NWP radio transmission.

    \param          Mode    -  Continuous /  Packetized / Carrier Wave (CW)


    \return         Upon successful completion, the function shall return 0.
                    In case of failure, this function shall  return negative 
                    number.

    \sa             Tx_Task, radioTool_TxStart

 */
int32_t radioTool_TxStop(TxMode Mode)
{
    int32_t ret = 0;

    switch(Mode)
    {
    case TxMode_Packetized:
    {
        ret = sl_Close(radioTool_rawSocketdesc);
        break;
    }

    case TxMode_Continues:
    {
        ret = sl_Close(radioTool_rawSocketdesc);
        break;
    }

    case TxMode_CW:
    {
        sl_Send(radioTool_rawSocketdesc, NULL, 0, CW_STOP);
        ret = sl_Close(radioTool_rawSocketdesc);
        break;
    }

    default:
        ret = RADIO_TOOL_ERROR_TX_TYPE_UNKNOWN;
    }

    CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_TX_STARED);

    return(ret);
}

/*!
    \brief          Radio tool Rx start

    This routine shows how to open the NWP RX radio
    on a specific 802.11 channel for a given amount of time.
    Also, it collects RX statistics.

    \param          RxChannel       -  802.11 Wireless LAN channel number.

    \param          duration        -  The amount of time to perform Rx.
                                       [in mSec units].

    \return         Upon successful completion, 
                    the function shall return 0.
                    In case of failure,
                    this function shall return negative number.

    \sa             Rx_Task, radioTool_RxStop

 */
int32_t radioTool_RxStart(Channel RxChannel,
                          uint32_t duration)
{
    int32_t ret = 0;
    SlTimeval_t timeVal = {0};

    ret = sl_WlanRxStatStart();
    RET_ON_ERROR(ret);

    radioTool_rawSocketdesc = sl_Socket(SL_AF_RF, SL_SOCK_RAW, RxChannel);

    if(radioTool_rawSocketdesc < 0)
    {
        return(RADIO_TOOL_ERROR_RX_CREATING_RAW_SOCKET);
    }

    ret =
        sl_SetSockOpt(radioTool_rawSocketdesc, SL_SOL_SOCKET, SL_SO_RCVTIMEO,
                      &timeVal,
                      sizeof(timeVal));
    RET_ON_ERROR(ret);

    ret = sl_Recv(radioTool_rawSocketdesc, &Rx_frame, RX_BUFFER_SIZE, 0);

    return(0);
}

/*!
    \brief          Radio tool Rx start

    This routine shows how to open the NWP RX radio
    on a specific 802.11 channel for a given amount of time.

    \return         Upon successful completion, 
                    the function shall return 0.
                    In case of failure,
                    this function shall return negative number.

    \sa             Rx_Task, radioTool_RxStart

 */
int32_t radioTool_RxStop()
{
    int32_t ret = 0;

    sl_WlanRxStatStop();

    ret = sl_Close(radioTool_rawSocketdesc);

    CLR_STATUS_BIT(app_CB.Status, STATUS_BIT_TX_STARED);

    return(ret);
}

/*!
    \brief          Radio tool get statistics

    This routine shows how to collect RX statistics.

    \param          pStatResp       -  Points to RX stat respose buffer.

    \return         Upon successful completion, the function shall return 0.
                    In case of failure,
                    this function shall return negative number.

    \sa             Rx_Task, sl_WlanRxStatGet

 */
int32_t radioTool_GetStats(SlWlanGetRxStatResponse_t* pStatResp)
{
    int32_t ret = 0;
    SlWlanGetRxStatResponse_t rxStatResp;

    ret = sl_WlanRxStatGet(&rxStatResp, 0);
    if(ret < 0)
    {
        return(RADIO_TOOL_ERROR_GETTING_RX_STATS);
    }

    memcpy(pStatResp, &rxStatResp, sizeof(SlWlanGetRxStatResponse_t));

    return(0);
}
