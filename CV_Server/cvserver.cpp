#include "cvserver.h"
#include "ui_cvserver.h"

CV_Server::CV_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CVServer)
{
    ui->setupUi(this);
    //ip = getIP();
    ip = QString("127.0.0.1");
    ui->textEdit_IP->append(ip);
    ui->textEdit_IP->setReadOnly(true);
    ui->lineEdit->setText("8000");
    tcpServer = new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(newConnectSlot()));

    initUserAndmap();//记得删掉，测试用
    db = new ServerDB();
}

CV_Server::~CV_Server()
{
    delete ui;
}

void CV_Server::init()
{
    QString dk = ui->lineEdit->text();
    bool ok;
    int i = dk.toInt(&ok, 10);
    if(tcpServer->listen(QHostAddress::Any, i)){
        ui->textEdit->append("listen OK!");
    }else{
        ui->textEdit->append("listen error!");
    }
}

void CV_Server::newConnectSlot()
{
    QString tmp;
    QTcpSocket *tcp = tcpServer->nextPendingConnection();
    connect(tcp,SIGNAL(readyRead()),this,SLOT(readMessage()));
    userList << tcp;

    tmp.append(tcp->peerAddress().toString());
    //tmp.append(tcp->peerPort());
    tmp.append("发起了连接");
    ui->textEdit->append(tmp);

    connect(tcp,SIGNAL(disconnected()),this,SLOT(removeUserFormList()));
}



void CV_Server::readMessage()
{
    QByteArray tmp;
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    //qDebug() << socket->readAll();
    tmp.append(socket->readAll());
    string jsonStr = tmp.toStdString();
    JSPP msgJson = parse(jsonStr);
    if(msgJson.type == "login") {
        /*
         * 返回格式：好友列表+在线用户Ip与端口
         * 注：暂时不启用好友功能,注册用户默认为好友
         * vergil&CV*#*vergil=ip:port&dante=ip:port
         */
        bool ok = false;
        QString name = QString::fromStdString(msgJson.body).split("*#*")[0];
        QString pwd = QString::fromStdString(msgJson.body).split("*#*")[1];
        //认证
        JSPP resJson;
        resJson.type = "login";
        QString msg;
        if(0 != db->authUser(name, pwd))
        {
            resJson.code = "1";
        }
        else {
            onlineUser[socket] = name.toStdString();
            ui->textEdit->append(name + "上线了");
            msg = QString::fromStdString(db->getAllUser())
                    + "*#*"
                    + QString::fromStdString(db->getallMap());
            qDebug() << "Return friends info " << msg;
            /*
            QString c = "";
            for(auto m = user_ip_map.begin();m!=user_ip_map.end();++m) {
                msg += c + QString::fromStdString(m->first);
                c = "&";
            }
            c = "";
            msg += "*#*";
            for(auto m = user_ip_map.begin();m!=user_ip_map.end();++m) {
                msg += c + QString::fromStdString(m->first)
                        + QString("=")
                        + QString::fromStdString(m->second.address)
                        + QString(":")
                        + QString::fromStdString(m->second.port);
                c = "&";
            }
            */
        }

        resJson.body = msg.toStdString();
        QByteArray tmp;
        tmp.append(jspp_to_str(resJson).c_str());
        socket->write(tmp);
        socket->flush();
    }
    else if(msgJson.type == "reg") {
        QStringList regInfo = QString::fromStdString(msgJson.body).split("&");
        JSPP res;
        res.type = "reg";
        if(db->insertUser(regInfo[0], regInfo[1], regInfo[2], regInfo[3])) {
            res.code = "1";
        }
        QByteArray tmp;
        tmp.append(jspp_to_str(res).c_str());
        socket->write(tmp);
        socket->flush();
    }
    else if(msgJson.type == "chat")
    {
        //储存离线消息
        db->insertMsg(msgJson);

    }
    else if(msgJson.type == "reqmsg") {
        vector<JSPP> msgs= db->getMsg(msgJson.to);
        for(JSPP item: msgs){
            QByteArray tmp;
            tmp.append(jspp_to_str(item).c_str());
            socket->write(tmp);
            socket->flush();
        }
    }
    else if(msgJson.type == "port") {
        //保存用户名和端口
        /*
         * 消息格式:name&address:port
         */
        QStringList portInfo = QString::fromStdString(msgJson.body).split("&");
        QString userName = portInfo[0];
        QStringList add_port = portInfo[1].split(":");
        JSPP res;
        res.type = "port";
        IP_PORT insertS;
        insertS.address = add_port[0].toStdString();
        insertS.port = add_port[1].toStdString();
        if(0 != db->intsertMap(insertS, userName.toStdString())) {
            res.code = "1";
        }
        QByteArray tmp;
        tmp.append(jspp_to_str(res).c_str());
        socket->write(tmp);
        socket->flush();
    }
    else if(msgJson.type == "refresh") {
        //刷新用户状态
    }
}

QString CV_Server::getIP()  //获取ip地址
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list){
       if(address.protocol() == QAbstractSocket::IPv4Protocol){
           if (address.toString().contains("127.0.")){
               continue;
           }
           return address.toString();
       }
    }
    return 0;
}

void CV_Server::on_pushButton_clicked()  //开启服务器；
{
    init();
}

void CV_Server::removeUserFormList()
{
    QString tmp;

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
//    QTcpSocket *tcp = tcpServer->nextPendingConnection();

    socket->peerAddress().toString();

    if(userList.removeOne(socket)){
        tmp.append(socket->peerAddress().toString());
        tmp.append("断开了连接！");
        ui->textEdit->append(tmp);
        list();
    }

    if(onlineUser.find(socket) != onlineUser.end()) {
        string name = onlineUser[socket];
        ui->textEdit->append(QString::fromStdString(name) + "下线了");
        db->deleteMap(name);
    }

}

void CV_Server::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}

void CV_Server::list()
{
    QByteArray msg;
    QString aa;
    for(int i = 0; i < userList.count() && userList.at(i)->peerAddress().toString() != aa; i++){
        msg.append( userList.at(i)->peerAddress().toString());
         for(int k = 0; k <ipss.count(); k++){
             if( userList.at(i)->peerAddress().toString()== ipss.at(k)){
                 msg.append("\n");
                 msg.append(namess.at(k));
                 break;
             }
         }
        msg.append("   @");
        aa =  userList.at(i)->peerAddress().toString();
    }
    for(int j = 0; j <userList.count(); j++){
        userList.at(j)->write(msg);
    }
    msg.clear();
}

