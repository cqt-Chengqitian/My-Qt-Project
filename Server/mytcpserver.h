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
    QList<MyTcpSocket*> m_tcpSocketList;
    void deleteSocket(MyTcpSocket* mysocket);
    void resend(char* tarName,PDU* pdu);
    QThreadPool threadPool;//线程池
private:
    MyTcpServer();
    MyTcpServer(const MyTcpServer& other)=delete;
    MyTcpServer& operator =(const MyTcpServer&)=delete;
    void incomingConnection(qintptr handle) override;
};

#endif // MYTCPSERVER_H
