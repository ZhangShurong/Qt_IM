#include "test.h"
#include "IM/imserver_local.h"
#include "IM/user.h"
#include "IM/imclient.h"
#include <iostream>
#include <QThread>
#include <windows.h>
#include <QApplication>
#include "UI/chat.h"
using namespace std;
class ServerThread : public QThread
{
protected:
    void run()
    {
        IMServerLocal iMServerLocal("1024");
        iMServerLocal.start();
    }
};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    User *self = new User("CV");
    IMClient *im = &IMClient::Instance(self);
    cout << im->getCurrID() << endl;
    ServerThread serverThread;
    serverThread.start();

    Chat c;
    c.show();

    return a.exec();
}
