#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTcpSocket>
#include <QtCore>
#include "mainwindow.h"
#include "mythread.h"
#include "ui_mainwindow.h"
#include "search.h"
#include "adding_member.h"
#include "geraph.h"

MainWindow::MainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
      pain();
//    QObject::connect(ui->test,&QPushButton::clicked,this,&MainWindow::add_safebar);
      QObject::connect(ui->actionNew_Group,&QAction::triggered,this,&MainWindow::on_newgroup_clicked);
      QObject::connect(ui->actionlvl_3_graph,&QAction::triggered,this,&MainWindow::on_graph_clicked);

    client = new MyClient();
    get_user_info(id);
    ui->user_name->setText(user_data["name"].toString());
    get_user_contacts();
    client->disconnect_from_server();
    MyThread* thread = new MyThread(user_data["id"].toString());
    QObject::connect(thread, &MyThread::message_recieved1, this, &MainWindow::on_messagerecievd1);
    thread->start();

    client = new MyClient();
    client->connect_to_server();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::get_user_info(QString id)
{
    QJsonObject request;
    request["status"] = "userInfo";
    request["id"] = id;
    QJsonDocument request_d(request);
    QByteArray request_b = request_d.toJson();

    if(client->connect_to_server()){
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        user_data = response_d.object();
    }
}

void MainWindow::get_all_users()
{
    if(client->is_client_connectd()){
        QJsonObject request;
        request["status"] ="allUsersInfo";
        QJsonDocument requset_doc(request);
        QByteArray request_b=requset_doc.toJson();
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);

        all_users = response_d.object();
        QJsonArray data_arr = all_users["profiles"].toArray();

        QJsonObject user;
        for(QJsonValueRef user_ref:data_arr){
            user = user_ref.toObject();
            if(user["id"] == user_data["id"])
                continue;
            add_item_to_listwidget(user["id"].toString());
        }

    }
}

void MainWindow::get_user_contacts()
{
    QJsonObject req;
    req["status"] = "userContacts";
    req["id"] = user_data["id"];
    QJsonDocument req_doc(req);
    QByteArray req_b = req_doc.toJson();
    if(client->is_client_connectd()){
        QByteArray response_b = client->request_to_server(&req_b);
        QJsonDocument respones_d = QJsonDocument::fromJson(response_b);
        QJsonArray response_arr = respones_d.array();

        QFile file(user_data["id"].toString() + "%contacts.txt");
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);

        QJsonObject user;
        QString status, id;
        for(QJsonValueRef userRef:response_arr){
            user = userRef.toObject();
            status = user["status"].toString();
            id = user["id"].toString();
            add_item_to_listwidget(id);
            if(status == "user"){
                stream << id << ',';
            }
            else if(status == "group"){
                stream << "g%" << id << ',';
            }
            else if(status == "channel"){
                stream << "c%" << id << ',';
            }
        }
        file.close();
    }
}

bool MainWindow::is_already_added(QString id)
{
    QListWidget* list = ui->listWidget;
    QList<QListWidgetItem *> items = list->findItems(id, Qt::MatchExactly);
    if(items.size() > 0)
        return true;
    return false;
}

void MainWindow::add_item_to_listwidget(QString name)
{
    QListWidget* list = ui->listWidget;
    QListWidgetItem* item = new QListWidgetItem(name);
    list->addItem(item);
}
void MainWindow::get_allUsers_contacts()
{
    QJsonObject req;
    req["status"] = "allUsersContacts";
    QJsonDocument req_doc(req);
    QByteArray req_b = req_doc.toJson();
    if(client->is_client_connectd()){
        QByteArray response_b = client->request_to_server(&req_b);
        QJsonDocument respones_d = QJsonDocument::fromJson(response_b);
        QJsonObject response_obj = respones_d.object();
        this->all_users_contacts = response_obj;
    }
}

void MainWindow::on_usersFound(QStringList users)
{
    QFile file(user_data["id"].toString() + "%contacts.txt");
    file.open(QIODevice::Append);
    QTextStream stream(&file);

    QJsonObject user;
    QString user_id;
    for(QString id:users){
        user_id = user_data["id"].toString();
        if(id == user_id || is_already_added(id))
            continue;
        add_item_to_listwidget(id);
        stream << id << ',';
    }
    file.close();
}

