#include "chat.h"
#include "ui_chat.h"
#include <QDesktopWidget>
#include "IM/imclient.h"
#include "IM/connection.h"
#include <QUdpSocket>
#include "dialogrec.h"
#include <strstream>
using std::strstream;

Chat::Chat(QWidget *parent, User *peer_user) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->label_4->installEventFilter(this);
    ui->label_5->installEventFilter(this);
    ui->nameLabel->setText(QString::fromStdString(peer_user->getID()));
    QFile file(":/myqss/scrollbar.qss");
    file.open(QFile::ReadOnly);
    ui->textBrowser->verticalScrollBar()->setStyleSheet(file.readAll());
    ui->label_8->installEventFilter(this);
    move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height()-20)/2);

    this->peer_user = peer_user;
    timer  = new QTimer();

    dia = new DialogRec();
    setIP_port();

    initMsgSocket();
}

Chat::~Chat()
{
    delete ui;
}
void Chat::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->setMouseTracking(true);
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = event->pos();
        qDebug()<<"移动1";
        qDebug()<<event->source();
    }

}

void Chat::mouseReleaseEvent(QMouseEvent *event)
{
    //设置鼠标为未被按下
    mouse_press = false;
}

void Chat::mouseMoveEvent(QMouseEvent *event)
{
    //若鼠标左键被按下
    //  qDebug()<<"mouse_press="<<event->globalPos();
    if(mouse_press)
    {

        //鼠标相对于屏幕的位置
        QPoint move_pos = event->globalPos();
        //移动主窗体位置
        this->move(move_pos - move_point);
    }
}

bool Chat::eventFilter(QObject *object, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress&&object==ui->label_5)
    {
        close();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_4){
        showMinimized();
        qDebug()<<"最小化";
        qDebug()<<mouse_press;

    }
    //表情发送
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_8){

        qDebug()<<ui->label_8->x();
        qDebug()<<x();

    }
    return false;
}
/*
void Chat::setConv(Conversation *conv_new)
{
    if(conv_new)
        conv = conv_new;
    return;
}
*/
void Chat::recvMsg(QString msg)
{
    qDebug()<<msg.length();
    if(msg.length()>=23){
        QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:left;margin-right:15px;margin-left:15px;'><div style='background:rgb(205,215,226);padding-top:8px;float:left;margin-top:10px;margin-left:45px;padding:10px;margin-right:60px;'><span style='font-size:15px;font-family:'Microsoft YaHei';margin-top:2px;'>"+msg+"</span></div><br><br><br>";
        ui->textBrowser->insertHtml(s1);
    }
    else{
        int flag=60+(24-msg.length())*15;
        QString myflag=QString::number(flag, 10);;
        qDebug()<<myflag;
        QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:left;margin-right:15px;margin-left:15px;'><div style='background:rgb(205,215,226);padding-top:8px;float:left;margin-top:10px;margin-left:45px;padding:10px;margin-right:"+myflag+"px;'>";
        s1+="<span style='font-size:15px;font-family:'Microsoft YaHei';margin-top:2px;'>"+msg+"</span></div><br><br><br>";
        qDebug()<<s1;
        ui->textBrowser->insertHtml(s1);
    }
}

void Chat::recvMsg(vector<JSPP> msg_vec)
{
    for(JSPP item:msg_vec) {
        if(item.type == "chat")
            recvMsg(QString::fromStdString(item.body));
        else if(item.type == "file")
        {
            if(item.code == "0") {//对方请求发送文件
                dia->setFileReq(item);
                dia->show();
            }
            else if(item.code == "1") //对方接收文件
            {
                QString fileMsg = QString::fromStdString(item.body);
                QStringList infoList= fileMsg.split("*#*");
                sendFile(infoList[0], infoList[1]);
            }
            else //对方拒绝接收文件
            {
                QMessageBox::warning(this,tr("通知"),tr("对方拒绝接收文件！"),QMessageBox::Yes);
            }
        }
    }
}
void Chat::showEvent(QShowEvent *event)
{
    mouse_press=false;
    // if(!conv)
    //   conv = IMClient::Instance().getConversation(peer_user->getID());
    connect(timer, SIGNAL(timeout()), this, SLOT(checkMsg()));
    timer->start(100);
}

void Chat::checkMsg()
{
    recvMsg(IMClient::Instance().getUnreadMsg(peer_user->getID()));
}


