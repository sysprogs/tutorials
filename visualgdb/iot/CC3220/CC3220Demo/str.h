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

#ifndef __STR_H__
#define __STR_H__

#include "network_terminal.h"

extern char lineBreak[];

extern char cmdPromptStr[];
extern char usageStr[];
extern char *descriptionStr;

extern char helpStr[];
extern char helpUsageStr[];
extern char *helpDetailsStr;
extern char *help_optaionDetails;

extern char scanStr[];
extern char scanUsageStr[];
extern char *scanDetailsStr;
extern char *scan_n_optionDetailsStr;
extern char *scan_ext_optionDetailsStr;
extern char *scan_Note_optionDetailsStr;

extern char setPolicyStr[];
extern char setPolicyUsageStr[];
extern char *setPolicyDetailsStr;
extern char *setPolicy_i_optionDetailsStr;
extern char *setPolicy_h_optionDetailsStr;
extern char *setPolicy_c_optionDetailsStr;
extern char *setPolicy_r_optionDetailsStr;
extern char *setPolicy_o_optionDetailsStr;

extern char wlanConnectStr[];
extern char wlanConnectUsageStr[];
extern char *wlanConnectDetailsStr;
extern char *wlanConnect_s_optionDetailsStr;
extern char *wlanConnect_t_optionDetailsStr;
extern char *wlanConnect_p_optionDetailsStr;
extern char *wlanConnect_ip_optionDetailsStr;
extern char *wlanConnect_gw_optionDetailsStr;
extern char *wlanConnect_dns_optionDetailsStr;
extern char *wlanConnect_ent_optionDetailStr;

extern char ap_start_str[];
extern char ap_start_UsageStr[];
extern char *ap_start_DetailsStr;
extern char *ap_start_h_optionDetailsStr;
extern char *ap_start_txp_optionDetailsStr;
extern char *ap_start_channel_optionDetailsStr;
extern char *ap_start_l_optionDetailsStr;

extern char ap_stop_str[];
extern char ap_stop_UsageStr[];
extern char *ap_stop_DetailsStr;

extern char wlanDisconnectStr[];
extern char wlanDisconnectUsageStr[];
extern char *wlanDisConnectDetailsStr;

extern char ConnectedStationsStr[];
extern char ConnectedStationsUsageStr[];
extern char *ConnectedStationsDetailsStr;

extern char pingStr[];
extern char pingUsageStr[];
extern char *pingDetailsStr;
extern char *ping_h_optionDetailsStr;
extern char *ping_c_optionDetailsStr;
extern char *ping_s_optionDetailsStr;
extern char *ping_t_optionDetailsStr;
extern char *ping_i_optionDetailsStr;

extern char sendStr[];
extern char sendUsage1Str[];
extern char sendUsage2Str[];
extern char *sendDetailsStr;
extern char send_c_optionDetailsStr[];
extern char send_s_optionDetailsStr[];
extern char send_u_optionDetailsStr[];
extern char send_p_optionDetailsStr[];
extern char send_nb_optionDetailsStr[];
extern char send_n_optionDetailsStr[];
extern char send_6_optionDetailsStr[];

extern char recvStr[];
extern char *recvUsage1Str;
extern char *recvUsage2Str;
extern char *recvDetailsStr;
extern char *recv_c_optionDetailsStr;
extern char *recv_s_optionDetailsStr;
extern char *recv_u_optionDetailsStr;
extern char *recv_p_optionDetailsStr;
extern char *recv_nb_optionDetailsStr;
extern char send_n_r_optionDetailsStr[];
extern char *recv_6_optionDetailsStr;
extern char recv_Note_optionDetailsStr[];

extern char lsStr[];
extern char lsUsageStr[];
extern char *lsDetailsStr;
extern char *ls_l_optaionDetailsStr;
extern char *ls_lh_optaionDetailsStr;

extern char writeStr[];
extern char writeUsageStr[];
extern char *writeDetailsStr;
extern char *write_f_optaionDetailsStr;
extern char *write_c_optaionDetailsStr;
extern char *write_fs_optaionDetailsStr;
extern char *write_p_optaionDetailsStr;

extern char readStr[];
extern char readUsageStr[];
extern char *readDetailsStr;
extern char *read_f_optaionDetailsStr;
extern char *read_o_optaionDetailsStr;
extern char *read_l_optaionDetailsStr;
extern char *read_p_optaionDetailsStr;
extern char *read_x_optaionDetailsStr;

