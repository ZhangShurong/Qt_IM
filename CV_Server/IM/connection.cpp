#include "connection.h"
#include <iostream>

Connection::Connection()
    :connected(false),sock(INVALID_SOCKET)
{
    peer_id = "";
    self_id = "";
}

Connection::Connection(SOCKET sock)
    :connected(true),sock(sock)
{
    peer_id = "";
    self_id = "";
    recv_msg();
}

void Connection::recv_msg()
{
    int iResult;
    char recvbuf[LINE_BUF];
    int recvbuflen = LINE_BUF;
    do {
        iResult = recv(sock, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            JSPP msg = parse(string(recvbuf));
            if(peer_id == "") {
                peer_id = msg.from;
            }
            std::cout << msg.body << std::endl;
            msg_vec.push_back(msg);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(sock);
            WSACleanup();
            return ;
        }
    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return ;
    }

    // cleanup
    closesocket(sock);
    connected = false;
}
