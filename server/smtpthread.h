#ifndef SMTPTHREAD_H
#define SMTPTHREAD_H

#include <QThread>

class SmtpThread : public QThread
{
    Q_OBJECT
public:
    explicit SmtpThread(QObject *parent = nullptr);
    void sendMail();
    void run();

private slots:
    void mailSent(QString);

};

#endif // SMTPTHREAD_H
