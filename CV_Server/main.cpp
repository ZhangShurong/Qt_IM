#include <QApplication>
#include "cvserver.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    CV_Server w;
    w.show();

    return a.exec();
}
