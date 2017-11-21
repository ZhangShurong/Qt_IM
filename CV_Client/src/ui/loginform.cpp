#include "loginform.h"
#include "ui_loginform.h"
#include "mainform.h"

#include <iostream>
#include <QGridLayout>

LoginForm::LoginForm(QWidget *parent) :
    MoveableFramelessWindow(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    //关联登录按钮点击
    connect(ui->pushButton_login,SIGNAL(clicked()),this,SLOT(doLoginButClick()));
    ServerWorker *worker = new ServerWorker;
    serverThread = new QThread();
    worker->moveToThread(serverThread);
    connect(serverThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &LoginForm::readyForServer, worker, &ServerWorker::runServer);
    serverThread->start();
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
    emit readyForServer();
    //todo 此处设计有误，如有时间，应修改。
    Sleep(1000);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(connManage()));
    timer->start(1000);

    MainForm*m=new MainForm;
    m->show();
    this->hide();

}

void LoginForm::connManage()
{
    IMClient::Instance().mergeConn();
}



