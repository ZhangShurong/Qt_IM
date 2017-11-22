#include "dialogrec.h"
#include "ui_dialogrec.h"


DialogRec::DialogRec(QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogRec)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("文件传输！"));
    udpsocket = new QUdpSocket();
}

void DialogRec::setFileReq(JSPP fileReq)
{
    fileName = fileReq.body;
    peer_user = fileReq.from;
    ui->label->setText(QString::fromStdString(peer_user) +
                       "向您发送文件" +
                       QString::fromStdString(fileName).split("/").back() +
                       "是否接收？");
}


DialogRec::~DialogRec()
{
    delete ui;
}



void DialogRec::on_okBtn_clicked()
{
    //todo 动态获取端口
    QString body = QString::fromStdString(fileName) + QString::fromStdString("*#*") + QString::fromStdString("10086");
    JSPP msg;
    msg.type = "file";
    msg.body = body.toStdString();
    msg.from = IMClient::Instance().getCurrID();
    msg.to = peer_user;
    msg.code = "1";
    IMClient::Instance().sendMsg(msg);
    udpsocket->bind(QHostAddress::Any, 10086);
    connect(udpsocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void DialogRec::on_noBtn_clicked()
{
    QString body = "";
    JSPP msg;
    msg.type = "file";
    msg.body = body.toStdString();
    msg.from = IMClient::Instance().getCurrID();
    msg.code = "2";
    msg.to = peer_user;
    IMClient::Instance().sendMsg(msg);
    hide();
}
void DialogRec::readPendingDatagrams()
{
    QFile file(QString::fromStdString(fileName).split("/").back());
    if(!file.open(QIODevice::ReadWrite)) return;
    file.resize(0);//清空原有内容
    while(udpsocket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(udpsocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 Pic_port;
        udpsocket->readDatagram(datagram.data(),datagram.size(),&sender,&Pic_port);
        if(datagram!="End!") {
            file.write(datagram.data(),datagram.size());
            qDebug()<<datagram.size()<<endl;
        }else{
            break;
        }
    }
    file.close();
    //udpsocket->close();
    QMessageBox::warning(this,tr("通知"),tr("接收完成"),QMessageBox::Yes);
    hide();
}
