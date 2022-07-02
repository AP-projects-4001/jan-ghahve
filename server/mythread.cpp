#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "mythread.h"
#include "channel.h"
#include "smtpthread.h"

MyThread::MyThread(QMutex *inp_mutex,qintptr ID, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = ID;
    this->tr_mutex = inp_mutex;
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

QString MyThread::get_state()
{
    return state;
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

void MyThread::on_new_message_recieved(QString senderId, QString recieverId, QString message, QString chatId)
{
    QJsonObject message_obj;
    message_obj["status"] = "message";
    message_obj["sender"] = senderId;
    message_obj["reciever"] = recieverId;
    message_obj["message"] = message;
    message_obj["chat"] = chatId;
    QJsonDocument message_doc(message_obj);
    QByteArray message_b = message_doc.toJson();
    if(socket->ConnectedState){
        socket->write(message_b);
        socket->waitForBytesWritten(-1);
        qDebug() << this->socketDescriptor << " Data out" << message_b;
    }
}

void MyThread::on_new_group_created(QString id)
{
    QJsonObject message_obj;
    message_obj["status"] = "groupCreated";
    message_obj["id"] = id;
    QJsonDocument message_doc(message_obj);
    QByteArray message_b = message_doc.toJson();
    if(socket->ConnectedState){
        socket->write(message_b);
        socket->waitForBytesWritten(-1);
        qDebug() << this->socketDescriptor << " Data out" << message_b;
    }
}

void MyThread::readyRead()
{
    // get the information

    QByteArray Data = socket->readAll();
    //Decoding

    // will write on server side window
    qDebug() << socketDescriptor << " Data in: " << Data;

    QJsonDocument data_doc = QJsonDocument::fromJson(Data);
    QJsonObject data_obj = data_doc.object();
    Channel channel(tr_mutex);
    //-------TEST-----
    QString msg, status = data_obj["status"].toString();
    QByteArray response;

    data_obj.remove("status");
    //Checking request of client (register, login, message, userInfo, chatInfo, allUsersInfo, userContacts)
    if(status == "register")
    {
        msg = channel.signup(data_obj);
        response = msg.toUtf8();
//        socket->disconnect();
//        SmtpThread* smtpthread = new SmtpThread();
//        smtpthread->start();
//        Smtp* smtp = new Smtp("janghahve@gmail.com", "mxakpmwpmrjbqgzo", "smtp.gmail.com", 456);
//        connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
//        smtp->sendMail("janghahve@gmail.com", "alirezasamimi1381@gmail.com" , "salam","salam");
    }
    else if(status == "login")
    {
        msg = channel.signin(data_obj);
        response = msg.toUtf8();
    }
    else if(status == "userInfo"){
        response = channel.get_info(data_obj["id"].toString());
        set_userId(data_obj["id"].toString());
        state = data_obj["state"].toString();
        emit user_authenticated(this->socketDescriptor, this->userId);
    }
    else if(status == "message"){

        channel.send_message(data_obj);
        msg = "ok";
        response = msg.toUtf8();
        emit message_recieved(data_obj["id1"].toString(), data_obj["id2"].toString(), data_obj["message"].toString());
    }
    else if(status == "chatInfo")
    {
        response = channel.get_chat_info(data_obj["id1"].toString(), data_obj["id2"].toString(), data_obj["chat"].toString());
    }
    else if(status == "allUsersInfo"){
        response = channel.get_all_info();
    }
    else if(status == "userContacts"){
        response = channel.get_user_contacts(data_obj["id"].toString());
    }
    else if(status == "allUsersContacts"){
        response = channel.get_all_contacts();
    }
    else if(status == "createGroup"){
        QStringList allIds = channel.create_group_or_channel(data_obj, "group");
        if(allIds.empty()){
            msg = "not accepted";
        }else{
            emit group_or_channel_created(data_obj["name"].toString(), allIds);
            msg = "ok";
        }
        response = msg.toUtf8();
    }
    else if(status == "messageToGroup"){
        QStringList allIds = channel.send_message_to_group_or_channel(data_obj, "group");
        msg = "ok";
        response = msg.toUtf8();
        emit message_group_recieved(data_obj["id1"].toString(), data_obj["id2"].toString(), allIds, data_obj["message"].toString());
    }
    else if(status == "createChannel"){
        QStringList allIds = channel.create_group_or_channel(data_obj, "channel");
        if(allIds.empty()){
            msg = "not accepted";
        }else{
            emit group_or_channel_created(data_obj["name"].toString(), allIds);
            msg = "ok";
        }
        response = msg.toUtf8();
    }
    else if(status == "messageToChannel"){
        QStringList allIds = channel.send_message_to_group_or_channel(data_obj, "channel");
        msg = "ok";
        response = msg.toUtf8();
        emit message_group_recieved(data_obj["id1"].toString(), data_obj["id2"].toString(), allIds, data_obj["message"].toString());
    }
    else if(status == "channelInfo"){
        response = channel.channelInfo(data_obj["id"].toString());
    }
    else if(status == "modifyAdmins"){
        channel.modify_channel_admins(data_obj["id"].toString(), data_obj["admins"].toString());
        msg = "ok";
        response = msg.toUtf8();
    }

    //Get a responce from "channel", then Send it to the Client
    //Encoding
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
