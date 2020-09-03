#include <OSAL.h>

void DoSleep();

int main(void)
{
    led_init();

	for (;;)
	{
    	led_on();
    	DoSleep();
    	led_off();
    	DoSleep();
	}
}
