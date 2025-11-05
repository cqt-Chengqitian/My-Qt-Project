#include "protocol.h"


PDU *mkPDU(uint uiMsglen)
{
    uint uiPDUlen = sizeof(PDU)+uiMsglen;
    PDU* pdu = (PDU*)malloc(uiPDUlen);
    if(pdu == NULL){
        exit(1);
    }
    memset(pdu,0,uiPDUlen);//先清空内容
    pdu->uiMsglen=uiMsglen;//初始化长度
    pdu->uiPDUlen=uiPDUlen;
    return pdu;
}
