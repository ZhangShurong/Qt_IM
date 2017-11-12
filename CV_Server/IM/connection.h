#ifndef CONNECTION_H
#define CONNECTION_H
#include <QObject>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <QTimer>
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
    SOCKET peer_sock;
    SOCKET self_sock;
    bool connected;
    string peer_id;
    string self_id;

    QTimer *timer;
    void recv_msg();
    int connectToPeer();

    void linksigSlot();
private slots:
    void checkQueue();
public:
    Connection();
    ~Connection();
    Connection(SOCKET peer_sock);
    void setPeerSock(SOCKET peer_sock);
    int pushMsg(JSPP msg);
    JSPP popMsg();
    string getPeerid();
    size_t getUnsendCount();
    size_t getUnreadCount();
    void closeSock();
    SOCKET getpeer_sock();
    vector<JSPP> unread_msg_vec;
    vector<JSPP> unsend_msg_vec;
    void send_msg();//暂时公开用作测试
};

#endif // CONNECTION_H
