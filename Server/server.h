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
    QTcpSocket m_tcpSocket;
    QString m_strIP;
    quint16 m_usPort;
    QString m_strRootPath;

    ~Server();
    void loadconfig();
    static Server& getInstance();

private:
    Ui::Server *ui;
    explicit Server(QWidget *parent = 0);
};

#endif // SERVER_H
