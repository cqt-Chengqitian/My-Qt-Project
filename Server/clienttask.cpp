#include "clienttask.h"

#include <QThread>

ClientTask::ClientTask(QObject *parent) : QObject(parent)
{

}

ClientTask::~ClientTask()
{

}

ClientTask::ClientTask(MyTcpSocket *socket)
{
    mySocket = socket;
}

void ClientTask::run()
{
    //在这里关联就会在这个线程执行，因为socket不能跨线程执行
    connect(mySocket,&QTcpSocket::readyRead,mySocket,&MyTcpSocket::recvMsg);
    connect(mySocket,&QTcpSocket::disconnected,mySocket,&MyTcpSocket::clientOffline);//连接下线信号
    mySocket->moveToThread(QThread::currentThread());
}

