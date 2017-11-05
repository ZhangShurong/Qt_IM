#include "interface/click_label.h"

#include <QMouseEvent>

// mark: public:--------------------------------------------------
ClickLabel::ClickLabel(QWidget *parent) :
    QLabel(parent)
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    setPalette(pa);
}

ClickLabel::ClickLabel(const QString &text, QWidget *parent) :
    QLabel(parent)
{
    setText(text);
}

// mark: protected:------------------------------------------------
void ClickLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
        emit clicked();//(this);
}

void ClickLabel::enterEvent(QEvent * )
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::blue);
    setPalette(pa);
}

void ClickLabel::leaveEvent(QEvent *)
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    setPalette(pa);
}
