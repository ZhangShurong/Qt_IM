/*
 * 文件接收方
*/
#ifndef DIALOGREC_H
#define DIALOGREC_H

#include <QDialog>
#include <QFileDialog>

#include <QUdpSocket>
#include <QFile>
#include "IM/imclient.h"
#include <QMessageBox>
#include <QTcpServer>
#include <QDateTime>

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
    QTcpSocket * tcpSocket; //通信
    QString fileName;       //文件名字
    bool isStart;
    int fileSize;
    int recvSize;
    QFile file;
    qint64 start_date_time;
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
    int time;

};

#endif // DIALOGREC_H
