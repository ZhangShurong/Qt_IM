#ifndef CHAT_H
#define CHAT_H

#include <QWidget>
#include <QMouseEvent>
#include <QtGui>
#include <QDebug>
#include <QScrollBar>
#include <QFile>
#include "IM/conversation.h"
#include "IM/user.h"
#include "protocol/jspp.h"

//#include "emotion.h"
namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT
    Conversation *conv;
    User *peer_user;
    QTimer *timer;

public:
    explicit Chat(QWidget *parent, User *peer_user);
    ~Chat();
    QPoint move_point; //移动的距离
    bool mouse_press; //鼠标按下
    //鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    //鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
    //鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    //最小化及关闭
    bool eventFilter(QObject *object, QEvent *e);
    void setConv(Conversation *conv_new);

protected:
    void showEvent(QShowEvent *event);

private slots:
    //void sendemotion(QString s);
    void checkMsg();

    void on_sndBtn_clicked();

    void recvMsg(QString msg);
    void recvMsg(vector<JSPP> msg_vec);

private:
    Ui::Chat *ui;
    //Emotion* emotion;
};

#endif // CHAT_H
