#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTcpSocket>
#include <QtCore>
#include <QIcon>
#include <QMenu>
#include <QSpacerItem>
#include <QScrollBar>
#include "mainwindow.h"
#include "mythread.h"
#include "ui_mainwindow.h"
#include "search.h"
#include "adding_member.h"
#include "geraph.h"
#include "groupprofile.h"
#include "profile.h"
#include "setting.h"

MainWindow::MainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    pain();
    QObject::connect(ui->actionNew_Group,&QAction::triggered,this,&MainWindow::on_newgroup_clicked);
    QObject::connect(ui->actionlvl_3_graph,&QAction::triggered,this,&MainWindow::on_graph_clicked);
    connect(ui->actionNew_Channel,&QAction::triggered,this,&MainWindow::on_newchannel_clicked);
    QObject::connect(ui->actionprofile,&QAction::triggered,this,&MainWindow::on_setting_clicked);

    client = new MyClient();
    get_user_info(id);
    get_user_contacts();
    ui->user_name->setText(user_data["id"].toString());

    MyThread* thread = new MyThread(user_data["id"].toString());
    QObject::connect(thread, &MyThread::message_recieved, this, &MainWindow::on_messagerecievd);
    QObject::connect(thread, &MyThread::group_created, this, &MainWindow::on_groupcreated);
    QObject::connect(thread, &MyThread::user_authenticated, this, &MainWindow::on_userauthenticated);
    QObject::connect(thread, &MyThread::user_unauthenticated, this, &MainWindow::on_userunauthenticated);
    thread->start();



    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/images/resourses/logo.png"));
    tray->setVisible(true);

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

bool MainWindow::is_admin(QString id, QStringList admins_list)
{
    for(QString user:admins_list){
        if(id == user)
            return true;
    }
    return false;
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
                                "background-color:rgb(1, 125, 180);"
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
                                  "background-color:rgb(253, 240, 213);"
                                  "border-radius:8px;"
                                  "font:url(:/new/prefix1/Lato-Italic.ttf)"
                                  "}"
                                  );
}

void MainWindow::add_message(bool flag, QString sender, QString message)
{
    //flag sarfan baraye test bode mitoni baresh dari va chiz dg bezari ya inke kol tabe yeja dg copypaste koni
    QWidget* container = new QWidget(ui->scrollAreaWidgetContents);
//    ui->scrollAreaWidgetContents->layout()->spacerItem().
    container->setStyleSheet("background: transparent;");
    container->setAutoFillBackground(false);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QHBoxLayout* containerLayout = new QHBoxLayout();
    QListWidget* massage = new QListWidget();
    massage->setAutoFillBackground(false);

    //payam ro to add item mitoni benevisi

//    massage->addItem("Name");
    massage->setWordWrap(true);
//    QListWidgetItem *item = new QListWidgetItem;
    massage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    massage->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    massage->addItem(sender+ "\n" + message);
    massage->setMaximumWidth(270);



    if(flag)
    {
        //age khod karbar msg ro dad in if biad
        massage->setStyleSheet("background-color:rgb(255, 238, 221);");
        containerLayout->addStretch(0);
        containerLayout->addWidget(massage);
        //flag ++;
    }else
    {
        //age kesi dg msg ro dad in if biad
        massage->setStyleSheet("background-color:rgb(189, 244, 255);");
        containerLayout->addWidget(massage);
        containerLayout->addStretch(0);
    }

    container->setLayout(containerLayout);
    ui->scrollAreaWidgetContents->layout()->addWidget(container);
    ui->scrollAreaWidgetContents->setStyleSheet("QWidget#scrollAreaWidgetContents"
                                                "{"
                                                "background-color:rgb(0, 94, 140);"
                                                "}"
                                                "QListWidget"
                                                "{"
                                                "color:black;"
                                                "}"
                                                "*{border-radius:10px; "
                                                "background-color: palette(base);"
                                                "font-size:16px;}");
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

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
    QLayoutItem *child;
    int i = ui->scrollAreaWidgetContents->layout()->count();
    while (i>1) {
        child = ui->scrollAreaWidgetContents->layout()->itemAt(1);
        ui->scrollAreaWidgetContents->layout()->removeItem(child);
        delete child->widget();
        delete child;
        i--;
    }



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
        QString message, sender;
        int max = chat["max"].toInt();
        bool flag;
        for(int i=1; i<=max; i++){
            sender = chat[QString::number(i)].toObject()["sender"].toString();
            message = chat[QString::number(i)].toObject()["message"].toString();
            if(sender == user_data["id"].toString())
                flag = true;
            else
                flag = false;
            add_message(flag, sender, message);
        }
        int a = ui->scrollArea->verticalScrollBar()->maximum() + 100;
        ui->scrollArea->verticalScrollBar()->setSliderPosition(a);
    }

    QString status = contact_info["status"].toString();
    ui->pbn_profile->setEnabled(true);
    bool online = contact_info["online"].toBool();
    if(online){
        ui->lbl_status->setVisible(true);
        ui->pbn_status->setVisible(true);
    }
    else{
        ui->lbl_status->setVisible(false);
        ui->pbn_status->setVisible(false);
    }
    if(status == "channel"){
        QStringList admins_list = contact_info["admins"].toString().split('%');
        QString user_id = user_data["id"].toString();
        if(is_admin(user_id, admins_list) || user_id == contact_info["1"].toString()){
            ui->pbn_send->setEnabled(true);
            ui->ted_message->setReadOnly(false);
        }else{
            ui->pbn_send->setEnabled(false);
            ui->ted_message->setReadOnly(true);
        }
    }else{
        ui->pbn_send->setEnabled(true);
        ui->ted_message->setReadOnly(false);
    }

}


