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

char lineBreak[]                = "\n\r";

char cmdPromptStr[]             = "user:";
char usageStr[]                 = "Usage: \n\r\t";
char *descriptionStr            = "\n\rDescription:\n\r\t";

char helpStr[]                  = "help";
char helpUsageStr[]             = " [command name] \n\r";
char *helpDetailsStr            = "To know more about the command name.\n\r";
char *help_optaionDetails       = "\n\r\t-help\tDisplay this help\n\r\n\r";

char clearStr[]                 = "clear";
char *clearDetailsStr           = "To clear the terminal screen.\n\r";

char scanStr[]                  = "scan";
char scanUsageStr[]             = 
" [-help] [-n <number of AP to scan>]\n\r";
char *scanDetailsStr            = "Scan for available APs. Maximum 30 APs can"
								  " be listed. \n\r";

char *scan_n_optionDetailsStr    = "\n\r\t-n\tMaximum number of Scan"
								   " results to show\n\r";

char *scan_Note_optionDetailsStr = "\tNote:\tIf no policy is set,"
								   " 'Scan' command issue One-shot scan, "
								   "which shuts off immediately\n\r";


char setPolicyStr[]              = "setpolicy";
char setPolicyUsageStr[]         = " [-help] [-i <Scan interval in Sec>] "
								   "[-h <hidden scan>] "
								   "[-c <Channel Mask in HEX>]\n\r"
                                  "\t[-r <RSSI threshold>]"
								  " [-o <Turn scans off>]\n\r";
char *setPolicyDetailsStr        = "Set scan policy parameters.\n\r";

char *setPolicy_i_optionDetailsStr    = "\n\r\t-i\tSets scan interval in"
										" seconds (default is 10 seconds)\n\r";
char *setPolicy_h_optionDetailsStr    = "\n\r\t-h\tScan for hidden AP as well"
										" (hidden scan = [YES, NO])\n\r";
char *setPolicy_c_optionDetailsStr    = "\n\r\t-c\tSets specific scan"
										" channels, using HEX base bitmask"
										" (if bit i is set, scan for channel"
										" i is enabled)\n\r";
char *setPolicy_r_optionDetailsStr    = "\n\r\t-r\tSets RSSI threshold for "
										"scan in dBm units (range: "
										"[-95,0])\n\r";
char *setPolicy_o_optionDetailsStr    = "\n\r\t-o\tTurn scans off "
										"(YES = scans running in background "
										"would stop immediately)\n\r";


char wlanConnectStr[]             = "wlanconnect";
char wlanConnectUsageStr[]        = " [-help] [-s <\"ssid name\">] "
									"[-t <security type>]"
                                  " [-p <\"password\">]\n\r\t[-ip <static ip>]"
								  " [-gw <static gw>] [-dns <static dns>] "
								  "[-ent <\"ent_username\">]\n\r";
char *wlanConnectDetailsStr       = "Connect to an AP.\n\r";

char *wlanConnect_s_optionDetailsStr    = "\n\r\t-s\tSSID\n\r";
char *wlanConnect_t_optionDetailsStr    = "\n\r\t-t\tType of security "
										  "(security type = "
										  "[OPEN, WPA/WPA2, WEP, WPS])\n\r";
char *wlanConnect_p_optionDetailsStr    = "\n\r\t-p\tPassword in ascii "
										  "character (pin in case of WPS)\n\r";
char *wlanConnect_ip_optionDetailsStr   = "\n\r\t-ip\tStatic IP in '.' format"
										  " for ipv4 and in ':' for ipv6\n\r";

char *wlanConnect_gw_optionDetailsStr   = "\n\r\t-gw\tStatic Gateway "
										  "(relevant in case of using"
										  " a static IP)\n\r";

char *wlanConnect_dns_optionDetailsStr  = "\n\r\t-dns\tStatic DNS "
										  "(relevant in case of using a"
										  " static IP)\n\r";

