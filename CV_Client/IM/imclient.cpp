#include "imclient.h"
#include "user.h"
#include <iostream>
#include "connection.h"
#include "conversation.h"
#include "user.h"
Conversation *IMClient::createConversation(User *friend_user)
{
    unkown_conn_mutex.lock();
    Conversation *t = new Conversation(friend_user);
    conv_vec.push_back(t);
    unkown_conn_mutex.unlock();
}

Conversation *IMClient::getConversation(std::string peer_id)
{
    unkown_conn_mutex.lock();
    std::map<string, size_t> index;
    for(size_t i = 0;i < conv_vec.size();++i) {
        index[conv_vec.at(i)->getPeerID()] = i;
    }
    std::map<string,size_t>::iterator it = index.find(peer_id);
    if (it != index.end()){
        unkown_conn_mutex.unlock();
        return conv_vec.at(index[peer_id]);
    }
    else{
        unkown_conn_mutex.unlock();
        createConversation(new User(peer_id));
        return conv_vec.back();
    }
    unkown_conn_mutex.unlock();
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


void IMClient::mergeConn()
{
    unkown_conn_mutex.lock();
    qDebug() << "unkown_conn size is" << unkown_conn.size();
    qDebug() << "conve size is" << conv_vec.size();
    if(unkown_conn.size() == 0){
        unkown_conn_mutex.unlock();
        return;
    }

    std::map<string, size_t> index;
    for(size_t i = 0;i < conv_vec.size();++i) {
        index[conv_vec.at(i)->getPeerID()] = i;
    }
    for(list<Connection *>::iterator iter = unkown_conn.begin();
        iter != unkown_conn.end();)
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
        unkown_conn.erase(iter_erase);
    }

    qDebug() << " Now we have " <<conv_vec.size() << " Conversation";
    unkown_conn_mutex.unlock();
}

int IMClient::sendMsg(string peer_id, string msg)
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
    qDebug() << "unkown_conn size is" << unkown_conn.size();
    qDebug() << "conve size is" << conv_vec.size();
    unkown_conn_mutex.unlock();
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
    //delete timer;
}
