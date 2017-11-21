#ifndef IMCLIENT_H
#define IMCLIENT_H

#include <QObject>
#include <string>
#include <QTimer>
#include <QMutex>
#include <QDebug>
#include "protocol/jspp.h"
#include <list>
using std::string;
using std::vector;
using std::list;
class User;
class Conversation;
class Connection;
class JSPP;
class IMClient : public QObject
{
    Q_OBJECT
    vector<Conversation *> conv_vec;
    list<Connection *> unkown_conn;
    User *self;
    QMutex unkown_conn_mutex;

    QMutex msg_mutex;
    list<JSPP> unread_msg_list;
    vector<JSPP> unsend_msg_vec;

public:
    int sendMsg(string peer_id, string msg);
    static IMClient& Instance(User *user = 0) {
            static IMClient client(user);
            return client;
    }
    string getCurrID();
    void newConnection(Connection *newConn);
    Conversation* getConversation(string peer_id);
    void pushMsg(JSPP msg);
    vector<JSPP> getUnreadMsg(string peer_id);
public slots:
    void recvMsg(JSPP msg);

private:
    IMClient() = delete;
    IMClient(User *user);                            // ctor hidden
    IMClient(IMClient const&) = delete;           // copy ctor hidden
    IMClient& operator=(IMClient const&) = delete; // assign op. hidden
    ~IMClient();// dtor hidden
    Conversation* createConversation(User *friend_user);

public slots:
    void mergeConn();
};

#endif // IMCLIENT_H
