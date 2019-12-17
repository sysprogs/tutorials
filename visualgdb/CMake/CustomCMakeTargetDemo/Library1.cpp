#include <stdio.h>
#ifndef TESTMACRO
#error TESTMACRO is not defined
#endif

void StaticLibraryFunction1()
{
	printf("Hello from static library");
}
