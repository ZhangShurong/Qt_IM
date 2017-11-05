#include "message_list.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMenu>
#include <QDebug>
#include <QMouseEvent>
#include "main_window.h"

MessageList::MessageList(MainWindow *mainWindow, QWidget *parent):
        QWidget(parent),  m_layout(new QVBoxLayout), m_mainWindow(mainWindow)
{
    m_mesCounter = 0;
        if (NULL == m_mainWindow)
            return;
        QVBoxLayout *mainLayout=new QVBoxLayout();
        mainLayout->setContentsMargins(0,0,0,0);
        mainLayout->setSpacing(0);

        m_contentsWidget = new QWidget;
        m_layout->setContentsMargins(0,0,0,0);
        m_layout->setSpacing(0);
        m_contentsWidget->setLayout(m_layout);
        m_contentsWidget->setStyleSheet("QWidget{border: 0;}");

        m_scrollArea = new QScrollArea(this);
        m_scrollArea->setWidgetResizable(true);
        m_scrollArea->setAlignment(Qt::AlignLeft);

        m_scrollArea->setWidget(m_contentsWidget);

        mainLayout->addWidget(m_scrollArea);
        setLayout(mainLayout);

        setStyleSheet("QWidget{border: 0;}");

        //创建右键菜单
        creatMenu();
}

void MessageList::addItem(QWidget *item)
{
    // Remove last spacer item if present.
    int count = m_layout->count();

    if (count > 1)
    {
        m_layout->removeItem(m_layout->itemAt(count - 1));
    }

    // Add item and make sure it stretches the remaining space.
    m_layout->addWidget(item);
    m_layout->addStretch();

    emit timerStatus(true);
}

void MessageList::removeItem(QWidget *item)
{
    m_layout->removeWidget(item);
    if (m_layout->count() == 1)
    {
        m_layout->removeItem(m_layout->itemAt(0));

        emit timerStatus(false);
    }
}

int MessageList::size()
{
    m_mesCounter = m_layout->count();
    if (m_mesCounter > 1)
        --m_mesCounter;

    return m_mesCounter;
}

void MessageList::creatMenu()
{
    m_menu = new QMenu(this);
    QAction *showAll = new QAction(tr("查看所有消息"), this);
    QAction *ignoreAll = new QAction(tr("忽略所有消息"), this);

    connect(showAll, SIGNAL(triggered()),
        m_mainWindow, SLOT(showAllChatRoom()));
    connect(ignoreAll, SIGNAL(triggered()),
        m_mainWindow, SLOT(removeAllLinkButton()));

    m_menu->addAction(showAll);
    m_menu->addAction(ignoreAll);
}

void MessageList::showMenu()
{
    if (m_menu == NULL)
    {
        //创建右键菜单
        creatMenu();
    }
    m_menu->exec(QCursor::pos());
}

void MessageList::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        showMenu();
        return;
    }
    QWidget::mousePressEvent(event);
}
