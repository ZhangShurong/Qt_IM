#include "regisdialog.h"
#include "ui_regisdialog.h"
#include <QMessageBox>

RegisDialog::RegisDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()), this, SLOT(doRegis()));
}

RegisDialog::~RegisDialog()
{
    delete ui;
}

void RegisDialog::doRegis()
{
    QString name = ui->nameEdit->text();
    QString pwd = ui->pwdEdit->text();
    QString que = ui->queEdit->text();
    QString ans = ui->ansEdit->text();
    if(!name.size() || !pwd.size() || !que.size() || !ans.size()) {
        QMessageBox::warning(this,tr("通知"),tr("请填完所有空格"),QMessageBox::Yes);
    }
}
