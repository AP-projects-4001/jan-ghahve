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
#include "image_convertation.h"
#include "msg_options.h"
#include "loading.h"

MainWindow::MainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->led_search->hide();
    ui->pbn_search_2->setEnabled(false);
    setFixedSize(size());
    ui->groupBox_2->hide();
    ui->groupBox_4->hide();
    ui->scrollArea->hide();
    pain();
    QObject::connect(ui->actionNew_Group,&QAction::triggered,this,&MainWindow::on_newgroup_clicked);
    QObject::connect(ui->actionlvl_3_graph,&QAction::triggered,this,&MainWindow::on_graph_clicked);
    connect(ui->actionNew_Channel,&QAction::triggered,this,&MainWindow::on_newchannel_clicked);
    QObject::connect(ui->actionprofile,&QAction::triggered,this,&MainWindow::on_setting_clicked);
    QObject::connect(ui->actionLogout,&QAction::triggered,this,&MainWindow::on_logout_clicked);

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
//    ui->menubar->resize(20,5);
    ui->menubar->setStyleSheet(
                               "font-size:15px;"
                               );

    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->menusetting->setStyleSheet("background-color:rgb(253, 240, 213);"
//                                   "border-radius: 20%;"
//                                   );
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
    QListWidget* massage_widget = new QListWidget();
    massage_widget->setAutoFillBackground(false);

    //payam ro to add item mitoni benevisi

//    massage->addItem("Name");
    massage_widget->setWordWrap(true);
//    QListWidgetItem *item = new QListWidgetItem;
    massage_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    massage_widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    massage_widget->addItem(sender+ "\n" + message);
    massage_widget->setMaximumWidth(270);

    if(sender == user_data["id"].toString())
        connect(massage_widget, &QListWidget::itemClicked , this, &MainWindow::on_chat_message_clicked);

    if(flag)
    {
        //age khod karbar msg ro dad in if biad
        massage_widget->setStyleSheet("background-color:rgb(255, 238, 221);");
        containerLayout->addStretch(0);
        containerLayout->addWidget(massage_widget);
        //flag ++;
    }else
    {
        //age kesi dg msg ro dad in if biad
        massage_widget->setStyleSheet("background-color:rgb(189, 244, 255);");
        containerLayout->addWidget(massage_widget);
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

void MainWindow::append_message_in_database(QString message, QString senderId)
{
    QFile file(user_data["id"].toString() + "%" + contact_info["id"].toString() + ".json");
    file.open(QIODevice::ReadOnly);
    QByteArray data_b = file.readAll();
    file.close();
    QJsonDocument data_doc = QJsonDocument::fromJson(data_b);
    QJsonObject data_obj = data_doc.object();
    int number =data_obj["max"].toInt() + 1;
    data_obj["max"] = number;
    QJsonObject msg;
    msg["sender"] = senderId;
    msg["message"] = message;
    data_obj[QString::number(number)] = msg;
    QJsonDocument d_doc(data_obj);
    QFile file2(user_data["id"].toString() + "%" + contact_info["id"].toString() + ".json");
    file2.open(QIODevice::WriteOnly);
    file2.write(d_doc.toJson());
    file2.close();
}

void MainWindow::get_chat()
{
    //clearing chat page
    QLayoutItem *child;
    int i = ui->scrollAreaWidgetContents->layout()->count();
    while (i>1) {
        child = ui->scrollAreaWidgetContents->layout()->itemAt(1);
        ui->scrollAreaWidgetContents->layout()->removeItem(child);
        delete child->widget();
        delete child;
        i--;
    }

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
        QFile file(user_data["id"].toString() + "%" + contact_info["id"].toString() + ".json");
        file.open(QIODevice::WriteOnly);
        file.write(response);
        file.close();
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        chat = response_d.object();
        QString message, sender;
        int max = chat["max"].toInt();
        bool flag;
        QJsonObject msg;
        for(int i=1; i<=max; i++){
            msg = chat[QString::number(i)].toObject();
            if(msg.isEmpty())
                continue;
            sender = msg["sender"].toString();
            message = msg["message"].toString();
            if(sender == user_data["id"].toString() && contact_info["status"].toString() != "channel")
                flag = true;
            else
                flag = false;
            add_message(flag, sender, message);
        }
        int a = ui->scrollArea->verticalScrollBar()->maximum() + 100;
        ui->scrollArea->verticalScrollBar()->setMaximum(a);
        ui->scrollArea->verticalScrollBar()->setSliderPosition(a);
    }
}

void MainWindow::set_msesage_widget_to_default(int i)
{
    ui->scrollAreaWidgetContents->layout()->itemAt(i)->widget()->setStyleSheet("background-color:rgb(0, 94, 140);");
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //get contact info
    ui->pbn_search_2->setEnabled(true);
    ui->groupBox_2->show();
    ui->groupBox_4->show();
    ui->scrollArea->show();
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
        ui->pbn_contact_name->setText(contact_info["name"].toString());
    }

    //---------- SHOW PROFILE IMAGE ---------
    // PV
    if(contact_info["status"].isNull())
    {
        QStringList permissions = contact_info["permissions"].toString().split('%');
        if(permissions.contains(user_data["id"].toString()))
        {
            //show default profile pic if user has no permission
            QPixmap pix(":/images/resourses/default_profile.png");
            ui->lbl_profile->setPixmap(pix);
            ui->lbl_profile->setScaledContents( true );
            ui->lbl_profile->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

        }
        else
        {
            //Get image profile data from server
            QJsonObject req;
            req["status"] = "getProfileImage";
            req["id"] = contact_info["id"].toString();
            QJsonDocument req_doc(req);
            QByteArray req_b = req_doc.toJson();
            QJsonValue img_val;
            if(client->is_client_connectd())
            {
                QByteArray resp_b = client->request_to_server(&req_b);
                QJsonDocument resp_d = QJsonDocument::fromJson(resp_b);
                QJsonObject resp_obj = resp_d.object();
                img_val = resp_obj[contact_info["id"].toString()];
            }
            ImageConvertation *imageConvertor = new ImageConvertation();
            QPixmap pix = imageConvertor->pixmapFrom(img_val);
            ui->lbl_profile->setPixmap(pix);
            ui->lbl_profile->setScaledContents( true );
            ui->lbl_profile->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

        }
    }
    // CHANNEL OR GROUP
    else
    {
        QJsonObject req;
        req["status"] = "getProfileImage";
        req["id"] = contact_info["id"].toString();
        QJsonDocument req_doc(req);
        QByteArray req_b = req_doc.toJson();
        QJsonValue img_val;
        if(client->is_client_connectd())
        {
            QByteArray resp_b = client->request_to_server(&req_b);
            QJsonDocument resp_d = QJsonDocument::fromJson(resp_b);
            QJsonObject resp_obj = resp_d.object();
            img_val = resp_obj[contact_info["id"].toString()];
        }
        ImageConvertation *imageConvertor = new ImageConvertation();
        QPixmap pix = imageConvertor->pixmapFrom(img_val);
        ui->lbl_profile->setPixmap(pix);
        ui->lbl_profile->setScaledContents( true );
        ui->lbl_profile->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    }

    //ui->scrollAreaWidgetContents->layout()->addWidget()

    //get chat
    get_chat();

    QString status = contact_info["status"].toString();
    ui->lbl_profile->setEnabled(true);
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
        ui->scrollArea->verticalScrollBar()->setMaximum(a);
        ui->scrollArea->verticalScrollBar()->setSliderPosition(a);
        append_message_in_database(message, senderId);
    }
    else if(senderId == contact_info["id"].toString() && chatId.isEmpty()){
        add_message(false, senderId, message);
        int a = ui->scrollArea->verticalScrollBar()->maximum() + 100;
        ui->scrollArea->verticalScrollBar()->setMaximum(a);
        ui->scrollArea->verticalScrollBar()->setSliderPosition(a);
        append_message_in_database(message, senderId);
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
    if(message_content.isEmpty())
        return;
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
    if(contact_info["status"].toString() == "channel")
        add_message(false, user_data["id"].toString(), message_content);
    else
        add_message(true, user_data["id"].toString(), message_content);
    int a = ui->scrollArea->verticalScrollBar()->maximum() + 100;
    ui->scrollArea->verticalScrollBar()->setMaximum(a);
    ui->scrollArea->verticalScrollBar()->setSliderPosition(a);
    append_message_in_database(message_content, user_data["id"].toString());
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
    QJsonObject req;
    req["status"] = "allUsersContacts";
    QJsonObject all_users_contacts;
    QJsonDocument req_doc(req);
    QByteArray req_b = req_doc.toJson();
    if(client->is_client_connectd()){
        QByteArray response_b = client->request_to_server(&req_b);
        QJsonDocument respones_d = QJsonDocument::fromJson(response_b);
        QJsonObject response_obj = respones_d.object();
        all_users_contacts = response_obj;
    }
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

void MainWindow::on_pbn_contact_name_clicked()
{
    if(contact_info["status"].isNull())
    {
        Profile *profile= new Profile(user_data["id"].toString(), contact_info, this);
        profile->show();
    }
    else
    {
        GroupProfile* groupProfile = new GroupProfile(contact_info["id"].toString(),user_data["id"].toString(), this);
        groupProfile->show();
    }
}

void MainWindow::on_logout_clicked()
{
    client->disconnect_from_server();
    this->close();
    this->destroy(true, true);
    this->deleteLater();
    loading *loading_page = new loading();
    loading_page->show();
}


void MainWindow::on_setting_clicked()
{
    QJsonObject user_alldata;
    QJsonObject request;
    request["status"] = "userInfo_forEdit";
    request["id"] = user_data["id"];
    QJsonDocument request_d(request);
    QByteArray request_b = request_d.toJson();

    if(client->is_client_connectd()){
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        user_alldata = response_d.object();
    }

    QJsonObject req;
    req["status"] = "getProfileImage";
    req["id"] = user_data["id"];
    QJsonDocument req_doc(req);
    QByteArray req_b = req_doc.toJson();
    QJsonValue img_val;

    if(client->is_client_connectd())
    {
        QByteArray resp_b = client->request_to_server(&req_b);
        QJsonDocument resp_d = QJsonDocument::fromJson(resp_b);
        QJsonObject resp_obj = resp_d.object();
        img_val = resp_obj[user_data["id"].toString()];
    }
    setting *setting_window = new setting(img_val ,user_alldata);
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

void MainWindow::on_chat_message_clicked(QListWidgetItem *item)
{
    QFile file(user_data["id"].toString() + "%" + contact_info["id"].toString() + ".json");
    file.open(QIODevice::ReadOnly);
    QByteArray data_b = file.readAll();
    file.close();
    QJsonDocument data_doc = QJsonDocument::fromJson(data_b);
    QJsonObject data_obj = data_doc.object();
    int deleted=0;
    QJsonObject message;
    int index = ui->scrollAreaWidgetContents->layout()->indexOf(item->listWidget()->parentWidget());
    int counter = 0, i =1;
    while (counter != index) {
        message = data_obj[QString::number(i)].toObject();
        if(message.isEmpty())
            deleted++;
        else
            counter++;
        i++;
    }
    QStringList message_spliter = item->text().split("\n");
    QJsonObject data;
    data["message"] = message_spliter[1];
    if(contact_info["status"].isNull())
        data["chat"] = "pv";
    else
        data["chat"] = contact_info["status"];

    data["sender"] = user_data["id"];
    data["id2"] = contact_info["id"];
    data["index"] = index + deleted;
    Msg_Options* options = new Msg_Options(client, data, this);
    connect(options, &Msg_Options::message_edited, this, &MainWindow::on_message_edited);
    options->show();
}

void MainWindow::on_pbn_search_2_clicked()
{
    if(ui->led_search->isHidden()){
        ui->led_search->show();
        return;
    }
    QString text = ui->led_search->text();
    ui->led_search->clear();
    ui->led_search->hide();
    if(text.isEmpty())
        return;
    QFile file(user_data["id"].toString() + "%" + contact_info["id"].toString() + ".json");
    file.open(QIODevice::ReadOnly);
    QByteArray data_b = file.readAll();
    file.close();
    QJsonDocument data_doc = QJsonDocument::fromJson(data_b);
    QJsonObject data_obj = data_doc.object();
    int max = data_obj["max"].toInt(), index = -1;
    int deleted=0;
    QJsonObject message;
    for(int i=1;i<=max;i++){
        message = data_obj[QString::number(i)].toObject();
        if(message.isEmpty()){
            deleted++;
            continue;
        }
        if(message["message"].toString().contains(text)){
            int numb = i - deleted;
            if(i - deleted >0)
                numb = i - deleted - 1;
            ui->scrollArea->verticalScrollBar()->setSliderPosition(numb*80);
            ui->scrollAreaWidgetContents->layout()->itemAt(i - deleted)->widget()->setStyleSheet("background-color:rgb(14, 19, 26);");
            index = i - deleted;
            break;
        }
    }
    if(index < 0){
        return;
    }
    QTimer* timer = new QTimer();
    timer->setInterval(1000); //Time in milliseconds
    timer->setSingleShot(true); //Setting this to true makes the timer run only once
    connect(timer, &QTimer::timeout, this, [=](){
        ui->scrollAreaWidgetContents->layout()->itemAt(index)->widget()->setStyleSheet("background-color:rgb(0, 94, 140);");
    });
    timer->start();
}

void MainWindow::on_message_edited()
{
    get_chat();
}


