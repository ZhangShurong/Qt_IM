#include "chat_widget.h"

#include <QLabel>
#include <qdebug.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QPixmap>
#include <QPushButton>
#include <QFrame>
#include <QFontComboBox>
#include <QComboBox>
#include <QTextBrowser>
#include <QDockWidget>

#include <QMessageBox>

#include <QCloseEvent>
#include <QColorDialog>
#include <QTextCharFormat>
#include <QFileDialog>



ChatWidget::ChatWidget(const ChatInformation &chatInf, QWidget *parent)
{
    initIMChatWidget();
}

void ChatWidget::initIMChatWidget()
{
    setWidgetToolBtn();
    /*********好友信息以及交互工具栏**************/
        QHBoxLayout *hLayoutFriend = new QHBoxLayout;
        QVBoxLayout *vLayoutFriend = new QVBoxLayout;
        QVBoxLayout *vLayoutFriendInfo = new QVBoxLayout;
        QHBoxLayout *hLayoutFriendInter = new QHBoxLayout;

    //    m_pmHeadPortrait = new QPixmap;
    //    m_frameHeadPortrait = new QFrame;
        m_labelHead = new IMClickLabel;
        m_labelName = new QLabel();
        m_labelMark = new QLabel();

        vLayoutFriendInfo->addWidget(m_labelName);
        vLayoutFriendInfo->addWidget(m_labelMark);

        hLayoutFriendInter->addWidget(m_toolBtnFile);

        hLayoutFriendInter->addStretch();

        hLayoutFriend->addWidget(m_labelHead);
        hLayoutFriend->addLayout(vLayoutFriendInfo);
        hLayoutFriend->addStretch();
    //    vLayoutFriend->addLayout(hLayoutFriendInter);

    //    hLayoutFriend->addWidget(m_frameHeadPortrait);
    //    hLayoutFriend->addWidget(m_labelHead);
        vLayoutFriend->addLayout(hLayoutFriend);
        vLayoutFriend->addLayout(hLayoutFriendInter);
    //    vLayoutFriend->addStretch();

        /*********输入编辑框工具栏**************/
        QHBoxLayout *hLayoutInputTool = new QHBoxLayout;

    //    hLayoutInputTool->addWidget(m_toolBtnFont);
        hLayoutInputTool->addWidget(m_cbFont);
        hLayoutInputTool->addWidget(m_cbSize);
        hLayoutInputTool->addStretch();

        hLayoutInputTool->addWidget(m_toolBtnBold);
        hLayoutInputTool->addWidget(m_toolBtnItalic);
        hLayoutInputTool->addWidget(m_toolBtnUnderline);
        hLayoutInputTool->addWidget(m_toolBtnColor);
        hLayoutInputTool->addWidget(m_toolBtnClear);
        hLayoutInputTool->addWidget(m_toolBtnHistory);


        /*********底部ip显示以及按钮**************/
        QHBoxLayout *hLayoutBtm = new QHBoxLayout;
        m_labelFriendIP = new QLabel(tr("ip"));
        m_btnClose = new QPushButton(tr("关闭"));
        m_btnSend = new QPushButton(tr("发送"));
        m_btnSend->setDefault(true);
    //    m_btnSend->setFocus();
        hLayoutBtm->addWidget(m_labelFriendIP);
        hLayoutBtm->addStretch();
        hLayoutBtm->addWidget(m_btnClose);
        hLayoutBtm->addWidget(m_btnSend);

        /******************左侧布局**********************/
        m_tbMessageShow = new QTextBrowser;
        m_tbMessageShow->setMinimumSize(400, 280);
        m_textInput = new IMInputTextEdit;
        m_textInput->setMinimumWidth(100);
        m_textInput->setMaximumHeight(170);
        QVBoxLayout *vLayoutLeft = new QVBoxLayout;
        //vLayoutLeft->addLayout(hLayoutFriend);
        vLayoutLeft->addWidget(m_tbMessageShow);
        vLayoutLeft->addLayout(hLayoutInputTool);
        vLayoutLeft->addWidget(m_textInput);
        vLayoutLeft->addLayout(hLayoutBtm);

        /******************中间整体布局**********************/
        QHBoxLayout *hLayoutMid = new QHBoxLayout;


    //    m_tbHistoryShow->setFixedWidth(300);
        m_dockHistoty = new QWidget;
        m_dockHistoty->hide();//setHidden(true);
        m_dockHistoty->setFixedWidth(250);
        m_tbHistoryShow = new QTextBrowser(m_dockHistoty);
        QLabel *lableHistory = new QLabel(tr("消息记录"));
        QVBoxLayout *vLayoutHistory = new QVBoxLayout;
        vLayoutHistory->addWidget(lableHistory);
        vLayoutHistory->addWidget(m_tbHistoryShow);
        vLayoutHistory->setContentsMargins(0,0,0,0);
        m_dockHistoty->setLayout(vLayoutHistory);
    //    m_dockHistoty->setFeatures(QDockWidget::DockWidgetMovable
    //                               | QDockWidget::DockWidgetFloatable
    //                               | QDockWidget::DockWidgetClosable);
    //    m_dockHistoty->setFeatures(QDockWidget::DockWidgetClosable);
    //    m_dockHistoty->setAllowedAreas(Qt::RightDockWidgetArea);
    //    m_dockHistoty->setWidget(m_tbHistoryShow);
    //    this->addDockWidget(Qt::LeftDockWidgetArea,dock);
        //去除标题栏
    //    QWidget* lTitleBar = m_dockHistoty->titleBarWidget();
    //    QWidget* lEmptyWidget = new QWidget();
    //    m_dockHistoty->setTitleBarWidget(lEmptyWidget);
    //    delete lTitleBar;
        //end


        /*
        QDockWidget *dock = new QDockWidget(tr("DockWindow 1"),this);
        dock->setFeatures(QDockWidget::DockWidgetMovable);
        dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        QTextEdit *title1 = new QTextEdit();
        title1->setText("Window 1");
        dock->setWidget(title1);
        this->addDockWidget(Qt::LeftDockWidgetArea,dock);
        */
        hLayoutMid->addLayout(vLayoutLeft);
        hLayoutMid->addWidget(m_dockHistoty);

        /***********************整体布局**************************/


        QVBoxLayout *vLayoutMain = new QVBoxLayout(this);

    //    vLayoutMain->setSizeConstraint(QLayout::SetFixedSize);
        vLayoutMain->addLayout(vLayoutFriend);
        vLayoutMain->addLayout(hLayoutMid);
    //    hLayoutMain->addWidget(m_tbHistoryShow);
    //    hLayoutMain->addWidget(m_dockHistoty);
        setLayout(vLayoutMain);

    //    setCentralWidget();
        m_textInput->setFocus();
}

