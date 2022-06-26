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
    void send_message(QJsonObject data);

private:
    QString path;
    void write_to_file(QString file_path, QJsonObject result);
    QJsonObject read_from_file(QString file_path);
};

#endif // CHANNEL_H
