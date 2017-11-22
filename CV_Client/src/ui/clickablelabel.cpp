#include "clickablelabel.h"
#include <QMouseEvent>

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent)
    : QLabel(parent)
{
    setText(text);
}

ClickableLabel::ClickableLabel(QWidget *parent)
     : QLabel(parent)
{

}

ClickableLabel::~ClickableLabel()
{

}
void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }

    QLabel::mousePressEvent(event);//将该事件传给父类处理
}
