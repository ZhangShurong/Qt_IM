#include "main_window.h"
#include <QLabel>
#include <qdebug.h>
#include <QListWidget>
#include <QHBoxLayout>
#include <QToolBox>
#include <QGroupBox>
#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QToolButton>
#include <QLineEdit>
#include <QMap>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>
#include <QMouseEvent>
#include <QDir>
#include <QFile>
#include <QTime>
#include <QTimerEvent>
#include "friends_list.h"
#include "friend_item.h"
#include "message_list.h"
#include "im_btn.h"
void MainWindow::initMainWindow()
{
        m_toolBtnSearch = new QToolButton();
        m_toolBtnSearch->setIcon(QIcon(":/image/search.png"));
        m_toolBtnSearch->setStyleSheet("border: 0px;");
        m_toolBtnSearch->setEnabled(false);

        QHBoxLayout *layoutTopSearch = new QHBoxLayout;
        layoutTopSearch->setContentsMargins(0, 0, 0, 0);
        //layoutTopSearch->addWidget(m_leSearch);
        layoutTopSearch->addWidget(m_toolBtnSearch);

        //searchLabel->setLayout(layoutTopSearch);


//        m_searchListWidget = new IMSearchListWidget;
//        m_searchListWidget->setVisible(false);

        m_tabWidget= new QTabWidget;

//        m_friendListWidget = new IMFriendListWidget(this, this);
//        m_flockListWidget = new IMFlockListWidget(this, this);
//        m_discussionListWidget = new IMDiscussionListWidget(this, this);
//        m_messageListWidget = new IMLatestMessageListWidget(this, this);
        p_messageList = new MessageList(this, this);
        p_friendsList = new FriendsList(this,this);

        p_messageList->addItem(new FriendItem(this));
        p_messageList->addItem(new FriendItem(this));
        p_messageList->addItem(new FriendItem(this));
        p_messageList->addItem(new FriendItem(this));
        p_messageList->addItem(new FriendItem(this));
        p_messageList->addItem(new FriendItem(this));

        m_tabWidget->addTab(p_friendsList, tr("联系人"));
//        m_tabWidget->addTab(m_flockListWidget,tr("IM群"));
//        m_tabWidget->addTab(m_discussionListWidget, tr("讨论组"));
        m_tabWidget->addTab(p_messageList, tr("新消息(0)"));
        //p_messageList->addItem(new IMBtn(this));

        m_labelOnlineNum = new QLabel("在线人数: 0");
        m_labelOnlineNum->setHidden(true);

        m_toolBtnAddFlock = new QToolButton;
        m_toolBtnAddFlock->setIcon(QIcon(":/image/addFlock.png"));
        m_toolBtnAddFlock->setAutoRaise(true);
        m_toolBtnAddFlock->setToolTip(tr("加群"));
        m_toolBtnAddFlock->setStatusTip(tr("添加群"));


        m_toolBtnConfig = new QToolButton;
        m_toolBtnConfig->setIcon(QIcon(":/image/config.png"));
        m_toolBtnConfig->setAutoRaise(true);
        m_toolBtnConfig->setToolTip(tr("系统管理"));
        m_toolBtnConfig->setStatusTip(tr("打开系统管理"));

        m_toolBtnAddFriend = new QToolButton;
        m_toolBtnAddFriend->setIcon(QIcon(":/image/addFriend.png"));
        m_toolBtnAddFriend->setAutoRaise(true);
        m_toolBtnAddFriend->setToolTip(tr("加好友"));
        m_toolBtnAddFriend->setStatusTip(tr("添加好友"));

        /****************头部*********/
        QVBoxLayout *layoutTopRight = new QVBoxLayout;

        //layoutTopRight->addWidget(m_labelNickname);


        QHBoxLayout *layoutTop = new QHBoxLayout;
        //layoutTop->addWidget(m_labelHead);
        layoutTop->addLayout(layoutTopRight);

        QHBoxLayout *layoutTopTool = new QHBoxLayout;
//        layoutTopTool->addWidget(m_cbStatus);
//        layoutTopTool->addWidget(m_toolBtnSpace);
//        layoutTopTool->addWidget(m_toolBtnMail);
//        layoutTopTool->addWidget(m_toolBtnMesBox);
//        layoutTopTool->addWidget(m_toolBtnNewMes);
        layoutTopTool->addStretch();


        QVBoxLayout *layoutHead = new QVBoxLayout;
        layoutHead->addLayout(layoutTop);
        layoutHead->addLayout(layoutTopTool);
        //layoutHead->addWidget(searchLabel);

        /***********ennd 头部************/

        QHBoxLayout *layoutBtm = new QHBoxLayout;
        layoutBtm->addWidget(m_labelOnlineNum);
        layoutBtm->addStretch();
        layoutBtm->addWidget(m_toolBtnAddFlock);

        layoutBtm->addWidget(m_toolBtnAddFriend);

        layoutBtm->addWidget(m_toolBtnConfig);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addLayout(layoutHead);
        layout->addWidget(m_tabWidget);
        //layout->addWidget(m_searchListWidget);
        layout->addLayout(layoutBtm);

        setLayout(layout);

        setMinimumSize(278, 300);
        resize(278,500);
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    initMainWindow();
}
