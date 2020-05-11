#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    char tmp[512];
#ifdef IS_WINDOWS_BUILD
    strcpy(tmp, "Windows");
#else
    gethostname(tmp, sizeof(tmp));
#endif
    cout << "The current host name is " << tmp << std::endl;
    return 0;
}