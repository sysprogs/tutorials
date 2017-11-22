#include <gtest/gtest.h>
#include <stdio.h>
#include <memory.h>

template <class _Element> class Queue
{
private:
    _Element *m_pData = nullptr;
    int m_ReadOffset = 0, m_WriteOffset = 0, m_Allocated = 0;
    
public:
    Queue(int reservedCount = 4)
    {
        if (reservedCount)
        {
            m_Allocated = reservedCount;
            m_pData = (_Element *)malloc(reservedCount * sizeof(_Element));
            for (int i = 0; i < reservedCount; i++)
                new(&m_pData[i]) _Element();
        }
    }
    
    ~Queue()
    {
        if (m_pData)
            free(m_pData);
    }
    
    int GetCount()
    {
        return m_WriteOffset - m_ReadOffset;
    }
    
    void Enqueue(const _Element *pElement)
    {
        if (GetCount() >= m_Allocated)
        {
            m_Allocated *= 2;
            m_pData = (_Element *)realloc(m_pData, m_Allocated * sizeof(_Element));
            for (int i = m_Allocated / 2; i < m_Allocated; i++)
                new(&m_pData[i]) _Element();
        }
        
        if (m_ReadOffset)
        {
            int base = m_ReadOffset;
            memmove(m_pData, m_pData + base, (m_Allocated - base) * sizeof(_Element));
            m_ReadOffset -= base;
            m_WriteOffset -= base;
        }
        
        m_pData[m_WriteOffset++] = *pElement;
    }
    
    bool Dequeue(_Element *pElement)
    {
        if (m_ReadOffset >= m_WriteOffset)
            return false;
        
        *pElement = m_pData[m_ReadOffset++];
        return true;
    }
};

void EnqueueConsecutive(Queue<int> &queue, int first, int count)
{
    for (int i = 0; i < count; i++)
    {
        int tmp = i + first;
        queue.Enqueue(&tmp);
    }
}

void DequeueConsecutive(Queue<int> &queue, int first, int count)
{
    for (int i = 0; i < count; i++)
    {
        int tmp;
        ASSERT_TRUE(queue.Dequeue(&tmp));
        ASSERT_EQ(i + first, tmp);
    }
}

void CheckEndOfQueue(Queue<int> &queue)
{
    int tmp;
    ASSERT_FALSE(queue.Dequeue(&tmp));
}

TEST(QueueTests, BasicTest)
{
    Queue<int> queue;
    EnqueueConsecutive(queue, 0, 3);
    DequeueConsecutive(queue, 0, 3);
    CheckEndOfQueue(queue);
}

class SmartPointer
{
private:
    char *m_Pointer;
    
public:
    SmartPointer()
    {
        m_Pointer = nullptr;
    }
    
    SmartPointer(const SmartPointer &right)
    {
        if (right.m_Pointer)
            m_Pointer = strdup(right.m_Pointer);
        else
            m_Pointer = nullptr;
    }
    
    void operator=(const SmartPointer &right)
    {
        if (m_Pointer)
            free(m_Pointer);
        if (right.m_Pointer)
            m_Pointer = strdup(right.m_Pointer);
        else
            m_Pointer = nullptr;
    }
    
    ~SmartPointer()
    {
        if (m_Pointer)
            free(m_Pointer);
    }
};

TEST(QueueTests, ReallocTest)
{
    Queue<int> queue;
    EnqueueConsecutive(queue, 0, 16);
    DequeueConsecutive(queue, 0, 3);
    EnqueueConsecutive(queue, 16, 32);
    DequeueConsecutive(queue, 3, 16 + 32 - 3);
    CheckEndOfQueue(queue);
}

TEST(QueueTests, SmartPointerTest)
{
    Queue<SmartPointer> queue;
    SmartPointer sp;
    for (int i = 0; i < 10; i++)
        queue.Enqueue(&sp);
}