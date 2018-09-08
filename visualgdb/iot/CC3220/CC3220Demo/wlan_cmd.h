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

#ifndef __WLAN_CMD_H__
#define __WLAN_CMD_H__

#include <stdint.h>

/* Function prototypes */

int32_t cmdWlanConnectCallback(void *arg);

int32_t printWlanConnectUsage(void *arg);

int32_t cmdScanCallback(void *arg);

int32_t printScanUsage(void *arg);

int32_t cmdSetPolicyCallback(void *arg);

int32_t printSetPolicyUsage(void *arg);

int32_t cmdWlanStartApCallback(void *arg);

int32_t printWlanStartApUsage(void *arg);

int32_t cmdWlanStopApCallback(void *arg);

int32_t printWlanStopApUsage(void *arg);

int32_t cmdWlanDisconnectCallback(void *arg);

int32_t printWlanDisconnectUsage(void *arg);

int32_t cmdConnectedStationsCallback(void *arg);

int32_t printConnectedStationsUsage(void *arg);

int32_t cmdCreateFilterCallback(void *arg);

int32_t printCreateFilterUsage(void *arg);

int32_t cmdEnableFilterCallback(void *arg);

int32_t printEnableFilterUsage(void *arg);

int32_t cmdDisableFilterCallback(void *arg);

int32_t printDisableFilterUsage(void *arg);

int32_t cmdDeleteFilterCallback(void *arg);

int32_t printDeleteFilterUsage(void *arg);

int32_t cmdEnableWoWLANCallback(void *arg);

int32_t printEnableWoWLANUsage(void *arg);

int32_t cmdP2PModecallback(void *arg);

int32_t printP2PStartUsage(void *arg);

int32_t cmdP2PStopcallback(void *arg);

int32_t printP2PStopUsage(void *arg);

int32_t printCountrycodeeUsage(void *arg);

int32_t cmdCountrycodecallback(void *arg);

int32_t CheckSubFrameType(uint8_t *pSubFrameType);

void printFrameSubTyps(void);

#endif /* __WLAN_CMD_H__ */
