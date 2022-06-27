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
    //Start to connect to the server
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
    //Sending data to the server and waiting for getting a response
    clientSocket->write(*data);
    while(clientSocket->waitForBytesWritten(-1));
    if(clientSocket->waitForReadyRead(-1)){
        //Getting the responce and returning it
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

bool MyClient::is_client_connectd()
{
    return clientSocket->ConnectedState;
}

QByteArray MyClient::message_recieved()
{
    if(clientSocket->waitForReadyRead(-1)){
        return clientSocket->readAll();
    }
    return 0;
}


//void MyClient::send_message(QByteArray *message, QString id1, QString id2)
//{

//}






