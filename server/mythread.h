#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QMutex>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QMutex *inp_mutex, qintptr ID, QObject *parent = nullptr);
    qintptr get_socketdiscriptor();
    QString get_userId();
    void set_userId(QString id);
    QString get_state();

    void run();
    void on_new_message_recieved(QString senderId, QString recieverId, QString message, QString chatId);
    void on_new_group_created(QString id);
    void on_any_user_authenticated(QString id);
    void on_any_user_unauthenticated(QString id);
    void add_online_users(QString id);
    void reduce_online_users(QString id);
    void get_online_usres(QStringList ids);

signals:
    void error(QTcpSocket::SocketError sockererror);
    void thread_finished(qintptr,QString);
    void message_recieved(QString senderId, QString recieverId, QString message);
    void message_group_recieved(QString, QString, QStringList, QString);
    void user_authenticated(qintptr socketdiscriptor, QString id);
    void group_or_channel_created(QString,QStringList);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QString userId;
    QString state="";
    QMutex *tr_mutex;
    int authentication_code;
    QStringList online_users;
};

#endif // MYTHREAD_H