void MainWindow::on_messagerecievd(QString senderId, QString message, QString chatId)
{
    if(chatId == contact_info["id"].toString() && !chatId.isEmpty()){
        add_message(false, senderId, message);
        int a = ui->scrollArea->verticalScrollBar()->maximum() + 100;
        ui->scrollArea->verticalScrollBar()->setSliderPosition(a);
    }
    else if(senderId == contact_info["id"].toString() && chatId.isEmpty()){
        add_message(false, senderId, message);
        int a = ui->scrollArea->verticalScrollBar()->maximum() + 100;
        ui->scrollArea->verticalScrollBar()->setSliderPosition(a);
    }else{
        QFile file(user_data["id"].toString() + "%contacts.txt");
        file.open(QIODevice::Append);
        QTextStream stream(&file);
        if(!is_already_added(senderId)){
            add_item_to_listwidget(senderId);
            stream << senderId << ',';
        }
        file.close();
        tray->showMessage("Message from "+ senderId + " :", message ,QIcon(":/images/resourses/chat.png"));
    }
}


void MainWindow::on_pbn_send_clicked()
{
    //    send message
    QString message_content = ui->ted_message->text();
    ui->ted_message->clear();
    QJsonObject message;
    QString status = contact_info["status"].toString();
    if(status == "group"){
        message["status"] = "messageToGroup";
    }else if(status == "channel"){
        message["status"] = "messageToChannel";
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
    add_message(true, user_data["id"].toString(), message_content);
    int a = ui->scrollArea->verticalScrollBar()->maximum() + 100;
    ui->scrollArea->verticalScrollBar()->setSliderPosition(a);

}

void MainWindow::on_newgroup_clicked()
{
    adding_member* add_member = new adding_member(user_data["id"].toString(), client, "group", this);
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
    add_item_to_listwidget(id);
}

void MainWindow::on_newchannel_clicked()
{
    adding_member* add_member = new adding_member(user_data["id"].toString(), client, "channel", this);
    add_member->show();
}


void MainWindow::on_pbn_profile_clicked()
{
    if(contact_info["status"].isNull()){
        Profile *profile= new Profile(user_data["id"].toString(), contact_info, this);
        profile->show();
    }else{
        GroupProfile* groupProfile = new GroupProfile(contact_info["id"].toString(), this);
        groupProfile->show();
    }
}

void MainWindow::on_setting_clicked()
{

//    Profile *profile_window = new Profile(user_data["id"].toString());
//    profile_window->show();
    QJsonObject user_alldata;
    QJsonObject request;
    request["status"] = "userInfo_forEdit";
    request["id"] = user_data["id"];
    QJsonDocument request_d(request);
    QByteArray request_b = request_d.toJson();

    if(client->connect_to_server()){
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        user_alldata = response_d.object();
    }
    setting *setting_window = new setting(user_alldata);
    //this->close();
    setting_window->show();
    get_user_info(user_data["id"].toString());
}

void MainWindow::on_userauthenticated(QString id)
{
    if(id == contact_info["id"].toString()){
        ui->lbl_status->setVisible(true);
        ui->pbn_status->setVisible(true);
    }
}

void MainWindow::on_userunauthenticated(QString id)
{
    if(id == contact_info["id"].toString()){
        ui->lbl_status->setVisible(false);
        ui->pbn_status->setVisible(false);
    }
}

void MainWindow::on_pbn_search_2_clicked()
{

}