extern char createFilterStr[];
extern char createFilterUsageStr[];
extern char *creteFilterDetailsStr;
extern char *createFilter_f_optionDetailsStr;
extern char *createFilter_v_optionDetailsStr;
extern char *createFilter_e_optionDetailsStr;
extern char *createFilter_a_optionDetailsStr;
extern char *createFilter_m_optionDetailsStr;
extern char *createFilter_o_optionDetailsStr;
extern char *createFilter_i_optionDetailsStr;

extern char enableFilterStr[];
extern char *enableFilterDetailsStr;

extern char disableFilterStr[];
extern char *disableFilterDetailsStr;

extern char deleteFilterStr[];
extern char *deleteFilterDetailsStr;

extern char enableWoWLANStr[];
extern char enableWoWLANUsageStr[];
extern char *enableWoWLANDetailsStr;
extern char *enableWoWLAN_v_optionDetailsStr;
extern char *enableWoWLAN_p_optionDetailsStr;
extern char *enableWoWLAN_u_optionDetailsStr;
extern char *enableWoWLAN_i_optionDetailsStr;
extern char *enableWoWLAN_ch_optionDetailsStr;
extern char *enableWoWLAN_Note_optionDetailsStr;

extern char mDNSAdvertiseStr[];
extern char mDNSAdvertiseUsageStr[];
extern char *mDNSAdvertiseDetailsStr;
extern char *mDNSAdvertise_n_optionDetailsStr;
extern char *mDNSAdvertise_t_optionDetailsStr;
extern char *mDNSAdvertise_so_optionDetailsStr;
extern char *mDNSAdvertise_o_optionDetailsStr;
extern char *mDNSAdvertise_st_optionDetailsStr;
extern char *mDNSAdvertise_ttl_optionDetailsStr;
extern char *mDNSAdvertise_p_optionDetailsStr;

extern char mDNSQueryStr[];
extern char mDNSdQueryUsageStr[];
extern char *mDNSdQueryDetailsStr;
extern char *mDNSdQuery_n_optionDetailsStr;
extern char *mDNSdQuery_st_optionDetailsStr;
extern char *mDNSdQuery_so_optionDetailsStr;
extern char *mDNSdQuery_o_optionDetailsStr;

extern char radiotool_Str[];
extern char radiotool_PromptStr[];
extern char radiotoolDetailsStr[];

extern char TxtaskStr[];
extern char TxtaskUsageStr[];
extern char *TxtaskDetailsStr;
extern char *Txtask_tx_optionDetailsStr;
extern char *Txtask_tx1_optionDetailsStr;
extern char *Txtask_tx2_optionDetailsStr;
extern char *Txtask_tx3_optionDetailsStr;
extern char *Txtask_t_optionDetailsStr;
extern char *Txtask_c_optionDetailsStr;
extern char *Txtask_m_optionDetailsStr;
extern char *Txtask_m1_optionDetailsStr;
extern char *Txtask_m2_optionDetailsStr;
extern char *Txtask_m3_optionDetailsStr;
extern char *Txtask_m4_optionDetailsStr;
extern char *Txtask_m5_optionDetailsStr;
extern char *Txtask_m6_optionDetailsStr;
extern char *Txtask_m7_optionDetailsStr;
extern char *Txtask_m8_optionDetailsStr;
extern char *Txtask_m9_optionDetailsStr;
extern char *Txtask_m10_optionDetailsStr;
extern char *Txtask_m11_optionDetailsStr;
extern char *Txtask_m12_optionDetailsStr;
extern char *Txtask_m13_optionDetailsStr;
extern char *Txtask_m14_optionDetailsStr;
extern char *Txtask_m15_optionDetailsStr;
extern char *Txtask_m16_optionDetailsStr;
extern char *Txtask_m17_optionDetailsStr;
extern char *Txtask_m18_optionDetailsStr;
extern char *Txtask_m19_optionDetailsStr;
extern char *Txtask_m20_optionDetailsStr;
extern char *Txtask_m21_optionDetailsStr;
extern char *Txtask_txp_optionDetailsStr;
extern char *Txtask_p_optionDetailsStr;
extern char *Txtask_p0_optionDetailsStr;
extern char *Txtask_p1_optionDetailsStr;
extern char *Txtask_p2_optionDetailsStr;
extern char *Txtask_p3_optionDetailsStr;
extern char *Txtask_p4_optionDetailsStr;
extern char *Txtask_tone_optionDetailsStr;
extern char *Txtask_d_optionDetailsStr;
extern char *Txtask_dp_optionDetailsStr;
extern char *Txtask_dp0_optionDetailsStr;
extern char *Txtask_dp1_optionDetailsStr;
extern char *Txtask_dp2_optionDetailsStr;
extern char *Txtask_dp3_optionDetailsStr;
extern char *Txtask_s_optionDetailsStr;
extern char *Txtask_a_optionDetailsStr;

