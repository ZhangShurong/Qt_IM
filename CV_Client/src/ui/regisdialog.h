#ifndef REGISDIALOG_H
#define REGISDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class RegisDialog;
}

class RegisDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisDialog(QWidget *parent = 0);
    ~RegisDialog();

private:
    Ui::RegisDialog *ui;
    QTcpSocket *tcpsocket;
    bool connected;
private slots:
    void doRegis();
    void errorSlot();
    void connectedSlot();
    void readMessage();
};

#endif // REGISDIALOG_H
