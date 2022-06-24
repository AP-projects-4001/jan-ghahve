#include "mythread.h"
#include <QDebug>
MyThread::MyThread(qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
}

void MyThread::run()
{
    qDebug() << " Thread started";

    socket = new QTcpSocket();

    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        // something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    // We'll have multiple clients, we want to know which is which
    qDebug() << socketDescriptor << " Client connected";

    // make this thread a loop,
    // thread will stay alive so that signal/slot to function properly
    // not dropped out in the middle when thread dies

    exec();
}

void MyThread::readyRead()
{
    // get the information
    QByteArray Data = socket->readAll();

    // will write on server side window
    qDebug() << socketDescriptor << " Data in: " << Data;

    socket->write(Data);
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";

    socket->deleteLater();
    exit(0);
}
