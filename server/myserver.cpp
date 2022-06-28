#include "myserver.h"

MyServer::MyServer(QObject *parent)
    : QTcpServer{parent}
{

    server_mutex = new QMutex;
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
            threads.at(i)->on_new_message_recieved(senderId, recieverId, message);
            break;
        }
    }

}

void MyServer::on_user_authenticated(qintptr socketdiscriptor, QString id)
{
    data.insert(socketdiscriptor, id);
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
    thread->start();
}
