#ifndef IMCLIENT_H
#define IMCLIENT_H

#include <QObject>
#include <string>
#include <QTimer>
#include <QMutex>
#include <QDebug>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "protocol/jspp.h"
#include <list>
#include <map>
#include <QTcpSocket>
#include "user.h"
#define LINE_BUF 2048
using std::string;
using std::vector;
using std::list;
using std::map;

class User;
//class Conversation;
class Connection;
class JSPP;

class IMClient : public QObject
{
    Q_OBJECT
    /*
    list<Connection *> conn_list;
    vector<Conversation *> conv_vec;
    QMutex conn_list_mutex;

*/  QMutex msg_mutex;
    list<JSPP> unread_msg_list;
    vector<JSPP> unsend_msg_vec;

public:
    User *self;
    int sendMsg(string peer_id, string msg);
    int sendMsg(JSPP msg);
    static IMClient& Instance(User *user = 0) {
            static IMClient client(user);
            return client;
    }
    string getCurrID();
    void pushMsg(JSPP msg);
    map<string, IP_PORT> frient_ip_map;
    vector<JSPP> getUnreadMsg(string peer_id);
    int getIP_Port(string peer_id, IP_PORT &res);
private:
    IMClient() = delete;
    IMClient(User *user);                            // ctor hidden
    IMClient(IMClient const&) = delete;           // copy ctor hidden
    IMClient& operator=(IMClient const&) = delete; // assign op. hidden
    ~IMClient();// dtor hidden
    SOCKET connectPeer(string peer_id);
};

#endif // IMCLIENT_H
