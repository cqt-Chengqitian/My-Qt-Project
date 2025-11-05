#include "reshandler.h"
#include <QMessageBox>
#include "client.h"
#include "uploader.h"
#include <index.h>

ResHandler::ResHandler(PDU* pdu)
{
    this->pdu = pdu;
}

ResHandler::~ResHandler()
{

}

void ResHandler::regist()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(ret));
    if(ret){
        QMessageBox::information(&Client::getInstance(),"注册","注册成功");
    }else{
        QMessageBox::information(&Client::getInstance(),"注册","注册失败");
    }
}

void ResHandler::login()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(ret));
    if(ret){
        Index::getInstance().show();//登录成功显示用户界面
        Client::getInstance().hide();//隐藏登录界面
    }else{
        QMessageBox::information(&Client::getInstance(),"登录","登录失败");
    }
}

void ResHandler::finduser()
{
    char caName[32]={'\0'};
    memcpy(caName,pdu->caData+32,32);//取查找的名字
    int ret;
    memcpy(&ret,pdu->caData,sizeof(ret));
    if(ret == 1){
        int ret_add = QMessageBox::information(&Index::getInstance(),"查找用户","用户在线","添加好友","取消");
        if(ret_add == 0){
            PDU* pdu =mkPDU();
            pdu->uiMsgtype = ENUM_MSG_TYPE_ADDFRIENT_REQUEST;
            memcpy(pdu->caData,Client::getInstance().m_strLoginName.toStdString().c_str(),32);
            memcpy(pdu->caData+32,caName,32);
            Client::getInstance().sendMsg(pdu);
        }
    }else if(ret == 0){
        QMessageBox::information(&Index::getInstance(),"查找用户","用户未在线");
    }else if(ret == -1){
        QMessageBox::information(&Index::getInstance(),"查找用户","用户查找失败");
    }else{
        QMessageBox::information(&Index::getInstance(),"查找用户","用户不存在");
    }
}

void ResHandler::addfriend()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(ret));
    if(!ret){
        QMessageBox::information(&Index::getInstance(),"添加好友","不满足条件");
    }
}

void ResHandler::addfriendresend()
{
    char cacurName[32]={'\0'};
    memcpy(cacurName,pdu->caData,32);
    int ret = QMessageBox::question(&Index::getInstance(),"添加好友",QString("是否同意%1的好友请求").arg(cacurName));
    if(ret != QMessageBox::Yes){
        return;
    }
    PDU* respdu = mkPDU();
    memcpy(respdu->caData,pdu->caData,64);
    respdu->uiMsgtype = ENUM_MSG_TYPE_AGREEADDFRIENT_REQUEST;
    Client::getInstance().sendMsg(respdu);
}

void ResHandler::addfriendagree()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(ret));
    if(ret){
        QMessageBox::information(&Index::getInstance(),"添加好友","添加好友成功");
        Index::getInstance().getFriend()->flushfriend();
    }else{
        QMessageBox::information(&Index::getInstance(),"添加好友","添加好友失败");
    }
}

void ResHandler::flushfriend()
{
    QStringList friendlist;
    char caTemp[32];
    for(int i=0;i<pdu->uiMsglen/32;i++){
        memcpy(caTemp,pdu->caMsg+i*32,32);
        friendlist.append(caTemp);
        qDebug()<<"flushfriend"<<caTemp;
    }
    Index::getInstance().getFriend()->updatefriend_Lw(friendlist);
}

void ResHandler::getchat()
{
    Chat* c = Index::getInstance().getFriend()->m_pChat;
    if(c->isHidden()){
        qDebug()<<"展示窗口";
        c->show();
    }
    qDebug()<<"接收消息";
    char caChatname[32] = {'\0'};;
    memcpy(caChatname,pdu->caData,32);
    c->m_strChatname = caChatname;//将当前的消息发送者存起来方便直接回消息
    c->setWindowTitle(caChatname);
    c->updateshow(pdu->caMsg);

}

void ResHandler::mkdir()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(ret));
    if(ret){
        QMessageBox::information(&Index::getInstance(),"创建文件夹","创建文件夹成功");
    }else{
        QMessageBox::information(&Index::getInstance(),"创建文件夹","创建文件夹失败");
    }
}

void ResHandler::flushfile()
{
    int iCount = pdu->uiMsglen/sizeof(FileInfo);
    qDebug()<<"iCount:"<<iCount;

    QList<FileInfo*> pFileList;
    for(int i=0;i<iCount;i++){
        FileInfo* pFileInfo = new FileInfo;
        memcpy(pFileInfo,pdu->caMsg+i*sizeof(FileInfo),sizeof(FileInfo));
        pFileList.append(pFileInfo);
    }
    Index::getInstance().getFile()->updateFile_LW(pFileList);
}

void ResHandler::movefile()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"moveFile ret:"<<ret;
    if(ret){
        Index::getInstance().getFile()->flushFile();
    }else{
        QMessageBox::information(&Index::getInstance(),"移动文件","移动文件失败");
    }
}

void ResHandler::uploadFileInit()
{
    bool ret;
    memcpy(&ret,pdu->caData,sizeof(bool));
    qDebug()<<"uploadFileInit ret:"<<ret;
    if(ret){
        Index::getInstance().getFile()->uploadFile();
    }else{
        QMessageBox::information(&Index::getInstance(),"上传文件","上传文件失败");
    }
}

void ResHandler::uoloadFileData()
{
    Index::getInstance().getFile()->flushFile();
}

void ResHandler::startUpload()
{
    Uploader* uploader = new Uploader(Index::getInstance().getFile()->m_strUploadFilePath);
    uploader->start();
}

