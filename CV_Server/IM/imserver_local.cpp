#include "imserver_local.h"
#include <iostream>
#include "imclient.h"
#include "user.h"

IMServerLocal::IMServerLocal(string port)
    :port(port),ListenSocket(INVALID_SOCKET),winsockStarted(false)
{
    im = &IMClient::Instance(new User(2));
    std::cout << im->getCurrID() << std::endl;
    WSADATA WSAData = {0};
    int status = WSAStartup(MAKEWORD(2, 0), &WSAData);
    if (status != 0)
        std::cout << "[ERROR]: " << status << " Unable to start Winsock." << std::endl;
    else
        winsockStarted = true;
    initSock();
    linkSignalWithSlot();
}
int IMServerLocal::start()
{
    while(1){
        SOCKET ClientSocket = INVALID_SOCKET;
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
        emit friendConnected(ClientSocket);
    }
    return 0;
}

IMServerLocal::~IMServerLocal()
{
    stop();

    if (winsockStarted)
        WSACleanup();
}

void IMServerLocal::connectFromFriend(SOCKET ClientSocket)
{

    //消息在这里集中解析分发
    int iResult,iSendResult;
    char recvbuf[512];
    int recvbuflen = 512;
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            emit parseOver(string(recvbuf));
            // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return ;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return ;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return ;
    }

    // cleanup
    closesocket(ClientSocket);

}

//初始化socket并开始监听
bool IMServerLocal::initSock()
{
    stop();
    struct addrinfo hints = {0};
    struct addrinfo *res = NULL;

    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int status = getaddrinfo(NULL, port.c_str(), &hints, &res);
    if (status != 0)
    {
        std::cout << "[ERROR]: " << status << " Unable to get address info for Port " << port << "." << std::endl;
        return false;
    }

    SOCKET newsock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (newsock == INVALID_SOCKET)
    {
        std::cout << "[ERROR]: " << WSAGetLastError() << " Unable to create Socket." << std::endl;
        freeaddrinfo(res);
        return false;
    }

    if (bind(newsock, res->ai_addr, res->ai_addrlen) == SOCKET_ERROR)
    {
        std::cout << "[ERROR]: " << WSAGetLastError() << " Unable to bind Socket." << std::endl;
        freeaddrinfo(res);
        closesocket(newsock);
        return false;
    }

    freeaddrinfo(res);
    int maxConnections = 100;
    if (listen(newsock, maxConnections) == SOCKET_ERROR)
    {
        std::cout << "[ERROR]: " << WSAGetLastError() << " Unable to Listen on Port " << port << "." << std::endl;
        closesocket(newsock);
        return false;
    }

    ListenSocket = newsock;
    return true;
}

void IMServerLocal::linkSignalWithSlot()
{
    connect(this,SIGNAL(friendConnected(SOCKET)), this, SLOT(connectFromFriend(SOCKET)));
    connect(this, SIGNAL(parseOver(string)), im, SLOT(recvMsg(string)));
}
void IMServerLocal::stop()
{
    if (ListenSocket != INVALID_SOCKET)
    {
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
    }
}
