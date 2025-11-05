#include "reqhandler.h"
#include "operatedb.h"
#include "mytcpserver.h"
#include "server.h"
#include <QDebug>
#include <QDir>

ReqHandler::ReqHandler()
{
    this->pdu = pdu;
}

ReqHandler::~ReqHandler()
{

}

PDU *ReqHandler::regist()
{
    char caName[32]={'\0'};
    char caPwd[32]={'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handleRegist(caName,caPwd);
    qDebug()<<"regist ret:"<<ret;
    if(ret){//当注册成功之后创建用户对应的文件夹
        QDir dir;
        dir.mkdir(QString("%1/%2").arg(Server::getInstance().m_strRootPath).arg(caName));
    }
    PDU* respdu = mkPDU();
    respdu->uiMsgtype = ENUM_MSG_TYPE_REGIST_RESPOND;//类型为响应
    memcpy(respdu->caData,&ret,sizeof(ret));
    return respdu;
}

PDU *ReqHandler::login(QString& strLoginname)
{
    char caName[32]={'\0'};
    char caPwd[32]={'\0'};
    memcpy(caName,pdu->caData,32);
    memcpy(caPwd,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handleLogin(caName,caPwd);
    qDebug()<<"login ret:"<<ret;
    if(ret){
        strLoginname = caName;
    }
    PDU* respdu = mkPDU();
    respdu->uiMsgtype = ENUM_MSG_TYPE_LOGIN_RESPOND;//类型为响应
    memcpy(respdu->caData,&ret,sizeof(ret));
    return respdu;
}

PDU *ReqHandler::finduser()
{
    char caName[32]={'\0'};
    memcpy(caName,pdu->caData,32);
    int ret = OperateDB::getInstance().handleFinduser(caName);
    qDebug()<<"finduser ret:"<<ret;
    PDU* respdu = mkPDU();
    respdu->uiMsgtype = ENUM_MSG_TYPE_FINDUSER_RESPOND;//类型为响应
    memcpy(respdu->caData,&ret,sizeof(ret));
    memcpy(respdu->caData+32,caName ,32);
    return respdu;
}

PDU *ReqHandler::addfriend()
{
    char cacurName[32]={'\0'};
    char catarName[32]={'\0'};
    memcpy(cacurName,pdu->caData,32);
    memcpy(catarName,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().handlAddfriend(cacurName,catarName);
    qDebug()<<"addFriend ret"<<ret;
    if(ret){
        MyTcpServer::getInstance().resend(catarName,pdu);
    }
    PDU* respdu = mkPDU();
    respdu->uiMsgtype = ENUM_MSG_TYPE_ADDFRIENT_RESPOND;//类型为响应
    memcpy(respdu->caData,&ret,sizeof(ret));
    return respdu;
}

PDU *ReqHandler::agreeaddfriend()
{
    char cacurName[32]={'\0'};
    char catarName[32]={'\0'};
    memcpy(cacurName,pdu->caData,32);
    memcpy(catarName,pdu->caData+32,32);
    bool ret = OperateDB::getInstance().agreeHandleaddfriend(cacurName,catarName);
    qDebug()<<"agreeAddFriend ret"<<ret;
    PDU* respdu = mkPDU();
    respdu->uiMsgtype = ENUM_MSG_TYPE_AGREEADDFRIENT_RESPOND;//类型为响应
    memcpy(respdu->caData,&ret,sizeof(ret));
    MyTcpServer::getInstance().resend(cacurName,respdu);
    return respdu;
}

PDU *ReqHandler::flushfriend()
{
    char cacurName[32]={'\0'};
    memcpy(cacurName,pdu->caData,32);
    QStringList result = OperateDB::getInstance().handleFlushfriend(cacurName);
    PDU* respdu = mkPDU(result.size()*32);
    respdu->uiMsgtype = ENUM_MSG_TYPE_FLUSHFRIENT_RESPOND;
    for(int i=0;i<result.size();i++){
        memcpy(respdu->caMsg+i*32,result.at(i).toStdString().c_str(),32);
        qDebug()<<result.at(i).toStdString().c_str();
    }
    qDebug()<<"flushfriend"<<respdu->caMsg;
    return respdu;
}

PDU *ReqHandler::chat()
{
    char catarName[32]={'\0'};
    memcpy(catarName,pdu->caData+32,32);
    MyTcpServer::getInstance().resend(catarName,pdu);
    qDebug()<<"转发消息";
    return NULL;//不响应，直接给目标客户端
}

PDU *ReqHandler::mkdir()
{
    char caNewDir[32]={'\0'};
    memcpy(caNewDir,pdu->caData,32);
    QString strCurPath = pdu->caMsg;
    QDir dir;
    bool ret = true;
    if(!dir.exists(strCurPath)){
        ret = false;
    }else{
        QString strNewPath = QString("%1/%2").arg(strCurPath).arg(caNewDir);
        if(dir.exists(strNewPath)){
            ret = false;
        }else{
            ret = dir.mkdir(strNewPath);
        }
    }
    PDU* respdu = mkPDU();
    respdu->uiMsgtype = ENUM_MSG_TYPE_MKDIR_RESPOND;//类型为响应
    memcpy(respdu->caData,&ret,sizeof(ret));
    return respdu;
}

PDU *ReqHandler::flushfile()
{
    QString strCurPath = pdu->caMsg;
    QDir dir(strCurPath);
    QFileInfoList fileInfoList = dir.entryInfoList();
    int iFileCount = fileInfoList.size();
    PDU* respdu = mkPDU(sizeof(FileInfo)*iFileCount-2);//筛去.与..
    respdu->uiMsgtype = ENUM_MSG_TYPE_FLUSHFILE_RESPOND;
    int t = 0;//避免continue造成的位置偏移
    for(int i=0;i<fileInfoList.size();i++){
        if(fileInfoList.at(i).fileName()=="."||fileInfoList.at(i).fileName()==".."){
            continue;
        }
        FileInfo* pFileInfo = (FileInfo*)respdu->caMsg+t++;
        memcpy(pFileInfo->caName,fileInfoList.at(i).fileName().toStdString().c_str(),32);
        if(fileInfoList.at(i).isDir()){
            pFileInfo->iType = 0;
        }else{
            pFileInfo->iType = 1;
        }
    }
    return respdu;
}

PDU *ReqHandler::movefile()
{
    int iSrcLen = 0;
    int iTarLen = 0;
    memcpy(&iSrcLen,pdu->caData,sizeof(int));
    memcpy(&iTarLen,pdu->caData+32,sizeof(int));
    char* pSrcPath = new char[iSrcLen+1];
    char* pTarPath = new char[iTarLen+1];
    memset(pSrcPath,'\0',iSrcLen+1);
    memset(pTarPath,'\0',iTarLen+1);
    memcpy(pSrcPath,pdu->caMsg,iSrcLen);
    memcpy(pTarPath,pdu->caMsg+iSrcLen,iTarLen);
    qDebug()<<"pSrcPath:"<<pSrcPath
           <<"pTarPath:"<<pTarPath;
    bool ret = QFile::rename(pSrcPath,pTarPath);
    qDebug()<<"moveFile ret:"<<ret;
    PDU* respdu = mkPDU();
    respdu->uiMsgtype = ENUM_MSG_TYPE_MOVEFILE_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(ret));
    delete[] pSrcPath;
    delete[] pTarPath;
    pSrcPath = NULL;
    pTarPath = NULL;
    return respdu;
}

PDU *ReqHandler::uploadFileInit()
{
    char caFileName[32] = {'\0'};
    memcpy(caFileName,pdu->caData,32);
    m_iUploadFileSize = 0;
    memcpy(&m_iUploadFileSize,pdu->caData+32,sizeof(qint64));
    m_fUploadFile.setFileName(QString("%1/%2").arg(pdu->caMsg).arg(caFileName));
    bool ret = m_fUploadFile.open(QIODevice::WriteOnly);//创建并打开文件
    qDebug()<<"m_fUploadFile ret:"<<ret;
    m_iUploadReceived = 0;
    PDU* respdu = mkPDU();
    respdu->uiMsgtype = ENUM_MSG_TYPE_UPLOADINIT_RESPOND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *ReqHandler::uploadFileData()
{
   m_fUploadFile.write(pdu->caMsg,pdu->uiMsglen);
   m_iUploadReceived += pdu->uiMsglen;
   if(m_iUploadReceived < m_iUploadFileSize){
       return NULL;
   }
   m_fUploadFile.close();//传完后关闭文件
   PDU* respdu = mkPDU();
   respdu->uiMsgtype = ENUM_MSG_TYPE_UPLOADDATA_RESPOND;
   return respdu;
}
