/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include <Board_LED.h>
 
#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif
 
void Thread1Body(void *argument)
{
    for (;;)
    {
        LED_On(0);
        LED_Off(0);
    }
}

void Thread2Body(void *argument)
{
    for (;;)
    {
        LED_On(1);
        LED_Off(1);
    }
}

int IsValidCodeAddress(void *pAddr)
{
    return (pAddr >= (void *)0x08000000 && pAddr <= (void *)0x080fffff);
}

int IsValidStackAddress(void **pStackSlot)
{
    return pStackSlot >= (void *)0x20000000 && pStackSlot < (void *)0x2001ffff;
}

#include <SysprogsProfiler.h>
 
int main(void) {
 
    // System Initialization
    SystemCoreClockUpdate();
    LED_Initialize();
    InitializeSamplingProfiler();
    
    const osThreadAttr_t thread1_attr = {
        .name = "Thread 1"
    };
 
    const osThreadAttr_t thread2_attr = {
        .name = "Thread 2"
    };
 
    osKernelInitialize();                  // Initialize CMSIS-RTOS
    osThreadNew(Thread1Body, NULL, &thread1_attr);        // Create application main thread
    osThreadNew(Thread2Body, NULL, &thread2_attr);        // Create application main thread
    osKernelStart();                       // Start thread execution
    for(;  ;) {}
}

void thread_switch_helper()
{
}