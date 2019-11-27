#include <SPI.h>
#include <CRC32.h>
#include <LwIP.h>
#include <Dhcp.h>
#include <Dns.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <lwipopts.h>
#include <lwipopts_default.h>
#include <STM32Ethernet.h>
#include <MyDemoLibrary.h>

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
    CRC32 crc;
    crc.update("Test");
    uint32_t result = crc.finalize();
}

void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}
