#include "imclient.h"
#include "user.h"
#include <iostream>
#include "connection.h"
#include "conversation.h"
#include "user.h"
/*
Conversation *IMClient::createConversation(User *friend_user)
{
    conn_list_mutex.lock();
    Conversation *t = new Conversation(friend_user);
    conv_vec.push_back(t);
    conn_list_mutex.unlock();
}

Conversation *IMClient::getConversation(std::string peer_id)
{
    conn_list_mutex.lock();
    std::map<string, size_t> index;
    for(size_t i = 0;i < conv_vec.size();++i) {
        index[conv_vec.at(i)->getPeerID()] = i;
    }
    std::map<string,size_t>::iterator it = index.find(peer_id);
    if (it != index.end()){
        conn_list_mutex.unlock();
        return conv_vec.at(index[peer_id]);
    }
    else{
        conn_list_mutex.unlock();
        createConversation(new User(peer_id));
        return conv_vec.back();
    }
    conn_list_mutex.unlock();
}

void IMClient::mergeConn()
{
    conn_list_mutex.lock();
    qDebug() << "conn_list size is" << conn_list.size();

    if(conn_list.size() == 0){
        conn_list_mutex.unlock();
        return;
    }

    std::map<string, size_t> index;
    for(size_t i = 0;i < conv_vec.size();++i) {
        index[conv_vec.at(i)->getPeerID()] = i;
    }
    for(list<Connection *>::iterator iter = conn_list.begin();
        iter != conn_list.end();)
    {
        list<Connection *>::iterator iter_erase = iter ++;
        string p_id = (*iter_erase)->getPeerid();
        if(p_id == "")
            continue;
        std::map<string,size_t>::iterator it = index.find(p_id);
        if (it != index.end()){
            conv_vec.at(index[p_id])->setConn(*iter_erase);
        }
        else{
            createConversation(new User(p_id));
            conv_vec.back()->setConn(*iter_erase);
        }
        conn_list.erase(iter_erase);
    }

    qDebug() << " Now we have " <<conv_vec.size() << " Conversation";
    conn_list_mutex.unlock();
}

void IMClient::newConnection(Connection *newConn)
{
    conn_list_mutex.lock();
    conn_list.push_back(newConn);
    qDebug() << "conn_list size is" << conn_list.size();
    qDebug() << "conve size is" << conv_vec.size();
    conn_list_mutex.unlock();
}

void IMClient::recvMsg(JSPP msg)
{
    std::cout << msg.body << std::endl;
}

*/
SOCKET IMClient::connectPeer(std::string peer_id)
{
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

    return sendMsg(msg_json);
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
    int iResult = send( self_sock, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(self_sock);
        self_sock = INVALID_SOCKET;
        return -1;
    }
    printf("Bytes Sent: %ld\n", iResult);
    closesocket(self_sock);
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
