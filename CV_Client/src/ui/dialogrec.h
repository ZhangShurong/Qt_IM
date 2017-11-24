#ifndef DIALOGREC_H
#define DIALOGREC_H

#include <QDialog>
#include <QFileDialog>

#include <QUdpSocket>
#include <QFile>
#include "IM/imclient.h"
#include <QMessageBox>
#include <QTcpServer>

#define TCP
#define BUF_SIZE 4096

namespace Ui {
class DialogRec;
}

class DialogRec : public QDialog
{
    Q_OBJECT

   // string fileName;
    string peer_user;
    string port;
#ifndef TCP
    QUdpSocket *udpsocket;//UDP文件传输,不可靠
#else
    QTcpServer * tcpServer; //监听
    QTcpSocket * tcpSocket; //通信
    QFile file;             //文件对象
    QString fileName;       //文件名字
    qint64 fileSize;        //文件大小
    qint64 sendSize;        //已经发送大小
    QTimer timer;           //定时器
    void sendHeader();
private slots:
    void sendData();
    //QTcpSocket *tcpSocket;//TCP文件传输
#endif

public:
    explicit DialogRec(QWidget *parent = 0);
    void setFileReq(JSPP fileReq);
    ~DialogRec();

private slots:
    void on_okBtn_clicked();
    void readPendingDatagrams();
    void on_noBtn_clicked();

private:
    Ui::DialogRec *ui;

};

#endif // DIALOGREC_H
