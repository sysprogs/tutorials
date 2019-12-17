#include <stdio.h>

#ifndef TESTMACRO
#error TESTMACRO is not defined
#endif

void StaticLibraryFunction3()
{
    printf("Hello from static library");
}
