#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QString>
#include <QTcpSocket>

class MyThread : public QThread
{
    Q_OBJECT
public:
    // constructor
    // set name using initializer
    explicit MyThread(QString id);

    // overriding the QThread's run() method
    void run();
public slots:
    void readyRead();
    void disconnected();

signals:
    void message_recieved(QString,QString,QString);
    void group_created(QString);

private:
    QString id;
    QTcpSocket* socket;
};

#endif // MYTHREAD_H




