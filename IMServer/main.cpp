#include "view/IMServerWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set UTF-8 encoding for localization
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
#endif
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

//    // Ensure only one instance runs
//    QSystemSemaphore sema("IMServerKey",1,QSystemSemaphore::Open);
//    // Operate shared memory in critical section
//    sema.acquire();
//    QSharedMemory mem("IMServerObject");

//    // If global object already exists then exit
//    if (!mem.create(1))
//    {
//        QMessageBox::warning(NULL, "error",
//            "A server has already been running.");
//        sema.release();
//        return 0;
//    }
//    sema.release();

    IMServerWindow w;
    w.show();

    return a.exec();
}
