#include "serverdb.h"
#include <QDebug>
#include "utils.h"

ServerDB::ServerDB()
{
    QString dbName = "CVDB";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if (!db.open()) {
        qDebug() << "Database Error!";
        exit(1);
    }
    QString create_user = "create table user (name varchar(30) primary key,"
                          "pwd varchar(30),"
                          "friends text,"
                          "qus text,"
                          "ans text)";

    QString create_map = "create table ip_map (name varchar(30) primary key, "
                         "ip text,"
                         "port text)";

    QString create_msg = "create table msg (from_user varchar(30), "
                         "to_user varchar(30),"
                         "jspp text,"
                         "id INTEGER PRIMARY KEY AUTOINCREMENT)";
    QSqlQuery query(db);
    query.exec(create_user);
    query.exec(create_map);
    query.exec(create_msg);
}

int ServerDB::insertUser(QString id, QString pwd, QString qus, QString ans)
{
    QSqlQuery sql_query(db);
    QString insert_sql = "insert into user values (?, ?, ?, ?, ?)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(encodeQstr(id));
    sql_query.addBindValue(encodeQstr(pwd));
    sql_query.addBindValue("");//留空
    sql_query.addBindValue(encodeQstr(qus));
    sql_query.addBindValue(encodeQstr(ans));
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return -1;
    }

    return 0;

}

int ServerDB::authUser(QString id, QString pwd)
{
    QSqlQuery sql_query(db);
    //select * from user where name = 'CV' and pwd = 'vergil';
    QString select_sql = QString("select * from user where name = :id and pwd = :pwd");

    sql_query.prepare(select_sql);
    sql_query.bindValue(":id",encodeQstr(id));
    sql_query.bindValue(":pwd",encodeQstr(pwd));
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return -1;
    }
    else
    {
        while(sql_query.next())
        {
            return 0;
        }

    }
    return -2;
}

std::string ServerDB::getPwd(std::string id, std::string qus, std::string ans)
{
    QSqlQuery sql_query(db);
    QString select_sql = QString("select pwd from user where name = ? and qus = ? and ans = ?");
    sql_query.prepare(select_sql);
    sql_query.addBindValue(encodeQstr(QString::fromStdString(id)));
    sql_query.addBindValue(encodeQstr(QString::fromStdString(qus)));
    sql_query.addBindValue(encodeQstr(QString::fromStdString(ans)));
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return "";
    }
    else
    {
        if(sql_query.next())
        {
            QString PWD = sql_query.value(0).toString();
            return decodeQstr(PWD).toStdString();
        }

    }
    return "";
}

int ServerDB::deleteUser(std::string id)
{
    QSqlQuery sql_query(db);
    QString delete_sql = "delete from user where id = ?";
    sql_query.prepare(delete_sql);
    sql_query.addBindValue(encodeQstr(QString::fromStdString(id)));
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return -1;
    }
    return 0;

}

string ServerDB::getAllUser()
{
    //qDebug() << QString::fromStdString(to) <<"请求离线消息";
    QSqlQuery sql_query(db);
    string res = "";
    QString select_sql = QString("select * from user");
    sql_query.prepare(select_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return res;
    }
    else
    {
        QString c = "";
        QString msg = "";
        while(sql_query.next())
        {
            msg += c + decodeQstr(sql_query.value(0).toString());
            c = "&";
        }
        res = msg.toStdString();
    }
    return res;
}

int ServerDB::intsertMap(IP_PORT ip_port, std::string id)
{
    QSqlQuery sql_query(db);
    QString insert_sql = "insert into ip_map values (?, ?, ?)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(encodeQstr(QString::fromStdString(id)));
    sql_query.addBindValue(encodeQstr(QString::fromStdString(ip_port.address)));
    sql_query.addBindValue(encodeQstr(QString::fromStdString(ip_port.port)));
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return -1;
    }

    return 0;
}

int ServerDB::deleteMap(std::string id)
{
    QSqlQuery sql_query(db);
    QString del_sql = "delete from ip_map where name = ?";
    sql_query.prepare(del_sql);
    sql_query.addBindValue(encodeQstr(QString::fromStdString(id)));

    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return -1;
    }

    return 0;
}

string ServerDB::getallMap()
{
    string res = "";
    qDebug() <<"请求用户端口";
    QSqlQuery sql_query(db);
    QString select_sql = QString("select * from ip_map");
    sql_query.prepare(select_sql);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return res;
    }
    else
    {
        QString c = "";
        QString msg;
        while(sql_query.next())
        {
            msg +=c + decodeQstr(sql_query.value(0).toString())
                    + "="
                    + decodeQstr(sql_query.value(1).toString())
                    + ":"
                    + decodeQstr(sql_query.value(2).toString());
            c = "&";
        }
        res = msg.toStdString();
    }
    qDebug() << "IP PORT" <<QString::fromStdString(res);
    return res;
}

int ServerDB::insertMsg(JSPP msg)
{
    QSqlQuery sql_query(db);
    QString insert_sql = "insert into msg values (?, ?, ?, null)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(encodeQstr(QString::fromStdString(msg.from)));
    sql_query.addBindValue(encodeQstr(QString::fromStdString(msg.to)));
    sql_query.addBindValue(encodeQstr(QString::fromStdString(jspp_to_str(msg))));

    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return -1;
    }

    return 0;
}

int ServerDB::deleteMsg(std::string to)
{
    qDebug() << QString::fromStdString(to) <<"删除离线消息";
    QSqlQuery sql_query(db);
    vector<JSPP> res;
    QString select_sql = QString("delete from msg where to_user = :to");
    sql_query.prepare(select_sql);
    sql_query.bindValue(":to",encodeQstr(QString::fromStdString(to)));
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return -1;
    }
    return 0;
}

vector<JSPP> ServerDB::getMsg(std::string to)
{
    qDebug() << QString::fromStdString(to) <<"请求离线消息";
    QSqlQuery sql_query(db);
    vector<JSPP> res;
    QString select_sql = QString("select jspp from msg where to_user = :to order by id");
    sql_query.prepare(select_sql);
    sql_query.bindValue(":to",encodeQstr(QString::fromStdString(to)));
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return res;
    }
    else
    {
        while(sql_query.next())
        {
            res.push_back(parse(decodeQstr(sql_query.value(0).toString()).toStdString()));
        }
        deleteMsg(to);

    }
    return res;
}



ServerDB::~ServerDB()
{

}

