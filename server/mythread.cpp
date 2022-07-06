#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include "mythread.h"
#include "channel.h"
#include "smtp.h"
#include "Encryption/myencryption.h"

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
        MyEncryption *encryption = new MyEncryption();
        QByteArray encoded_message = encryption->myEncode(message_b);
        socket->write(encoded_message);
        //socket->write(message_b);
        socket->waitForBytesWritten(-1);
        delete encryption;
        //qDebug() << this->socketDescriptor << " Data out" << message_b;
    }
}

void MyThread::on_new_group_created(QString id)
{
    QJsonObject message_obj;
    message_obj["status"] = "groupCreated";
    message_obj["id"] = id;
    QJsonDocument message_doc(message_obj);
    QByteArray message_b = message_doc.toJson();
    if(socket->ConnectedState)
    {
        //Encodeing
        MyEncryption *encryption = new MyEncryption();
        QByteArray encoded_message = encryption->myEncode(message_b);
        socket->write(encoded_message);
        socket->waitForBytesWritten(-1);
        delete encryption;
        qDebug() << this->socketDescriptor << " Data out" << message_b;
    }
}

void MyThread::on_any_user_authenticated(QString id)
{
    QJsonObject message_obj;
    message_obj["status"] = "userAuthenticated";
    message_obj["id"] = id;
    QJsonDocument message_doc(message_obj);
    QByteArray message_b = message_doc.toJson();
    if(socket->ConnectedState)
    {
        //Encodeing
        MyEncryption *encryption = new MyEncryption();
        QByteArray encoded_message = encryption->myEncode(message_b);
        socket->write(encoded_message);
        socket->waitForBytesWritten(-1);
        delete encryption;
        qDebug() << this->socketDescriptor << " Data out" << message_b;
    }
}

void MyThread::on_any_user_unauthenticated(QString id)
{
    if(id.isEmpty())
        return;
    QJsonObject message_obj;
    message_obj["status"] = "userUnAuthenticated";
    message_obj["id"] = id;
    QJsonDocument message_doc(message_obj);
    QByteArray message_b = message_doc.toJson();
    if(socket->ConnectedState)
    {
        //Encodeing
        MyEncryption *encryption = new MyEncryption();
        QByteArray encoded_message = encryption->myEncode(message_b);
        socket->write(encoded_message);
        socket->waitForBytesWritten(-1);
        delete encryption;
        qDebug() << this->socketDescriptor << " Data out" << message_b;
    }
}

void MyThread::add_online_users(QString id)
{
    online_users.append(id);
}

void MyThread::reduce_online_users(QString id)
{
    online_users.removeAll(id);
}

void MyThread::get_online_usres(QStringList ids)
{
    online_users = ids;
}

void MyThread::readyRead()
{
    // get the information
    QByteArray encoded_Data = socket->readAll();
    //Decoding
    // will write on server side window
    MyEncryption *encryption = new MyEncryption();
    QByteArray Data = encryption->myDecode(encoded_Data);
    //qDebug() << socketDescriptor << " Data in: " << Data;

    QJsonDocument data_doc = QJsonDocument::fromJson(Data);
    QJsonObject data_obj = data_doc.object();
    Channel channel(tr_mutex);
    QString msg, status = data_obj["status"].toString();
    QByteArray response;

    data_obj.remove("status");
    //Checking request of client (register, login, message, userInfo, chatInfo, allUsersInfo, userContacts)
    if(status == "register")
    {
        msg = channel.signup_check(data_obj);
        response = msg.toUtf8();
        if(msg == "accepted"){
            std::uniform_int_distribution<int> distribution(99999,999999);
            int number = distribution(*QRandomGenerator::global());
            authentication_code = number;

            QString const uname = "janghahve@gmail.com";
            QString const rcpt = data_obj["email"].toString();
            QString const subject = "Authentication code";
            QString const msg = QString::number(number);
            QString const paswd = "mxakpmwpmrjbqgzo";
            QString const server = "smtp.gmail.com";
            int const port = 465;
            Smtp* smtp = new Smtp(uname, paswd, server, port);
            smtp->sendMail(uname, rcpt , subject,msg);
        }
    }
    else if(status == "login")
    {
        msg = channel.signin(data_obj);
        response = msg.toUtf8();
        if(msg == "accepted"){
            std::uniform_int_distribution<int> distribution(99999,999999);
            int number = distribution(*QRandomGenerator::global());
            authentication_code = number;

            QString const uname = "janghahve@gmail.com";
            QString const rcpt = data_obj["email"].toString();
            QString const subject = "Authentication code";
            QString const msg = QString::number(number);
            QString const paswd = "mxakpmwpmrjbqgzo";
            QString const server = "smtp.gmail.com";
            int const port = 465;
            Smtp* smtp = new Smtp(uname, paswd, server, port);
            smtp->sendMail(uname, rcpt , subject,msg);
        }
    }
    else if(status == "authenticationCode"){
//        if(data_obj["auth"].toInt() == authentication_code){
//            if(data_obj["state"].toString() == "signup"){
//                data_obj.remove("state");
//                data_obj.remove("auth");
//                msg = channel.signup(data_obj);
//            }
//            msg = "accepted";
//        }else{
//            msg = "The code is wrong!";
//        }
        if(data_obj["state"].toString() == "signup"){

            data_obj.remove("state");
            data_obj.remove("auth");
            msg = channel.signup(data_obj);
        }
        msg = "accepted";
        response = msg.toUtf8();
    }
    else if(status == "userInfo"){
        QString id = data_obj["id"].toString();
        QJsonObject user = channel.get_info(id);
        set_userId(id);
        if(online_users.contains(data_obj["id"].toString()))
            user["online"] = true;
        else
            user["online"] = false;
        QJsonDocument user_doc(user);
        response = user_doc.toJson();
    }
    else if(status == "autherize"){
        QString id = data_obj["id"].toString();
        online_users.append(id);
        set_userId(id);
        state = data_obj["state"].toString();
        emit user_authenticated(this->socketDescriptor, id);
    }
    else if(status == "userInfo_forEdit"){
        response = channel.get_info_forEdit(data_obj["id"].toString());
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
    else if(status == "edit_profile"){
        msg = channel.edit_profile(data_obj);
        response = msg.toUtf8();
    }
    else if(status == "getProfileImage")
    {
        response = channel.get_profile_image(data_obj);
    }
    else if(status == "Channel_Group_Profile_Edited")
    {
        msg = channel.channel_group_profile_edited(data_obj);
        response = msg.toUtf8();
    }
    else if(status == "editMessage"){
        channel.edit_message(data_obj);
        msg = "ok";
        response = msg.toUtf8();
    }

    //Get a responce from "channel", then Send it to the Client
    //Encoding
    QByteArray encoded_response = encryption->myEncode(response);
    socket->write(encoded_response);
    socket->waitForBytesWritten(-1);
    delete encryption;
}


void MyThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    emit thread_finished(this->socketDescriptor, userId);
    socket->deleteLater();
    exit(0);
}
