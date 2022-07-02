#include <QDebug>
#include "smtpthread.h"
#include "smtp.h"

SmtpThread::SmtpThread(QObject *parent)
    : QThread{parent}
{

}

void SmtpThread::sendMail()
{
    Smtp* smtp = new Smtp("janghahve@gmail.com", "mxakpmwpmrjbqgzo", "smtp.gmail.com", 456);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    smtp->sendMail("janghahve@gmail.com", "alirezasamimi1381@gmail.com" , "salam","salam");
}

void SmtpThread::run()
{
    sendMail();
}

void SmtpThread::mailSent(QString)
{
    qDebug() << "email sent!";
}
