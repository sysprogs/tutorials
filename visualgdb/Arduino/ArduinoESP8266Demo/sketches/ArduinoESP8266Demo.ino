#include <GDBStub.h>

//Comment out the definition below if you don't want to use the ESP8266 gdb stub.
#define ESP8266_USE_GDB_STUB

#ifdef ESP8266_USE_GDB_STUB
extern "C" int gdbstub_init();
extern "C" int gdbstub_do_break();
#endif

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(74880);
    
#ifdef ESP8266_USE_GDB_STUB
    gdbstub_init();
    gdbstub_do_break();
#endif
}

void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
}
