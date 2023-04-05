#include<winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>

main()
{
    WSADATA wsa;

    /** initializing */
    system("color A");

    /** initializing winsock */
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {
        system("color C");
        printf("error to init WinSock...\nexiting...");
        return 1;
    }
    printf("successfully initialized winsock...\n");
    system("cls");

    return 0;
}
