#include "imclient.h"
#include "user.h"
#include <iostream>
#include <QMessageBox>
#include <cstdlib>
#include "connection.h"
#include "conversation.h"
#include "user.h"

QTcpSocket* IMClient::connectPeer(std::string peer_id)
{
    if(frient_ip_map.find(peer_id) == frient_ip_map.end())
        return nullptr;

    // Resolve the server address and port
    string add = IP_PORT(frient_ip_map[peer_id]).address;
    string port = IP_PORT(frient_ip_map[peer_id]).port;

    qDebug() << "Connecting to:" << QString::fromStdString(add)
             << ":" << QString::fromStdString(port);

    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost(QString::fromStdString(add),
                         QString::fromStdString(port).toInt());

    if (!socket->waitForConnected(5000)) {
        qDebug() << "Connection failed:" << socket->errorString();
        delete socket;
        return nullptr;
    }

    qDebug() << "Connection established";
    return socket;
}


void IMClient::pushMsg(JSPP msg)
{
    msg_mutex.lock();
    unread_msg_list.push_back(msg);
    msg_mutex.unlock();
}

vector<JSPP> IMClient::getUnreadMsg(std::string peer_id)
{
    msg_mutex.lock();

    vector<JSPP> result;
    for(auto iter = unread_msg_list.begin();
        iter != unread_msg_list.end();) {
        list<JSPP>::iterator iter_erase = iter ++;
        if(iter_erase->from == peer_id) {
            result.push_back(*iter_erase);
            unread_msg_list.erase(iter_erase);
        }
    }
    msg_mutex.unlock();
    return result;
}

int IMClient::getIP_Port(string peer_id, IP_PORT &res)
{

    map<string, IP_PORT>::iterator iter= frient_ip_map.find(peer_id);
    if(iter!=frient_ip_map.end()){
        res = iter->second;
        return 0;
    }
    else

        return -1;
}



int IMClient::sendMsg(string peer_id, string msg)
{

    JSPP msg_json;
    msg_json.body = msg;
    msg_json.from = IMClient::Instance().getCurrID();
    msg_json.to = peer_id;
    msg_json.type = "chat";

    int res = sendMsg(msg_json);
    return res;
}

int IMClient::sendMsg(JSPP msg)
{
    QTcpSocket *self_sock = connectPeer(msg.to);
    if(!self_sock) {
        //链接失败
        return -1;
    }
    JSPP msg_json = msg;

    string send_str = jspp_to_str(msg_json);

    QByteArray sendData(send_str.c_str());
    // Send an initial buffer
#ifdef ENCRYPT
    char *realSnd = new char[send_str.size() + 1];
    realSnd[send_str.size()] = 0;
    memcpy(realSnd, send_str.c_str(), send_str.size());
    encode(realSnd, send_str.size());
    qDebug() << "send is" << realSnd;
    qint64 iResult = self_sock->write(realSnd, send_str.size());
    delete[] realSnd;  // Fix memory leak
#else
    qint64 iResult = self_sock->write(sendData);
#endif
    if (iResult < 0) {
        qDebug() << "send failed with error:" << self_sock->errorString();
        delete self_sock;
        return -1;
    }
    qDebug() << "Bytes Sent:" << iResult;
    self_sock->close();
    delete self_sock;
    return static_cast<int>(iResult);
}

string IMClient::getCurrID()
{
    return self->getID();
}

IMClient::IMClient(User *user)
{
    self = user;
}

IMClient::~IMClient()
{
    //delete timer;
}
