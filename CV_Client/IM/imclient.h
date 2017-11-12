#ifndef IMCLIENT_H
#define IMCLIENT_H

#include <QObject>
#include <string>
#include <QTimer>
#include <QMutex>
#include <QDebug>
#include "protocol/jspp.h"
using std::string;
using std::vector;
class User;
class Conversation;
class Connection;
class JSPP;
class IMClient : public QObject
{
    Q_OBJECT
    vector<Conversation *> conv_vec;
    vector<Connection *> unkown_conn;
    User *self;
    QTimer *timer;
    QMutex unkown_conn_mutex;
public:
    int sendMsg(string peer_id, string msg);
    static IMClient& Instance(User *user) {
            static IMClient client(user);
            return client;
    }
    string getCurrID();
    void newConnection(Connection *newConn);
public slots:
    void recvMsg(JSPP msg);

private:
    IMClient() = delete;
    IMClient(User *user);                            // ctor hidden
    IMClient(IMClient const&) = delete;           // copy ctor hidden
    IMClient& operator=(IMClient const&) = delete; // assign op. hidden
    ~IMClient();// dtor hidden
    Conversation* createConversation(User *friend_user);
private slots:
    void mergeConn();
};

#endif // IMCLIENT_H
