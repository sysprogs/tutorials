#include <HardwareSerial.h>

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

int g_Iter = 0;

void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);
	delay(1000);
    
    char tmp[32];
    sprintf(tmp, "Iteration %d\r\n", g_Iter++);
    Serial.write(tmp);
}
