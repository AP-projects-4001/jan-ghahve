#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QJsonObject>
class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(QObject *parent = nullptr);
    QString signup(QJsonObject data);
    QString signin(QJsonObject data);
    QByteArray get_info(QString id);
private:
    QString path;

};

#endif // CHANNEL_H
