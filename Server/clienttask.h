#ifndef CLIENTTASK_H
#define CLIENTTASK_H

#include "mytcpsocket.h"

#include <QObject>
#include <QRunnable>

class ClientTask : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit ClientTask(QObject *parent = 0);
    ~ClientTask();
    ClientTask(MyTcpSocket* socket);
    void run() override;
    MyTcpSocket*mySocket;

signals:

public slots:
};

#endif // CLIENTTASK_H
