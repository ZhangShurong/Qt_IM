#include "connection.h"
#include <iostream>

Connection::Connection()
    :connected(false),
      peer_sock(nullptr),
      self_sock(nullptr)
{
    timer = new QTimer(this);
    peer_id = "";
    self_id = "";
    im = &IMClient::Instance();
    linksigSlot();

}

Connection::~Connection()
{
    if(timer)
        delete timer;
    closeSock();
}

Connection::Connection(QTcpSocket *sock)
    :connected(false),
      peer_sock(sock),
      self_sock(nullptr)
{
    timer = new QTimer(this);
    peer_id = "";
    self_id = "";
    im = &IMClient::Instance();
    linksigSlot();
}

void Connection::setPeerSock(QTcpSocket *peer_sock)
{
    if(!peer_sock)
        return;
    if(this->peer_sock){
        this->peer_sock->close();
        delete this->peer_sock;
        connected = false;
    }
    this->peer_sock = peer_sock;
}

/*
 * 接收消息，向vector中写数据
 */
void Connection::recv_msg()
{
    if(!peer_sock)
        return;

    connected = true;
    while (peer_sock->bytesAvailable() > 0 || peer_sock->waitForReadyRead(1000)) {
        QByteArray recvData = peer_sock->readAll();
        if (recvData.isEmpty()) {
            qDebug() << "Connection closing...";
            break;
        }

        JSPP msg = parse(recvData.toStdString());
        if(peer_id.empty()) {
            peer_id = msg.from;
        }
        qDebug() << "Bytes received:" << recvData.size() << "body:" << QString::fromStdString(msg.body);
        im->pushMsg(msg);
    }

    // cleanup
    if(peer_sock) {
        peer_sock->close();
    }
    connected = false;
    delete peer_sock;
    peer_sock = nullptr;
}

int Connection::connectToPeer()
{
    string add = IP_PORT(im->frient_ip_map[peer_id]).address;
    string port = IP_PORT(im->frient_ip_map[peer_id]).port;

    self_sock = new QTcpSocket(this);
    self_sock->connectToHost(QString::fromStdString(add),
                            QString::fromStdString(port).toInt());

    if (!self_sock->waitForConnected(5000)) {
        qDebug() << "connectToPeer failed:" << self_sock->errorString();
        delete self_sock;
        self_sock = nullptr;
        return 1;
    }
    return 0;
}

/*
 * 发送消息，从unsend_vector中读数据
 */
void Connection::send_msg(string msg)
{
    if(!self_sock){
        connectToPeer();
    }
    string send_str = msg;

    QByteArray sendData(send_str.c_str());
    qint64 iResult = self_sock->write(sendData);
    if (iResult < 0) {
        qDebug() << "send failed with error:" << self_sock->errorString();
        self_sock->close();
        delete self_sock;
        self_sock = nullptr;
        return ;
    }

    qDebug() << "Bytes Sent:" << iResult;
}

void Connection::linksigSlot()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(checkQueue()));
    timer->start(100);
}

void Connection::checkQueue()
{

    if(!connected)
        recv_msg();
}

int Connection::pushMsg(JSPP msg)
{
    im->pushMsg(msg);
    return 0;
}

JSPP Connection::popMsg()
{
    JSPP msg;
    return msg;
}

string Connection::getPeerid()
{
    return peer_id;
}

void Connection::setPeer_id(string id)
{
    peer_id = id;
}

 size_t Connection::getUnsendCount()
{
     //todo 此接口的作用
     return 0;
}

  size_t Connection::getUnreadCount()
{
    return im->getUnreadMsg(peer_id).size();
}

void Connection::closeSock()
{
    if(peer_sock){
        peer_sock->close();
        delete peer_sock;
        connected = false;
        peer_sock = nullptr;
    }
}

QTcpSocket* Connection::getpeer_sock()
{
    return peer_sock;
}
