#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>

class OperateDB : public QObject
{
    Q_OBJECT
public:
    ~OperateDB();
    QSqlDatabase m_db; // 数据库对象
    static OperateDB& getInstance();
    void connect(); // 连接数据库
    bool handleRegist(const char* name,const char* pwd); // 处理注册数据库操作
    bool handleLogin(const char* name,const char* pwd); // 处理登录数据库操作
    void handleOffline(const char* name); // 处理下线数据库操作
    int handleFinduser(const char* name); // 处理刷新好友数据库读取操作
    bool handlAddfriend(const char* curName,const char* tarName); // 处理添加好友申请查找数据库操作
    bool agreeHandleaddfriend(const char* curName,const char* tarName); // 同意好友申请数据库操作
    QStringList handleFlushfriend(const char* curName); // 处理刷新好友数据库操作
private:
    explicit OperateDB(QObject *parent = 0); // 保证只要使用一个数据库，使用单例模式
    OperateDB(const OperateDB& other) = delete;
    OperateDB& operator =(const OperateDB&) = delete;
};

#endif // OPERATEDB_H
