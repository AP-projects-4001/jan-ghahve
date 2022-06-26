#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "channel.h"

Channel::Channel(QObject *parent)
    : QObject{parent}
{
    path = "database.json";
}

//returns "Username already taken" or "accepted"
QString Channel::signup(QJsonObject data)
{
    QJsonArray json_arr, profiles_arr;
    QJsonObject json_obj;
    json_obj = read_from_file(path);
    json_arr = json_obj["users"].toArray();
    profiles_arr = json_obj["profiles"].toArray();

    bool username_uique = true;
    QJsonObject user;
    for(QJsonValueRef user_ref:qAsConst(json_arr))
    {
        user = user_ref.toObject();
        if(user["id"] == data["id"]){
            username_uique = false;
            break;
        }
    }
    if(!username_uique)
        return "Username already taken";

    user["id"] = data["id"];
    user["password"] = data["password"];
    json_arr.append(user);
    data.remove("password");
    profiles_arr.append(data);

    QJsonObject result;
    result["users"] = json_arr;
    result["profiles"] = profiles_arr;

    write_to_file(path, result);

    return "accepted";
}

QString Channel::signin(QJsonObject data)
{
    QJsonArray json_arr;
    QJsonObject json_obj;
    json_obj = read_from_file(path);
    json_arr = json_obj["users"].toArray();

    QJsonObject user;
    for(QJsonValueRef user_ref:qAsConst(json_arr))
    {
        user = user_ref.toObject();
        //qDebug()<<"user in database : "<<user["id"]<<"     "<<user["password"]<<"  |  "<<"data inputed : "<<data["id"]<<"     "<<data["password"];
        if((user["id"] == data["id"])&&(user["password"]==data["password"]))
        {
            return "accepted login";
        }
        else if((user["id"] == data["id"])&&(user["password"]!=data["password"]))
        {
            return "incorrect password";
        }
    }
    return "incorrect Username";
}

QByteArray Channel::get_info(QString id)
{
    QJsonObject json_obj;
    QJsonArray profiles_arr;
    json_obj = read_from_file(path);
    profiles_arr = json_obj["profiles"].toArray();

    QJsonObject user;
    for(QJsonValueRef user_ref:qAsConst(profiles_arr))
    {
        user = user_ref.toObject();
        if(user["id"] == id){
            QJsonDocument temp_doc(user);
            return temp_doc.toJson();
        }
    }
    return 0;
}

void Channel::send_message(QJsonObject data)
{
    QString file_path = data["id1"].toString() + "%" + data["id2"].toString() + ".json";
    QJsonObject msg_obj;
    QJsonArray msg_arr;

    msg_obj = read_from_file(file_path);
    if(msg_obj.empty()){
        file_path = data["id2"].toString() + "%" + data["id1"].toString() + ".json";
        msg_obj = read_from_file(file_path);
    }

    msg_arr = msg_obj["messages"].toArray();
    QJsonObject message;
    message["sender"] = data["id1"];
    message["message"] = data["message"];
    msg_arr.append(message);

    QJsonObject result;
    result["messages"] = msg_arr;
    write_to_file(file_path, result);
}

QByteArray Channel::get_chat_info(QString id1, QString id2)
{
    QString file_path = id1 + "%" + id2 + ".json";
    QJsonObject chat_obj;

    chat_obj = read_from_file(file_path);
    if(chat_obj.empty()){
        file_path = id2 + "%" + id1 + ".json";
        chat_obj = read_from_file(file_path);
    }
    QJsonDocument chat_doc(chat_obj);
    return chat_doc.toJson();
}

void Channel::write_to_file(QString file_path, QJsonObject result)
{
    QJsonDocument result_doc(result);
    QFile file(file_path);
    file.open(QIODevice::WriteOnly);
    file.write(result_doc.toJson());
    file.close();
}

QJsonObject Channel::read_from_file(QString file_path)
{
    QFile file(file_path);
    QJsonObject json_obj;
    if(file.open(QIODevice::ReadOnly)){
        QByteArray b = file.readAll();
        file.close();
        QJsonDocument json_doc = QJsonDocument::fromJson(b);
        json_obj = json_doc.object();
    }
    return json_obj;
}
