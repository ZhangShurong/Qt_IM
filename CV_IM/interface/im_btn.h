#ifndef IMBTN_H
#define IMBTN_H
#include <QPushButton>
#include "communication/protocal.h"
class QMenu;
class QLabel;
class MainWindow;

class IMBtn : public QPushButton
{
    Q_OBJECT
public:
    IMBtn(QWidget *parent = 0);
    virtual ~IMBtn(){}
    void setLatestMessage(const TalkMessage & mes);

    // 设置button显示信息
    virtual void setButtonStatus() = 0;

signals:
    void deleteLinkButton(const QString & id);


public slots:
    // 左键 双击
    virtual void onDoubleClickButton() = 0;
    // 右键点击按键
    virtual void onClickRightButton(/*const QPoint &*/);
    // 忽略消息
    virtual void ignoreMessage() = 0;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);

protected:
    QLabel *m_labelHead;
    QLabel *m_labelInfo;
    QLabel *m_labelMes;

    // 右键菜单
    QMenu *m_menu;

private:
    // 创建右键菜单
    void creatMenu(/*const QList<QString> *groutList*/);
};

#endif // IMBTN_H
