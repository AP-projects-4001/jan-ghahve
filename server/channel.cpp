#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "channel.h"

Channel::Channel(QMutex *inp_mutex ,QObject *parent)
    : QObject{parent}
{
    path = "users.json";
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
    //if id belongs to a user
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

    //if id belongs to a group
    json_obj = read_from_file("groups.json");
    user = json_obj[id].toObject();
    if(!user.isEmpty()){
        user["id"] = id;
        user["name"] = id;
        user["status"] = "group";
        user.remove("max");
        QJsonDocument user_doc(user);
        return user_doc.toJson();
    }

    //if id belong to a channel
    json_obj = read_from_file("channels.json");
    user = json_obj[id].toObject();
    if(!user.isEmpty()){
        user["id"] = id;
        user["name"] = id;
        user["status"] = "channel";
        user.remove("max");
        QJsonDocument user_doc(user);
        return user_doc.toJson();
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
    add_contact(id1, id2, "user");

    //adding id1 to id2's contacts
    add_contact(id2, id1, "user");

    qDebug()<<data["id1"]<<"%"<<data["id2"]<<" want to unLock the file";
    //---- UnLock -----
    ch_mutex->unlock();
    qDebug()<<data["id1"]<<"%"<<data["id2"]<<" unLocked the file";
}

QByteArray Channel::get_chat_info(QString id1, QString id2, QString chat)
{
    qDebug()<<id1<<"%"<<id2<<" want to Lock the file";
    //----LOCK ----
    ch_mutex->lock();
    qDebug()<<id1<<"%"<<id2<<" Locked the file";

    QString file_path;
    QJsonObject chat_obj;

    if(chat.isEmpty()){
        //file name can be : id1 + "%" + id2 + ".json"
        file_path = id1 + "%" + id2 + ".json";

        chat_obj = read_from_file(file_path);

        if(chat_obj.empty()) //the file didn't open
        {
            //file name can be : id2 + "%" + id1 + ".json"
            file_path = id2 + "%" + id1 + ".json";
            chat_obj = read_from_file(file_path);
        }
    }
    else{
        file_path = id2 + ".json";
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
QByteArray Channel::get_all_contacts()
{
    //----LOCK ----
    ch_mutex->lock();
    QJsonObject contacts = read_from_file("contacts.json");
    QJsonDocument contacts_doc(contacts);
    //---- UnLock -----
    ch_mutex->unlock();
    return contacts_doc.toJson();
}

QString Channel::create_group_or_channel(QJsonObject data, QString chat)
{
    //----LOCK ----
    ch_mutex->lock();
    //modify group file
    QJsonObject all_data = read_from_file(chat + "s.json");
    QString name = data["name"].toString();
    int max = data["max"].toInt();
    if(!all_data[name].isNull()){
        //---- UnLock -----
        ch_mutex->unlock();
        return "not accepted";
    }
    data.remove("name");
    all_data[name] = data;
    write_to_file(chat + "s.json", all_data);

    //modify contacts file
    QString id1;
    for(int i=1;i<=max;i++){
        id1 = data[QString::number(i)].toString();
        add_contact(id1, name, chat);
    }
    //---- UnLock -----
    ch_mutex->unlock();
    return "accepted";
}

void Channel::add_contact(QString id1, QString id2, QString status)
{
    QJsonObject contact;
    contact["id"] = id2;
    contact["status"] = status;
    QJsonObject contacts_obj;
    contacts_obj = read_from_file("contacts.json");

    QJsonArray userContacts = contacts_obj[id1].toArray();
    QJsonObject user;
    bool contact_found = false;
    for(QJsonValueRef userRef:userContacts){
        user = userRef.toObject();
        if(user["id"].toString() == id2){
            contact_found = true;
            break;
        }
    }
    if(!contact_found){
        userContacts.append(contact);
    }
    contacts_obj[id1] = userContacts;
    write_to_file("contacts.json", contacts_obj);
}

QStringList Channel::send_message_to_group_or_channel(QJsonObject data, QString chat)
{
    //----LOCK ----
    ch_mutex->lock();
    if(chat == "channel"){
        //cheking permissions
    }
    //saving message to database
    QString file_path = data["id2"].toString() + ".json";
    QJsonObject msg_obj = read_from_file(file_path);
    if(msg_obj.empty()){
        msg_obj["max"] = 0;
    }
    int max = msg_obj["max"].toInt() + 1;
    msg_obj["max"] = max;

    QString senderId = data["id1"].toString();
    QJsonObject message;
    message["sender"] = senderId;
    message["message"] = data["message"];
    msg_obj[QString::number(max)] = message;
    //new chat messages are overwritten in the database
    write_to_file(file_path, msg_obj);

    //sendig message to all online users
    msg_obj = read_from_file(chat + "s.json");
    data = msg_obj[data["id2"].toString()].toObject();
    int max_number = data["max"].toInt();
    QString id;
    QStringList all_ids;
    for(int i=1;i<=max_number; i++){
        id = data[QString::number(i)].toString();
        if(id == senderId)
            continue;
        all_ids.append(id);
    }
    //---- UnLock -----
    ch_mutex->unlock();
    return all_ids;
}

//------------------ Working with Files ------------------
void Channel::write_to_file(QString file_path, QJsonObject result)
{
    //Encoding
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
        //Decoding
        file.close();
        QJsonDocument json_doc = QJsonDocument::fromJson(b);
        json_obj = json_doc.object();
    }
    return json_obj;
}
