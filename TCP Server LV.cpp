#include <iostream>
#include <WS2tcpip.h>
#include <stdlib.h>

#pragma comment (lib, "ws2_32.lib")



int main()
{
    FreeConsole();
    std::cout << "program start\n";
    //initalze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        std::cerr << "Winsock nott initialze! \n";
    }

    //create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET) {
        std::cerr << "Socket not created!\n";
    }

    //bind ip & port
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    //tell winsock its only lisnig
    listen(listening, SOMAXCONN);

    //wait for connection
    sockaddr_in client;
    int clientsize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Client Socket fail!\n";
    }

    char host[NI_MAXHOST];      //client's Remote Name
    char service[NI_MAXHOST];   //sercice (i.e. port) The Client is connected on

    ZeroMemory(host, NI_MAXHOST); // same as:Memset(HOST,0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXHOST);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0)
    {
        //std::cout << host << " connected on port " << service << "\n";
    }
    else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " <<
            ntohs(client.sin_port) << "\n";
    }

    //close listen socket
    closesocket(listening);

    //while loop: accept end echo massage back to cliant
    char buf[255];

    while (true) {
        ZeroMemory(buf, 255);

        //waiting for client to send data
        int bytsReceived = recv(clientSocket, buf, 255, 0);
        if (bytsReceived == SOCKET_ERROR) {
            std::cerr << "Error In Recv()!!\n";
            break;
        }
        if (bytsReceived == 0) {
            std::cout << "Client Disconnected \n";
            break;
        }

        //echo massage backi to client
        send(clientSocket, buf, bytsReceived + 1, 0);
        std::cout << buf << "\n";

        char shutD[255] = "shutDown\n";
        int shutDtrue = std::strcmp(shutD, buf);
        //std::cout << shutDtrue << "\n";
        if (shutDtrue == 0) {
            std::cout << "shutig pc off\n";
            system("c:\\windows\\system32\\shutdown /s /t 1\n\n");
        }

    }

    //close the sock
    closesocket(clientSocket);

    //cleanup wnsock
    WSACleanup();

    return 0;
}