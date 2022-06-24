#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <qbytearray.h>

class MyClient : public QObject
{
    Q_OBJECT
public:
    explicit MyClient(QByteArray* data, QObject *parent = nullptr);


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
