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
    explicit MyThread(QMutex *inp_mutex,qintptr ID, QObject *parent = nullptr);

    void run();
signals:
    void error(QTcpSocket::SocketError sockererror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
    QMutex *tr_mutex;
};

#endif // MYTHREAD_H
