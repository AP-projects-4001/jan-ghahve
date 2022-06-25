#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class MyClient : public QObject
{
    Q_OBJECT
public:
    explicit MyClient(QObject *parent = nullptr);
    ~MyClient();
    bool connect_to_server();
    QByteArray authentication(QByteArray* data);
    void disconnect_from_server();
    void send_message(QByteArray* message, QString id1, QString id2);

private:
    QTcpSocket *clientSocket;
    QByteArray *data;
};

#endif // MYCLIENT_H
