#include "regisdialog.h"
#include "ui_regisdialog.h"
#include <QMessageBox>
#include "IM/user.h"
#include "protocol/jspp.h"
#include "utils/utils.h"
#include "config.h"

RegisDialog::RegisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()), this, SLOT(doRegis()));
    tcpsocket = new QTcpSocket();
    connected = false;
    connect(tcpsocket, SIGNAL(connected()), this, SLOT(connectedSlot()));
    connect(tcpsocket,SIGNAL(readyRead()),this, SLOT(readMessage()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(errorSlot()));
}

RegisDialog::~RegisDialog()
{
    delete ui;
}

void RegisDialog::doRegis()
{
    tcpsocket->connectToHost(QString::fromStdString(SERVER_HOST),SERVER_PORT_NUM);
}

void RegisDialog::errorSlot()
{
    QMessageBox::warning(this,tr("通知"),tr("连接服务器失败"),QMessageBox::Yes);
}

void RegisDialog::connectedSlot()
{
    qDebug()<<"connect OK!";
    connected = true;
    QString name = ui->nameEdit->text();
    QString pwd = ui->pwdEdit->text();
    QString que = ui->qusEdit->text();
    QString ans = ui->ansEdit->text();
    if(!name.size() || !pwd.size() || !que.size() || !ans.size()) {
        QMessageBox::warning(this,tr("通知"),tr("请填完所有空格"),QMessageBox::Yes);
    }
    /*
     * 注册格式:
     * type = "reg"
     * body = name&pwd&que&ans
     */
    JSPP regJson;
    regJson.type = "reg";
    regJson.body = QString(name + "&" + pwd +"&" + que + "&" + ans).toStdString();
    QByteArray tmp;
    tmp.append(jspp_to_str(regJson).c_str());;
    tcpsocket->write(tmp);
    tcpsocket->flush();
}

void RegisDialog::readMessage()
{
    QString tmp;
    tmp = tcpsocket->readAll();
    JSPP res = parse(tmp.toStdString());
    if(res.code == "0") {
        QMessageBox::warning(this,tr("通知"),tr("注册成功"),QMessageBox::Yes);
    }
    else {
        QMessageBox::warning(this,tr("通知"),tr("注册失败"),QMessageBox::Yes);
    }
}
