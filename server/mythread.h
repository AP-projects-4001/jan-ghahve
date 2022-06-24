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

    void run();
signals:
    void error(QTcpSocket::SocketError sockererror);

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket* socket;
    qintptr socketDescriptor;
};

#endif // MYTHREAD_H
