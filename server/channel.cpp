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
    QString id1 = data["id1"].toString(), id2=data["id2"].toString();
    QString file_path = id1+ "%" + id2 + ".json";
    QJsonObject msg_obj;
    QJsonArray msg_arr;

    msg_obj = read_from_file(file_path);

    if(msg_obj.empty())//the file didn't open
    {
        //file name can be : id2 + "%" + id1 + ".json"
        file_path = id2 + "%" +id1 + ".json";
        msg_obj = read_from_file(file_path);
    }
    //chat messages loaded to msg_obj

    if(msg_obj.empty()){
        msg_obj["max"] = 0;
    }
    int max = msg_obj["max"].toInt() + 1;
    msg_obj["max"] = max;

    QJsonObject message;
    message["sender"] = data["id1"];
    message["message"] = data["message"];
    msg_obj[QString::number(max)] = message;

    //new chat messages(msg_arr) are overwritten in the database
    write_to_file(file_path, msg_obj);

    //modifying contacts file
    // adding id2 to id1's contacts
    QJsonObject contact;
    contact["id"] = id2;
    contact["status"] = "user";
    QJsonObject contacts_obj;
    contacts_obj = read_from_file("contacts.json");

    QJsonArray userContacts = contacts_obj[id1].toArray();
    QJsonObject user;
    bool contact2_found = false, contact1_found = false;
    for(QJsonValueRef userRef:userContacts){
        user = userRef.toObject();
        if(user["id"].toString() == id2){
            contact2_found = true;
            break;
        }
    }
    if(!contact2_found){
        userContacts.append(contact);
    }
    contacts_obj[id1] = userContacts;

    //adding id1 to id2's contacts
    contact["id"] = id1;
    userContacts = contacts_obj[id2].toArray();
    for(QJsonValueRef userRef:userContacts){
        user = userRef.toObject();
        if(user["id"].toString() == id1){
            contact1_found = true;
            break;
        }
    }
    if(!contact1_found){
        userContacts.append(contact);
    }
    contacts_obj[id2] = userContacts;

    //writing contacts to file
    write_to_file("contacts.json", contacts_obj);

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

QByteArray Channel::get_user_contacts(QString id)
{
    //----LOCK ----
    ch_mutex->lock();

    QJsonObject all_contacts = read_from_file("contacts.json");
    QJsonArray userContacts = all_contacts[id].toArray();
    QJsonDocument userContacts_doc(userContacts);
    //---- UnLock -----
    ch_mutex->unlock();
    return userContacts_doc.toJson();
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
