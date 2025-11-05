#ifndef REQHANDLER_H
#define REQHANDLER_H

#include "protocol.h"
#include <QFile>
#include <QObject>

class ReqHandler
{
public:
    ReqHandler();
    ~ReqHandler();
    PDU* pdu;
    QFile m_fUploadFile;
    qint64 m_iUploadFileSize;//上传的文件大小
    qint64 m_iUploadReceived;//已经接收的文件大小
    PDU* regist();
    PDU* login(QString& strLoginname);
    PDU* finduser();
    PDU* addfriend();
    PDU* agreeaddfriend();
    PDU* flushfriend();
    PDU* chat();
    PDU* mkdir();
    PDU* flushfile();
    PDU* movefile();
    PDU* uploadFileInit();
    PDU* uploadFileData();
};

#endif // REQHANDLER_H
