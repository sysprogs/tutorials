#include <stdio.h>

void SharedLibraryFunction1()
{
	printf("Hello from shared library");
    int **p = 0;
    p[0] = 0;
}
