#include <QCoreApplication>
#include "myserver.h"
#include "mythread.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <qbytearray.h>
#include <QString>
#include <vector>
#include <QList>
#include <thread>

//------------------ TEST ------------------
void thread_test()
{
        QString default_email = "aa";
        for(int i=0 ; i<20 ; i++)
        {
            QTcpSocket *clientSocket = new QTcpSocket;
            clientSocket->connectToHost("127.0.0.1",1025);

            default_email+="a";
            QJsonObject user;
            user["status"] = "register";
            user["id"] = 2000+i;
            user["name"] = "Mohammad";
            user["password"] = "10";
            user["email"] = "abc" + default_email;
            user["birthdate"] = "۰۱/۰۱/۲۰۰۰";
            QJsonDocument user_d(user);
            QByteArray user_b = user_d.toJson();

            clientSocket->write(user_b);

            //if the client didn't disconnect, multi-threads(clients) are running at the same time

            //clientSocket->disconnect();
        }
}

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    MyServer server;
    server.startServer();
    //-------- TEST --------
    thread_test();

    return a.exec();
}
