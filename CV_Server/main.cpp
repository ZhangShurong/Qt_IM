#include "test.h"
#include "IM/imserver_local.h"
#include "IM/user.h"
#include "IM/imclient.h"
#include <iostream>
#include <thread>
#include <windows.h>
using namespace std;
int main() {
    thread t([]
    {
        Sleep(10);
        test();
    }
    );
    t.detach();

    User *self = new User(0);
    IMClient *im = &IMClient::Instance(self);
    cout << im->getCurrID() << endl;

    IMServerLocal imserver("1024");
    imserver.start();

    return 0;
}
