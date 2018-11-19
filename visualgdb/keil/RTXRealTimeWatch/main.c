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
 
#include <SysprogsProfiler.h>

osMutexId_t g_Mutex;

void ExclusiveWait(int delay)
{
    osMutexAcquire(g_Mutex, osWaitForever);
    osDelay(delay);
    osMutexRelease(g_Mutex);
}

void Thread1Body(void *argument)
{
    for (;;)
    {
        LED_On(0);
        ExclusiveWait(100);
        LED_Off(0);
        ExclusiveWait(100);
    }
}

void Thread2Body(void *argument)
{
    for (;;)
    {
        LED_On(1);
        ExclusiveWait(200);
        LED_Off(1);
        ExclusiveWait(200);
    }
}
 

void thread_switch_helper()
{
}

int main(void) {
    
    osMutexAttr_t mutexAttr = { .name = "TestMutex" };
    g_Mutex = osMutexNew(&mutexAttr);
    
    InitializeInstrumentingProfiler();
 
    // System Initialization
    SystemCoreClockUpdate();
    LED_Initialize();
    
    const osThreadAttr_t thread1_attr = {
        .name = "Thread 1",
        .stack_size = 4096,
    };
 
    const osThreadAttr_t thread2_attr = {
        .name = "Thread 2",
        .stack_size = 4096,
    };
 
    osKernelInitialize();                  // Initialize CMSIS-RTOS
    
    osThreadNew(Thread1Body, NULL, &thread1_attr);        // Create application main thread
    osThreadNew(Thread2Body, NULL, &thread2_attr);        // Create application main thread
    osKernelStart();                       // Start thread execution
    
    for(;  ;) {}
}
