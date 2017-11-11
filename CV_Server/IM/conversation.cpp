#include "conversation.h"
#include <string>
#include <iostream>
void Conversation::addMsg(std::__cxx11::string msg)
{
    msg_vec.push_back(msg);
    std::cout << msg << std::endl;
    unread_count ++;
    std::cout << "unread_count is " << unread_count << std::endl;
}

Conversation::Conversation(User *peer_user)
    :conn(nullptr),initiator(false),unread_count(0)
{
    if(!peer_user)
        exit(1);
    this->peer_user = peer_user;
}
