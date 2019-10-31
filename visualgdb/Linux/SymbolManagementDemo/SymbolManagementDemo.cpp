#include <iostream>

using namespace std;

void SharedLibraryFunction1();
void SharedLibraryFunction2();

int main(int argc, char *argv[])
{
    SharedLibraryFunction1();
    SharedLibraryFunction2();

    char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
	cout << sz << endl;	//<================= Put a breakpoint here
	return 0;
}