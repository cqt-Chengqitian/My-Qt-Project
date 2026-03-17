#include "client.h"
#include "ui_client.h"
#include <QFile>
#include <QDebug>
#include <QHostAddress>
#include "index.h"
#include "protocol.h"
#include <QMessageBox>
#include "reshandler.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    loadConfig(); // 加载配置
    connect(&m_tcpSocket,&QTcpSocket::connected,this,&Client::showConnect);
    connect(&m_tcpSocket,&QTcpSocket::readyRead,this,&Client::recvMsg);
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort); // 判断是否成功，当成功时发出信号
}

Client::~Client()
{
    delete ui;
}

void Client::loadConfig(){
    QFile file(":/client.config");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"打开配置文件失败";
        return;
    }
    QByteArray baDate = file.readAll(); // 读取整个配置文件
    QString strData = QString(baDate);
    QStringList strList = strData.split("\r\n"); // 按行分割
    m_strIP = strList.at(0);
    m_usPort = strList.at(1).toUShort();
    m_strRootPath = strList.at(2);
    qDebug()<<"IP: "<<m_strIP<<"  Port:"<<m_usPort<<"  m_strRootPath: "<<m_strRootPath;
}

void Client::showConnect()
{
    qDebug()<<"连接服务器成功";
}

Client &Client::getInstance()
{
    static Client instance;
    return instance;
}

void Client::sendMsg(PDU *pdu)
{
    if(pdu == NULL){
        return;
    }
    m_tcpSocket.write((char*)pdu,pdu->uiPDUlen);

    qDebug()<<"sendMsg  uiMsgtype: "<<pdu->uiMsgtype
           <<"  uiPDUlen: "<<pdu->uiPDUlen
          <<"  caData: "<<pdu->caData
         <<"  caData: "<<pdu->caData+32
         <<"  caMsg: "<<pdu->caMsg;

    // 发送完消息之后就回收PDU
    free(pdu);
    pdu = NULL;
}

void Client::recvMsg()
{
    qDebug()<<"接收消息长度:"<<m_tcpSocket.bytesAvailable();
    // 读取Socket中当前所有可用数据
    QByteArray data = m_tcpSocket.readAll();
    // 追加到缓冲区，暂存未处理的不完整数据，解决半包
    buffer.append(data);
    // 循环解析缓冲区中的完整PDU，解决粘包，至少能容纳PDU头部才尝试解析
    while(buffer.size()>=int(sizeof(PDU))){
        // 把缓冲区数据转为PDU指针
        PDU* pdu = (PDU*)buffer.data();
        // 合法性校验：避免非法 PDU 破坏内存
        // 校验1：PDU 总长度不能小于头部长度
        if(pdu->uiPDUlen < sizeof(PDU)){
            qWarning() << "非法PDU：uiPDUlen小于头部长度，丢弃错误数据";
            buffer.remove(0, sizeof(PDU)); // 只删除错误的头部，保留后续数据
            continue;
        }
        // 判断是否为半包
        if(buffer.size()<int(pdu->uiPDUlen)){
            break; // 半包则跳出循环，等后续数据补充
        }
        // 处理完整的PDU包，分发到业务逻辑
        handleMsg(pdu);
        // 移除已处理的PDU数据，继续处理剩余的粘包数据
        buffer.remove(0,pdu->uiPDUlen);
    }
}

void Client::handleMsg(PDU* pdu)
{
    qDebug()<<"recvMsg  uiMsgtype: "<<pdu->uiMsgtype
           <<"  uiPDUlen: "<<pdu->uiPDUlen
          <<"  caData: "<<pdu->caData
         <<"  caData: "<<pdu->caData+32
         <<"  caMsg: "<<pdu->caMsg << endl;

    ResHandler rh(pdu);
    switch(pdu->uiMsgtype){
    case ENUM_MSG_TYPE_REGIST_RESPOND:{
        rh.regist();
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:{
        rh.login();
        break;
    }
    case ENUM_MSG_TYPE_FINDUSER_RESPOND:{
        rh.finduser();
        break;
    }
    case ENUM_MSG_TYPE_ADDFRIENT_RESPOND:{
        rh.addfriend();
        break;
    }
    case ENUM_MSG_TYPE_ADDFRIENT_REQUEST:{ // 因为服务器判断可以加好友就直接将消息转发了所以消息类型为addfriend
        rh.addfriendresend();
        break;
    }
    case ENUM_MSG_TYPE_AGREEADDFRIENT_RESPOND:{
        rh.addfriendagree();
        break;
    }
    case ENUM_MSG_TYPE_FLUSHFRIENT_RESPOND:{
        rh.flushfriend();
        break;
    }
    case ENUM_MSG_TYPE_CHAT_REQUEST:{ // 直接转发了，还是requst
        rh.getchat();
        break;
    }case ENUM_MSG_TYPE_MKDIR_RESPOND:{
        rh.mkdir();
        break;
    }case ENUM_MSG_TYPE_FLUSHFILE_RESPOND:{
        rh.flushfile();
        break;
    }case ENUM_MSG_TYPE_MOVEFILE_RESPOND:{
        rh.movefile();
        break;
    }case ENUM_MSG_TYPE_UPLOADINIT_RESPOND:{
        rh.uploadFileInit();
        break;
    }
    case ENUM_MSG_TYPE_UPLOADDATA_RESPOND:{
        rh.uploadFileData();
        break;
    }
    default:
        break;
    }
}

void Client::on_regist_Pb_clicked()
{
    QString strName = ui->name_Le->text(); // 从输入框获取用户名与密码
    QString strPwd = ui->pwd_Le->text();
    if(strName.toStdString().size()>32||strName.isEmpty()||strPwd.toStdString().size()>32||strPwd.isEmpty()){
        QMessageBox::warning(this,"注册","用户名或密码非法");
        return;
    }
    PDU* pdu = mkPDU();
    pdu->uiMsgtype = ENUM_MSG_TYPE_REGIST_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32); // +32是偏移
    sendMsg(pdu);
}

void Client::on_login_Pb_clicked()
{
    QString strName = ui->name_Le->text(); // 从输入框获取用户名与密码
    QString strPwd = ui->pwd_Le->text();
    if(strName.toStdString().size()>32||strName.isEmpty()||strPwd.toStdString().size()>32||strPwd.isEmpty()){
        QMessageBox::warning(this,"登录","用户名或密码非法");
        return;
    }
    PDU* pdu = mkPDU();
    pdu->uiMsgtype = ENUM_MSG_TYPE_LOGIN_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,strPwd.toStdString().c_str(),32); // +32是偏移
    m_strLoginName = strName;
    sendMsg(pdu);
}
