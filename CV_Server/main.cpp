#include <QApplication>
#include "cvserver.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set UTF-8 encoding for localization
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    CV_Server w;
    w.show();

    return a.exec();
}
