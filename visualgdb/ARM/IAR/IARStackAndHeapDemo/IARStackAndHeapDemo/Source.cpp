
#include <iar_dlmalloc.h>
#include <stdlib.h>

struct mallinfo IARHeapInfo;

void RecursionTest(int level)
{
    volatile int test = 123;
    if (level > 0)
        RecursionTest(level - 1);
}

int main()
{
    RecursionTest(1);
    RecursionTest(2);
    RecursionTest(3);
    
    IARHeapInfo = __iar_dlmallinfo();
    void *p = malloc(123);
    IARHeapInfo = __iar_dlmallinfo();
    free(p);
    IARHeapInfo = __iar_dlmallinfo();

}