char *wlanConnect_ent_optionDetailStr   = "\n\r\t-ent\tEnterprise user"
										  " name credentials\n\r";


char ap_start_str[]         = "wlan_ap_start";
char ap_start_UsageStr[]    = " [-help] [-s <\"ssid name\">] "
							  "[-t <security type>] [-p <\"password\">]"
                              "[-h <hidden AP>]\n\r\t[-txp <Tx power [0-15]>]"
                              "[-c <Wlan channel [1-13]>] "
							  "[-l <STA connection limit [1-4]>]\n\r";
char *ap_start_DetailsStr    = "Set device in AP mode.\n\r";

char ap_stop_str[]          = "wlan_ap_stop";
char ap_stop_UsageStr[]     = " [-help] \n\r";
char *ap_stop_DetailsStr    = "Set device in Station mode.\n\r";

char *ap_start_h_optionDetailsStr       = "\n\r\t-h\tStart the AP in hidden"
										  " mode (hidden mode = "
										  "[YES, NO])\n\r";
char *ap_start_txp_optionDetailsStr     = "\n\r\t-txp\tSet Wlan "
										  "Tx power (0 = Max Power)\n\r";
char *ap_start_channel_optionDetailsStr = "\n\r\t-c\tSet channel "
										  "for the AP\n\r";
char *ap_start_l_optionDetailsStr       = "\n\r\t-l\tLimit the number of"
										  " connected stations\n\r";

char wlanDisconnectStr[]         = "wlandisconnect";
char wlanDisconnectUsageStr[]    = " [-help] \n\r";
char *wlanDisConnectDetailsStr   = "To disconnect from AP.\n\r";

char ConnectedStationsStr[]         = "connected_stations";
char ConnectedStationsUsageStr[]    = " [-help] \n\r";
char *ConnectedStationsDetailsStr   = "Shows the Stations that are connected "
									  "to the AP.\n\r";

char pingStr[]                   = "ping";
char pingUsageStr[]              = " [-help] [-h <host name or ip>] "
								   "[-c <Num of packets>] [-s <data size>]"
								   " [-i <delay interval>] [-t <timeout>]"
								   "  \n\r";
char *pingDetailsStr             = "To ping mentioned host.\n\r";

char *ping_h_optionDetailsStr    = "\n\r\t-h\tHost name or ip "
								   "(In IPv4 '.' format) \n\r";
char *ping_c_optionDetailsStr    = "\n\r\t-c\tSend <Num of packets> "
								   "number of ECHO_REQUEST "
                                        "packets (Default is 10) \n\r";
char *ping_i_optionDetailsStr    = "\n\r\t-i\tWait <delay interval> time "
								   "(in seconds) between sending "
                                        "each packet (Default is 1)\n\r";
char *ping_s_optionDetailsStr    = "\n\r\t-s\tSpecify the number of data "
								   "bytes to be sent (Default is 56)\n\r";
char *ping_t_optionDetailsStr    = "\n\r\t-t\tSpecify a timeout (in seconds)"
								   " before ping exits"
                                   " regardless of how many packets"
						           " have been received "
                                   "(Default is 3)\n\r";

char sendStr[]                   = "send";
char sendUsage1Str[]             = " [-help] [-c <server ip address>] [-u] "
								   "[-p <port number>]  "
                                   "[-nb] [-n <number of packets>] \n\r";
char sendUsage2Str[]             = " [-help] [-s] [-u] [-p <port number>]  "
                                   "[-V] [-nb] [-n <number of packets>] \n\r";
char *sendDetailsStr             = "To send ip packets over network.\n\r";
char send_c_optionDetailsStr[]   = "\n\r\t-c\tRun in client mode and connect"
									" to mentioned server -\n\r"
                                        "\t  \tIP should be in '.' format for"
										" ipv4 and in ':' for ipv6\n\r";
