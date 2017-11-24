#include "collapseview.h"
#include "ui_collapseview.h"


#include "collapseviewitem.h"
#include <QDebug>

CollapseView::CollapseView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CollapseView)
{
    ui->setupUi(this);

    ui->widget->layout()->addWidget(new CollapseViewItem("我的好友",this));


}

CollapseView::~CollapseView()
{
    delete ui;
}

void CollapseView::butClick()
{
    //ui->listWidget->setHidden(!ui->listWidget->isHidden());
    qDebug()<<"xxx";
}
