#ifndef FRIEND_H
#define FRIEND_H

#include "chat.h"

#include <QWidget>

namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = 0);
    ~Friend();
    void flushfriend(); // 刷新好友列表
    void updatefriend_Lw(QStringList friendlist); // 添加好友到好友列表
    Chat* m_pChat; // 聊天窗口

private slots:
    void on_find_Pb_clicked();

    void on_flush_Pb_clicked();

    void on_chat_Pb_clicked();

private:
    Ui::Friend *ui;
};

#endif // FRIEND_H