extern char *Txtask_cca_optionDetailsStr;
extern char *cca_th_optionDetailStr;
extern char *cca_th1_optionDetailStr;
extern char *cca_th2_optionDetailStr;
extern char *cca_th3_optionDetailStr;
extern char *cca_th4_optionDetailStr;
extern char *cca_th5_optionDetailStr;
extern char *cca_th6_optionDetailStr;

extern char RxtaskStr[];
extern char RxtaskUsageStr[];
extern char *RxtaskDetailsStr;
extern char *Rxtask_c_optionDetailsStr;
extern char *Rxtask_d_optionDetailsStr;
extern char *Rxtask_s_optionDetailsStr;

extern char p2pStartcmdStr[];
extern char *p2pStartDetailsStr;

extern char p2pStopcmdStr[];
extern char *p2pStopDetailsStr;

extern char SetCountryCodeStr[];
extern char SetCountryCodeUsageStr[];
extern char *CountryCodeDetailsStr;
extern char *CountryCode_g_optionDetailsStr;
extern char *CountryCode_Note_optionDetailsStr;

extern char rmStr[];
extern char rmUsageStr[];
extern char *rmDetailsStr;
extern char *rm_f_optaionDetailsStr;
extern char *rm_p_optaionDetailsStr;

extern char clearStr[];
extern char *clearDetailsStr;

extern char exitStr[];
extern char exitUsageStr[];
extern char *exitDetailsStr;

extern char a_optionStr[];
extern char b_optionStr[];
extern char c_optionStr[];
extern char f_optionStr[];
extern char g_optionStr[];
extern char r_optionStr[];
extern char h_optionStr[];
extern char i_optionStr[];
extern char l_optionStr[];
extern char n_optionStr[];
extern char o_optionStr[];
extern char p_optionStr[];
extern char s_optionStr[];
extern char t_optionStr[];
extern char st_optionStr[];
extern char tx_optionStr[];
extern char tone_optionStr[];
extern char cca_optionStr[];
extern char u_optionStr[];
extern char v_optionStr[];
extern char d_optionStr[];
extern char e_optionStr[];
extern char x_optionStr[];
extern char m_optionStr[];
extern char ch_optionStr[];
extern char fs_optionStr[];
extern char nb_optionStr[];
extern char lh_optionStr[];
extern char dp_optionStr[];
extern char V_optionStr[];
extern char ip_optionStr[];
extern char dns_optionStr[];
extern char gw_optionStr[];
extern char ent_optionStr[];
extern char help_optionStr[];
extern char txpow_optionStr[];
extern char ttl_optionStr[];
extern char so_optionStr[];
extern char ext_optionStr[];
extern char WPA_str[];
extern char WPA2_str[];
extern char WEP_str[];
extern char WPS_str[];
extern char OPEN_str[];
extern char WPAWPA2_str[];
extern char YES_str[];
extern char NO_str[];
extern char space_str[];
extern char CONNECTED_str[];
extern char PROMISCUOUS_str[];
extern char SOURCE_MAC_str[];
extern char DESTINATION_MAC_str[];
extern char BSSID_str[];
extern char S_IP_str[];
extern char D_IP_str[];
extern char FRAME_TYPE_str[];
extern char FRAME_SUBTYPE_str[];
extern char PATTERN_str[];
extern char *MangmentFrames_str[];
extern char *CtrlFrames_str[];
extern char *DataFrames_str[];

extern char fsError[];
extern char socketError[];
extern char netappError[];
extern char cmdError[];
extern char nwError[];

void printBorder(char ch,
                 int n);

#endif /* __STR_H__ */
