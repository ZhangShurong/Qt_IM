#include "test.h"
#include "IM/imserver_local.h"
#include "IM/user.h"
#include "IM/imclient.h"
#include <iostream>
#include <thread>
#include <windows.h>
#include <QApplication>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    User *self = new User("0");
    IMClient *im = &IMClient::Instance(self);
    cout << im->getCurrID() << endl;

    IMServerLocal imserver("1024");
    imserver.start();

    return a.exec();
}
