#include "chat.h"
#include "client.h"
#include "protocol.h"
#include "ui_chat.h"

Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::updateshow(QString strMsg)
{
    ui->show_Te->append(strMsg);
}

void Chat::on_send_Pb_clicked()
{
    QString strMsg = ui->input_Le->text();
    if(strMsg.isEmpty()){
        return;
    }
    ui->input_Le->clear();//发送后清空
    PDU* pdu = mkPDU(strMsg.toStdString().size()+1);
    pdu->uiMsgtype = ENUM_MSG_TYPE_CHAT_REQUEST;
    memcpy(pdu->caData,Client::getInstance().m_strLoginName.toStdString().c_str(),32);//发送人与接收人
    memcpy(pdu->caData+32,m_strChatname.toStdString().c_str(),32);
    memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}
