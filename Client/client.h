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
    QTcpSocket m_tcpSocket; // 使用得的socket
    QString m_strIP; // 客户端的IP地址
    quint16 m_usPort; // 使用的端口号
    QString m_strLoginName; // 登录名
    QString m_strRootPath; // 根目录，用于文件传输
    QByteArray buffer; // 接收消息的缓冲区

    ~Client();
    void loadConfig(); // 加载基础配置
    void showConnect();
    static Client& getInstance(); // 单例模式获取client对象
    void sendMsg(PDU* pdu); // 发送消息
    void recvMsg(); // 接收消息
    void handleMsg(PDU* pdu); // 处理消息

private slots:
    void on_regist_Pb_clicked(); // 点击注册

    void on_login_Pb_clicked(); // 点击登录

private:
    Ui::Client *ui;
    explicit Client(QWidget *parent = 0);
    Client(const Client& other) = delete;
    Client& operator =(const Client*) = delete;//防止所有私自创建对象的方式，实现单例模式
};

#endif // CLIENT_H
