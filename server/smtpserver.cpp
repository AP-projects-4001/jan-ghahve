#include "smtpserver.h"
#include <QDebug>
#include "smtp.h"

SmtpServer::SmtpServer(QObject *parent)
    : QObject{parent}
{

}

void SmtpServer::sendMail()
{
    Smtp* smtp = new Smtp("janghahve@gmail.com", "mxakpmwpmrjbqgzo", "smtp.gmail.com", 456);
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));
    smtp->sendMail("janghahve@gmail.com", "alirezasamimi1381@gmail.com" , "salam","salam");

}

void SmtpServer::mailSent(QString)
{
    qDebug() << "message sent";
}
