#include "connection.h"
#include <iostream>

Connection::Connection()
    :connected(false),
      peer_sock(INVALID_SOCKET),
      self_sock(INVALID_SOCKET)
{
    timer = new QTimer(this);
    peer_id = "";
    self_id = "";
    im = &IMClient::Instance();
    linksigSlot();

}

Connection::~Connection()
{
    if(timer)
        delete timer;
}

Connection::Connection(SOCKET sock)
    :connected(false),
      peer_sock(sock),
      self_sock(INVALID_SOCKET)
{
    timer = new QTimer(this);
    peer_id = "";
    self_id = "";
    im = &IMClient::Instance();
    linksigSlot();
}

void Connection::setPeerSock(SOCKET peer_sock)
{
    if(peer_sock == INVALID_SOCKET)
        return;
    if(this->peer_sock != INVALID_SOCKET){
        closesocket(this->peer_sock);
        connected = false;
    }
    this->peer_sock = peer_sock;
}

/*
 * 接收消息，向vector中写数据
 */
void Connection::recv_msg()
{
    if(peer_sock == INVALID_SOCKET)
        return;
    int iResult;
    char recvbuf[LINE_BUF] = {0};
    int recvbuflen = LINE_BUF;
    connected = true;
    do {
        iResult = recv(peer_sock, recvbuf, recvbuflen, 0);
        if (iResult > 0) {

            JSPP msg = parse(string(recvbuf));
            if(peer_id == "") {
                peer_id = msg.from;
            }
            printf("Bytes received: %d and body is %s\n", iResult, msg.body.c_str());
            im->pushMsg(msg);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(peer_sock);
            break ;
        }
    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(peer_sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(peer_sock);
    }

    // cleanup
    closesocket(peer_sock);
    connected = false;
    peer_sock = INVALID_SOCKET;
}

int Connection::connectToPeer()
{
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;

    int iResult;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", "1314", &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        self_sock = socket(ptr->ai_family, ptr->ai_socktype,
                           ptr->ai_protocol);
        if (self_sock == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            return 1;
        }

        // Connect to server.
        iResult = ::connect( self_sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(self_sock);
            self_sock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (self_sock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        return 1;
    }
    return 0;
}

/*
 * 发送消息，从unsend_vector中读数据
 */
void Connection::send_msg(string msg)
{
//    if(unsend_msg_vec.size() ==  0)
//        return;
    if(self_sock == INVALID_SOCKET){
        connectToPeer();
    }
    string send_str = msg;

    const char *sendbuf = send_str.c_str();
    // Send an initial buffer
    int iResult = send( self_sock, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(self_sock);
        self_sock = INVALID_SOCKET;
        return ;
    }

    printf("Bytes Sent: %ld\n", iResult);

//    shutdown the since no more data will be sent
//    iResult = shutdown(self_sock, SD_SEND);
//    if (iResult == SOCKET_ERROR) {
//        printf("shutdown failed with error: %d\n", WSAGetLastError());
//        closesocket(self_sock);
//        self_sock = INVALID_SOCKET;
//        return ;
//    }
}

void Connection::linksigSlot()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(checkQueue()));
    timer->start(100);
}

void Connection::checkQueue()
{

    if(!connected)
        recv_msg();
}

int Connection::pushMsg(JSPP msg)
{
    im->pushMsg(msg);
    return 0;
}

JSPP Connection::popMsg()
{
    JSPP msg;

    /*
    if(im->unread_msg_list.size() > 0) {
        msg = im->unread_msg_list.back();
        im->unread_msg_list.pop_back();
    }
    */
    return msg;
}

string Connection::getPeerid()
{
    return peer_id;
}

void Connection::setPeer_id(string id)
{
    peer_id = id;
}

 size_t Connection::getUnsendCount()
{
     //todo 此接口的作用
    //return im->unsend_msg_vec.size();
     return 0;
}

  size_t Connection::getUnreadCount()
{
    return im->getUnreadMsg(peer_id).size();
}

void Connection::closeSock()
{
    if(peer_sock != INVALID_SOCKET){
        closesocket(peer_sock);
        connected = false;
    }
}

SOCKET Connection::getpeer_sock()
{
    return peer_sock;
}
