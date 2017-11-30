
#include "src/ui/loginform.h"

#include "src/ui/mainform.h"\

#include <QApplication>
#include "config.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Config::SERVER_HOST = "127.0.0.1";
    Config::SERVER_PORT = "8000";
    Config::SERVER_PORT_NUM = 8000;

    LoginForm l;
    l.show();

    return a.exec();
}
