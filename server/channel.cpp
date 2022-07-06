#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include "channel.h"
#include "Encryption/myencryption.h"

Channel::Channel(QMutex *inp_mutex ,QObject *parent)
    : QObject{parent}
{
    path = "users.json";
    this->ch_mutex = inp_mutex;
}

QString Channel::signup_check(QJsonObject data)
{
    //----LOCK ----
    ch_mutex->lock();
    QJsonArray json_arr;
    QJsonObject json_obj;

    json_obj = read_from_file(path);
    json_arr = json_obj["profiles"].toArray();

    bool username_uique = true, number_uique = true, email_uique = true;
    QJsonObject user;
    for(QJsonValueRef user_ref:qAsConst(json_arr))
    {
        user = user_ref.toObject();
        if(user["id"] == data["id"]){
            username_uique = false;
            break;
        }
        if(user["number"] == data["number"]){
            number_uique = false;
            break;
        }
        else if(user["email"]==data["email"])
        {
            email_uique = false;
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
    if(!number_uique)
    {
        qDebug()<<data["id"]<<" want to unLock the file";
        //---- UnLock -----
        ch_mutex->unlock();
        qDebug()<<data["id"]<<" unLocked the file";

        return "PhoneNumber already taken";
    }
    if(!email_uique)
    {
        qDebug()<<data["id"]<<" want to unLock the file";
        //---- UnLock -----
        ch_mutex->unlock();
        qDebug()<<data["id"]<<" unLocked the file";

        return "Email already taken";
    }

    //checking in groups
    json_obj = read_from_file("groups.json");
    QString name = json_obj["id"].toString();
    if(!json_obj[name].isNull()){
        //---- UnLock -----
        ch_mutex->unlock();
        return "Username already taken";
    }

    //checking in channels
    json_obj = read_from_file("channels.json");
    if(!json_obj[name].isNull()){
        //---- UnLock -----
        ch_mutex->unlock();
        return "Username already taken";
    }

    //---- UnLock -----
    ch_mutex->unlock();
    return "accepted";
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

    QJsonObject user;   
    user["id"] = data["id"];
    user["password"] = data["password"];
    json_arr.append(user);
    data.remove("password");
    QJsonValue img = data["img"];
    data.remove("img");
    data["permissions"] = "";
    profiles_arr.append(data);

    QJsonObject result;
    result["users"] = json_arr;
    result["profiles"] = profiles_arr;

    write_to_file(path, result);

    QJsonObject images_json_obj = read_from_file("images.json");
    //qDebug()<<"-------BEFORE WRITE IN FILE ---------";
    //qDebug()<<"image : "<<img;
    images_json_obj[user["id"].toString()] = img;
    write_to_file("images.json",images_json_obj);

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
QJsonObject Channel::get_info(QString id)
{
    //----LOCK ----
    ch_mutex->lock();
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
            //---- UnLock -----
            ch_mutex->unlock();
            return user;
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
        //---- UnLock -----
        ch_mutex->unlock();
        return user;
    }

    //if id belong to a channel
    json_obj = read_from_file("channels.json");
    user = json_obj[id].toObject();
    if(!user.isEmpty()){
        user["id"] = id;
        user["name"] = id;
        user["status"] = "channel";
        user.remove("max");
        //---- UnLock -----
        ch_mutex->unlock();
        return user;
    }
    //---- UnLock -----
    ch_mutex->unlock();
    return user;
}

QByteArray Channel::get_info_forEdit(QString id)
{
    QJsonObject json_obj;
    //if id belongs to a user
    QJsonArray profiles_arr;
    QJsonArray users_arr;
    json_obj = read_from_file(path);
    profiles_arr = json_obj["profiles"].toArray();
    users_arr = json_obj["users"].toArray();
    QJsonObject user;
    for(QJsonValueRef user_ref:qAsConst(profiles_arr))
    {
        user = user_ref.toObject();
        //Find id in the whole database, then return it
        if(user["id"] == id)
        {
            QJsonObject user2;
            for(QJsonValueRef user_ref:qAsConst(users_arr))
            {
                user2 = user_ref.toObject();
                //Find id in the whole database, then return it
                if(user2["id"] == id)
                {
//                    QJsonObject pass;
//                    pass["password"] = user2["password"].toString();
                    user["password"] = user2["password"];
                    break;
                }
            }
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

QStringList Channel::create_group_or_channel(QJsonObject data, QString chat)
{
    //----LOCK ----
    ch_mutex->lock();
    //modify group file
    QJsonObject all_data;
    QString name = data["name"].toString();
    QStringList all_ids;

    //check in users
    QJsonArray json_arr;
    all_data = read_from_file(path);
    json_arr = all_data["users"].toArray();
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
    if(!username_uique){
        //---- UnLock -----
        ch_mutex->unlock();
        return all_ids;
    }

    //check in groups and channels
    if(chat == "group")
        all_data = read_from_file("channels.json");
    else
        all_data = read_from_file("groups.json");

    if(!all_data[name].isNull()){
        //---- UnLock -----
        ch_mutex->unlock();
        return all_ids;
    }

    all_data = read_from_file(chat + "s.json");
    int max = data["max"].toInt();
    if(!all_data[name].isNull()){
        //---- UnLock -----
        ch_mutex->unlock();
        return all_ids;
    }


    //---- set image in database(images.json)
    QJsonValue img = data["img"];
    data.remove("img");
    QJsonObject images_json_obj = read_from_file("images.json");
    images_json_obj[data["name"].toString()] = img;
    write_to_file("images.json",images_json_obj);
    data.remove("name");

    all_data[name] = data;
    all_data["admins"] = "";
    write_to_file(chat + "s.json", all_data);

    //modify contacts file
    QString id1;
    for(int i=1;i<=max;i++){
        id1 = data[QString::number(i)].toString();
        add_contact(id1, name, chat);
        all_ids.append(id1);
    }
    //---- UnLock -----
    ch_mutex->unlock();
    return all_ids;
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

QByteArray Channel::channelInfo(QString id)
{
    QJsonObject channels = read_from_file("channels.json");
    QJsonObject data_obj = channels[id].toObject();
    QJsonDocument data_doc(data_obj);
    return data_doc.toJson();
}

void Channel::modify_channel_admins(QString id, QString admins)
{
    //----LOCK ----
    ch_mutex->lock();
    QJsonObject data_obj = read_from_file("channels.json");
    QJsonObject channel_obj = data_obj[id].toObject();
    channel_obj["admins"] = admins;
    data_obj[id] = channel_obj;
    write_to_file("channels.json", data_obj);
    //---- UnLock -----
    ch_mutex->unlock();
}

QByteArray Channel::get_profile_image(QJsonObject data)
{
    QJsonObject json_obj = read_from_file("images.json");
    QJsonValue img_val = json_obj[data["id"].toString()];
    QJsonObject out_data;
    out_data[data["id"].toString()] = img_val;
    QJsonDocument out_doc(out_data);
    QByteArray out_b = out_doc.toJson();
    return out_b;

}

QString Channel::edit_profile(QJsonObject data)
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
    //-------------permissions shoud add----------
    bool number_uique = true;
    bool email_uique = true;
    QJsonObject user;
    for(QJsonValueRef user_ref:qAsConst(profiles_arr))
    {
        user = user_ref.toObject();
        if( (user["number"] == data["number"]) && (user["id"]!=data["id"]) ){
            number_uique = false;
            break;
        }
        else if( (user["email"]==data["email"]) && (user["id"]!=data["id"]) )
        {
            email_uique = false;
            break;
        }
    }
    if(!number_uique)
    {
        qDebug()<<data["id"]<<" want to unLock the file";
        //---- UnLock -----
        ch_mutex->unlock();
        qDebug()<<data["id"]<<" unLocked the file";

        return "PhoneNumber already taken";
    }
    if(!email_uique)
    {
        qDebug()<<data["id"]<<" want to unLock the file";
        //---- UnLock -----
        ch_mutex->unlock();
        qDebug()<<data["id"]<<" unLocked the file";

        return "Email already taken";
    }


    int location = 0;
    for(QJsonValueRef user_ref:qAsConst(profiles_arr))
    {
        user = user_ref.toObject();
        if(user["id"] == data["id"])
            break;
        else
            location++;
    }
    json_arr.removeAt(location);
    profiles_arr.removeAt(location);
    QJsonObject temp;
    temp["id"] = data["id"];
    temp["password"] = data["password"];
    json_arr.append(temp);
    data.remove("password");
    QJsonValue img_val = data["img"];
    data.remove("img");
    profiles_arr.append(data);

    QJsonObject result;
    result["users"] = json_arr;
    result["profiles"] = profiles_arr;
    write_to_file(path, result);

    QJsonObject images_json_obj = read_from_file("images.json");
    images_json_obj[user["id"].toString()] = img_val;
    write_to_file("images.json",images_json_obj);

    qDebug()<<data["id"]<<" want to unLock the file";
    //---- UnLock -----
    ch_mutex->unlock();
    qDebug()<<data["id"]<<" unLocked the file";
    return "accepted";
}

QString Channel::channel_group_profile_edited(QJsonObject data)
{
    qDebug()<<data["id"]<<" want to Lock the file";
    //----LOCK ----
    ch_mutex->lock();
    qDebug()<<data["id"]<<" Locked the file";

    QJsonObject images_json_obj = read_from_file("images.json");
    images_json_obj[data["id"].toString()] = data["img"];
    write_to_file("images.json",images_json_obj);

    qDebug()<<data["id"]<<" want to unLock the file";
    //---- UnLock -----
    ch_mutex->unlock();
    qDebug()<<data["id"]<<" unLocked the file";
    return "accepted";

}

void Channel::edit_message(QJsonObject data)
{
    //----LOCK ----
    ch_mutex->lock();
    //if it's a pv chat
    QString id1 = data["sender"].toString();
    QString id2 = data["id2"].toString();
    QString file_path;
    QJsonObject msg_obj;
    if(data["chat"].toString() == "pv"){
        file_path = id1+ "%" + id2 + ".json";

        msg_obj = read_from_file(file_path);

        if(msg_obj.empty())//the file didn't open
        {
            //file name can be : id2 + "%" + id1 + ".json"
            file_path = id2 + "%" +id1 + ".json";
            msg_obj = read_from_file(file_path);
        }
    }else{
        file_path = id1 + ".json";
        msg_obj = read_from_file(file_path);
    }

    int index = data["index"].toInt();
    if(data["message"].isNull()){
        msg_obj.remove(QString::number(index));
    }
    else{
        QJsonObject message;
        message["message"] = data["message"];
        message["sender"] = data["sender"];
        msg_obj[QString::number(index)] = message;
    }
    write_to_file(file_path, msg_obj);
    //---- UnLock -----
    ch_mutex->unlock();
}

//------------------ Working with Files ------------------
void Channel::write_to_file(QString file_path, QJsonObject result)
{
    QJsonDocument result_doc(result);
    QFile file(file_path);
    file.open(QIODevice::WriteOnly);
    QByteArray data_b = result_doc.toJson();
    //Encoding
    MyEncryption *encryption = new MyEncryption();;
    QByteArray encoded_Data = encryption->myEncode(data_b);
    file.write(encoded_Data);
    delete encryption;
    file.close();
}

QJsonObject Channel::read_from_file(QString file_path)
{
    QFile file(file_path);
    QJsonObject json_obj;
    if(file.open(QIODevice::ReadOnly)){
        QByteArray encoded_data = file.readAll();
        file.close();
        MyEncryption *encryption = new MyEncryption();;
        QByteArray decoded_Data = encryption->myDecode(encoded_data);
        delete encryption;
        //Decoding
        QJsonDocument json_doc = QJsonDocument::fromJson(decoded_Data);
        json_obj = json_doc.object();
    }
    return json_obj;
}
