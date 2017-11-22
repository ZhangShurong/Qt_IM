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
private slots:
    void doRegis();
};

#endif // REGISDIALOG_H
