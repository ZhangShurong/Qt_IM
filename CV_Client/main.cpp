
#include "src/ui/loginform.h"

#include "src/ui/mainform.h"\

#include <QApplication>
#include "config.h"

string SERVER_HOST;
string SERVER_PORT;
int SERVER_PORT_NUM;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SERVER_HOST = "10.10.254.229";
    SERVER_PORT = "8000";
    SERVER_PORT_NUM = 8000;

    LoginForm l;
    l.show();

    return a.exec();
}
