#include "clienttask.h"
#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{

}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"新客户端连接";
    MyTcpSocket* pSocket = new MyTcpSocket;//一个服务器对应多个客户端，用list存起来
    pSocket->setSocketDescriptor(handle);
    m_tcpSocketList.append(pSocket);
    connect(pSocket,&MyTcpSocket::offLine,this,&MyTcpServer::deleteSocket);//连接删除socket的槽函数

    //加入线程池
    ClientTask* task = new ClientTask(pSocket);//pSocket不是线程池中的线程，是主线程的，而后移交给线程池
    threadPool.start(task);
}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

MyTcpServer::~MyTcpServer()
{
    threadPool.setMaxThreadCount(6);//设置最大线程数
}

void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    m_tcpSocketList.removeOne(mysocket);
    mysocket->deleteLater();//因为之前是new出来的，但还需要断开连接的过程，所以需要延迟释放
    mysocket = NULL;
    //测试
    for(int i = 0 ;i<m_tcpSocketList.size();i++){
        qDebug()<<m_tcpSocketList.at(i)->m_strLoginName;
    }
}

void MyTcpServer::resend(char *tarName, PDU *pdu)
{
    if(tarName ==NULL || pdu == NULL){
        return;
    }
    foreach (MyTcpSocket* it, m_tcpSocketList) {
        if(it->m_strLoginName == tarName){
            it->sendMsg(pdu);
            it->write((char*)pdu,pdu->uiPDUlen);
            qDebug()<<"resend uiMsgtype:"<<pdu->uiMsgtype//打印
                   <<"uiPDUlen:"<<pdu->uiPDUlen
                  <<"caData:"<<pdu->caData
                 <<"caData:"<<pdu->caData+32
                 <<"caMsg:"<<pdu->caMsg;
            break;
        }
    }
}

