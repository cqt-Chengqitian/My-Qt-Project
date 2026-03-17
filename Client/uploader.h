#ifndef UPLOADER_H
#define UPLOADER_H

#include "protocol.h"

#include <QObject>

class Uploader : public QObject
{
    Q_OBJECT
public:
    explicit Uploader(QObject *parent = 0);
    ~Uploader();
    void start();

    QString m_strUploadFilePath; // 上传文件路径
    Uploader(QString strFilePath);

public slots:
    void uploadFile();

signals:
    void erroSignal(QString error); // 错误信号
    void sendPDU(PDU* pdu); // 发送消息
    void finshed(); // 线程结束的信号
};

#endif // UPLOADER_H
