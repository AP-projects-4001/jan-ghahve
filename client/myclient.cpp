#include "myclient.h"
#include "siginup.h"
#include "signin.h"
#include <QDebug>



MyClient::MyClient(QString status ,QByteArray *data,QObject *parent)
    : QObject{parent}
{
    if(status == "login")
    {
        signin* signin_ = new signin();
        connect(this, SIGNAL(response_recieved(QByteArray)), signin_, SLOT(on_response_recieved(QByteArray))); //SIGN IN
    }
    else if(status=="register")
    {
        siginup* signup = new siginup();
        connect(this, SIGNAL(response_recieved(QByteArray)), signup, SLOT(on_response_recieved(QByteArray)));  //SIGN UP
    }

    this->data = data;
    qDebug() << "Start\n";
    clientSocket = new QTcpSocket;
    clientSocket->connectToHost("127.0.0.1",1025);
    qDebug() << "connecting...\n";

    if(clientSocket->waitForConnected(-1)){
        qDebug() << "connected Successfully!\n";
        clientSocket->write(*data);
        while(clientSocket->waitForBytesWritten(-1));
        if(clientSocket->waitForReadyRead(-1)){
            QByteArray response = clientSocket->readAll();
            emit response_recieved(response);
        }
    }
    connect(clientSocket,SIGNAL(connected()),this,SLOT(connectedToServer()));
    connect(clientSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(writingData()));
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readingData()));

    //connect(clientSocket,SIGNAL(disconnect()),this,SLOT(disconnectedFromServer()));
}



void MyClient::readingData()
{
    QByteArray response = clientSocket->readAll();
}

void MyClient::writingData()
{
    qDebug() << "writing successfully! \n";
}

void MyClient::connectedToServer()
{
    qDebug() << "connected Successfully!\n";
    clientSocket->write(*data);
}

void MyClient::disconnectedFromServer()
{
    qDebug() << "connection lost\n";
}



