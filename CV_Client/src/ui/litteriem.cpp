#include "litteriem.h"
#include "ui_litteriem.h"
#include "IM/imclient.h"
#include <QPixmap>

/*
LitterIem::LitterIem(QString picPath, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LitterIem)
{
    ui->setupUi(this);

    QPixmap p(picPath);
    ui->label_pic->setPixmap(p);
    chat_ui = new Chat();

}
*/
LitterIem::LitterIem(QString picPath, User* user, QWidget *parent):
    QWidget(parent),
    ui(new Ui::LitterIem)
{
    ui->setupUi(this);
    QPixmap p(picPath);
    ui->label_pic->setPixmap(p);
    this->user = user;
    chat_ui = new Chat(0, user);
    ui->label_name->setText(QString::fromStdString(user->getID()));
    //conv = nullptr;
}


LitterIem::~LitterIem()
{
    delete ui;
    delete chat_ui;
}

void LitterIem::mouseDoubleClickEvent(QMouseEvent *)
{

    chat_ui->show();
}
