#include <stdlib.h>

int main(int argc, char *argv[])
{
    srand(123);
    for (;;)
    {
        int randomVar = rand();
        if ((randomVar % 10) == 0)
            *((int *)0) = 1;
    }
    return 0;
}