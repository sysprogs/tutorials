#include <iostream>

#if PROJECT_VERSION == 1
#error Version 1
#endif
#if PROJECT_VERSION == 2
#error Version 2
#endif

using namespace std;

int main(int argc, char *argv[])
{
	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
	cout << sz << endl;	//<================= Put a breakpoint here
	return 0;
}