char send_s_optionDetailsStr[]   = "\n\r\t-s\tRun in server mode\n\r";
char send_u_optionDetailsStr[]   = "\n\r\t-u\tUse UDP rather than TCP\n\r";
char send_p_optionDetailsStr[]   = "\n\r\t-p\tPort number to send/receive data"
								   " (Default is 5001)\n\r";
char send_nb_optionDetailsStr[]  = "\n\r\t-nb\tCreate non-blocking socket "
								   "rather than blocking\n\r";
char send_n_optionDetailsStr[]   = "\n\r\t-n\tNumber of packets to transmit"
								   " (Default is 1000)\n\r";
char send_6_optionDetailsStr[]   = "\n\r\t-V\tUse IPv6 rather than IPv4\n\r";


char recvStr[]                  = "recv";
char *recvUsage1Str             = sendUsage1Str;
char *recvUsage2Str             = sendUsage2Str;
char *recvDetailsStr            = "To receive ip packets over network.\n\r";
char *recv_c_optionDetailsStr   = send_c_optionDetailsStr;
char *recv_s_optionDetailsStr   = send_s_optionDetailsStr;
char *recv_u_optionDetailsStr   = send_u_optionDetailsStr;
char *recv_p_optionDetailsStr   = send_p_optionDetailsStr;
char *recv_nb_optionDetailsStr  = send_nb_optionDetailsStr;
char send_n_r_optionDetailsStr[] = "\n\r\t-n\tNumber of packets to receive "
								   "(Default is 1000)\n\r";
char *recv_6_optionDetailsStr      = send_6_optionDetailsStr;
char recv_Note_optionDetailsStr[] = "\n\r\tNote:\tUDP triggers a timeout "
									"which waits 60 seconds unless all the"
									" packets were received\n\r";

char createFilterStr[]                  = "createfilter";
char createFilterUsageStr[]             = "[-help] [-f <filter Type>] "
										  "[-v <Compare value>] "
										  "[-e <Compare type>]"
                                           "\n\r\t[-a <Action type>]"
										   " [-m <Mode>] [-o <Offset>] "
										   "[-i <Parent filter ID>]\n\r";
char *creteFilterDetailsStr             =  (char*)    "Create a filter to "
"filter out the packets.\n\r";
char *createFilter_f_optionDetailsStr   =  (char*)
 "\n\r\t-f\tFilter Types:"
"\n\r\t\t[S_MAC, D_MAC, "
"BSSID, S_IP, D_IP, FRAME_TYPE, FRAME_SUBTYPE, PATTERN]\n\r"
"\t\t[S - Source, D - Destination]\n\r"
"\t\t[MAC - MAC address, Value: xx:xx:xx:xx:xx:xx]\n\r"
"\t\t[BSSID - MAC address OF THE AP, Value: xx:xx:xx:xx:xx:xx]\n\r"
"\t\t[IP - IP Address, Values: IPv4 - xxx.xxx.xxx.xxx,"
" IPv6 - xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx]\n\r"
"\t\t[FRAME_TYPE Values: management, control, data]\n\r";
char *createFilter_v_optionDetailsStr   =  (char*)    
"\n\r\t-v\tComparision value used by the filter "
"(sets the filter match criteria)\n\r";
char *createFilter_e_optionDetailsStr   =  (char*)   
 "\n\r\t-e\tCompare type (Compare type = [equals, not_equals])\n\r";
char *createFilter_a_optionDetailsStr   =  (char*)    
"\n\r\t-a\tAction on match (Action type = [event, drop, pass]),"
" Either async event, drop the packet or let it pass\n\r";
char *createFilter_m_optionDetailsStr   =  (char*)    
"\n\r\t-m\tFilter Mode (Mode = [L4_CON, L4, L1])\n\r\t\tL4_CON - "
"L4 connected to AP, L4 - L4 not connected to AP and  L1 - "
"transceiver mode\r\n";
char *createFilter_o_optionDetailsStr   =  (char*)    
"\n\r\t-o\tOffset from L1 / L4 Header, according to chosen 'Mode' field "
"(Valid only for 'PATTERN' Type)\n\r";
char *createFilter_i_optionDetailsStr   =  (char*)   
 "\n\r\t-i\tParant filter for a leaf filter, otherwise: 0\r\n";


