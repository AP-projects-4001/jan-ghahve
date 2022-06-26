#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "mythread.h"
#include "channel.h"

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
    Channel channel;
    QString msg, status = data_obj["status"].toString();
    QByteArray response;

    data_obj.remove("status");
    //Checking request of client (register, login, message, getInfo, chatInfo)
    if(status == "register")
    {
        msg = channel.signup(data_obj);
        response = msg.toUtf8();
    }
    else if(status == "login")
    {
        msg = channel.signin(data_obj);
        response = msg.toUtf8();
    }
    else if(status == "getInfo")
    {
        response = channel.get_info(data_obj["id"].toString());
    }
    else if(status == "message")
    {
        channel.send_message(data_obj);
        msg = "ok";
        response = msg.toUtf8();
    }
    else if(status == "chatInfo")
    {
        response = channel.get_chat_info(data_obj["id1"].toString(), data_obj["id2"].toString());
    }

    //Get a responce from "channel", then Send it to the Client
    socket->write(response);
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";

    socket->deleteLater();
    exit(0);
}
