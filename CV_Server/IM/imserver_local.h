#ifndef IMSERVERLOCAL_H
#define IMSERVERLOCAL_H

#include <QObject>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "protocol/jspp.h"
#define LINE_BUF 2048
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
    void msg_distribution(SOCKET ClientSocket);
    JSPP parse(string json_str);
public:
    IMServerLocal(string port);
    int start();
    ~IMServerLocal();
};

#endif // IMSERVERLOCAL_H
