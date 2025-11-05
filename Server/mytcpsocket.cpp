#include "mytcpsocket.h"
#include "protocol.h"
#include "operatedb.h"
#include "reqhandler.h"

MyTcpSocket::MyTcpSocket()
{

}

MyTcpSocket::~MyTcpSocket()
{

}

void MyTcpSocket::recvMsg()
{
    QByteArray data = this->readAll();
    buffer.append(data);//定义成员变量存半包数据
    while(buffer.size()>=int(sizeof(PDU))){//判断是否是一个完整包
        PDU* pdu = (PDU*)buffer.data();
        if(buffer.size()<int(pdu->uiPDUlen)){//处理半包
            break;
        }
        //处理完整包
        PDU* respdu = handleMsg(pdu);//pdu不是用mkPDU申请来的不能够释放
        sendMsg(respdu);
        //处理粘包
        buffer.remove(0,pdu->uiPDUlen);//移除前半部分的包,而后处理后半部分的包
    }
}

void MyTcpSocket::clientOffline()//下线需要知道是哪个用户，所以需要把登录的用户信息存起来
{
    qDebug()<<"下线用户:"<<m_strLoginName.toStdString().c_str();
    OperateDB::getInstance().handleOffline(m_strLoginName.toStdString().c_str());
    emit offLine(this);
}

PDU *MyTcpSocket::readMsg()//处理粘包半包后不再使用
{
    qDebug()<<"接收消息长度:"<<this->bytesAvailable();
    uint uiPDUlen = 0;
    this->read((char*)&uiPDUlen,sizeof(uint));//读出协议总长度
    uint uiMsglen = uiPDUlen - sizeof(PDU);
    PDU* pdu = mkPDU(uiMsglen);
    pdu->uiPDUlen = uiPDUlen;
    this->read((char*)pdu+sizeof(uint),uiPDUlen-sizeof(uint));//因为socket中已经没有uiPDUlen了,所以可以直接把长度偏移回去
    qDebug()<<"recvMsg uiMsgtype:"<<pdu->uiMsgtype
           <<"uiPDUlen:"<<pdu->uiPDUlen
          <<"caData:"<<pdu->caData
         <<"caData:"<<pdu->caData+32
         <<"caMsg:"<<pdu->caMsg;
    return pdu;
}

PDU* MyTcpSocket::handleMsg(PDU* pdu)
{
    if(pdu ==NULL){
        return NULL;
    }
    qDebug()<<"接收消息长度:"<<this->bytesAvailable();
    qDebug()<<"recvMsg uiMsgtype:"<<pdu->uiMsgtype
           <<"uiPDUlen:"<<pdu->uiPDUlen
          <<"caData:"<<pdu->caData
         <<"caData:"<<pdu->caData+32
         <<"caMsg:"<<pdu->caMsg;
    PDU* respdu = NULL;
    rh.pdu = pdu;
    switch(pdu->uiMsgtype){
    case ENUM_MSG_TYPE_REGIST_REQUEST:{//接收到注册信息
        respdu = rh.regist();
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_REQUEST:{//接收到登录信息
        respdu = rh.login(m_strLoginName);
        break;
    }
    case ENUM_MSG_TYPE_FINDUSER_REQUEST:{
        respdu = rh.finduser();
        break;
    }
    case ENUM_MSG_TYPE_ADDFRIENT_REQUEST:{
        respdu = rh.addfriend();
        break;
    }
    case ENUM_MSG_TYPE_AGREEADDFRIENT_REQUEST:{
        respdu = rh.agreeaddfriend();
        break;
    }
    case ENUM_MSG_TYPE_FLUSHFRIENT_REQUEST:{
        respdu = rh.flushfriend();
        break;
    }
    case ENUM_MSG_TYPE_CHAT_REQUEST:{
        respdu = rh.chat();
        break;
    }
    case ENUM_MSG_TYPE_MKDIR_REQUEST:{
        respdu = rh.mkdir();
        break;
    }
    case ENUM_MSG_TYPE_FLUSHFILE_REQUEST:{
        respdu = rh.flushfile();
        break;
    }
    case ENUM_MSG_TYPE_MOVEFILE_REQUEST:{
        respdu = rh.movefile();
        break;
    }
    case ENUM_MSG_TYPE_UPLOADINIT_REQUEST:{
        respdu = rh.uploadFileInit();
        break;
    }
    case ENUM_MSG_TYPE_UPLOADDATA_REQUEST:{
        respdu = rh.uploadFileData();
        break;
    }
    default:
        break;
    }
    free(pdu);
    pdu = NULL;
    return respdu;
}

void MyTcpSocket::sendMsg(PDU *pdu)
{
    if(pdu ==NULL){
        return;
    }
    this->write((char*)pdu,pdu->uiPDUlen);
    qDebug()<<"regist uiMsgtype:"<<pdu->uiMsgtype//打印
           <<"uiPDUlen:"<<pdu->uiPDUlen
          <<"caData:"<<pdu->caData
         <<"caData:"<<pdu->caData+32
         <<"caMsg:"<<pdu->caMsg;
}

