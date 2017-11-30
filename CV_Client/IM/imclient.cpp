#include "imclient.h"
#include "user.h"
#include <iostream>
#include <QMessageBox>
#include "connection.h"
#include "conversation.h"
#include "user.h"

SOCKET IMClient::connectPeer(std::string peer_id)
{
    if(frient_ip_map.find(peer_id) == frient_ip_map.end())
        return INVALID_SOCKET;
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    SOCKET self_sock = INVALID_SOCKET;
    int iResult;

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    string add = IP_PORT(frient_ip_map[peer_id]).address;
    string port = IP_PORT(frient_ip_map[peer_id]).port;

    iResult = getaddrinfo(add.c_str(), port.c_str(), &hints, &result);
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
        return INVALID_SOCKET;
    }
    //closesocket(self_sock);
    return self_sock;
}



void IMClient::pushMsg(JSPP msg)
{
    msg_mutex.lock();
    unread_msg_list.push_back(msg);
    msg_mutex.unlock();
}

vector<JSPP> IMClient::getUnreadMsg(std::string peer_id)
{
    msg_mutex.lock();

    vector<JSPP> result;
    for(auto iter = unread_msg_list.begin();
        iter != unread_msg_list.end();) {
        list<JSPP>::iterator iter_erase = iter ++;
        if(iter_erase->from == peer_id) {
            result.push_back(*iter_erase);
            unread_msg_list.erase(iter_erase);
        }
    }
    msg_mutex.unlock();
    return result;
}

int IMClient::getIP_Port(string peer_id, IP_PORT &res)
{

    map<string, IP_PORT>::iterator iter= frient_ip_map.find(peer_id);
    if(iter!=frient_ip_map.end()){
        res = iter->second;
        return 0;
    }
    else

        return -1;
}



int IMClient::sendMsg(string peer_id, string msg)
{

    JSPP msg_json;
    msg_json.body = msg;
    msg_json.from = IMClient::Instance().getCurrID();
    msg_json.to = peer_id;
    msg_json.type = "chat";

    int res = sendMsg(msg_json);
    return res;
}

int IMClient::sendMsg(JSPP msg)
{
    SOCKET self_sock = connectPeer(msg.to);
    if(self_sock == INVALID_SOCKET) {
        //链接失败
        return -1;
    }
    JSPP msg_json = msg;

    string send_str = jspp_to_str(msg_json);

    const char *sendbuf = send_str.c_str();
    // Send an initial buffer
#ifdef ENCRYPT

    QString encRes = clsEncrypt::encrypt(QString::fromStdString(send_str));
    encRes.append("\n\n");
    QByteArray sndArray(encRes.toLatin1());
    printf("send is %x", sndArray.data());
    QByteArray newArray = sndArray;
    const char* testSnd = sndArray.data();
    int iResult = send( self_sock, sndArray.data(), sndArray.size(), 0 );
    qDebug() << "encRes is" <<testSnd
             << "sndArray.size() is" << sndArray.size()
             << "and dncRes is " << clsEncrypt::deEncrypt(QString(newArray));
#else
    int iResult = send( self_sock, sendbuf, (int)strlen(sendbuf), 0 );
#endif
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(self_sock);
        self_sock = INVALID_SOCKET;
        return -1;
    }
    printf("Bytes Sent: %ld\n", iResult);
    closesocket(self_sock);
#ifdef ENCRYPT
    //delete []send_tmp;
    //delete []tmp;
#endif
    return iResult;
}

string IMClient::getCurrID()
{
    return self->getID();
}

IMClient::IMClient(User *user)
{
    self = user;
}

IMClient::~IMClient()
{
    //delete timer;
}
