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

#ifndef __RADIOTOOL_H__
#define __RADIOTOOL_H__

#include <stdint.h>
#include <ti/drivers/net/wifi/simplelink.h>

#define RET_ON_ERROR(x) \
    if(x < 0) \
    { \
        return x; \
    }

#define MSEC_SLEEP(x) \
    if((x * 1000) >= 1000000) \
    { \
        sleep(x / 1000); \
    } \
    else \
    { \
        usleep(1000 * x); \
    }

#define CW_LOW_TONE                                 (-25)
#define CW_HIGH_TONE                                (25)
#define CW_STOP                                     (128)

/* error codes */
#define RADIO_TOOL_ERROR_STARTING_RADIO_TOOL        (-101)
#define RADIO_TOOL_ERROR_STOPPING_RADIO_TOOL        (-102)
#define RADIO_TOOL_ERROR_GETTING_MAC_ADDR           (-110)
#define RADIO_TOOL_ERROR_GETTING_DEV_VERSION        (-112)
#define RADIO_TOOL_ERROR_TX_TYPE_UNKNOWN            (-100)
#define RADIO_TOOL_ERROR_TX_CREATING_RAW_SOCKET     (-101)
#define RADIO_TOOL_ERROR_TX_FULL_SIZE_DATA          (-102)
#define RADIO_TOOL_ERROR_RX_CREATING_RAW_SOCKET     (-103)
#define RADIO_TOOL_ERROR_GETTING_RX_STATS           (-111)

/* Definitions for template frame */
#define FRAME_SIZE                                  (1500)
#define FRAME_TYPE              0xC8
#define FRAME_CONTROL           0x01
#define DURATION                0x00, 0x00
#define RECEIVE_ADDR            0x55, 0x44, 0x33, 0x22, 0x11, 0x00
#define TRANSMITTER_ADDR        0x00, 0x11, 0x22, 0x33, 0x44, 0x55
#define DESTINATION_ADDR        0x55, 0x44, 0x33, 0x22, 0x11, 0x00
#define FRAME_NUMBER            0x00, 0x00
#define QOS_CONTROL             0x00, 0x00

#define RA_OFFSET               (4)
#define TA_OFFSET               (10)
#define DA_OFFSET               (16)
#define RX_BUFFER_SIZE          (1470)
#define RX_RECV_TIMEOUT_SEC     (1)
#define RX_RECV_TIMEOUT_USEC    (20000)

#define RADIO_STOP_SIG           0xFF /*(SIGUSR1)*/

/* Types */

typedef enum
{
    TxMode_Continues = 1,
    TxMode_Packetized = 2,
    TxMode_CW = 3,
    TxMode_MaxRadioTxMode = 4
}TxMode;

typedef uint32_t Channel;

typedef enum
{
    PowerLevel_HighGainStep_7 = 0,
    PowerLevel_HighGainStep_6 = 1,
    PowerLevel_HighGainStep_5 = 2,
    PowerLevel_HighGainStep_4 = 3,
    PowerLevel_HighGainStep_3 = 4,
    PowerLevel_HighGainStep_2 = 5,
    PowerLevel_HighGainStep_1 = 6,
    PowerLevel_HighGainStep_0 = 7,
    PowerLevel_MidGainStep_3 = 8,
    PowerLevel_MidGainStep_2 = 9,
    PowerLevel_MidGainStep_1 = 10,
    PowerLevel_MidGainStep_0 = 11,
    PowerLevel_LowGainStep_3 = 12,
    PowerLevel_LowGainStep_2 = 13,
    PowerLevel_LowGainStep_1 = 14,
    PowerLevel_LowGainStep_0 = 15,
    PowerLevel_MaxPowerLevel = 16
}PowerLevel;

typedef enum
{
    Preamble_longMode = 0,
    Preamble_shortMode = 1,
    Preamble_OFDMMode = 2,
    Preamble_N_MixedMode = 3,
    Preamble_GreenfieldMode = 4,
    Preamble_MaxNumMode = 5
}Preamble;

typedef enum
{
    DataPattern_all_0 = 0,
    DataPattern_all_1 = 1,
    DataPattern_incemental = 2,
    DataPattern_decremental = 3,
    DataPattern_pn9 = 4,
    DataPattern_pn15 = 5,
    DataPattern_pn23 = 6,
    DataPattern_max_num = 7
}DataPattern;

/* Function prototypes */

int32_t radioTool_Open(void *arg);

int32_t radioTool_Close(void *arg);

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
                          uint8_t *pDestMac);

int32_t radioTool_TxStop(TxMode Mode);

int32_t radioTool_RxStart(Channel RxChannel,
                          uint32_t duration);

int32_t radioTool_RxStop();

int32_t radioTool_GetStats(SlWlanGetRxStatResponse_t* pStatResp);

int32_t radioTool_GetDeviceVersion(uint8_t *pDeviceVersion);

#endif /* __RADIOTOOL_H__ */
