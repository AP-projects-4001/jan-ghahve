#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "authentication.h"

Authentication::Authentication(QObject *parent)
    : QObject{parent}
{
    path = "database.json";
}

QString Authentication::signup(QJsonObject data)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
        return "";
    QByteArray b = file.readAll();
    file.close();

    bool username_uique = true;
    QJsonDocument json_doc = QJsonDocument::fromJson(b);
    QJsonObject json_obj = json_doc.object();
    QJsonArray json_arr = json_obj["user"].toArray();
    QJsonObject user;
    for(QJsonValueRef user_ref:json_arr){
        user = user_ref.toObject();
        if(user["username"] == data["username"]){
            username_uique = false;
            break;
        }
    }
    if(!username_uique)
        return "Username already taken";

    json_arr.append(data);
    QJsonDocument result(json_arr);
    QFile file2(path);
    file2.open(QIODevice::WriteOnly);
    file2.write(result.toJson());
    file2.close();
    return "accepted";
}
