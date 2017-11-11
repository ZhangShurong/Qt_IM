#ifndef CONNECTION_H
#define CONNECTION_H
#include <QObject>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
using std::string;
class Connection : public QObject
{
    Q_OBJECT
private:
    SOCKET sock;
    bool connected;
signals:
    void parseOver(string msg);

public:
    Connection();
    int senMsg(string msg);
};

#endif // CONNECTION_H
