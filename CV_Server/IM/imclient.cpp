#include "imclient.h"
#include "user.h"
#include <iostream>
Conversation *IMClient::createConversation(User *friend_user)
{

}

int IMClient::sendMsg(unsigned int peer_id)
{

}

int IMClient::getCurrID()
{
 return self->getID();
}

void IMClient::recvMsg(JSPP msg)
{
    std::cout << msg.body << std::endl;
}

IMClient::IMClient(User *user)
{
    self = user;
}

IMClient::~IMClient()
{

}
