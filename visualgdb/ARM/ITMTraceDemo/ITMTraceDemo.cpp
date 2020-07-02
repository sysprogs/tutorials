#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "boards.h"
	
#ifdef __cplusplus
}
#endif

const uint8_t leds_list[LEDS_NUMBER] = LEDS_LIST;

extern "C" int _write(int fd, char *pBuffer, int size)
{
    for (int i = 0; i < size; i++)
        ITM_SendChar(pBuffer[i]);
    return size;
}

int main(void)
{
	LEDS_CONFIGURE(LEDS_MASK);

	for (;;)
	{
		for (int i = 0; i < LEDS_NUMBER; i++)
		{
            printf("LED #%d\r\n", i);
            LEDS_INVERT(1 << leds_list[i]);
			nrf_delay_ms(500);
		}
	}
}
