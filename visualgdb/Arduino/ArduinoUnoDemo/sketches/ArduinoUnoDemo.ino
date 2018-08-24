#include <SoftwareSerial.h>

SoftwareSerial serial(10, 11);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.begin(57600);
    while (!Serial)
    {
    }
}

int g_Iteration;

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    
    char tmp[32];
    sprintf(tmp, "Iteration %d\r\n", g_Iteration++);
    Serial.write(tmp);
}
