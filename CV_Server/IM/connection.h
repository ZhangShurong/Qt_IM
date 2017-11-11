#ifndef CONNECTION_H
#define CONNECTION_H
#include <QObject>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include "utils/utils.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#define LINE_BUF 2048
using std::string;
using std::vector;
class Connection : public QObject
{
    Q_OBJECT
private:
    SOCKET sock;
    bool connected;
    string peer_id;
    string self_id;
    vector<JSPP> unread_msg_vec;
    vector<JSPP> unsend_msg_vec;
public:
    Connection();
    Connection(SOCKET sock);
    void recv_msg();
    int senMsg(JSPP msg);
};

#endif // CONNECTION_H
