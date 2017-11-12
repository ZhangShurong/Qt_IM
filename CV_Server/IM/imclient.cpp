#include "imclient.h"
#include "user.h"
#include <iostream>
#include "connection.h"
#include "conversation.h"
#include "user.h"
Conversation *IMClient::createConversation(User *friend_user)
{
    Conversation *t = new Conversation(friend_user);
    conv_vec.push_back(t);
}

void IMClient::mergeConn()
{
    unkown_conn_mutex.lock();
    if(unkown_conn.size() == 0)
        return;
    std::map<string, size_t> index;
    for(size_t i = 0;i < conv_vec.size();++i) {
        index[conv_vec.at(i)->getPeerID()] = i;
    }
    for(Connection *connitem : unkown_conn) {
        string p_id = connitem->getPeerid();
        if(p_id == "")
            continue;
        std::map<string,size_t>::iterator it = index.find(p_id);
        if (it != index.end())
            conv_vec.at(index[p_id])->setConn(connitem);
        else{
            createConversation(new User(p_id));
            conv_vec.back()->setConn(connitem);
        }
    }
    qDebug() << " Now we have " <<conv_vec.size() << " Conversation";
    unkown_conn_mutex.unlock();
}

int IMClient::sendMsg(string peer_id, std::__cxx11::string msg)
{
    std::cout  << peer_id << std::endl;
    for(size_t i = 0; i < conv_vec.size(); ++i) {
        if(conv_vec.at(i)->getPeerID() == peer_id){
            conv_vec.at(i)->sendMsg(msg);
            return 0;
        }
    }
    createConversation(new User(peer_id));
    conv_vec.back()->sendMsg(msg);
    return 0;
}

string IMClient::getCurrID()
{
    return self->getID();
}

void IMClient::newConnection(Connection *newConn)
{
    unkown_conn_mutex.lock();
    unkown_conn.push_back(newConn);
    unkown_conn_mutex.unlock();
}

void IMClient::recvMsg(JSPP msg)
{
    std::cout << msg.body << std::endl;
}

IMClient::IMClient(User *user)
{
    self = user;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(mergeConn()));
    timer->start(100);
}

IMClient::~IMClient()
{
    delete timer;
}
