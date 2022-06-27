#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qintptr ID, QObject *parent = nullptr);
    qintptr get_socketdiscriptor();
    QString get_userId();
    void set_userId(QString id);
    QString get_state();
    void run();
    void on_new_message_recieved(QString senderId, QString recieverId, QString message);

signals:
    void error(QTcpSocket::SocketError sockererror);
    void thread_finished(qintptr socketdiscriptor);
    void message_recieved(QString senderId, QString recieverId, QString message);
    void user_authenticated(qintptr socketdiscriptor, QString id);

public slots:
    void readyRead();
    void disconnected();


private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QString userId;
    QString state="";
};

#endif // MYTHREAD_H
