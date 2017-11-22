#include "serverdb.h"
#include <QDebug>

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
    sql_query.addBindValue(id);
    sql_query.addBindValue(pwd);
    sql_query.addBindValue("");//留空
    sql_query.addBindValue(qus);
    sql_query.addBindValue(ans);
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
    sql_query.bindValue(":id",id);
    sql_query.bindValue(":pwd",pwd);
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
    QString select_sql = QString("select * from user where name = ")
            + "\'"
            + QString::fromStdString(id)
            + "\'"
            + " and qus = \'"
            + QString::fromStdString(qus)
            + "\' and ans = \'"
            + QString::fromStdString(ans)
            + "\'";
    if(!sql_query.exec(select_sql))
    {
        qDebug()<<sql_query.lastError();
        return "";
    }
    else
    {
        if(sql_query.next())
        {
            QString PWD = sql_query.value(1).toString();
            return PWD.toStdString();
        }

    }
    return "";
}

int ServerDB::deleteUser(std::string id)
{
    QSqlQuery sql_query(db);
    QString delete_sql = "delete from user where id = ?";
    sql_query.prepare(delete_sql);
    sql_query.addBindValue(QString::fromStdString(id));
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
        return -1;
    }
    return 0;

}

int ServerDB::intsertMap(IP_PORT ip_port, std::string id)
{
    QSqlQuery sql_query(db);
    QString insert_sql = "insert into ip_map values (?, ?, ?)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(QString::fromStdString(id));
    sql_query.addBindValue(QString::fromStdString(ip_port.address));
    sql_query.addBindValue(QString::fromStdString(ip_port.port));
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError();
        return -1;
    }

    return 0;
}



ServerDB::~ServerDB()
{

}
