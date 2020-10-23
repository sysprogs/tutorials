#ifndef SIMULATION
#include <stm32f4xx_hal.h>
#endif
#include <gtest/gtest.h>

#ifndef SIMULATION
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
#endif

#ifdef SIMULATION
int main(int argc, char *argv[])
{
#else
int main(void)
{
    HAL_Init();
    char *pargv = (char *)"";
    char **argv = &pargv;
    int argc = 1;
#endif

	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
