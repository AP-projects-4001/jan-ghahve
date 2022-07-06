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
    QString signup_check(QJsonObject data);
    QString signup(QJsonObject data);
    QString signin(QJsonObject data);
    QJsonObject get_info(QString id);
    QByteArray get_info_forEdit(QString id);
    void send_message(QJsonObject data);
    QByteArray get_chat_info(QString id1, QString id2, QString chat);
    QByteArray get_all_info();
    QByteArray get_user_contacts(QString id);
    QByteArray get_all_contacts();
    QStringList create_group_or_channel(QJsonObject data, QString chat);
    void add_contact(QString id1, QString id2, QString status);
    QStringList send_message_to_group_or_channel(QJsonObject data, QString chat);
    QByteArray channelInfo(QString id);
    void modify_channel_admins(QString id, QString admins);
    QByteArray get_profile_image(QJsonObject data);
    QString edit_profile(QJsonObject data);
    QString channel_group_profile_edited(QJsonObject data);
    void edit_message(QJsonObject data);

private:
    QString path;
    void write_to_file(QString file_path, QJsonObject result);
    QJsonObject read_from_file(QString file_path);
    QMutex *ch_mutex;
};

#endif // CHANNEL_H
