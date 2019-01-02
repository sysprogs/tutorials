#include "mbed.h"

DigitalOut g_LED(LED1);

int main()
{
    for (;;)
	{
        g_LED = !g_LED;
        wait(0.5f);
    }
}