char enableFilterStr[]                   = "enablefilter";
char *enableFilterDetailsStr             = (char*)"Enable all the configured"
" filters.\n\r";

char disableFilterStr[]                  = "disablefilter";
char *disableFilterDetailsStr            = (char*)"Disable all the configured"
" filters.\n\r";

char deleteFilterStr[]                   = "deletefilter";
char *deleteFilterDetailsStr             = (char*)"Delete all the configured"
" filters.\n\r";

char enableWoWLANStr[]                   = "enablewowlan";
char enableWoWLANUsageStr[]              =     
"[-help] [-v <\"pattern value\">] [-u <Action ID>] [-i <pattern offset>]\n\r";

char *enableWoWLANDetailsStr             =  (char*)    
"To enable the Wake on WLAN.\n\r";
char *enableWoWLAN_v_optionDetailsStr    =          
"\n\r\t-v\tPattern Value used by the Filter (string,"
" of less than 16 chars\n\r";
char *enableWoWLAN_u_optionDetailsStr    =  (char*)    
"\n\r\t-u\tWoWLAN filter Action ID, should be less than 255\r\n";
char *enableWoWLAN_i_optionDetailsStr    =  (char*)   
 "\n\r\t-i\tPattern offset in payload default 0\r\n";
char *enableWoWLAN_Note_optionDetailsStr =  (char*)    
"\n\r\tNote:\tUser can wake up the MCU by pressing SW3,"
"\n\r\tAlso, since the MCU going to LPDS, any debug session would "
"be disconnected.\r\n";

char mDNSAdvertiseStr[]                  = "mdnsadvertise";
char mDNSAdvertiseUsageStr[]             = 
" [-help] [-n <Device name>] [-st <Service type>] [-so <Service over>]\r\n\t"
" [-ttl <Service TTL>] [-t <\"Adv. Text\">] [-p <Service port>]\r\n";
char *mDNSAdvertiseDetailsStr            =  (char*)
"Advertise a service over mDNS. service would be Advertised as: "
"'DeviceName._serviceType._serviceOver._local'.\n\r";
char *mDNSAdvertise_n_optionDetailsStr   =  
(char*)"\n\r\t-n\tSets Device name\n\r";
char *mDNSAdvertise_so_optionDetailsStr  =  
(char*)"\n\r\t-so\tSpecify L4 protocol over which the service runs\n\r";
char *mDNSAdvertise_st_optionDetailsStr  =  
(char*)"\n\r\t-st\tSets service type\n\r";
char *mDNSAdvertise_ttl_optionDetailsStr =  
(char*)"\n\r\t-ttl\tSets the time to stop advertising [seconds]\n\r";
char *mDNSAdvertise_t_optionDetailsStr   =  
(char*)"\n\r\t-t\tAdditinal text to be sent for query response\n\r";
char *mDNSAdvertise_p_optionDetailsStr   =  
(char*)"\n\r\t-p\tSpecify port over which the service runs\n\r";


char mDNSQueryStr[]                    = "mdnsquery";
char mDNSdQueryUsageStr[]              = 
" [-help] [-n <Device name>] [-st <Service type>] "
"[-so <Service over>] [-o <Oneshot>]\r\n";
char *mDNSdQueryDetailsStr             =  (char*)
"performs mDNS query for a given service, for example: "
"'PC1._ipp._tcp._local' will query for"
"\n\r\tdevice name PC1 who offers printing service type over TCP.\n\r"
"\n\r\tQuery can be One shot, i.e return the first result the query gets and "
"stop, or continues to get more answers.\n\r";
char *mDNSdQuery_n_optionDetailsStr  =  (char*)
"\n\r\t-n\tDevice name for query, should be character string (optional)\n\r";
char *mDNSdQuery_st_optionDetailsStr =  (char*)
"\n\r\t-st\tService Type for query,"
" should be character string (mandatory)\n\r";
char *mDNSdQuery_so_optionDetailsStr =  (char*)
"\n\r\t-so\tSet L4 protocol over which the service operates,"
" should be character string (mandatory)\n\r";
char *mDNSdQuery_o_optionDetailsStr     =  (char*)
"\n\r\t-o\tOneShot/Continues (Oneshot = [YES, NO (default)])\n\r";

