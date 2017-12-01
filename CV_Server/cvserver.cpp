#include "cvserver.h"
#include "ui_cvserver.h"

CV_Server::CV_Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CVServer)
{
    ui->setupUi(this);
    ip = getIP();
    //ip = QString("127.0.0.1");
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
/*
    tmp.append(tcp->peerAddress().toString());
    tmp.append("发起了连接");
    ui->textEdit->append(tmp);
*/
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
            doRefresh();
            msg = QString::fromStdString(db->getAllUser())
                    + "*#*"
                    + QString::fromStdString(db->getallMap());
            qDebug() << "Return friends info " << msg;
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
        insertS.address = socket->peerAddress().toString().toStdString();
        insertS.port = add_port[1].toStdString();
        if(0 != db->intsertMap(insertS, userName.toStdString())) {
            res.code = "1";
        }
        QByteArray tmp;
        tmp.append(jspp_to_str(res).c_str());
        socket->write(tmp);
        socket->flush();
        doRefresh();
    }
    else if(msgJson.type == "refresh") {
        //刷新用户状态
    }
    else if (msgJson.type == "findpwd") {
        //找回密码
        qDebug() << "findpwd";
        JSPP res_find;
        res_find.type = "findpwd";
        QStringList info_list = QString::fromStdString(msgJson.body).split("&");
        if(info_list.size() != 3){
            res_find.code = "1";
        }
        else {
            string pwd_str = db->getPwd(info_list[0].toStdString(), info_list[1].toStdString(), info_list[2].toStdString());
            if(pwd_str == "")
                res_find.code = "1";
            else
                res_find.body = pwd_str;
        }
        QByteArray tmp;
        tmp.append(jspp_to_str(res_find).c_str());
        qDebug() << jspp_to_str(res_find).c_str();
        socket->write(tmp);
        socket->flush();
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
            if(address.toString().contains("192.168."))
                continue;
            if(address.toString().contains("10."))
                return address.toString();
        }
    }
    return "127.0.0.1";
}

void CV_Server::on_pushButton_clicked()  //开启服务器；
{
    init();
}

void CV_Server::removeUserFormList()
{
    QString tmp;

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    socket->peerAddress().toString();

    if(userList.removeOne(socket)){
        list();
    }

    if(onlineUser.find(socket) != onlineUser.end()) {
        string name = onlineUser[socket];
        ui->textEdit->append(QString::fromStdString(name) + "下线了");
        onlineUser.erase(onlineUser.find(socket));
        doRefresh();
        db->deleteMap(name);
    }

}

void CV_Server::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}

void CV_Server::doRefresh()
{

    //刷新好友列表以及在线状态
    qDebug() << "Do refresh";
    JSPP resJson;
    resJson.type = "refresh";
    QString msg;

    msg = QString::fromStdString(db->getAllUser())
            + "*#*"
            + QString::fromStdString(db->getallMap());
    qDebug() << "Return friends info " << msg;


    resJson.body = msg.toStdString();
    QByteArray tmp;
    tmp.append(jspp_to_str(resJson).c_str());

    for(map<QTcpSocket *,string>::iterator iter = onlineUser.begin(); iter != onlineUser.end(); iter ++) {

        QTcpSocket *socket = iter->first;
        socket->write(tmp);
        socket->flush();
    }
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

