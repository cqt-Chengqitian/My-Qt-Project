#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include "protocol.h"
#include "reqhandler.h"

#include <QObject>
#include <QTcpSocket>

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    ~MyTcpSocket();
    QString m_strLoginName; // 登录的用户名
public:
    void recvMsg(); // 接收消息
    void clientOffline(); // 客户端下线
    PDU* handleMsg(PDU* pdu); // 处理消息，转到reqhandler类中处理
    void sendMsg(PDU* pdu); // 发送消息
    QByteArray buffer; // 维护的缓冲区
    ReqHandler rh;
signals:
    void offLine(MyTcpSocket* mysocket);
};

#endif // MYTCPSOCKET_H
