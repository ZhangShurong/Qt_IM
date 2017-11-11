#ifndef IMSERVERLOCAL_H
#define IMSERVERLOCAL_H

#include <QObject>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
using std::string;
class IMClient;
class IMServerLocal:public QObject
{
    Q_OBJECT
private:
    IMClient *im;
    string port;
    SOCKET ListenSocket;
    bool initSock();
    void linkSignalWithSlot();
    bool winsockStarted;
    void stop();
public:
    IMServerLocal(string port);
    int start();
    ~IMServerLocal();
signals:
    void friendConnected(SOCKET clientSock);
    void parseOver(string msg);
private slots:
    void connectFromFriend(SOCKET clientSock);
};

#endif // IMSERVERLOCAL_H