char radiotool_Str[]                 = "radiotool";
char radiotoolDetailsStr[]           = 
"Puts device in Tranceiver mode to Test TX or RX operations.";

char radiotool_PromptStr[]           = "radiotool@CC32xx:";
char TxtaskStr[]                     = "txtask";
char TxtaskUsageStr[]                =
 " -help [-tx <Tx Mode>] [-t <Tx duration>] [-c <Tx Channel>] \n\r"
"        [-m <Modulation>] [-txp <\"Tx Power\">] "
"[-p <\"Preamble type\">] [-tone <\"CW tone offset\">] \n\r"
"         [-d <\"Dest. MAC \">] [-dp <\"TX Frame data pattern\">]"
" [-cca <\"cca Override\">] \n\r"
"         [-s <\"Frame size \">] [-a <\"Frames amount\">] \n\r";
char *TxtaskDetailsStr             =  
" Open Transmitter on a specific Wlan channel,"
" to Transmit in 3 different Tx Modes.\n\r";
char *Txtask_tx_optionDetailsStr   =  (char*)
"\n\r\t-tx\tSets Tx Mode. In range: [1,3]. Available modes:\n\r";
char *Txtask_tx1_optionDetailsStr  =  (char*)
"\n\r\t\t1\tStarts Transmitter in continues mode. This mode TX frames"
" generated internally by NWP, for a given amount time.\n\r";
char *Txtask_tx2_optionDetailsStr  =  (char*)
"\n\r\t\t2\tStarts Transmitter in packetized mode. "
"This mode sends each TX frame from host proceessor to the NWP TX queue.\n\r";
char *Txtask_tx3_optionDetailsStr  =  (char*)
"\n\r\t\t3\tStarts Transmitter in carrier wave mode. "
"User can set CW tone offset.\n\r";
char *Txtask_t_optionDetailsStr    =  (char*)
"\n\r\t-t\tSets Tx time duration for tx test. In mSec units.\n\r";
char *Txtask_c_optionDetailsStr    =  (char*)
"\n\r\t-c\tChannel to transmit data or CW on. In range: [1,13]"
", 5Ghz [36,169].\n\r";
char *Txtask_m_optionDetailsStr    =  (char*)
"\n\rSpecify desired Rate, or Modulation/Coding. In range: [1,21].\n\r";
char *Txtask_m1_optionDetailsStr   =  (char*)
"\n\r\t1.  1 Mbps (DSSS)\n\r";
char *Txtask_m2_optionDetailsStr   =  (char*)
"\n\r\t2.  2 Mbps (DSSS)\n\r";
char *Txtask_m3_optionDetailsStr   =  (char*)
"\n\r\t3.  5 Mbps (CCK)\n\r";
char *Txtask_m4_optionDetailsStr   =  (char*)
"\n\r\t4.  11 Mbps (CCK)\n\r";
char *Txtask_m5_optionDetailsStr   =  (char*)
"\n\r\t5.  Deprecated.\n\r";
char *Txtask_m6_optionDetailsStr   =  (char*)
"\n\r\t6.  6 Mbps. (OFDM)\n\r";
char *Txtask_m7_optionDetailsStr   =  (char*)
"\n\r\t7.  9 Mbps. (OFDM)\n\r";
char *Txtask_m8_optionDetailsStr   =  (char*)
"\n\r\t8.  12 Mbps. (OFDM)\n\r";
char *Txtask_m9_optionDetailsStr   =  (char*)
"\n\r\t9.  18 Mbps. (OFDM)\n\r";
char *Txtask_m10_optionDetailsStr  =  (char*)
"\n\r\t10. 24 Mbps. (OFDM)\n\r";
char *Txtask_m11_optionDetailsStr  =  (char*)
"\n\r\t11. 36 Mbps. (OFDM)\n\r";
char *Txtask_m12_optionDetailsStr  =  (char*)
"\n\r\t12. 48 Mbps. (OFDM)\n\r";
char *Txtask_m13_optionDetailsStr  =  (char*)
"\n\r\t13. 54 Mbps. (OFDM)\n\r";
char *Txtask_m14_optionDetailsStr  =  (char*)
"\n\r\t14. MCS0 : Modulation - BPSK,  Code rate - 1/2.\n\r";
char *Txtask_m15_optionDetailsStr  =  (char*)
"\n\r\t15. MCS1 : Modulation - QPSK,  Code rate - 1/2.\n\r";
char *Txtask_m16_optionDetailsStr  =  (char*)
"\n\r\t16. MCS2 : Modulation - QPSK,  Code rate - 3/4.\n\r";
char *Txtask_m17_optionDetailsStr  =  (char*)
"\n\r\t17. MCS3 : Modulation - 16QAM, Code rate - 1/2.\n\r";
char *Txtask_m18_optionDetailsStr  =  (char*)
"\n\r\t18. MCS4 : Modulation - 16QAM, Code rate - 3/4.\n\r";
char *Txtask_m19_optionDetailsStr  =  (char*)
"\n\r\t19. MCS5 : Modulation - 64QAM, Code rate - 2/3.\n\r";
char *Txtask_m20_optionDetailsStr  =  (char*)
"\n\r\t20. MCS6 : Modulation - 64QAM, Code rate - 3/4.\n\r";
char *Txtask_m21_optionDetailsStr  =  (char*)
"\n\r\t21. MCS7 : Modulation - 64QAM, Code rate - 5/6.\n\r";

