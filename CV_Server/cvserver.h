#ifndef SERVERS_1_H
#define SERVERS_1_H

#include <QWidget>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <vector>
#include <map>
#include "user.h"
#include "utils.h"
#include "jspp.h"
#include "serverdb.h"

using std::vector;
using std::map;

namespace Ui {
    class CVServer;
}

class CV_Server : public QWidget
{
    Q_OBJECT

    //定义用户
    vector<User> users;
    map<string, IP_PORT> user_ip_map;
public:
    explicit CV_Server(QWidget *parent = 0);
    ~CV_Server();
    QTcpServer *tcpServer;
    void init();
    QString getIP();

    void uselist();

    void NAME(QTcpSocket* ,QByteArray);
    void list();

    void initUserAndmap()
    {
        User *CV = new User("CV");
        User *vergil = new User("vergilzhang");
        User *dante = new User("dante");
        CV->addFriend(vergil);
        vergil->addFriend(CV);
        users.push_back(*CV);
        users.push_back(*vergil);
        users.push_back(*dante);
        IP_PORT vergilIP;
        vergilIP.address = "127.0.0.1";
        vergilIP.port = "1314";
        IP_PORT cvIP;
        cvIP.address = "127.0.0.1";
        cvIP.port = "1024";
        user_ip_map["vergilzhang"] = vergilIP;
        user_ip_map["CV"] = cvIP;
    }


private slots:
    void on_pushButton_clicked();

    void newConnectSlot();
    void readMessage();
    void removeUserFormList();

    void on_pushButton_2_clicked();

private:
    Ui::CVServer *ui;
    ServerDB *db;
    map<QTcpSocket *,string> onlineUser;
    void doRefresh();

    QString ip;
    QList <QTcpSocket *> userList;
    QList <QByteArray> userName;
    QList <QString> userIP;
    QStringList ipname,ipss,namess;
    int ii;
    QString ips,names;
};

#endif // SERVERS_1_H
