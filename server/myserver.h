#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QMutex>

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    void startServer();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QMutex *server_mutex;
};

#endif // MYSERVER_H