void Chat::on_sndBtn_clicked()
{
    QString content=ui->textEdit->toPlainText();
    qDebug()<<content.length();
    if(content.length()>=23){
        qDebug()<<"进入了这个";
        QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:right;margin-left:15px;margin-right:15px;'><div style='background:rgb(205,215,226);padding-top:8px;float:right;margin-top:10px;margin-right:45px;padding:10px;margin-left:60px;'><span style='font-size:15px;font-family:'Microsoft YaHei';margin-top:2px;'>"+content+"</span></div><br><br><br>";
        ui->textBrowser->insertHtml(s1);
    }
    else{
        int flag=60+(24-content.length())*15;
        QString myflag=QString::number(flag, 10);;
        qDebug()<<myflag;
        QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:right;margin-left:15px;margin-right:15px;'><div style='background:rgb(205,215,226);padding-top:8px;float:right;margin-top:10px;margin-right:45px;padding:10px;margin-left:"+myflag+"px;'>";
        s1+="<span style='font-size:15px;font-family:'Microsoft YaHei';margin-top:2px;'>"+content+"</span></div><br><br><br>";
        qDebug()<<s1;
        ui->textBrowser->insertHtml(s1);
    }
    if(IMClient::Instance().sendMsg(peer_user->getID(), content.toStdString()) <= 0)
    {
        //发送失败
        qDebug() << "消息发送失败！开始发送消息至服务器";
        sendOffMsg(peer_user->getID(), content.toStdString());
    }
    ui->textEdit->clear();
}

void Chat::on_sndFileBtn_clicked()
{
    QString fileName=QFileDialog::getOpenFileName(this);
    qDebug()<<fileName<<endl;
    if(fileName.isEmpty()){
        return;
    }

    JSPP msg;
    msg.type = "file";
    msg.body = fileName.toStdString();
    msg.from = IMClient::Instance().getCurrID();
    msg.to = peer_user->getID();
    IMClient::Instance().sendMsg(msg);
}

void Chat::sendFile(QString fileName, QString fileport)
{
    QUdpSocket PicSocket;
    QHostAddress localaddr1;
    ssize_t i = 0;
    if(peer_ip == "")
            return;
    localaddr1.setAddress(QString::fromStdString(peer_ip));

    int port;
    strstream ss;
    ss << fileport.toStdString();
    ss >> port;

    QFile file;
    file.setFileName(fileName);
    if(!file.open(QIODevice::ReadOnly)) return;
    while(!file.atEnd()){
        QByteArray line=file.read(8000);
        PicSocket.writeDatagram(line,localaddr1,port);
        i+=line.size();
    }
    QByteArray str = "End!";
    PicSocket.writeDatagram(str.data(),str.size(),localaddr1,port);
    qDebug() << "size is" << i << "-----";
    QMessageBox::warning(this,tr("通知"),tr("发送完成"),QMessageBox::Yes);
}

void Chat::setIP_port()
{
    IP_PORT res;
    if(!IMClient::Instance().getIP_Port(peer_user->getID(), res))
    {
        peer_ip = res.address;
        peer_port = res.port;
    }
    else {

        peer_ip = "";
        peer_port = "";
    }
}

void Chat::initMsgSocket()
{
    tcpsocket = new QTcpSocket();
    connected = false;
    connect(tcpsocket, SIGNAL(connected()), this, SLOT(connectedSlot()));
    connect(tcpsocket,SIGNAL(readyRead()),this, SLOT(readMessage()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(errorSlot()));
    connect(tcpsocket,SIGNAL(disconnected()),this, SLOT(disConnected()));
    tcpsocket->connectToHost(QString::fromStdString(SERVER_HOST),SERVER_PORT_NUM);
}

void Chat::connectedSlot()
{
    connected = true;
    reqMsg();
}
int Chat::sendOffMsg(string peer_id, string msg)
{
    JSPP msg_json;
    msg_json.body = msg;
    msg_json.from = IMClient::Instance().getCurrID();
    msg_json.to = peer_id;
    msg_json.type = "chat";

    tcpsocket->connectToHost(QString::fromStdString(SERVER_HOST),SERVER_PORT_NUM);
    if(!connected) {
        qDebug() << "未连接离线消息服务器";
        return -1;
    }
    QByteArray tmp;
    tmp.append(jspp_to_str(msg_json).c_str());
    int res = tcpsocket->write(tmp);
    tcpsocket->flush();
    return res;
}
void Chat::reqMsg()
{
    qDebug() << "开始请求离线消息";
    if(!connected) {
        qDebug() << "未连接离线消息服务器";
        return;
    }
    JSPP msg;
    msg.type = "reqmsg";
    msg.to = IMClient::Instance().getCurrID();
    QByteArray tmp;
    tmp.append(jspp_to_str(msg).c_str());
    tcpsocket->write(tmp);
    tcpsocket->flush();
}

void Chat::readMessage()
{
    QString tmp;
    tmp = tcpsocket->readAll();
    JSPP res = parse(tmp.toStdString());
    if(res.type == "chat") {
        qDebug() << "收到离线消息";
        recvMsg(QString::fromStdString(res.body));
        //pushMsg(res);
    }
}
void Chat::errorSlot()
{
    if(!connected) {
        qDebug() << "连接离线消息服务器失败";
    }
}

void Chat::disConnected()
{
    qDebug() << "与离线消息服务器断开了连接";
}
