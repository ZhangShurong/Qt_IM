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

    ServerThread *worker = new ServerThread;
    worker->moveToThread(&serverThread);
    connect(&serverThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &LoginForm::readyForServer, worker, &ServerThread::runServer);
    serverThread.start();

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

    MainForm*m=new MainForm;
    m->show();
    this->hide();

}


//class ServerThread : public QThread
//{
//protected:
//    void run()
//    {
//        IMServerLocal iMServerLocal("1024");
//        iMServerLocal.start();
//    }
//};

