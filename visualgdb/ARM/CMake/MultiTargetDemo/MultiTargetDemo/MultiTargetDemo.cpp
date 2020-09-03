#include <OSAL.h>


int main(void)
{
    led_init();

	for (;;)
	{
    	led_on();
    	delay(500);
    	led_off();
    	delay(500);
	}
}
