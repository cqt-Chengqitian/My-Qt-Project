#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThreadPool>
#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getInstance();
    ~MyTcpServer();
    QList<MyTcpSocket*> m_tcpSocketList; // 维护的socket列表，表示连接的所有客户端
    void deleteSocket(MyTcpSocket* mysocket); // 删除列表中的socket，表示客户端下线
    void resend(char* tarName,PDU* pdu); // 转发消息
    QThreadPool threadPool; // 线程池
private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer& other)=delete;
    MyTcpServer& operator =(const MyTcpServer&)=delete;
    void incomingConnection(qintptr handle) override;
};

#endif // MYTCPSERVER_H
