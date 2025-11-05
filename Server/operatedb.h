#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>

class OperateDB : public QObject
{
    Q_OBJECT
public:
    ~OperateDB();
    QSqlDatabase m_db;//数据库对象
    static OperateDB& getInstance();
    void connect();
    bool handleRegist(const char* name,const char* pwd);
    bool handleLogin(const char* name,const char* pwd);
    void handleOffline(const char* name);
    int handleFinduser(const char* name);
    bool handlAddfriend(const char* curName,const char* tarName);
    bool agreeHandleaddfriend(const char* curName,const char* tarName);
    QStringList handleFlushfriend(const char* curName);
private:
    explicit OperateDB(QObject *parent = 0);//保证只要使用一个数据库，使用单例模式
    OperateDB(const OperateDB& other) = delete;
    OperateDB& operator =(const OperateDB&) = delete;
};

#endif // OPERATEDB_H
