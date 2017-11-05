#include "login_window.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QTime>
#include <QTimerEvent>
#include <QInputDialog>
#include <time.h>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    initIMLoginWidget();
    linkSignalWithSlot();
    setWindowTitle(tr("登录IM"));
}

LoginWindow::~LoginWindow()
{

}

void LoginWindow::onClickBtnLogin()
{
    MainWindow *m = new MainWindow();
    m->show();
}

void LoginWindow::initIMLoginWidget()
{
    m_isLogin = true;

    m_counter = 0;
    m_closeTimer = true;
    m_timerID = 0;
    qsrand(time(NULL));

    QVBoxLayout *vLayoutMidTop   = new QVBoxLayout;
    QVBoxLayout *vLayoutRightTop = new QVBoxLayout;
    QHBoxLayout *hLayoutTop      = new QHBoxLayout;

    QHBoxLayout *hLayoutMid  = new QHBoxLayout;
    QHBoxLayout *hLayoutBtm  = new QHBoxLayout;
    QVBoxLayout *vLayoutMain = new QVBoxLayout;

    QLabel *labelHeadLine = new QLabel(tr("登录即时通信系统"));
    labelHeadLine->setAlignment(Qt::AlignCenter);
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::black);
    labelHeadLine->setPalette(pa);
    QFont ft;
    ft.setPointSize(15);
    labelHeadLine->setFont(ft);

    m_leUserID = new QLineEdit;
    m_leUserID->setPlaceholderText(tr("帐号"));
    m_leUserPwd = new QLineEdit;
    m_leUserPwd->setPlaceholderText(tr("密码"));
    m_leUserPwd->setEchoMode(QLineEdit::Password);
    vLayoutMidTop->addWidget(m_leUserID);
    vLayoutMidTop->addWidget(m_leUserPwd);

    m_labelRegister = new ClickLabel;
    m_labelRegister->setText(tr("注册帐号"));
    m_labelForgotPwd = new ClickLabel;
    m_labelForgotPwd->setText(tr("忘记密码"));
    vLayoutRightTop->addWidget(m_labelRegister);
    vLayoutRightTop->addWidget(m_labelForgotPwd);

    hLayoutTop->addLayout(vLayoutMidTop);
    hLayoutTop->addLayout(vLayoutRightTop);
    hLayoutTop->setMargin(10);
    //end top

    //mid
    m_cbKeepPwd = new QCheckBox(tr("记住密码"));
    m_cbAutoLogin = new QCheckBox(tr("自动登录"));
    hLayoutMid->addWidget(m_cbKeepPwd);
    hLayoutMid->addWidget(m_cbAutoLogin);
    //end mid

    //bottom
    m_moreButton = new QPushButton(tr("设置"));
    m_moreButton->setCheckable(true);
    m_moreButton->setAutoDefault(false);
    m_cbxStatus = new QComboBox();
    m_cbxStatus->addItem(tr("在线"));
    m_cbxStatus->addItem(tr("隐身"));
    m_cbxStatus->addItem(tr("忙碌"));
    m_cbxStatus->addItem(tr("离开"));
    m_cbxStatus->addItem("请勿打扰");
    m_btnLogin = new QPushButton(tr("登录"));
    m_btnLogin->setDefault(true);

    hLayoutBtm->addWidget(m_moreButton);
    hLayoutBtm->addWidget(m_cbxStatus);
    hLayoutBtm->addWidget(m_btnLogin);
    //end bottom

    //hide
    m_hostAddress = new QLineEdit;;
    m_hostAddress->setPlaceholderText(tr("服务器IP地址"));
    m_hostPort = new QLineEdit;
    m_hostPort->setFixedWidth(60);
    m_hostPort->setPlaceholderText(tr("端口号"));
    m_extendedWidget = new QWidget;
    QHBoxLayout *hLayoutExtended = new QHBoxLayout;
    hLayoutExtended->setMargin(0);
    hLayoutExtended->addWidget(m_hostAddress);
    hLayoutExtended->addWidget(m_hostPort);
    m_extendedWidget->setLayout(hLayoutExtended);
    //end hide

    m_labelStatus = new QLabel;
    m_labelStatus->setText(tr("你好,请登录系统."));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_labelStatus->setSizePolicy(sizePolicy);

    vLayoutMain->setSizeConstraint(QLayout::SetFixedSize);
    vLayoutMain->addWidget(labelHeadLine);
    vLayoutMain->addLayout(hLayoutTop);
    vLayoutMain->addLayout(hLayoutMid);
    vLayoutMain->addLayout(hLayoutBtm);
    vLayoutMain->addWidget(m_extendedWidget);
    vLayoutMain->addWidget(m_labelStatus);
    vLayoutMain->setMargin(5);

    setLayout(vLayoutMain);

    m_extendedWidget->hide();

    // 读取自动登录配置文件
    //readAutoLoginFile(AUTO_LOGIN_FILE_NAME);
}

void LoginWindow::linkSignalWithSlot()
{
    connect(m_btnLogin, SIGNAL(clicked()),
            this, SLOT(onClickBtnLogin()));

}
