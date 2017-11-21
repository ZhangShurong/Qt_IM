#ifndef LOGINFORM_H
#define LOGINFORM_H


#include "moveableframelesswindow.h"
#include "IM/imserver_local.h"
#include "IM/user.h"
#include "IM/imclient.h"
#include <iostream>
#include <QThread>
#include <QWidget>

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
signals:
    void readyForServer();
private:
    Ui::LoginForm *ui;
    QThread *serverThread;
    QTimer *timer;
};

class ServerWorker : public QObject
{
    Q_OBJECT
public slots:
    void runServer()
    {
        User *self = new User("CV");
        IMClient *im = &IMClient::Instance(self);
        std::cout << im->getCurrID() << std::endl;

        IMServerLocal iMServerLocal("1024");
        iMServerLocal.start();
    }
};

#endif // LOGINFORM_H
