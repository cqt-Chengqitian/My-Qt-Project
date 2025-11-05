#ifndef CLIENT_H
#define CLIENT_H

#include "protocol.h"

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    QTcpSocket m_tcpSocket;
    QString m_strIP;
    quint16 m_usPort;
    QString m_strLoginName;
    QString m_strRootPath;
    QByteArray buffer;

    ~Client();
    void loadConfig();
    void showConnect();
    static Client& getInstance();
    void sendMsg(PDU* pdu);
    void recvMsg();
    PDU* readMsg();
    void handleMsg(PDU* pdu);

private slots:
    void on_regist_Pb_clicked();

    void on_login_Pb_clicked();

private:
    Ui::Client *ui;
    explicit Client(QWidget *parent = 0);
    Client(const Client& other) = delete;
    Client& operator =(const Client*) = delete;//防止所有私自创建对象的方式，实现单例模式
};

#endif // CLIENT_H