void ChatWidget::setWidgetToolBtn()
{
    m_toolBtnFile = new QToolButton;
    m_toolBtnFile->setIcon(QIcon(":/image/send_file.png"));
    m_toolBtnFile->setAutoRaise(true);
    m_toolBtnFile->setToolTip(tr("发送文件"));
    m_toolBtnFile->setStatusTip(tr("发送文件"));
    m_toolBtnFile->setIconSize(QSize(CHAT_WIDGET_TOOL_SIZE,CHAT_WIDGET_TOOL_SIZE));


    m_cbFont = new QFontComboBox;
    m_cbFont->setToolTip(tr("字体"));
    m_cbFont->setStatusTip(tr("设置字体"));
    m_cbSize = new QComboBox;
    m_cbSize->setToolTip(tr("大小"));
    m_cbSize->setStatusTip(tr("设置字体大小"));
    for (int i=9;  i<=22; ++i)
    {
        m_cbSize->addItem(QString::number(i));
    }
    m_cbSize->setCurrentIndex(0);

    m_cbSize->setMaxVisibleItems(10);
    m_cbFont->setMaxVisibleItems(10);

    m_toolBtnBold = new QToolButton;
    m_toolBtnBold->setCheckable(true);
    m_toolBtnBold->setIcon(QIcon(":/image/font_bold.png"));
    m_toolBtnBold->setAutoRaise(true);
    m_toolBtnBold->setIconSize(QSize(CHAT_WIDGET_TOOL_SMALL_SIZE,CHAT_WIDGET_TOOL_SMALL_SIZE));
    m_toolBtnBold->setToolTip(tr("加粗"));
    m_toolBtnBold->setStatusTip(tr("是否加粗"));

    m_toolBtnItalic = new QToolButton;
    m_toolBtnItalic->setCheckable(true);
    m_toolBtnItalic->setIcon(QIcon(":/image/font_italic.png"));
    m_toolBtnItalic->setAutoRaise(true);
    m_toolBtnItalic->setIconSize(QSize(CHAT_WIDGET_TOOL_SMALL_SIZE,CHAT_WIDGET_TOOL_SMALL_SIZE));
    m_toolBtnItalic->setToolTip(tr("倾斜"));
    m_toolBtnItalic->setStatusTip(tr("是否倾斜"));

    m_toolBtnUnderline = new QToolButton;
    m_toolBtnUnderline->setCheckable(true);
    m_toolBtnUnderline->setIcon(QIcon(":/image/font_underline.png"));
    m_toolBtnUnderline->setAutoRaise(true);
    m_toolBtnUnderline->setIconSize(QSize(CHAT_WIDGET_TOOL_SMALL_SIZE,CHAT_WIDGET_TOOL_SMALL_SIZE));
    m_toolBtnUnderline->setToolTip(tr("下划线"));
    m_toolBtnUnderline->setStatusTip(tr("是否添加下划线"));

    m_toolBtnColor = new QToolButton;
    m_toolBtnColor->setIcon(QIcon(":/image/colour.png"));
    m_toolBtnColor->setAutoRaise(true);
    m_toolBtnColor->setIconSize(QSize(CHAT_WIDGET_TOOL_SMALL_SIZE,CHAT_WIDGET_TOOL_SMALL_SIZE));
    m_toolBtnColor->setToolTip(tr("颜色"));
    m_toolBtnColor->setStatusTip(tr("设置颜色"));

    m_toolBtnClear = new QToolButton;
    m_toolBtnClear->setIcon(QIcon(":/image/clear.png"));
    m_toolBtnClear->setAutoRaise(true);
    m_toolBtnClear->setIconSize(QSize(CHAT_WIDGET_TOOL_SMALL_SIZE,CHAT_WIDGET_TOOL_SMALL_SIZE));
    m_toolBtnClear->setToolTip(tr("清空"));
    m_toolBtnClear->setStatusTip(tr("清空消息显示"));

    m_toolBtnHistory = new QToolButton;
    m_toolBtnHistory->setIcon(QIcon(":/image/message.png"));
    m_toolBtnHistory->setAutoRaise(true);
    m_toolBtnHistory->setCheckable(true);
    m_toolBtnHistory->setIconSize(QSize(CHAT_WIDGET_TOOL_SMALL_SIZE,CHAT_WIDGET_TOOL_SMALL_SIZE));
    m_toolBtnHistory->setToolTip(tr("消息记录"));
    m_toolBtnHistory->setStatusTip(tr("打开消息记录"));
}
