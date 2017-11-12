#include "conversation.h"
#include "connection.h"
#include "user.h"
#include <iostream>

//void Conversation::addMsg(string msg)
//{
//    msg_vec.push_back(msg);
//    std::cout << msg << std::endl;
//    unread_count ++;
//    std::cout << "unread_count is " << unread_count << std::endl;
//}

void Conversation::sendMsg(string msg)
{
    std::cout << msg << std::endl;
    if(!conn){
        conn = new Connection();
        conn->setPeer_id(peer_user->getID());
    }

    conn->send_msg(msg);

}

string Conversation::getPeerID()
{
    return peer_user->getID();
}

Conversation::Conversation(User *peer_user)
    :conn(nullptr),initiator(false),unread_count(0)
{
    if(!peer_user)
        exit(1);
    this->peer_user = peer_user;
}

void Conversation::setConn(Connection *newconn)
{
    if(!newconn)
        return;
    if(!conn) {
        conn = newconn;
        return;
    }

    for(size_t i = 0; i < newconn->getUnreadCount(); ++i)
    {
        conn->unread_msg_vec.push_back(newconn->unread_msg_vec.at(i));
    }
    conn->closeSock();
    conn->setPeerSock(newconn->getpeer_sock());
    delete newconn;
}
