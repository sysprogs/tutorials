#include <windows.h>
#include <stdio.h>

extern "C" void SysTick_Handler(void)
{
}

void led_init()
{
}

void led_on()
{
    printf("[LED ON]\n");
}

void led_off()
{
    printf("[LED OFF]\n");
}

void delay(int msec)
{
    Sleep(msec);
}
