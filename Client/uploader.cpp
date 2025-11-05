#include "protocol.h"
#include "uploader.h"

#include <QThread>
#include <qfile.h>

Uploader::Uploader(QObject *parent) : QObject(parent)
{

}

Uploader::~Uploader()
{

}

void Uploader::start()
{
    //通过start函数与线程建立起联系
    QThread*thread = new QThread;
    this->moveToThread(thread);
    connect(thread,&QThread::started,this,&Uploader::uploadFile);//当前线程开始执行后调用上传文件，开始时建立联系
    connect(this,&Uploader::finshed,thread,&QThread::quit);//上传结束发出信号，线程退出
    connect(thread,&QThread::finished,thread,&QThread::deleteLater);// 结束后移除线程
    thread->start();
}

void Uploader::uploadFile()
{
    //创建QFile对象并打开文件
    QFile file(m_strUploadFilePath);
    if(!file.open(QIODevice::ReadOnly)){
        emit erroSignal("上传文件失败");
        emit finshed();
        return;
    }

    //构建pdu，循环读取文件内容
    while(true){
        PDU* pdu = mkPDU(4096);
        pdu->uiMsgtype = ENUM_MSG_TYPE_UPLOADDATA_REQUEST;
        qint64 ret = file.read(pdu->caMsg,4096);
        if(ret == 0){
            break;
        }
        if(ret < 0){
            emit erroSignal("读取文件失败");
            break;
        }
        pdu->uiMsglen = ret;
        pdu->uiPDUlen = ret + sizeof(PDU);
        //不能使用sendMsg，否则会在第一个循环结束后就直接把pdu给释放，而实际上pdu被循环使用
        emit sendPDU(pdu);
    }
    file.close();
    emit finshed();
}

Uploader::Uploader(QString strFilePath)
{
    m_strUploadFilePath = strFilePath;
}



