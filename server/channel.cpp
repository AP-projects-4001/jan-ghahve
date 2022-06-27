#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "channel.h"

Channel::Channel(QMutex *inp_mutex ,QObject *parent)
    : QObject{parent}
{
    path = "database.json";
    this->ch_mutex = inp_mutex;
}

//returns "Username already taken" or "accepted"
QString Channel::signup(QJsonObject data) //data = new user data
{
    qDebug()<<data["id"]<<" want to Lock the file";
    //----LOCK ----
    ch_mutex->lock();
    qDebug()<<data["id"]<<" Locked the file";

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
    {
        qDebug()<<data["id"]<<" want to unLock the file";
        //---- UnLock -----
        ch_mutex->unlock();
        qDebug()<<data["id"]<<" unLocked the file";

        return "Username already taken";
    }


    user["id"] = data["id"];
    user["password"] = data["password"];
    json_arr.append(user);
    data.remove("password");
    profiles_arr.append(data);

    QJsonObject result;
    result["users"] = json_arr;
    result["profiles"] = profiles_arr;

    write_to_file(path, result);
    qDebug()<<data["id"]<<" want to unLock the file";
    //---- UnLock -----
    ch_mutex->unlock();
    qDebug()<<data["id"]<<" unLocked the file";
    return "accepted";
}

//returns status of signing in: "accepted login", "incorrect username" or "incorrect password"
QString Channel::signin(QJsonObject data)
{
    qDebug()<<data["id"]<<" want to Lock the file";
    //----LOCK ----
    ch_mutex->lock();
    qDebug()<<data["id"]<<" Locked the file";

    QJsonArray json_arr;
    QJsonObject json_obj;
    json_obj = read_from_file(path);
    json_arr = json_obj["users"].toArray();

    QJsonObject user;
    for(QJsonValueRef user_ref:qAsConst(json_arr))
    {
        user = user_ref.toObject();
        if((user["id"] == data["id"])&&(user["password"]==data["password"]))
        {
            qDebug()<<data["id"]<<" want to unLock the file";
            //---- UnLock -----
            ch_mutex->unlock();
            qDebug()<<data["id"]<<" unLocked the file";
            return "accepted";
        }
        else if((user["id"] == data["id"])&&(user["password"]!=data["password"]))
        {
            qDebug()<<data["id"]<<" want to unLock the file";
            //---- UnLock -----
            ch_mutex->unlock();
            qDebug()<<data["id"]<<" unLocked the file";
            return "incorrect password";
        }
    }
    qDebug()<<data["id"]<<" want to unLock the file";
    //---- UnLock -----
    ch_mutex->unlock();
    qDebug()<<data["id"]<<" unLocked the file";
    return "incorrect Username";
}

//Getting profile data from database
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
        //Find id in the whole database, then return it
        if(user["id"] == id)
        {
            QJsonDocument temp_doc(user);
            return temp_doc.toJson();
        }
    }
    return 0;
}

//append new message in the messages databse
void Channel::send_message(QJsonObject data)
{
    qDebug()<<data["id1"]<<"%"<<data["id2"]<<" want to Lock the file";
    //----LOCK ----
    ch_mutex->lock();
    qDebug()<<data["id1"]<<"%"<<data["id2"]<<" Locked the file";

    //--------- at first open pv-chat file ---------
    //file name can be : id1 + "%" + id2 + ".json"
    QString file_path = data["id1"].toString() + "%" + data["id2"].toString() + ".json";
    QJsonObject msg_obj;
    QJsonArray msg_arr;

    msg_obj = read_from_file(file_path);

    if(msg_obj.empty())//the file didn't open
    {
        //file name can be : id2 + "%" + id1 + ".json"
        file_path = data["id2"].toString() + "%" + data["id1"].toString() + ".json";
        msg_obj = read_from_file(file_path);
    }
    //chat messages loaded to msg_obj

    msg_arr = msg_obj["messages"].toArray();
    QJsonObject message;
    message["sender"] = data["id1"];
    message["message"] = data["message"];
    //append new message in the messages array
    msg_arr.append(message);

    //new chat messages(msg_arr) are overwritten in the database
    QJsonObject result;
    result["messages"] = msg_arr;
    write_to_file(file_path, result);

    qDebug()<<data["id1"]<<"%"<<data["id2"]<<" want to unLock the file";
    //---- UnLock -----
    ch_mutex->unlock();
    qDebug()<<data["id1"]<<"%"<<data["id2"]<<" unLocked the file";
}

QByteArray Channel::get_chat_info(QString id1, QString id2)
{
    qDebug()<<id1<<"%"<<id2<<" want to Lock the file";
    //----LOCK ----
    ch_mutex->lock();
    qDebug()<<id1<<"%"<<id2<<" Locked the file";

    //file name can be : id1 + "%" + id2 + ".json"
    QString file_path = id1 + "%" + id2 + ".json";
    QJsonObject chat_obj;

    chat_obj = read_from_file(file_path);

    if(chat_obj.empty()) //the file didn't open
    {
        //file name can be : id2 + "%" + id1 + ".json"
        file_path = id2 + "%" + id1 + ".json";
        chat_obj = read_from_file(file_path);
    }
    //return whole chat messages
    QJsonDocument chat_doc(chat_obj);

    qDebug()<<id1<<"%"<<id2<<" want to unLock the file";
    //---- UnLock -----
    ch_mutex->unlock();
    qDebug()<<id1<<"%"<<id2<<" unLocked the file";

    return chat_doc.toJson();
}


QByteArray Channel::get_all_info()
{
    QJsonObject data_obj;
    data_obj = read_from_file(path);
    data_obj.remove("users");
    QJsonDocument data_doc(data_obj);
    return data_doc.toJson();
}

//------------------ Working with Files ------------------
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
