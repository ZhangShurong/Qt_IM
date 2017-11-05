#ifndef FRIENDSLIST_H
#define FRIENDSLIST_H

#include <QWidget>
class QVBoxLayout;
class QScrollArea;
class QMenu;
class MainWindow;

class FriendsList : public QWidget
{
    Q_OBJECT
public:
    FriendsList(MainWindow *mainWindow, QWidget *parent = 0);
    void addItem(QWidget *item);
    //创建右键菜单
    void creatMenu(){}

signals:

public slots:
      void showMenu(){}

protected:
    void mousePressEvent(QMouseEvent *e){}

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_friendsScrollArea;

    //右键菜单
    QMenu *m_menu;

    MainWindow *m_mainWindow;
};

#endif // FRIENDSLIST_H
