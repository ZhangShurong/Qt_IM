#include "loginform.h"
#include "ui_loginform.h"
#include "mainform.h"
#include <QNetworkInterface>
#include <iostream>
#include <QGridLayout>
#include <QMessageBox>
#include "regisdialog.h"
#include "forgotdia.h"
#include "config.h"
LoginForm::LoginForm(QWidget *parent) :
    MoveableFramelessWindow(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    logedin = false;
    //关联登录按钮点击
    connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(doLoginButClick()));
    connect(ui->reglabel, SIGNAL(clicked()), this, SLOT(doRegisClick()));
    connect(ui->forgetLabel, SIGNAL(clicked()), this, SLOT(doForgetPwd()));

    worker = new ServerWorker;
    serverThread = new QThread();
    worker->moveToThread(serverThread);
    connect(serverThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &LoginForm::readyForServer, worker, &ServerWorker::runServer);
    serverThread->start();

    tcpsocket = new QTcpSocket();
    connected = false;
    connect(tcpsocket, SIGNAL(connected()), this, SLOT(connectedSlot()));
    connect(tcpsocket,SIGNAL(readyRead()),this, SLOT(readMessage()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(errorSlot()));
}

LoginForm::~LoginForm()
{
    delete ui;
}

QWidget *LoginForm::getDragnWidget()
{
    return ui->login_top_widget;
}



void LoginForm::doLoginButClick()
{
    SERVER_HOST = ui->serverEdit->text().toStdString();
    SERVER_PORT = ui->portEdit->text().toStdString();
    SERVER_PORT_NUM = ui->portEdit->text().toInt();
    if(connected)
        connectedSlot();
    tcpsocket->connectToHost(QString::fromStdString(SERVER_HOST),SERVER_PORT_NUM);
    qDebug() << "Server address is " << QString::fromStdString(SERVER_HOST)
             << "Server Port is " << SERVER_PORT_NUM;
}

void LoginForm::connManage()
{
    //IMClient::Instance().mergeConn();
}

void LoginForm::errorSlot()
{
    if(!connected)
        QMessageBox::warning(this,tr("通知"),tr("连接服务器失败"),QMessageBox::Yes);
}

void LoginForm::connectedSlot()
{
    qDebug()<<"connect OK!";
    connected = true;
    QString un = ui->lineEdit_un->text();
    QString pwd = ui->lineEdit_pass->text();
    login(un + QString("*#*") + pwd);
}

void LoginForm::readMessage()
{
    QString tmp;
    tmp = tcpsocket->readAll();
    JSPP res = parse(tmp.toStdString());
    if(res.type == "login") {
        if(res.code == "1") {
            QMessageBox::warning(this,tr("通知"),tr("登陆失败"),QMessageBox::Yes);
        }
        else if(res.code == "0") {
            User *self = new User(ui->lineEdit_un->text().toStdString());
            QString body = QString::fromStdString(res.body);
            QStringList bodyList = body.split("*#*");

            //生成朋友
            for(QString f : bodyList[0].split("&")) {
                User *tmp = new User(f.toStdString());
                self->addFriend(tmp);
            }

            //生成ip对应关系
            map<string, IP_PORT> ip_map;
            if(bodyList.size() == 2) {
                if(bodyList[1].size() != 0){
                    for(QString m : bodyList[1].split("&")) {
                        IP_PORT fIP;
                        fIP.address = m.split("=")[1].split(":")[0].toStdString();
                        fIP.port = m.split("=")[1].split(":")[1].toStdString();
                        ip_map[m.split("=")[0].toStdString()] = fIP;
                    }
                }
            }
            loginOK(self, ip_map);
            /*
             * {"body":"vergilzhang*#*CV=127.0.0.1:1024&vergilzhang=127.0.0.1:1314","code":"0","from":"","to":"","type":"login"}"
             */
        }
    }
    else if(res.type == "port") {
        if(res.code == "1") {
            QMessageBox::warning(this,tr("通知"),tr("同步端口信息失败"),QMessageBox::Yes);
            return;
        }
        portOK();
    }
    else if (res.type == "refresh") {
        if(!logedin)
            return;
        QString body = QString::fromStdString(res.body);
        QStringList bodyList = body.split("*#*");
        //生成朋友
        for(QString f : bodyList[0].split("&")) {
            User *tmp = new User(f.toStdString());
            IMClient::Instance().self->addFriend(tmp);
        }

        //生成ip对应关系
        map<string, IP_PORT> ip_map;
        if(bodyList.size() == 2) {
            if(bodyList[1].size() != 0){
                for(QString m : bodyList[1].split("&")) {
                    IP_PORT fIP;
                    fIP.address = m.split("=")[1].split(":")[0].toStdString();
                    fIP.port = m.split("=")[1].split(":")[1].toStdString();
                    ip_map[m.split("=")[0].toStdString()] = fIP;
                }
            }
        }
        IMClient::Instance().frient_ip_map = ip_map;
    }
}

void LoginForm::doRegisClick()
{
    RegisDialog *r_dia = new RegisDialog();
    r_dia->show();
}

void LoginForm::doForgetPwd()
{
    ForgotDia *dia = new ForgotDia();
    dia->show();
}

void LoginForm::login(QString userInfo)
{
    JSPP login;
    login.type = "login";
    login.body = userInfo.toStdString();
    QByteArray loginJson;
    loginJson.append(QString::fromStdString(jspp_to_str(login)));
    tcpsocket->write(loginJson);
    tcpsocket->flush();
}

void LoginForm::loginOK(User *self, map<string, IP_PORT> user_ip)
{

    emit readyForServer(self);

    //todo 此处设计有误，如有时间，应修改。
    Sleep(1000);

    IMClient::Instance().frient_ip_map = user_ip;

    timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(connManage()));
    timer->start(1000);

    //显示主界面前，将本地端口通知服务器

    string port_str = "";
    int retry = 0;
    while((port_str == "" || port_str == "0") && retry < 10 ) {
        port_str = worker->getLocalPort();
        Sleep(1000);
        retry ++;
    }
    if(retry == 10 && port_str == "") {
        QMessageBox::warning(this,tr("通知"),tr("获取本地端口失败"),QMessageBox::Yes);
        tcpsocket->close();;
        exit(1);
    }

    JSPP port_jspp;
    port_jspp.type = "port";
    port_jspp.body = (QString::fromStdString(self->getID())
                      +"&" + getIP() + ":" + QString::fromStdString(port_str)).toStdString();

    QByteArray tmp;
    tmp.append(QString::fromStdString(jspp_to_str(port_jspp)));
    tcpsocket->write(tmp);
    tcpsocket->flush();
}

void LoginForm::portOK()
{
    logedin = true;
    MainForm*m=new MainForm();
    m->setNick(QString::fromStdString(IMClient::Instance().getCurrID()));
    m->show();
    this->hide();
}
QString LoginForm::getIP()  //获取ip地址
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list){
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            if (address.toString().contains("127.0.")){
                continue;
            }
            if(address.toString().contains("192.168."))
                return address.toString();
        }
    }
    return "127.0.0.1";
}
