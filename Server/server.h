#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    QTcpSocket m_tcpSocket; // 使用的socket
    QString m_strIP; // 配置的IP地址
    quint16 m_usPort; // 配置的端口号
    QString m_strRootPath; // 根路径

    ~Server();
    void loadconfig(); // 加载配置
    static Server& getInstance();

private:
    Ui::Server *ui;
    explicit Server(QWidget *parent = 0);
};

#endif // SERVER_H
