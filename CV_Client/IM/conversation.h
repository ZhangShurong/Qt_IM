#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QObject>
#include <string>
#include <vector>
using std::vector;
using std::string;

class Connection;
class User;
class Conversation : public QObject
{
    Q_OBJECT
    Connection *conn;
    User* peer_user;
    bool initiator; //标记是主动发起还是被动接收
    vector<string> msg_vec;//暂时这样储存消息
    unsigned int unread_count;
public:
    void sendMsg(string msg);//组装成字符串
    string getPeerID();
public:
    Conversation(User* peer_user);
    void setConn(Connection *newconn);
    Connection *getConn();
};

#endif // CONVERSATION_H
