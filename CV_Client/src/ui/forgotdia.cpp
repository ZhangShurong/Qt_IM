#include "forgotdia.h"
#include "ui_forgotdia.h"
#include <QMessageBox>
#include "utils/utils.h"
#include "protocol/jspp.h"
#include "config.h"

ForgotDia::ForgotDia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgotDia)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(acceptSlot()));

    tcpsocket = new QTcpSocket();
    connected = false;
    connect(tcpsocket, SIGNAL(connected()), this, SLOT(connectedSlot()));
    connect(tcpsocket,SIGNAL(readyRead()),this, SLOT(readMessage()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(errorSlot()));
}

ForgotDia::~ForgotDia()
{
    delete ui;
}
void ForgotDia::connectedSlot()
{
    connected = true;
    QString name = ui->nameEdit->text();
    QString qus = ui->qusEdit->text();
    QString ans = ui->ansEdit->text();
    if(name.size() && qus.size() && ans.size()) {
        /*
         * 找回密码格式:
         * type = "reg"
         * body = name&que&ans
         */
        JSPP regJson;
        regJson.type = "findpwd";
        regJson.body = QString(name +"&" + qus + "&" + ans).toStdString();
        QByteArray tmp;
        tmp.append(jspp_to_str(regJson).c_str());;
        tcpsocket->write(tmp);
        tcpsocket->flush();
    }
    else {
        QMessageBox::warning(this,tr("通知"),tr("请填完所有信息"),QMessageBox::Yes);
    }
}
void ForgotDia::readMessage()
{
    QString tmp;
    tmp = tcpsocket->readAll();
    JSPP res = parse(tmp.toStdString());
    if(res.type != "findpwd")
        return;
    if(res.code == "0") {
        QString tr_str = "您的密码是：" + QString::fromStdString(res.body);
        QMessageBox::warning(this,tr("通知"),tr(tr_str.toStdString().c_str()),QMessageBox::Yes);
    }
    else {
        QMessageBox::warning(this,tr("通知"),tr("找回失败：无此用户或者密保有误"),QMessageBox::Yes);
    }
}
void ForgotDia::errorSlot()
{

}

void ForgotDia::acceptSlot()
{
    tcpsocket->connectToHost(QString::fromStdString(SERVER_HOST),SERVER_PORT_NUM);
}
