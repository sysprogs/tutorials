#include <driverlib.h>

#include <stdint.h>
#include <stdbool.h>

int main(void)
{
    volatile uint32_t ii;

    WDT_A_holdTimer();
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);

    while (1)
    {
        for(ii=0; ii < 10000; ii++);
        GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}
