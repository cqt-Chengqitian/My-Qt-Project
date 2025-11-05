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
    QString m_strLoginName;
public:
    void recvMsg();
    void clientOffline();
    PDU* readMsg();
    PDU* handleMsg(PDU* pdu);
    void sendMsg(PDU* pdu);
    QByteArray buffer;
    ReqHandler rh;
signals:
    void offLine(MyTcpSocket* mysocket);
};

#endif // MYTCPSOCKET_H
