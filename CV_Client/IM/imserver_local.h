#ifndef IMSERVERLOCAL_H
#define IMSERVERLOCAL_H

#include <QObject>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <QThread>
#include <QDebug>
#include "protocol/jspp.h"
#include "utils/utils.h"

using std::string;
class IMClient;
class IMServerLocal:public QObject
{
    Q_OBJECT
private:
    IMClient *im;
    string port;
    SOCKET ListenSocket;
    QThread distributorThread;
    bool initSock();
    void linkSignalWithSlot();
    bool winsockStarted;
    void stop();
    string realPort;
signals:
    void newConn(SOCKET ClientSocket);
public:
    IMServerLocal(string port);
    int start();
    string getrealPort();

    ~IMServerLocal();
};

class Distributor : public QObject
{
    Q_OBJECT
public slots:
    void msg_distribution(SOCKET ClientSocket);

signals:
    void resultReady(const QString &result);
};

#endif // IMSERVERLOCAL_H
