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
osSemaphoreId_t g_Semaphore;

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
        osSemaphoreAcquire(g_Semaphore, osWaitForever);
        osDelay(10);
    }
}

void Thread2Body(void *argument)
{
    for (;;)
    {
        for (int i = 0; i < 10; i++)
            osSemaphoreRelease(g_Semaphore);
        osDelay(200);
    }
}
 

void thread_switch_helper()
{
}

int main(void) {
    osMutexAttr_t mutexAttr = { .name = "TestMutex" };
    g_Mutex = osMutexNew(&mutexAttr);
    osSemaphoreAttr_t semaphoreAttr = { .name = "TestSemaphore" };
    g_Semaphore = osSemaphoreNew(1000, 0, &semaphoreAttr);
    
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
