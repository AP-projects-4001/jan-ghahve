#include <QDebug>
#include "smtpthread.h"
#include "smtp.h"

SmtpThread::SmtpThread(QObject *parent)
    : QThread{parent}
{

}

void SmtpThread::sendMail()
{
    QString const uname = "janghahve@gmail.com";
    QString const rcpt = "alirezasamimi1381@gmail.com";
    QString const subject = "salam";
    QString const msg = "salam";
    QString const paswd = "mxakpmwpmrjbqgzo";
    QString const server = "smtp.gmail.com";
    int const port = 465;
//    Smtp* smtp = new Smtp(uname, paswd, server, port);
//    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
//    smtp->sendMail(uname, rcpt , subject,msg);
}

void SmtpThread::run()
{
    sendMail();
}

void SmtpThread::mailSent(QString)
{
    qDebug() << "email sent!";
}
