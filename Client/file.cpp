#include "file.h"
#include "ui_file.h"
#include "client.h"
#include "uploader.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QFileDialog>


File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_strUserPath = QString("%1/%2").arg(Client::getInstance().m_strRootPath).arg(Client::getInstance().m_strLoginName);
    m_strCurPath = m_strUserPath;
    flushFile();//在登录的同时有刷新好友与刷新文件的操作，导出了粘包半包的问题
}

File::~File()
{
    delete ui;
}

void File::flushFile()
{
    PDU* pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgtype = ENUM_MSG_TYPE_FLUSHFILE_REQUEST;
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::updateFile_LW(QList<FileInfo *> pFileList)
{
    foreach(FileInfo* pFileInfo,m_pFileInfoList){//当改变路径的时候把之前的目录的内容给删除
        delete pFileInfo;
    }
    m_pFileInfoList = pFileList;
    ui->listWidget->clear();//刷新之前清空
    foreach(FileInfo* pFileInfo, pFileList){
        QListWidgetItem* pItem = new QListWidgetItem;
        if(pFileInfo->iType == 0){
            pItem->setIcon(QIcon(QPixmap(":/0.png")));
        }else if(pFileInfo->iType == 1){
            pItem->setIcon(QIcon(QPixmap(":/1.png")));
        }
        pItem->setText(pFileInfo->caName);
        ui->listWidget->addItem(pItem);
    }
}

void File::uploadFile()
{
    Uploader* uploader = new Uploader(m_strUploadFilePath);
    //控件与socket都不能夸线程，所以通过信号连接
    connect(uploader,&Uploader::erroSignal,this,&File::errorSlot);//报错的弹窗
    connect(uploader,&Uploader::sendPDU,&Client::getInstance(),&Client::sendMsg);//发送PDU的socket
    uploader->start();
}

void File::errorSlot(QStirng error)
{
    QMessageBox::information(this,"Tips","error")
}

void File::on_mkdir_Pb_clicked()
{
    QString strNewDir = QInputDialog::getText(this,"新建文件夹","新建文件夹名:");
    if(strNewDir.isEmpty()||strNewDir.toStdString().size()>32){
        QMessageBox::information(this,"新建文件夹","长度非法");
        return;
    }

    PDU* pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgtype = ENUM_MSG_TYPE_MKDIR_REQUEST;
    memcpy(pdu->caData,strNewDir.toStdString().c_str(),32);
    memcpy(pdu->caMsg, m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}

void File::on_flush_Pb_clicked()
{
    flushFile();
}

void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString strDirName = item->text();
    foreach(FileInfo* pFileInfo,m_pFileInfoList){
        if(pFileInfo->caName == strDirName&&pFileInfo->iType == 1){
            QMessageBox::warning(this,"Tips","文件不支持查看");
            return;
        }
    }
    m_strCurPath = m_strUserPath + "/" + strDirName;
    flushFile();
}

void File::on_return_Pb_clicked()
{
    if(m_strCurPath == m_strUserPath){
        return;
    }
    int index = m_strCurPath.lastIndexOf('/');//找最后一个斜杠的位置
    m_strCurPath.remove(index,m_strCurPath.size()-index);
    flushFile();
}



void File::on_remove_Pb_clicked()
{
    if(ui->remove_Pb->text() =="移动文件"){//确认是否是第一次点击
        QListWidgetItem* pItem = ui->listWidget->currentItem();
        if(pItem == NULL){
            QMessageBox::warning(this,"移动文件","请选择要移动的文件");
            return;
        }
        QMessageBox::information(this,"移动文件","请选择要移动到的目录");
        ui->remove_Pb->setText("确认/取消");
        //保存移动的文件名与完整路径
        m_strMoveFileName = pItem->text();
        m_strMoveFilePath = m_strCurPath+'/'+m_strMoveFileName;//完整路径
        return;
    }
    //拼接要移动到的完整路径
    QString strTarPath = m_strCurPath+'/'+m_strMoveFileName;
    ui->remove_Pb->setText("移动文件");
    int ret = QMessageBox::information(this,"移动文件",QString("是否移动到%1").arg(m_strCurPath),"确认","取消");
    if(ret != 0){
        return;
    }
    //将两个路径放入，将需要长度放入caData
    int iSrcLen = m_strMoveFilePath.toStdString().size();
    int iTarLen = strTarPath.toStdString().size();
    PDU* pdu = mkPDU(iSrcLen+iTarLen+1);
    pdu->uiMsgtype = ENUM_MSG_TYPE_MOVEFILE_REQUEST;
    memcpy(pdu->caData,&iSrcLen,32);
    memcpy(pdu->caData+32,&iTarLen,32);
    memcpy(pdu->caMsg,m_strMoveFilePath.toStdString().c_str(),iSrcLen);
    memcpy(pdu->caMsg+iSrcLen,strTarPath.toStdString().c_str(),iTarLen);
    qDebug()<<"m_strMoveFilePath:"<<m_strMoveFilePath
           <<"strTarPath:"<<strTarPath;
    Client::getInstance().sendMsg(pdu);
}

void File::on_upload_Pb_clicked()
{
    m_strUploadFilePath = QFileDialog::getOpenFileName();//打开选择文件的弹窗,获取打开文件的名字
    qDebug()<<"上传文件路径:"<<m_strUploadFilePath;
    if(m_strUploadFilePath.isEmpty()){
        return;
    }
    int index = m_strUploadFilePath.lastIndexOf('/');//用最后一个斜杠找文件名
    QString strFileName = m_strUploadFilePath.right(m_strUploadFilePath.size()-index-1);//因为index是下标,所以得在-1
    QFile file(m_strUploadFilePath);
    qint64 iFileSize = file.size();//通过qfile获得文件大小
    PDU* pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgtype = ENUM_MSG_TYPE_UPLOADINIT_REQUEST;
    memcpy(pdu->caData,strFileName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&iFileSize,32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());
    Client::getInstance().sendMsg(pdu);
}
