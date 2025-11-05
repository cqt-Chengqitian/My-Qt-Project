#include "client.h"
#include "friend.h"
#include "protocol.h"
#include "ui_friend.h"

#include <QInputDialog>
#include <QMessageBox>

Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    flushfriend();
    m_pChat = new Chat;
}

Friend::~Friend()
{
    delete ui;
    delete m_pChat;
}

void Friend::flushfriend()
{
    QString strName = Client::getInstance().m_strLoginName;
    PDU* pdu = mkPDU();
    pdu->uiMsgtype = ENUM_MSG_TYPE_FLUSHFRIENT_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    Client::getInstance().sendMsg(pdu);
}

void Friend::updatefriend_Lw(QStringList friendlist)
{
    ui->friend_Lw->clear();//刷新前先清空
    ui->friend_Lw->addItems(friendlist);
}

void Friend::on_find_Pb_clicked()
{
    QString strName = QInputDialog::getText(this,"搜索","搜索对象");
    if(strName.toStdString().size()>32||strName.isEmpty()){
        QMessageBox::warning(this,"搜索","用户名非法");
        return;
    }
    PDU* pdu = mkPDU();
    pdu->uiMsgtype = ENUM_MSG_TYPE_FINDUSER_REQUEST;
    memcpy(pdu->caData,strName.toStdString().c_str(),32);
    Client::getInstance().m_tcpSocket.write((char*)pdu,pdu->uiPDUlen);
    Client::getInstance().sendMsg(pdu);
}

void Friend::on_flush_Pb_clicked()
{
    flushfriend();
}

void Friend::on_chat_Pb_clicked()
{
    QListWidgetItem* pItem = ui->friend_Lw->currentItem();//列表框中的文字
    if(!pItem){
        return;
    }
    if(m_pChat->isHidden()){
        m_pChat->show();
    }
    m_pChat->m_strChatname = pItem->text();//保存是跟谁聊天
    m_pChat->setWindowTitle(pItem->text());//窗口改为对方名字
}
