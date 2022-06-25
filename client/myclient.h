#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>

class MyClient : public QObject
{
    Q_OBJECT
public:
    explicit MyClient(QString status ,QByteArray* data, QObject *parent = nullptr);

signals:
    void response_recieved(QByteArray);

public slots:
    //void connectingToServer();
    void readingData();
    void writingData();
    void connectedToServer();
    void disconnectedFromServer();

private:
    QTcpSocket *clientSocket;
    QByteArray *data;
};

#endif // MYCLIENT_H
