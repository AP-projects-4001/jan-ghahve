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
    QFile file(path);
    QJsonArray json_arr, profiles_arr;
    bool username_uique = true;

    if(file.open(QIODevice::ReadOnly)){
        QByteArray b = file.readAll();
        file.close();

        QJsonDocument json_doc = QJsonDocument::fromJson(b);
        QJsonObject json_obj = json_doc.object();
        json_arr = json_obj["users"].toArray();
        profiles_arr = json_doc["profiles"].toArray();
    }

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

    QJsonDocument result_doc(result);
    QFile file2(path);
    file2.open(QIODevice::WriteOnly);
    file2.write(result_doc.toJson());
    file2.close();
    return "accepted";
}

QString Channel::signin(QJsonObject data)
{

    QFile file(path);
    QJsonArray json_arr;
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray b = file.readAll();
        file.close();

        QJsonDocument json_doc = QJsonDocument::fromJson(b);
        QJsonObject json_obj = json_doc.object();
        json_arr = json_obj["users"].toArray();
    }

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
    QFile file(path);
    QJsonArray profiles_arr;

    if(file.open(QIODevice::ReadOnly)){
        QByteArray b = file.readAll();
        file.close();

        QJsonDocument json_doc = QJsonDocument::fromJson(b);
        QJsonObject json_obj = json_doc.object();
        profiles_arr = json_doc["profiles"].toArray();
    }

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
