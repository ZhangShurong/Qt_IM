#ifndef IMCLIENT_H
#define IMCLIENT_H

#include <QObject>
#include <string>
#include "protocol/jspp.h"
using std::string;
class User;
class Conversation;
class JSPP;
class IMClient : public QObject
{
    Q_OBJECT
    std::vector<Conversation *> conv_vec;
    User *self;
public:
    Conversation* createConversation(User *friend_user);
    int sendMsg(unsigned int peer_id);
    static IMClient& Instance(User *user) {
            static IMClient client(user);
            return client;
    }
    int getCurrID();
public slots:
    void recvMsg(JSPP msg);
private:
    IMClient() = delete;
    IMClient(User *user);                            // ctor hidden
    IMClient(IMClient const&) = delete;           // copy ctor hidden
    IMClient& operator=(IMClient const&) = delete; // assign op. hidden
    ~IMClient();// dtor hidden
};

#endif // IMCLIENT_H
