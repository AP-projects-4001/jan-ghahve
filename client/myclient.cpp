#include "myclient.h"
#include <QDebug>

MyClient::MyClient(QByteArray *data,QObject *parent)
    : QObject{parent}
{
    this->data = data;
    qDebug() << "Start\n";
    clientSocket = new QTcpSocket;
    clientSocket->connectToHost("127.0.0.1",1025);
    qDebug() << "connecting...\n";

    if(clientSocket->waitForConnected(-1)){
        qDebug() << "connected Successfully!\n";
        clientSocket->write(*data);
    }
    connect(clientSocket,SIGNAL(connected()),this,SLOT(connectedToServer()));
    connect(clientSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(writingData()));
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readingData()));
    //connect(clientSocket,SIGNAL(disconnect()),this,SLOT(disconnectedFromServer()));
}



void MyClient::readingData()
{
    //QByteArray data = clientSocket->readAll();
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



