#ifndef FORGOTDIA_H
#define FORGOTDIA_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class ForgotDia;
}

class ForgotDia : public QDialog
{
    Q_OBJECT

public:
    explicit ForgotDia(QWidget *parent = 0);
    ~ForgotDia();





private:
    Ui::ForgotDia *ui;
    QTcpSocket *tcpsocket;
    bool connected;
private slots:
    void acceptSlot();
    void connectedSlot();
    void errorSlot();
    void readMessage();
};

#endif // FORGOTDIA_H
