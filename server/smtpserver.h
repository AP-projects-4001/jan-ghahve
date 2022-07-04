#ifndef SMTPSERVER_H
#define SMTPSERVER_H

#include <QObject>

class SmtpServer : public QObject
{
    Q_OBJECT
public:
    explicit SmtpServer(QObject *parent = nullptr);
    void sendMail();

private slots:
    void mailSent(QString);

signals:

};

#endif // SMTPSERVER_H
