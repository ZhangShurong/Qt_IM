#ifndef LITTERIEM_H
#define LITTERIEM_H

#include <QWidget>
#include "chat.h"
#include "IM/user.h"
#include "IM/conversation.h"

namespace Ui {
class LitterIem;
}

class LitterIem : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief LitterIem
     * @param picPath 头像的文件路径
     * @param parent
     */
    explicit LitterIem(QString picPath,QString user_id,QWidget *parent = 0);
    ~LitterIem();

private:
    Ui::LitterIem *ui;
    Chat *chat_ui;
    User *user;
    Conversation *conv;
protected:
    void mouseDoubleClickEvent(QMouseEvent*);
};

#endif // LITTERIEM_H
