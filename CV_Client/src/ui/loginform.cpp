#include "loginform.h"
#include "ui_loginform.h"
#include "mainform.h"

#include <iostream>
#include <QGridLayout>
#include <QMessageBox>
#include "regisdialog.h"

LoginForm::LoginForm(QWidget *parent) :
    MoveableFramelessWindow(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    //关联登录按钮点击
    connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(doLoginButClick()));
    connect(ui->reglabel, SIGNAL(clicked()), this, SLOT(doRegisClick()));

    ServerWorker *worker = new ServerWorker;
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
    if(connected)
        connectedSlot();
    tcpsocket->connectToHost(QString::fromStdString(SERVER_HOST),SERVER_PORT_NUM);
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
                for(QString m : bodyList[1].split("&")) {
                    IP_PORT fIP;
                    fIP.address = m.split("=")[1].split(":")[0].toStdString();
                    fIP.port = m.split("=")[1].split(":")[1].toStdString();
                    ip_map[m.split("=")[0].toStdString()] = fIP;
                }
            }
            loginOK(self, ip_map);
            /*
             * {"body":"vergilzhang*#*CV=127.0.0.1:1024&vergilzhang=127.0.0.1:1314","code":"0","from":"","to":"","type":"login"}"
             */
        }
    }
}

void LoginForm::doRegisClick()
{
    RegisDialog *r_dia = new RegisDialog();
    r_dia->show();
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
    //得到user和朋友状态
//    User *self = new User("CV");
//    User *vergil = new User("vergilzhang");

//    self->addFriend(vergil);

//    IP_PORT vergilIP;
//    vergilIP.address = "127.0.0.1";
//    vergilIP.port = "1314";

    emit readyForServer(self);

    //todo 此处设计有误，如有时间，应修改。
    Sleep(1000);

    IMClient::Instance().frient_ip_map = user_ip;

    timer = new QTimer();
    //connect(timer, SIGNAL(timeout()), this, SLOT(connManage()));
    timer->start(1000);

    MainForm*m=new MainForm();
    m->setNick(QString::fromStdString(self->getID()));
    m->show();
    this->hide();
}