char *Txtask_txp_optionDetailsStr  =  (char*)
"\n\rSpecify desired Tx Power, as dB offset from max power "
"(0 indicates MAX power) In range: [0,15]:\n\r";
char *Txtask_txp0_optionDetailsStr =  (char*)
"\n\r\t0.\t\n\r";

char *Txtask_p_optionDetailsStr    =  (char*)
"\n\rSpecify desired Preamble type. In range: [0,4]. Available modes:\n\r";
char *Txtask_p0_optionDetailsStr   =  (char*)
"\n\r\t0. Long preamble.\n\r";
char *Txtask_p1_optionDetailsStr   =  (char*)
"\n\r\t1. Short preamble.\n\r";
char *Txtask_p2_optionDetailsStr   =  (char*)
"\n\r\t2. OFDM mode.\n\r";
char *Txtask_p3_optionDetailsStr   =  (char*)
"\n\r\t3. Mixed mode.\n\r";
char *Txtask_p4_optionDetailsStr   =  (char*)
"\n\r\t4. Greenfield mode.\n\r";

char *Txtask_tone_optionDetailsStr =  (char*)
"\n\rSpecify desired Tone offset from center Frequency. Value is in multiples"
" of 312.5kHz In range: [-25,25].\n\r";
char *Txtask_d_optionDetailsStr    =  (char*)
"\n\r\t-p\tSpecify destination MAC field. Must be 6 bytes seperated by colons:"
" XX:XX:XX:XX:XX:XX.\n\r";
char *Txtask_dp_optionDetailsStr   =  (char*)
"\n\r\t-p\tSpecify desired data in Frame payload field. In range: [0,7]."
" Available modes:\n\r";
char *Txtask_dp0_optionDetailsStr  =  (char*)
"\n\r\t\t0. \tAll zeros.\n\r";
char *Txtask_dp1_optionDetailsStr  =  (char*)
"\n\r\t\t1. \tAll ones.\n\r";
char *Txtask_dp2_optionDetailsStr  =  (char*)
"\n\r\t\t2. \tIncremental order.\n\r";
char *Txtask_dp3_optionDetailsStr  =  (char*)
"\n\r\t\t3. \tDecremental order.\n\r";
char *Txtask_cca_optionDetailsStr  =  (char*)
"\n\r\t-cca\tOverride Clear channel assessment mechanism. \"cca Override\" ="
" [YES, NO]. Default is YES\n\r";
char *Txtask_s_optionDetailsStr    =  (char*)
"\n\r\t-s\tSets Frame size byte units, In range: [1,1500]."
" Default is 1500.\n\r";
char *Txtask_a_optionDetailsStr    =  (char*)
"\n\r\t-s\tSets the number of frames to transmit. Default is 10000."
" valid only for mode 2.\n\r";

