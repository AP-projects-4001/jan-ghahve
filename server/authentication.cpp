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
    //Reading all data from Database

    QFile file(path);
    QJsonArray json_arr;
    if(file.open(QIODevice::ReadOnly)){
        QByteArray b = file.readAll();
        file.close();

        QJsonDocument json_doc = QJsonDocument::fromJson(b);
        QJsonObject json_obj = json_doc.object();
        json_arr = json_obj["users"].toArray();
    }

    //Search for similar username(id) in Database(is username uniqe or not)
    bool username_uique = true;
    QJsonObject user;
    for(QJsonValueRef user_ref:json_arr)
    {
        user = user_ref.toObject();
        if(user["id"] == data["id"]){
            username_uique = false;
            break;
        }
    }
    if(!username_uique)
        return "Username already taken";

    //Append new user's data, in Database
    json_arr.append(data);
    QJsonObject result;
    result["users"] = json_arr;
    QJsonDocument result_doc(result);
    QFile file2(path);
    file2.open(QIODevice::WriteOnly);
    file2.write(result_doc.toJson());
    file2.close();
    return "accepted";
}

QString Authentication::signin(QJsonObject data)
{
    //Reading all data from Database
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

    //Compare input data with Database
    QJsonObject user;
    for(QJsonValueRef user_ref:json_arr)
    {
        user = user_ref.toObject();
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
