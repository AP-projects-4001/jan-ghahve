#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QMap>
#include "mythread.h"
#include <QMutex>
#include <QTcpServer>

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    void startServer();

private slots:
    void on_thread_finished(qintptr socketdiscriptor, QString id);
    void on_message_recieved(QString senderId, QString recieverId, QString message);
    void on_message_group_recieved(QString senderId, QString chatId, QStringList recieverIds, QString message);
    void on_user_authenticated(qintptr socketdiscriptor, QString id);
    void on_group_or_channel_created(QString id, QStringList ids);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QList<MyThread*> threads;
    QMap<qintptr,QString> data;//{"socketdiscriptor":"id"}
    QMutex *server_mutex;
    QStringList online_users;
};

#endif // MYSERVER_H
