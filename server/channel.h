#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QJsonObject>
#include <qmutex.h>

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(QMutex *inp_mutex ,QObject *parent = nullptr);
    QString signup(QJsonObject data);
    QString signin(QJsonObject data);
    QByteArray get_info(QString id);
    void send_message(QJsonObject data);
    QByteArray get_chat_info(QString id1, QString id2);
    QByteArray get_all_info();
    QByteArray get_user_contacts(QString id);
    QByteArray get_all_contacts();

private:
    QString path;
    void write_to_file(QString file_path, QJsonObject result);
    QJsonObject read_from_file(QString file_path);
    QMutex *ch_mutex;
};

#endif // CHANNEL_H
