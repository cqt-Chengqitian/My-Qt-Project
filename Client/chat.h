#ifndef CHAT_H
#define CHAT_H

#include <QWidget>

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = 0);
    ~Chat();
    QString m_strChatname; // 聊天对象的名字
    void updateshow(QString strMsg); // 更新聊天窗口中的消息

private slots:
    void on_send_Pb_clicked(); // 点击发送按钮

private:
    Ui::Chat *ui;
};

#endif // CHAT_H
