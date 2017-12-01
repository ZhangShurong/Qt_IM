#include "interface/login_window.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 以下部分解决中文乱码
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
    // 以上部分解决中文乱码
    LoginWindow loginWindow;
    loginWindow.show();

    return a.exec();
}
