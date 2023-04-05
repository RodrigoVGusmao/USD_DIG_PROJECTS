#include<winsock2.h>
#include<stdio.h>

main()
{
    sockaddr_in sockaddr1;

    WSADATA wsa;
    SOCKET socks;

    bool close;

    system("color A");

    if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        system("color C");
        printf("error to init WinSock:%d", WSAGetLastError());
        return 1;
    }
    printf("sucessfully initialized WinSock...\n");
    Sleep(100);
    system("cls");

    socks = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socks == INVALID_SOCKET)
    {
        system("color C");
        printf("error to create socket:%d", WSAGetLastError());
        return 1;
    }
    printf("secessfully created a new socket...\n");
    Sleep(100);
    system("cls");

    sockaddr1.sin_family = AF_INET;
    sockaddr1.sin_port = htons(27015);
    sockaddr1.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socks, (SOCKADDR*)&sockaddr1, sizeof(sockaddr1)) == SOCKET_ERROR)
    {
        system("color C");
        printf("error to bind:%d", WSAGetLastError());
        return 1;
    }
    printf("sucessfully binded...\n");
    Sleep(100);
    system("cls");

    listen(socks, 1);

    while((socks = accept(socks, 0, 0)) == SOCKET_ERROR)
    {
        system("color E");
        printf("Waiting connection...\n");
        Sleep(1);
        system("cls");
    }
    system("color A");
    printf("cient connected\n");
    Sleep(100);
    system("cls");

    while(close == false)
    {
        printf("DEU");
        Sleep(1);
    }

    if(closesocket(socks) == SOCKET_ERROR)
    {
        system("color E");
        printf("error to close the socket:%d", WSAGetLastError());
        Sleep(100);
    }
    WSACleanup();

    printf("closing the application...");

    return 0;
}
