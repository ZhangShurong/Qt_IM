#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
#include <QPushButton>
#include "communication/protocal.h"
#include "im_btn.h"
class FriendItem : public IMBtn
{
    Q_OBJECT
public:
    FriendItem(QWidget *parent = 0);

    // 设置button显示信息
    void setButtonStatus();

public slots:
    // 左键 双击
    void onDoubleClickButton();
    // 忽略消息
    void ignoreMessage();

};

#endif // FRIENDITEM_H
