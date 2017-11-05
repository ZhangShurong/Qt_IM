#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMap>

class QListWidget;
class QListWidgetItem;
class QGroupBox;
class QVBoxLayout;
class QToolBox;
class QFrame;
class QLabel;
class QComboBox;
class QToolButton;
class QLineEdit;
class QTabWidget;
class QMenu;
class FriendsList;
class MessageList;
class MainWindow : public QWidget
{
    Q_OBJECT
    void initMainWindow();
public:
    MainWindow(QWidget *parent = 0);

signals:

public slots:

private:
        QLabel *m_labelNickname;
        QComboBox *m_cbStatus;
        QLabel *m_labelMark;
        QToolButton *m_toolBtnSpace;
        QToolButton *m_toolBtnMail;
        QToolButton *m_toolBtnMesBox;
        QToolButton *m_toolBtnNewMes;

        QLineEdit *m_leSearch;
        QToolButton *m_toolBtnSearch;
    //    QLabel *m_labelSearch;

        QLabel *m_labelOnlineNum;
        QToolButton *m_toolBtnAddFlock;
    //    QToolButton *m_toolBtnAddDiscussion;
        QToolButton *m_toolBtnAddFriend;
    //    QToolButton *m_toolBtnSearchAdd;
        QToolButton *m_toolBtnConfig;

        QTabWidget *m_tabWidget;
        int m_timerId; //ID of timer
        QMenu *m_menu;

        FriendsList *p_friendsList;
        MessageList *p_messageList;
};

#endif // MAINWINDOW_H