char *cca_th_optionDetailStr       =  (char*)
"\n\r\tCCA threshold determine the threshold in which the channel is"
" considered as occupied.\n\r\tThe following can be set:\n\r";
char *cca_th1_optionDetailStr      =  (char*)
"\n\r\t1. Min:       (-88dBm)\n\r";
char *cca_th2_optionDetailStr      =  (char*)
"\n\r\t2. Low:       (-78dBm)\n\r";
char *cca_th3_optionDetailStr      =  (char*)
"\n\r\t3. Default:       (-68dBm)\n\r";
char *cca_th4_optionDetailStr      =  (char*)
"\n\r\t4. Med:       (-58dBm)\n\r";
char *cca_th5_optionDetailStr      =  (char*)
"\n\r\t5. High:       (-48dBm)\n\r";
char *cca_th6_optionDetailStr      =  (char*)
"\n\r\t6. Max:       (-38dBm)\n\r";

char RxtaskStr[]                   = "rxtask";
char RxtaskUsageStr[]              = " -help [-c <\"Wlan channel\">] "
"[-t <\"Rx duration\">] [-s <\"Show Rx statistics\">] \n\r";
char *RxtaskDetailsStr             = " Open Receiver on a specific"
" Wlan channel, for Rx duration time. Test can also print RX statistics.\n\r";
char *Rxtask_c_optionDetailsStr    =  (char*)
    "\n\r\t-c\tChannel to open the receiver on. In range: [1,13]"
	", 5Ghz [36,169].\n\r";
char *Rxtask_d_optionDetailsStr    =  (char*)
    "\n\r\t-d\tRx duration. Duration time needs to be specified"
	" in mSec units.\n\r";
char *Rxtask_s_optionDetailsStr    =  (char*)
    "\n\r\t-s\tStatistics. \"Show Rx statistics\" = [YES, NO]\n\r";


char p2pStartcmdStr[]              = "p2pstart";
char *p2pStartDetailsStr           = 
"Starts the Device as discoverable in P2P role, shows the local "
"device name.\n\r";


char p2pStopcmdStr[]               = "p2pstop";
char *p2pStopDetailsStr            = 
"Stops the Device from being in P2P role. If device is connected to other"
" P2P device, it will disconnect.\n\r";


char SetCountryCodeStr[]                = "Countrycode";
char SetCountryCodeUsageStr[]          = 
" -help  -g <Country code i.e US> \n\r";
char *CountryCodeDetailsStr             = "Set device country code \n\r";
char *CountryCode_g_optionDetailsStr    = 
"\n\r\t-g\tSet the device country code";
char *CountryCode_Note_optionDetailsStr = (char*)    
"\n\r\tNote:\t Country code consist of two capital characters  \r\n";

