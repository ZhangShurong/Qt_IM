#ifndef SERVERDB_H
#define SERVERDB_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QObject>
#include "user.h"
#include "jspp.h"

class ServerDB : public QObject
{
    Q_OBJECT

    QSqlDatabase db;
public:
    ServerDB();

    int insertUser(QString id, QString pwd, QString qus, QString ans);
    int authUser(QString id, QString pwd);
    string getPwd(string id, string qus, string ans);
    int deleteUser(string id);

    int intsertMap(IP_PORT ip_port, string id);
    IP_PORT getMap(string id);

    int insertMsg(JSPP msg);
    int deleteMsg(string from, string to);
    JSPP getMsg(string to);

    ~ServerDB();
};

#endif // SERVERDB_H
