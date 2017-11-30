#include "imserver_local.h"
#include <iostream>
#include "imclient.h"
#include "user.h"
#include <thread>
#include "utils/jsoncpp/json.h"
#include "IM/connection.h"
#include <QDebug>
IMServerLocal::IMServerLocal(string port)
    :port(port),ListenSocket(INVALID_SOCKET),winsockStarted(false)
{
    realPort = "0";
    im = &IMClient::Instance();
    std::cout << im->getCurrID() << std::endl;
    WSADATA WSAData = {0};
    int status = WSAStartup(MAKEWORD(2, 0), &WSAData);
    if (status != 0)
        std::cout << "[ERROR]: " << status << " Unable to start Winsock." << std::endl;
    else
        winsockStarted = true;
    initSock();
    linkSignalWithSlot();
    qRegisterMetaType<SOCKET>("SOCKET");

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
            return 1;
        }
        else {
            emit newConn(ClientSocket);
        }
    }

    return 0;
}

std::string IMServerLocal::getrealPort()
{
    qDebug() << "get realport and realport is " << QString::fromStdString(realPort);
    return realPort;
}


IMServerLocal::~IMServerLocal()
{
    stop();
    if (winsockStarted)
        WSACleanup();
}

void Distributor::msg_distribution(SOCKET ClientSocket)
{
    if(ClientSocket == INVALID_SOCKET)
        return;
    IMClient *im = &IMClient::Instance();
    int iResult;
    char recvbuf[LINE_BUF] = {0};
    int recvbuflen = LINE_BUF;
    do {
#ifdef ENCRYPT
        char recvbuf_tmp[LINE_BUF] = {0};
        iResult = recv(ClientSocket, recvbuf_tmp, recvbuflen, 0);
        printf("recv is %s\n", recvbuf_tmp);
        decode(recvbuf_tmp);
        memcpy(recvbuf, recvbuf_tmp,LINE_BUF);
#else
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
#endif
        if (iResult > 0) {
            JSPP msg = parse(string(recvbuf));
            printf("Bytes received: %d and body is %s\n", iResult, msg.body.c_str());
            im->pushMsg(msg);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            break ;
        }
    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
    }
    closesocket(ClientSocket);
    //消息在这里集中解析分发
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

    //int status = getaddrinfo(NULL, NULL, &hints, &res);

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
    struct sockaddr sockName = {0};
    int len = sizeof(sockName);;
    getsockname(newsock, &sockName, &len);

    qDebug() << "Port is " << (((struct sockaddr_in*)(&sockName))->sin_port);
    u_short port_tmp = ntohs(((struct sockaddr_in*)(&sockName))->sin_port);

    realPort = QString::number(port_tmp).toStdString();
    qDebug() << "realPort is " << QString::fromStdString(realPort);

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
    Distributor *worker = new Distributor;
    worker->moveToThread(&distributorThread);
    connect(&distributorThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &IMServerLocal::newConn, worker, &Distributor::msg_distribution);
    distributorThread.start();
}
void IMServerLocal::stop()
{
    if (ListenSocket != INVALID_SOCKET)
    {
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
    }
}


