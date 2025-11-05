#include "server.h"
#include "ui_server.h"
#include "mytcpserver.h"

#include <QFile>

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    loadconfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);
    ui->setupUi(this);
}

Server::~Server()
{
    delete ui;
}

void Server::loadconfig()//加载配置
{
    QFile file(":/server.config");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"打开配置文件失败";
        return;
    }
    QByteArray baDate = file.readAll();
    QString strData = QString(baDate);
    QStringList strList = strData.split("\r\n");
    m_strIP = strList.at(0);
    m_usPort = strList.at(1).toUShort();
    m_strRootPath = strList.at(2);
    qDebug()<<"loadConfig IP"<<m_strIP<<"loadconfig Port"<<m_usPort<<"m_strRootPath:"<<m_strRootPath;
    file.close();
}

Server &Server::getInstance()
{
    static Server instance;
    return instance;
}

