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

    QString m_strUploadFilePath;
    Uploader(QString strFilePath);

public slots:
    void uploadFile();

signals:
    void erroSignal(QString error);
    void sendPDU(PDU* pdu);
    void finshed();//线程结束的信号
public slots:
};

#endif // UPLOADER_H
