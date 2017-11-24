#ifndef SERVERDB_H
#define SERVERDB_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QObject>
#include <vector>
#include "user.h"
#include "jspp.h"
using std::vector;
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
    std::string getAllUser();

    int intsertMap(IP_PORT ip_port, string id);
    int deleteMap(string id);
    std::string getallMap();

    int insertMsg(JSPP msg);
    int deleteMsg(string to);
    vector<JSPP> getMsg(string to);


    ~ServerDB();
};

#endif // SERVERDB_H
