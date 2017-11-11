#ifndef IMCLIENT_H
#define IMCLIENT_H

#include <QObject>
#include <string>
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
public:
    int sendMsg(unsigned int peer_id);
    static IMClient& Instance(User *user) {
            static IMClient client(user);
            return client;
    }
    int getCurrID();
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
};

#endif // IMCLIENT_H
