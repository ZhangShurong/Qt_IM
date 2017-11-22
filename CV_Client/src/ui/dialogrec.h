#ifndef DIALOGREC_H
#define DIALOGREC_H

#include <QDialog>
#include <QFileDialog>

#include <QUdpSocket>
#include <QFile>
#include "IM/imclient.h"
#include <QMessageBox>

namespace Ui {
    class DialogRec;
}

class DialogRec : public QDialog
{
    Q_OBJECT

    string fileName;
    string peer_user;
    QUdpSocket *udpsocket;
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
