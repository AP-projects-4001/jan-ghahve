#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new MyClient();
    get_user_info(id);
    get_all_users();
//    send message
//    QJsonObject message;
//    message["status"] = "message";
//    message["id1"] = user_data["id"];
//    message["id2"] = "d";//contact_info["id"];
//    message["message"] = "Hello!";
//    QJsonDocument message_d(message);
//    QByteArray message_b = message_d.toJson();

//    if(client->is_client_connectd()){
//        client->request_to_server(&message_b);
//    }



//    while (true) {
//        if(client->is_client_connectd()){
//            QByteArray response = client->message_recieved();
//            QJsonDocument response_d = QJsonDocument::fromJson(response);
//            QJsonObject response_obj = response_d.object();
//            qDebug() << response_obj["message"];
//        }
//    }

//    QObject::connect(ui->test,&QPushButton::clicked,this,&MainWindow::add_safebar);

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
        QListWidget* list = ui->listWidget;

        QJsonObject user;
        for(QJsonValueRef user_ref:data_arr){
            user = user_ref.toObject();
            if(user["id"] == user_data["id"])
                continue;
            QListWidgetItem* item = new QListWidgetItem(user["id"].toString());
        //    item->setBackground(Qt::blue);
            list->addItem(item);
        }

    }
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //mitoni itemi ke click shode estefade koni (to vorodi tabe ferestade)
    QString id = item->text();
    QJsonObject request;
    request["status"] = "userInfo";
    request["id"] = id;
    QJsonDocument request_doc(request);
    QByteArray request_b = request_doc.toJson();

    if(client->is_client_connectd()){
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        contact_info = response_d.object();
        ui->contact_name->setText(contact_info["name"].toString());
        ui->contact_id->setText(contact_info["id"].toString());
        ui->contact_phoneNumber->setText(contact_info["number"].toString());
        ui->contact_birhdate->setText(contact_info["birthdate"].toString());
    }

    //get chat
    QJsonObject chat;
    QJsonObject chatReq;
    chatReq["status"] = "chatInfo";
    chatReq["id1"] = user_data["id"];
    chatReq["id2"] = contact_info["id"];
    QJsonDocument chatReq_d(chatReq);
    QByteArray chatReq_b = chatReq_d.toJson();
    if(client->is_client_connectd()){
        QByteArray response = client->request_to_server(&chatReq_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        chat = response_d.object();
        QFile file(user_data["id"].toString() + "%" + contact_info["id"].toString());
        file.open(QIODevice::WriteOnly);
        file.write(response);
        file.close();
    }
}