char a_optionStr[]            = "-a";
char b_optionStr[]            = "-b";
char c_optionStr[]            = "-c";
char f_optionStr[]            = "-f";
char g_optionStr[]            = "-g";
char r_optionStr[]            = "-r";
char h_optionStr[]            = "-h";
char i_optionStr[]            = "-i";
char l_optionStr[]            = "-l";
char n_optionStr[]            = "-n";
char o_optionStr[]            = "-o";
char p_optionStr[]            = "-p";
char s_optionStr[]            = "-s";
char t_optionStr[]            = "-t";
char st_optionStr[]           = "-st";
char tx_optionStr[]           = "-tx";
char tone_optionStr[]         = "-tone";
char cca_optionStr[]          = "-cca";
char u_optionStr[]            = "-u";
char m_optionStr[]            = "-m";
char x_optionStr[]            = "-x";
char v_optionStr[]            = "-v";
char d_optionStr[]            = "-d";
char e_optionStr[]            = "-e";
char ch_optionStr[]           = "-ch";
char txpow_optionStr[]        = "-txp";
char fs_optionStr[]           = "-fs";
char nb_optionStr[]           = "-nb";
char lh_optionStr[]           = "-lh";
char dp_optionStr[]           = "-dp";
char V_optionStr[]            = "-V";
char ip_optionStr[]           = "-ip";
char dns_optionStr[]          = "-dns";
char gw_optionStr[]           = "-gw";
char ent_optionStr[]          = "-ent";
char help_optionStr[]         = "-help";
char ttl_optionStr[]          = "-ttl";
char so_optionStr[]           = "-so";

char WPA_str[]                = "WPA";
char WPA2_str[]               = "WPA2";
char WEP_str[]                = "WEP";
char WPS_str[]                = "WPS";
char OPEN_str[]               = "OPEN";
char WPAWPA2_str[]            = "WPA/WPA2";
char YES_str[]                = "YES";
char NO_str[]                 = "NO";
char CONNECTED_str[]          = "CON";
char PROMISCUOUS_str[]        = "TRANS";
char space_str[]              = " ";
char SOURCE_MAC_str[]         = "S_MAC";
char DESTINATION_MAC_str[]    = "D_MAC";
char BSSID_str[]              = "BSSID";
char S_IP_str[]               = "S_IP";
char D_IP_str[]               = "D_IP";
char FRAME_TYPE_str[]         = "FRAME_TYPE";
char FRAME_SUBTYPE_str[]      = "FRAME_SUBTYPE";
char PATTERN_str[]            = "PATTERN";

char *MangmentFrames_str[]    = 
{"ASSOCIATION REQ", "ASSOCIATION RESPONSE", "REASSOCIATION REQ" , 
"REASSOCIATION RESPONSE", "PROBE REQ", "PROBE RESPONSE", "BEACON", "ATIM",
 "DISASSOCIATION" , "AUTHENTICATION", "DEAUTHENTICATION","ACTION CTRL FRAMES"};

char *CtrlFrames_str[]        = 
{"BLOCK ACK REQ", "BLOCK ACK", "PS POLL", "RTS", "CTS", "ACK", "CF END", 
"CF END ACK" };

char *DataFrames_str[]        = 
{"DATA", "DATA CF ACK", "DATA CF POLL", "DATA CF ACK POLL", "NO DATA FRAME",
 "CF ACK", "CF POLL", "CF ACK POLL", "QOS DATA", "QOS DATA CF ACK",
 "QOS DATA CF POLL", "QOS DATA CF ACK POLL","QOS NO DATA FRAME" , 
 "QOS CF ACK", "QOS CF POLL", "QOS CF ACK POLL"};


char fsError[]                = 
"File system error, please refer  \"FS ERRORS CODES\" section in errors.h";
char socketError[]            = 
"BSD Socket error, please refer \"BSD SOCKET ERRORS CODES\" "
"section in errors.h";
char netappError[]            = 
"Netapp error, please refer \"NETAPP ERRORS CODES\" section in errors.h";
char cmdError[]               = 
"Invalid option/command.";
char nwError[]                = 
"Network error";
char radioToolError[]         = 
"Radiotool error";


void printBorder(char ch, int n)
{
    int        i = 0;

    for(i=0; i<n; i++)    putch(ch);
}





