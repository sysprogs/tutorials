#pragma once
/* This file has been generated by VisualGDB.
   DO NOT EDIT MANUALLY. THE FILE WILL BE OVERWRITTEN.
   Use VisualGDB Project Properties window to edit these settings instead. */

#include "r_ioport.h"
#define ARDUINO_A0_MIKROBUS_AN (IOPORT_PORT_00_PIN_00)
#define ARDUINO_A1 (IOPORT_PORT_00_PIN_01)
#define ARDUINO_A2 (IOPORT_PORT_00_PIN_02)
#define ARDUINO_A3 (IOPORT_PORT_00_PIN_03)
#define USER_SW1 (IOPORT_PORT_00_PIN_04)
#define ARDUINO_A4 (IOPORT_PORT_00_PIN_12)
#define ARDUINO_A5 (IOPORT_PORT_00_PIN_13)
#define USER_SW2 (IOPORT_PORT_00_PIN_15)
#define ARDUINO_MISO_MIKROBUS_MISO_PMOD1_MISO (IOPORT_PORT_01_PIN_00)
#define ARDUINO_MOSI_MIKROBUS_MOSI_PMOD1_MOSI (IOPORT_PORT_01_PIN_01)
#define ARDUINO_CLK_MIKROBUS_CLK_PMOD1_CLK (IOPORT_PORT_01_PIN_02)
#define ARDUINO_SS_MIKROBUS_SS_PMOD1_SS (IOPORT_PORT_01_PIN_03)
#define ARDUINO_D3 (IOPORT_PORT_01_PIN_04)
#define PMOD1_IO1 (IOPORT_PORT_01_PIN_05)
#define PMOD1_IO2 (IOPORT_PORT_01_PIN_06)
#define ARDUINO_D4 (IOPORT_PORT_01_PIN_07)
#define ARDUINO_D8 (IOPORT_PORT_01_PIN_09)
#define ARDUINO_D2_MIKROBUS_INT (IOPORT_PORT_01_PIN_10)
#define MIKROBUS_PWM (IOPORT_PORT_01_PIN_11)
#define ARDUINO_D9 (IOPORT_PORT_01_PIN_12)
#define ARDUINO_D7 (IOPORT_PORT_01_PIN_13)
#define ARDUINO_TX_MIKROBUS_TX (IOPORT_PORT_02_PIN_05)
#define ARDUINO_RX_MIKROBUS_RX (IOPORT_PORT_02_PIN_06)
#define ARDUINO_RESET_MIKROBUS_RST (IOPORT_PORT_02_PIN_07)
#define PMOD1_RST (IOPORT_PORT_02_PIN_08)
#define GROVE1_SCL_QWIIC_SCL (IOPORT_PORT_03_PIN_01)
#define GROVE1_SDA_QWIIC_SDA (IOPORT_PORT_03_PIN_02)
#define PMOD2_RST (IOPORT_PORT_03_PIN_03)
#define PMOD2_IO2 (IOPORT_PORT_03_PIN_04)
#define PMOD2_SCK (IOPORT_PORT_04_PIN_00)
#define PMOD2_TXD (IOPORT_PORT_04_PIN_01)
#define PMOD2_RXD (IOPORT_PORT_04_PIN_02)
#define PMOD2_CTS (IOPORT_PORT_04_PIN_03)
#define ARDUINO_SDA_MIKROBUS_SDA (IOPORT_PORT_04_PIN_07)
#define ARDUINO_SCL_MIKROBUS_SCL (IOPORT_PORT_04_PIN_08)
#define PMOD2_INT (IOPORT_PORT_04_PIN_09)
#define PMOD2_CS2 (IOPORT_PORT_04_PIN_10)
#define PMOD1_INT (IOPORT_PORT_04_PIN_11)
#define ARDUINO_D6 (IOPORT_PORT_05_PIN_00)
#define GROVE2_SDA (IOPORT_PORT_05_PIN_01)
#define GROVE2_SCL (IOPORT_PORT_05_PIN_02)
#define USER_LED3 (IOPORT_PORT_09_PIN_13)
#define USER_LED2 (IOPORT_PORT_09_PIN_14)
#define USER_LED1 (IOPORT_PORT_09_PIN_15)

extern const ioport_cfg_t g_bsp_pin_cfg;

void BSP_PinConfigSecurityInit();
