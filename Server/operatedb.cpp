#include "operatedb.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

OperateDB::OperateDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}
OperateDB::~OperateDB()
{
    m_db.close();//使用完需要关闭
}

OperateDB &OperateDB::getInstance()
{
    static OperateDB instance;
    return instance;
}

void OperateDB::connect()
{
    m_db.setHostName("localhost");
    m_db.setPort(3306);
    m_db.setUserName("root");
    m_db.setPassword("root");
    m_db.setDatabaseName("mydatabase");
    if(m_db.open()){
        qDebug()<<"数据库连接成功";
    }else{
        qDebug()<<"数据库连接失败"<<m_db.lastError().text();
    }
}

bool OperateDB::handleRegist(const char *name, const char *pwd)
{
    if(name == NULL||pwd == NULL){
        return false;
    }
    QString sql = QString("select * from user_info where name = '%1'").arg(name);//查询用户是否已存在
    qDebug()<<"检查用户是否存在"<<sql;
    QSqlQuery q;//通过该对象执行sql语句
    if(!q.exec(sql)||q.next()){//看执行结果并看有没有这个用户
        return false;
    }
    sql = QString("insert into user_info(name,pwd) values('%1','%2')").arg(name).arg(pwd);
    qDebug()<<"添加用户"<<sql;
    return q.exec(sql);
}

bool OperateDB::handleLogin(const char *name, const char *pwd)
{
    if(name == NULL||pwd == NULL){
        return false;
    }
    QString sql = QString("select * from user_info where name = '%1' and pwd = '%2'").arg(name).arg(pwd);//查询用户与密码是否正确
    qDebug()<<"检查用户是否存在"<<sql;
    QSqlQuery q;//通过该对象执行sql语句
    if(!q.exec(sql)||!q.next()){//看有没有执行结果
        return false;
    }
    sql = QString("update user_info set online = 1 where name = '%1' and pwd = '%2'").arg(name).arg(pwd);//更新用户在线
    qDebug()<<"更新用户在线"<<sql;
    return q.exec(sql);
}

void OperateDB::handleOffline(const char *name)
{
    if(name == NULL){
        return;
    }
    QSqlQuery q;//通过该对象执行sql语句
    QString sql = QString("update user_info set online = 0 where name = '%1'").arg(name);//更新用户在线
    qDebug()<<"用户下线"<<sql;
    q.exec(sql);
    return;
}

int OperateDB::handleFinduser(const char *name)
{
    if(name == NULL){
        return -1;
    }
    QSqlQuery q;//通过该对象执行sql语句
    QString sql = QString("select online from user_info where name ='%1'").arg(name);//查询用户是否在线
    qDebug()<<"用户是否在线"<<sql;
    if(!q.exec(sql)){
        return -1;//代表错误
    }
    if(q.next()){//说明用户存在
        return q.value(0).toInt();//不在线返回0在线返回1
    }else{
        return 2;
    }
}

bool OperateDB::handlAddfriend(const char *curName, const char *tarName)
{
    if(curName == NULL|| tarName == NULL){
        return false;
    }
    QString sql = QString("(select * from friend where(user_id = (select id from user_info where name ='%1')and friend_id =(select id from user_info where name ='%2')or(user_id = (select id from user_info where name ='%2')and friend_id =(select id from user_info where name ='%1'))))").arg(curName).arg(tarName);
    qDebug()<<"查询是否是好友"<<sql;
    QSqlQuery q;
    q.exec(sql);
    if(q.next()){
        return false;
    }
    return true;
}

bool OperateDB::agreeHandleaddfriend(const char *curName, const char *tarName)
{
    if(curName == NULL|| tarName == NULL){
        return false;
    }
    QString sql = QString("insert into friend(user_id,friend_id) select ul.id, u2.id from user_info ul, user_info u2 where ul.name='%1' and u2.name='%2'").arg(curName).arg(tarName);
    qDebug()<<"添加好友记录:"<<sql;
    QSqlQuery q;
    return q.exec(sql);
}

QStringList OperateDB::handleFlushfriend(const char *curName)
{
    QStringList result;
    if(curName == NULL){
        return result;
    }
    QString sql = QString("select name from user_info where id in (select friend_id from friend where user_id =(select id from user_info where name='%1')union select user_id from friend where friend_id =(select id from user_info where name='%1'))").arg(curName);
    qDebug()<<"刷新好友"<<sql;
    QSqlQuery q;
    q.exec(sql);
    while(q.next()){
        result.append(q.value(0).toString());
    }
    return result;
}
