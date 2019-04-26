/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
extern "C" {
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include <Board_LED.h>

}
 
void Thread1Body(void *argument)
{
	int bigvar[4096];
    for (;;)
    {
		bigvar[4095] = 0;
        LED_On(0);
        osDelay(100);
        LED_Off(0);
        osDelay(100);
    }
}

void Thread2Body(void *argument)
{
    for (;;)
    {
        LED_On(1);
        osDelay(200);
        LED_Off(1);
        osDelay(200);
    }
}
 
int main(void) {
 
    // System Initialization
    SystemCoreClockUpdate();
    LED_Initialize();
    
    const osThreadAttr_t thread1_attr = {
         "Thread 1"
    };
 
    const osThreadAttr_t thread2_attr = {
         "Thread 2"
    };
 
    osKernelInitialize();                  // Initialize CMSIS-RTOS
    osThreadNew(Thread1Body, NULL, &thread1_attr);        // Create application main thread
    osThreadNew(Thread2Body, NULL, &thread2_attr);        // Create application main thread
    osKernelStart();                       // Start thread execution
    for(;  ;) {}
}
