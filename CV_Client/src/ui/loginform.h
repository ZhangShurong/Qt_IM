#ifndef LOGINFORM_H
#define LOGINFORM_H


#include "moveableframelesswindow.h"
#include "IM/imserver_local.h"
#include "IM/user.h"
#include "IM/imclient.h"
#include <iostream>
#include <QThread>
#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class LoginForm;
}

class LoginForm :public MoveableFramelessWindow
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

protected:
      QWidget*getDragnWidget();
private slots:
    /**
     * @brief 登录按钮点击
     */
    void doLoginButClick();
    void connManage();
    void errorSlot();

    void connectedSlot();
    void readMessage();
    void doRegisClick();

signals:
    void readyForServer(User *self);
private:
    Ui::LoginForm *ui;
    QThread *serverThread;
    QTimer *timer;
    QTcpSocket *tcpsocket;

    bool connected;
    void login(QString userInfo);
//    void loginOK();
    void loginOK(User *self, map<string, IP_PORT> user_ip);
};

class ServerWorker : public QObject
{
    Q_OBJECT
public slots:
    void runServer(User *self)
    {
        //User *self = new User("CV");
        IMClient *im = &IMClient::Instance(self);
        std::cout << im->getCurrID() << std::endl;

        IMServerLocal iMServerLocal(PORT);
        iMServerLocal.start();
    }
};

#endif // LOGINFORM_H
