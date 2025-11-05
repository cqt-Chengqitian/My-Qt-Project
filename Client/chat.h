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
    QString m_strChatname;
    void updateshow(QString strMsg);

private slots:
    void on_send_Pb_clicked();

private:
    Ui::Chat *ui;
};

#endif // CHAT_H