void MainWindow::pain()
{
    ui->pbn_send->setStyleSheet("*{border-image: url(:/images/resourses/send.png);"
                                "background-color:rgb(191, 215, 234);"
                                "border-radius: 20%;"
                                "background-position:center;}"
                                "*:hover{background-color:rgb(30, 157, 230);}");
    ui->menubar->resize(20,5);
    ui->menubar->setStyleSheet(
                               "font-size:15px;"

                               );

    ui->menusetting->setStyleSheet("background-color:rgb(253, 240, 213);"
                                   "border-radius: 20%;"
                                   );
    ui->listWidget->setStyleSheet("QListWidget:item"
                                 "{"
                                    "height: 30px;"
                                    "font-weight: bold;"
                                    "border: 1px solid gray;"
                                    "border-right:hide;"
                                    "border-left:hide;"
                                    "border-top:hide"
                                 "}"
                                 "QListWidget:item:selected"
                                  "{"
                                       "background-color:rgb(213,213,213);"
                                       "border: 1px solid gray;"
                                       "border-right:hide;"
                                       "border-left:hide;"
                                       "border-top:hide;"
                                       "color:black;"
                                  "}"

                                 "*{"
                                  "border:hide;"
                                  "font-size:18px;"
                                  "background-color:rgb(240, 240, 240);;"
                                  "border-radius:8px;"
                                  "font:url(:/new/prefix1/Lato-Italic.ttf)"
                                  "}"
                                  );
}
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->ted_message->setReadOnly(false);
    QString id = item->text();
    QJsonObject request;
    request["status"] = "userInfo";
    request["id"] = id;
    QJsonDocument request_doc(request);
    QByteArray request_b = request_doc.toJson();

    if(client->is_client_connectd())
    {
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        contact_info = response_d.object();
        ui->contact_name->setText(contact_info["name"].toString());
    }
    ui->ted_chat->clear();

    //get chat
    QJsonObject chat;
    QJsonObject chatReq;
    chatReq["status"] = "chatInfo";
    chatReq["id1"] = user_data["id"];
    chatReq["id2"] = contact_info["id"];
    chatReq["chat"] = contact_info["status"];
    QJsonDocument chatReq_d(chatReq);
    QByteArray chatReq_b = chatReq_d.toJson();
    if(client->is_client_connectd()){
        QByteArray response = client->request_to_server(&chatReq_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        chat = response_d.object();
        QString message;
        int max = chat["max"].toInt();
        for(int i=1; i<=max; i++){
            message = chat[QString::number(i)].toObject()["sender"].toString() + \
                    ":" + chat[QString::number(i)].toObject()["message"].toString();
            ui->ted_chat->append(message);
        }
//        QFile file(user_data["id"].toString() + "%" + contact_info["id"].toString());
//        file.open(QIODevice::WriteOnly);
//        file.write(response);
//        file.close();
    }
}


void MainWindow::on_messagerecievd1(QString senderId, QString message, QString chatId)
{
    if(senderId == contact_info["id"].toString() || chatId == contact_info["id"].toString()){
        ui->ted_chat->append(senderId + ":" + message);
    }else{
        QFile file(user_data["id"].toString() + "%contacts.txt");
        file.open(QIODevice::Append);
        QTextStream stream(&file);
        if(!is_already_added(senderId)){
            add_item_to_listwidget(senderId);
            stream << senderId << ',';
        }
        file.close();
    }
}


void MainWindow::on_pbn_send_clicked()
{
    //    send message
    QString message_content = ui->ted_message->toPlainText();
    ui->ted_message->clear();
    QJsonObject message;
    if(contact_info["status"].toString() == "group"){
        message["status"] = "messageToGroup";
    }else{
        message["status"] = "message";
    }
    message["id1"] = user_data["id"];
    message["id2"] = contact_info["id"];
    message["message"] = message_content;
    QJsonDocument message_d(message);
    QByteArray message_b = message_d.toJson();

    if(client->is_client_connectd())
    {
        client->request_to_server(&message_b);
    }
    ui->ted_chat->append(user_data["id"].toString()+":" + message_content);
}

void MainWindow::on_newgroup_clicked()
{
    adding_member* add_member = new adding_member(user_data["id"].toString(), client, this);
    connect(add_member, &adding_member::group_created, this, &MainWindow::on_groupcreated);
    add_member->show();
}

void MainWindow::on_graph_clicked()
{

    QString myUsername;
    myUsername = user_data["id"].toString();
    get_allUsers_contacts();
    geraph *graph_window = new geraph(myUsername,all_users_contacts);
    graph_window->show();
}

void MainWindow::on_pbn_search_clicked()
{
    search* search_dialog = new search(client,this);
    connect(search_dialog, &search::users_found, this, &MainWindow::on_usersFound);
    search_dialog->show();
}

void MainWindow::on_groupcreated(QString id)
{
    QFile file(user_data["id"].toString() + "%contacts.txt");
    file.open(QIODevice::Append);
    QTextStream stream(&file);
    stream << "g%" << id << ',';
    file.close();
    add_item_to_listwidget(id);
}

