#include <stm32f4xx_hal.h>
#include <stm32_hal_legacy.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

struct QueueItem
{
    unsigned Value;
    QueueItem *Prev, *Next;
};

struct DemoQueue
{
    QueueItem *First, *Last;
    int ItemCount;
};

static DemoQueue s_Queues[4];
static int s_ItemsAllocated;

void EnqueueTestItem(unsigned value)
{
    QueueItem *item = (QueueItem *)malloc(sizeof(QueueItem));
    if (++s_ItemsAllocated > 16)
        asm("bkpt 255");
    item->Value = value;
	
    DemoQueue *queue = &s_Queues[value % 4];
    item->Prev = queue->Last;
    item->Next = NULL;
    
    if (queue->Last)
        queue->Last->Next = item;

    queue->Last = item;
    if (!queue->First)
        queue->First = item;
	
    queue->ItemCount++;
}

void DequeueItem(unsigned queueNumber)
{
    DemoQueue *queue = &s_Queues[queueNumber % 4];
    QueueItem *item = queue->First;
	
    if (!item)
        return;
	
    queue->First = item->Next;
    if (!queue->First)
        queue->Last = NULL;
    
    queue->ItemCount--;
    s_ItemsAllocated--;
	
    free(item);
}

int main(void)
{
    for (int i = 0;; i++)
    {
        EnqueueTestItem(rand());
        DequeueItem(rand());
    }
}
