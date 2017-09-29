#include <iostream>

#ifdef _WIN32
#include <winsock.h>
#pragma comment (lib, "ws2_32")
#else
#include <unistd.h>
#endif

#if defined(__arm__) && !defined(__linux__)
#include <string.h>

static void gethostname(char *pBuffer, int size)
{
    strncpy(pBuffer, "(embedded)", size);
}
#endif


int main()
{
#ifdef _WIN32
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);
#endif
    char name[512];
    gethostname(name, sizeof(name));
    std::cout << "Hello, " << name << std::endl;
    return 0;
}

