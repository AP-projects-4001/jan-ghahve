#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "mythread.h"
#include "authentication.h"

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

    QJsonDocument data_doc = QJsonDocument::fromJson(Data);
    QJsonObject data_obj = data_doc.object();
    //Check status, Does the user want to login or register?
    if(data_obj["status"] == "register"){
        Authentication auth;
        data_obj.remove("status");
        //Go to authentication to get appropriate response
        QString response = auth.signup(data_obj);
        QByteArray msg = response.toUtf8();
        //writing an answer to the client
        socket->write(msg);
    }
    else if(data_obj["status"] == "login"){
        Authentication auth;
        data_obj.remove("status");
        //Go to authentication to get appropriate response
        QString response = auth.signin(data_obj);
        QByteArray msg = response.toUtf8();
        //writing an answer to the client
        socket->write(msg);
    }
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";

    socket->deleteLater();
    exit(0);
}
