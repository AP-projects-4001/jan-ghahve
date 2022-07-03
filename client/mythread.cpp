#include "mythread.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "Encrypton/myencryption.cpp"

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
        //socket->write(req_doc.toJson());
        socket->write(encoded_data);
        socket->waitForBytesWritten(-1);
        if(socket->waitForReadyRead(-1)){
            socket->readAll();
//            QByteArray response = socket->readAll();
//            QByteArray decoded_response = encryption.myDecode(response);
//            return decoded_response;
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
    //---------
    MyEncryption* encryption = new MyEncryption();
    QByteArray Data = encryption->myDecode(encrypted_Data);
    delete encryption;

    QJsonDocument data_doc = QJsonDocument::fromJson(Data);
    QJsonObject data_obj = data_doc.object();
//    if(data_obj["reciever"].toString() == id){
        qDebug() << data_obj;
        emit message_recieved1(data_obj["sender"].toString(), data_obj["message"].toString(), data_obj["chat"].toString());
    //}
}

void MyThread::disconnected()
{
    socket->deleteLater();
    exit(0);
}
