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

qintptr MyThread::get_socketdiscriptor()
{
    return this->socketDescriptor;
}

QString MyThread::get_userId()
{
    return this->userId;
}

void MyThread::set_userId(QString id)
{
    this->userId = id;
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

void MyThread::on_new_message_recieved(QString senderId, QString message)
{
    QJsonObject message_obj;
    message_obj["sender"] = senderId;
    message_obj["message"] = message;
    QJsonDocument message_doc(message_obj);
    QByteArray message_b = message_doc.toJson();
    if(socket->ConnectedState){
        socket->write(message_b);
        socket->waitForBytesWritten(-1);
    }
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
    if(status == "register"){
        msg = channel.signup(data_obj);
        response = msg.toUtf8();
    }
    else if(status == "login"){
        msg = channel.signin(data_obj);
        response = msg.toUtf8();
    }
    else if(status == "userInfo"){
        response = channel.get_info(data_obj["id"].toString());
        set_userId(data_obj["id"].toString());
        emit user_authenticated(this->socketDescriptor, this->userId);
    }else if(status == "message"){
        channel.send_message(data_obj);
        msg = "ok";
        response = msg.toUtf8();
        emit message_recieved(data_obj["id2"].toString(), data_obj["message"].toString());
    }
    else if(status == "chatInfo"){
        response = channel.get_chat_info(data_obj["id1"].toString(), data_obj["id2"].toString());
    } else if(status == "allUsersInfo"){
        response = channel.get_all_info();
    }

    socket->write(response);
    socket->waitForBytesWritten(-1);
}

void MyThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    emit thread_finished(this->socketDescriptor);
    socket->deleteLater();
    exit(0);
}
