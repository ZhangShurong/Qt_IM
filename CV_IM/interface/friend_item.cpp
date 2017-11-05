#include "friend_item.h"
#include <QMenu>
#include <QBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>


FriendItem::FriendItem(QWidget *parent)
{
    setButtonStatus();
}

void FriendItem::setButtonStatus()
{
    QString str = QString(":/image/head/%1.bmp").
        arg(QString::number(1));
    QString text, mark;

    mark = "test";

    QPixmap pixmap;
    QIcon icon(str);
    pixmap = icon.pixmap(QSize(HEAD_SMALL_SIZE, HEAD_SMALL_SIZE));
    m_labelHead->setPixmap(pixmap);

    text = QString("<font color=green>%1(%2)</font>").
            arg(1).arg(mark);
    m_labelInfo->setText(text);;
}

void FriendItem::onDoubleClickButton()
{

}

void FriendItem::ignoreMessage()
{

}
