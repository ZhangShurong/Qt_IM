#include "friends_list.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include "main_window.h"

FriendsList::FriendsList(MainWindow *mainWindow, QWidget *parent):
    m_mainWindow(mainWindow), QWidget(parent), m_layout(new QVBoxLayout)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
        mainLayout->setContentsMargins(0,0,0,0);
        mainLayout->setSpacing(0);

        m_contentsWidget = new QWidget;
        m_layout->setContentsMargins(0,0,0,0);
        m_layout->setSpacing(0);
        m_contentsWidget->setLayout(m_layout);

        m_friendsScrollArea = new QScrollArea(this);
        m_friendsScrollArea->setWidgetResizable(true);
        m_friendsScrollArea->setAlignment(Qt::AlignLeft);

        m_friendsScrollArea->setWidget(m_contentsWidget);
        mainLayout->addWidget(m_friendsScrollArea);
        setLayout(mainLayout);
        setStyleSheet("QWidget{border: 0;}");
        creatMenu();
}

void FriendsList::addItem(QWidget *item)
{

}
