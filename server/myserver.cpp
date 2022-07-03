#include "myserver.h"
#include "smtp.h"

MyServer::MyServer(QObject *parent)
    : QTcpServer{parent}
{

    server_mutex = new QMutex;

//    QString const uname = "janghahve@gmail.com";
//    QString const rcpt = "alirezasamimi1381@gmail.com";
//    QString const subject = "salam";
//    QString const msg = "salam";
//    QString const paswd = "mxakpmwpmrjbqgzo";
//    QString const server = "smtp.gmail.com";
//    int const port = 465;
//    Smtp* smtp = new Smtp(uname, paswd, server, port);
//    //connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));


//    smtp->sendMail(uname, rcpt , subject,msg);
}

void MyServer::startServer()
{
    int port = 1025;

    if(!this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }
}


void MyServer::on_thread_finished(qintptr socketdiscriptor)
{

    for(int i=0; i<threads.length(); i++){
        if(threads.at(i)->get_socketdiscriptor() == socketdiscriptor){
            data.remove(socketdiscriptor);
            threads.removeAt(i);
        }
    }
}

void MyServer::on_message_recieved(QString senderId, QString recieverId, QString message)
{
    QString id;
    for(int i=0; i< threads.length(); i++){
        id = data.value(threads.at(i)->get_socketdiscriptor());
        if(id == recieverId && threads.at(i)->get_state() == "reciever"){
            threads.at(i)->on_new_message_recieved(senderId, recieverId, message, "");
            break;
        }
    }

}

void MyServer::on_message_group_recieved(QString senderId, QString chatId ,QStringList recieverIds, QString message)
{
    QString id;
    for(int i=0; i< threads.length(); i++){
        id = data.value(threads.at(i)->get_socketdiscriptor());
        if(recieverIds.contains(id) && threads.at(i)->get_state() == "reciever"){
            threads.at(i)->on_new_message_recieved(senderId, id, message, chatId);
        }
    }
}

void MyServer::on_user_authenticated(qintptr socketdiscriptor, QString id)
{
    data.insert(socketdiscriptor, id);
}

void MyServer::on_group_or_channel_created(QString id,QStringList ids)
{
    QString user_id;
    for(int i=0; i< threads.length(); i++){
        user_id = data.value(threads.at(i)->get_socketdiscriptor());
        if(ids.contains(user_id) && threads.at(i)->get_state() == "reciever"){
            threads.at(i)->on_new_group_created(id);
        }
    }
}



// This function is called by QTcpServer when a new connection is available.
void MyServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    // Every new connection will be run in a newly created thread
    MyThread *thread = new MyThread(server_mutex,socketDescriptor, this);

    threads.push_back(thread);
    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(thread_finished(qintptr)), this, SLOT(on_thread_finished(qintptr)));
    QObject::connect(thread, &MyThread::message_recieved, this, &MyServer::on_message_recieved);
    QObject::connect(thread, &MyThread::user_authenticated, this, &MyServer::on_user_authenticated);
    connect(thread, &MyThread::message_group_recieved, this, &MyServer::on_message_group_recieved);
    connect(thread, &MyThread::group_or_channel_created, this, &MyServer::on_group_or_channel_created);
    thread->start();


}
