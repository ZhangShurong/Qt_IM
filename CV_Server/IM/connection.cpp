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
    char recvbuf[LINE_BUF];
    int recvbuflen = LINE_BUF;
    connected = true;
    do {
        iResult = recv(peer_sock, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            JSPP msg = parse(string(recvbuf));
            if(peer_id == "") {
                peer_id = msg.from;
            }
            std::cout << msg.body << std::endl;
            unread_msg_vec.push_back(msg);
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

/*
 * 发送消息，从unsend_vector中读数据
 */
void Connection::send_msg()
{

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
    unsend_msg_vec.push_back(msg);
}

JSPP Connection::popMsg()
{
    JSPP msg;
    if(unread_msg_vec.size() > 0) {
        msg = unread_msg_vec.back();
        unread_msg_vec.pop_back();
    }
    return msg;
}

string Connection::getPeerid()
{
    return peer_id;
}

size_t Connection::getUnsendCount()
{
    return unsend_msg_vec.size();
}

size_t Connection::getUnreadCount()
{
    return unread_msg_vec.size();
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
