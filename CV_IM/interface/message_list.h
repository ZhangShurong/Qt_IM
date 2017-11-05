#ifndef MESSAGELIST_H
#define MESSAGELIST_H

#include <QWidget>
class QVBoxLayout;
class QScrollArea;
class QMenu;
class MainWindow;

class MessageList : public QWidget
{
    Q_OBJECT
public:
    MessageList(MainWindow *mainWindow, QWidget *parent = 0);
    void addItem(QWidget *item);
    void removeItem(QWidget *item);
    int size();
    //创建右键菜单
    void creatMenu();
signals:
    void timerStatus(bool isOpen);

public slots:
    void showMenu();
protected:
    void mousePressEvent(QMouseEvent *e);

private:
    QVBoxLayout *m_layout;
    QWidget *m_contentsWidget;
    QScrollArea *m_scrollArea;

    //右键菜单
    QMenu *m_menu;

    MainWindow *m_mainWindow;

    int m_mesCounter;
};

#endif // MESSAGELIST_H
