
#include "src/ui/loginform.h"

#include "src/ui/mainform.h"

#include <QApplication>
#include "config.h"

string SERVER_HOST;
string SERVER_PORT;
int SERVER_PORT_NUM;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginForm l;
    l.show();

    return a.exec();
}
