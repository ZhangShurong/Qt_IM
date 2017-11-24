#include "dialogrec.h"
#include "ui_dialogrec.h"


void DialogRec::sendHeader()
{
    //先发送文件头信息:文件名##大小
    //构造头部信息
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    //发送头部信息
    qint64 len = tcpSocket->write(head.toUtf8());

    if(len < 0){
        //关闭文件
        file.close();
    }
}

void DialogRec::sendData()
{
    qint64 len = 0;
        do{
            //一次发送的大小
            char buf[BUF_SIZE] = {0};
            len = 0;
            len = file.read(buf,BUF_SIZE);  //len为读取的字节数
            len = tcpSocket->write(buf,len);    //len为发送的字节数
            //已发数据累加
            sendSize += len;
        }while(len > 0);
}

DialogRec::DialogRec(QWidget *parent):
    QDialog(parent),
    ui(new Ui::DialogRec)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("文件传输！"));
    port = "0";
#ifndef TCP
    udpsocket = new QUdpSocket();
#else
    tcpSocket = new QTcpSocket();
#endif
}

void DialogRec::setFileReq(JSPP fileReq)
{
    fileName = QString::fromStdString(fileReq.body);
    peer_user = fileReq.from;
    ui->label->setText(QString::fromStdString(peer_user) +
                       "向您发送文件" +
                       fileName.split("/").back() +
                       "是否接收？");
}


DialogRec::~DialogRec()
{
    delete ui;
}



void DialogRec::on_okBtn_clicked()
{
#ifndef TCP
    udpsocket->bind(QHostAddress::Any, 10086);
    connect(udpsocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
    port = "10086";
#else
    //创建对象指定父对象
    tcpServer = new QTcpServer(this);
    //绑定监听
    tcpServer->listen(QHostAddress::Any);
    port = QString::number(tcpServer->serverPort()).toStdString();
    //如果客户端和服务器成功连接
    //tcpServer会自动触发newConnection()信号
    connect(tcpServer,&QTcpServer::newConnection,
            [=](){
        //获取通信套接字
        tcpSocket = tcpServer->nextPendingConnection();
        connect(tcpSocket, QTcpSocket::readyRead,
                [=](){
            QByteArray buf = tcpSocket->readAll();
            //采用回射信息进行粘包处理
            if("FileHead recv" == QString(buf)){
                //ui->textEdit->append("文件头部接收成功，开始发送文件...");
                sendData();
            }
            else if("file write done" == QString(buf)){
                //服务器发送的快，而客户端接收的慢，所以要等客户端接收完毕后才能断开连接，以免丢包
                QMessageBox::information(this,"完成","对端接收完成");
                //ui->textEdit->append("文件发送且接收完成");
                file.close();
                tcpSocket->disconnectFromHost();
                tcpSocket->close();
            }
        }
        );
    }
    );
#endif
    //todo 动态获取端口
    QString body = fileName + QString::fromStdString("*#*") + QString::fromStdString(port);
    JSPP msg;
    msg.type = "file";
    msg.body = body.toStdString();
    msg.from = IMClient::Instance().getCurrID();
    msg.to = peer_user;
    msg.code = "1";
    IMClient::Instance().sendMsg(msg);
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
    QFile file(fileName.split("/").back());
    if(!file.open(QIODevice::ReadWrite)) return;
    file.resize(0);//清空原有内容
#ifndef TCP
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
#else

#endif
    file.close();
    //udpsocket->close();
    QMessageBox::warning(this,tr("通知"),tr("接收完成"),QMessageBox::Yes);
    hide();
}
