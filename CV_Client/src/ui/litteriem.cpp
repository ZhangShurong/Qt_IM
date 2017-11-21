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
LitterIem::LitterIem(QString picPath, QString user_id, QWidget *parent):
    QWidget(parent),
    ui(new Ui::LitterIem)
{
    ui->setupUi(this);
    QPixmap p(picPath);
    ui->label_pic->setPixmap(p);
    user = new User(user_id.toStdString());
    chat_ui = new Chat(0, user);
    conv = nullptr;
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
