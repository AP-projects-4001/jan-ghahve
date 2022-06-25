#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QObject>
#include <QJsonObject>
class Authentication : public QObject
{
    Q_OBJECT
public:
    explicit Authentication(QObject *parent = nullptr);
    QString signup(QJsonObject data);
    QString signin(QJsonObject data);
private:
    QString path;

};

#endif // AUTHENTICATION_H
