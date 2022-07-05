#include "mythread.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "Encryption/myencryption.h"

MyThread::MyThread(QString id) : id(id)
{
}

// We overrides the QThread's run() method here
// run() will be called when a thread starts
// the code will be shared by all threads

void MyThread::run()
{
    qDebug() << " Thread started";

    socket = new QTcpSocket;
    socket->connectToHost("127.0.0.1",1025);
    qDebug() << "connecting...\n";

    if(socket->waitForConnected(-1)){
        qDebug() << "connected Successfully!\n";
        QJsonObject req;
        req["status"] = "userInfo";
        req["id"] = id;
        req["state"] = "reciever";
        QJsonDocument req_doc(req);
        QByteArray req_b = req_doc.toJson();
        MyEncryption *encryption = new MyEncryption();
        QByteArray encoded_data = encryption->myEncode(req_b);
        delete encryption;
        socket->write(encoded_data);
        socket->waitForBytesWritten(-1);
        if(socket->waitForReadyRead(-1)){
            socket->readAll();
        }
    }


    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    exec();

}

void MyThread::readyRead()
{
    // get the information
    QByteArray encrypted_Data = socket->readAll();
    //Decoding
    MyEncryption* encryption = new MyEncryption();
    QByteArray Data = encryption->myDecode(encrypted_Data);
    delete encryption;

    QJsonDocument data_doc = QJsonDocument::fromJson(Data);
    QJsonObject data_obj = data_doc.object();
    QString status = data_obj["status"].toString();
    qDebug() << data_obj;
    if(status == "message"){
        emit message_recieved(data_obj["sender"].toString(), data_obj["message"].toString(), data_obj["chat"].toString());
    }
    else if(status == "groupCreated"){
        emit group_created(data_obj["id"].toString());
    }
}

void MyThread::disconnected()
{
    socket->deleteLater();
    exit(0);
}
