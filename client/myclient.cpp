#include "myclient.h"
#include "qjsonobject.h"
#include "siginup.h"
#include "signin.h"
#include <QDebug>


MyClient::MyClient(QObject *parent)
    : QObject{parent}
{

}

MyClient::~MyClient()
{
    qDebug() << "client deleted\n";
}

bool MyClient::connect_to_server()
{
    qDebug() << "Start\n";
    clientSocket = new QTcpSocket;
    clientSocket->connectToHost("127.0.0.1",1025);
    qDebug() << "connecting...\n";

    if(clientSocket->waitForConnected(-1)){
        qDebug() << "connected Successfully!\n";
        return true;
    }
    return false;
}

QByteArray MyClient::request_to_server(QByteArray *data)
{
    clientSocket->write(*data);
    while(clientSocket->waitForBytesWritten(-1));
    if(clientSocket->waitForReadyRead(-1)){
        QByteArray response = clientSocket->readAll();
        return response;
    }
    return 0;
}

void MyClient::disconnect_from_server()
{
    clientSocket->disconnect();
    clientSocket->disconnectFromHost();
    clientSocket->deleteLater();
}


//void MyClient::send_message(QByteArray *message, QString id1, QString id2)
//{

//}






