#include "chat.h"
#include "ui_chat.h"
#include <QDesktopWidget>
#include "IM/imclient.h"
#include "IM/connection.h"

Chat::Chat(QWidget *parent, User *peer_user) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->label_4->installEventFilter(this);
    ui->label_5->installEventFilter(this);
    QFile file(":/myqss/scrollbar.qss");
    file.open(QFile::ReadOnly);
    ui->textBrowser->verticalScrollBar()->setStyleSheet(file.readAll());
    ui->label_8->installEventFilter(this);
    move((QApplication::desktop()->width() - width())/2,(QApplication::desktop()->height() - height()-20)/2);
    conv = nullptr;
    this->peer_user = peer_user;
    timer  = new QTimer();
    //    emotion=new Emotion();
    //    connect(emotion,SIGNAL(imgurl(QString)),this,SLOT(sendemotion(QString)));
}

Chat::~Chat()
{
    delete ui;
}
void Chat::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->setMouseTracking(true);
        mouse_press = true;
        //鼠标相对于窗体的位置（或者使用event->globalPos() - this->pos()）
        move_point = event->pos();
        qDebug()<<"移动1";
        qDebug()<<event->source();
    }

}

void Chat::mouseReleaseEvent(QMouseEvent *event)
{
    //设置鼠标为未被按下
    mouse_press = false;
}

void Chat::mouseMoveEvent(QMouseEvent *event)
{
    //若鼠标左键被按下
    //  qDebug()<<"mouse_press="<<event->globalPos();
    if(mouse_press)
    {

        //鼠标相对于屏幕的位置
        QPoint move_pos = event->globalPos();
        //移动主窗体位置
        this->move(move_pos - move_point);
    }
}

bool Chat::eventFilter(QObject *object, QEvent *e)
{
    if(e->type()==QEvent::MouseButtonPress&&object==ui->label_5)
    {
        close();
    }
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_4){
        showMinimized();
        qDebug()<<"最小化";
        qDebug()<<mouse_press;

    }
    //表情发送
    else if(e->type()==QEvent::MouseButtonPress&&object==ui->label_8){

        qDebug()<<ui->label_8->x();
        qDebug()<<x();
        //        emotion->move(x()-10,ui->label_8->y()-100);
        //        emotion->show();

    }
    return false;
}

void Chat::setConv(Conversation *conv_new)
{
    if(conv_new)
        conv = conv_new;
    return;
}

void Chat::recvMsg(QString msg)
{
    qDebug()<<msg.length();
    if(msg.length()>=23){
        QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:left;margin-right:15px;margin-left:15px;'><div style='background:rgb(205,215,226);padding-top:8px;float:left;margin-top:10px;margin-left:45px;padding:10px;margin-right:60px;'><span style='font-size:15px;font-family:'Microsoft YaHei';margin-top:2px;'>"+msg+"</span></div><br><br><br>";
        ui->textBrowser->insertHtml(s1);
    }
    else{
        int flag=60+(24-msg.length())*15;
        QString myflag=QString::number(flag, 10);;
        qDebug()<<myflag;
        QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:left;margin-right:15px;margin-left:15px;'><div style='background:rgb(205,215,226);padding-top:8px;float:left;margin-top:10px;margin-left:45px;padding:10px;margin-right:"+myflag+"px;'>";
        s1+="<span style='font-size:15px;font-family:'Microsoft YaHei';margin-top:2px;'>"+msg+"</span></div><br><br><br>";
        qDebug()<<s1;
        ui->textBrowser->insertHtml(s1);
    }
}

void Chat::recvMsg(vector<JSPP> msg_vec)
{
    for(JSPP item:msg_vec) {
        recvMsg(QString::fromStdString(item.body));
    }
}
void Chat::showEvent(QShowEvent *event)
{
    mouse_press=false;
    if(!conv)
        conv = IMClient::Instance().getConversation(peer_user->getID());
    connect(timer, SIGNAL(timeout()), this, SLOT(checkMsg()));
    timer->start(100);
}

void Chat::checkMsg()
{
    recvMsg(IMClient::Instance().getUnreadMsg(peer_user->getID()));
}


void Chat::on_sndBtn_clicked()
{
    QString content=ui->textEdit->toPlainText();
    qDebug()<<content.length();
    if(content.length()>=23){
        qDebug()<<"进入了这个";
        QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:right;margin-left:15px;margin-right:15px;'><div style='background:rgb(205,215,226);padding-top:8px;float:right;margin-top:10px;margin-right:45px;padding:10px;margin-left:60px;'><span style='font-size:15px;font-family:'Microsoft YaHei';margin-top:2px;'>"+content+"</span></div><br><br><br>";
        ui->textBrowser->insertHtml(s1);
    }
    else{
        int flag=60+(24-content.length())*15;
        QString myflag=QString::number(flag, 10);;
        qDebug()<<myflag;
        QString s1="<img width='30' height='30' src='qrc:/10.jpeg' style='float:right;margin-left:15px;margin-right:15px;'><div style='background:rgb(205,215,226);padding-top:8px;float:right;margin-top:10px;margin-right:45px;padding:10px;margin-left:"+myflag+"px;'>";
        s1+="<span style='font-size:15px;font-family:'Microsoft YaHei';margin-top:2px;'>"+content+"</span></div><br><br><br>";
        qDebug()<<s1;
        ui->textBrowser->insertHtml(s1);
    }
    conv->sendMsg(ui->textEdit->toPlainText().toStdString());
    ui->textEdit->clear();
}
