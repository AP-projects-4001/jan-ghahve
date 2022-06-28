#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTcpSocket>
#include "mainwindow.h"
#include "mythread.h"
#include "ui_mainwindow.h"
#include "search.h"

MainWindow::MainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pain();
//    QObject::connect(ui->test,&QPushButton::clicked,this,&MainWindow::add_safebar);

    client = new MyClient();
    get_user_info(id);
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

        QListWidget* list = ui->listWidget;
        QJsonObject user;
        for(QJsonValueRef userRef:response_arr){
            user = userRef.toObject();
            QListWidgetItem* item = new QListWidgetItem(user["id"].toString());
        //    item->setBackground(Qt::blue);
            list->addItem(item);
        }
    }
}

void MainWindow::on_usersFound(QStringList users)
{
    QListWidget* list = ui->listWidget;

    QJsonObject user;
    for(QString id:users){
        if(id == user_data["id"].toString())
            continue;
        QListWidgetItem* item = new QListWidgetItem(id);
    //    item->setBackground(Qt::blue);
        list->addItem(item);
    }
}

void MainWindow::pain()
{
    ui->pbn_send->setStyleSheet("*{border-image: url(:/images/resourses/send.png);"
                                "background-color:rgb(191, 215, 234);"
                                "border-radius: 20%;"
                                "background-position:center;}"
                                "*:hover{background-color:rgb(30, 157, 230);}");
}
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
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
    }
    ui->ted_chat->clear();
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


void MainWindow::on_messagerecievd1(QString senderId, QString message)
{
    if(senderId == contact_info["id"].toString()){
        ui->ted_chat->append(senderId + ":" + message);
    }
}


void MainWindow::on_pbn_send_clicked()
{
    //    send message
    QString message_content = ui->ted_message->toPlainText();
    ui->ted_message->clear();
    QJsonObject message;
    message["status"] = "message";
    message["id1"] = user_data["id"];
    message["id2"] = contact_info["id"];
    message["message"] = message_content;
    QJsonDocument message_d(message);
    QByteArray message_b = message_d.toJson();

    if(client->is_client_connectd()){
        client->request_to_server(&message_b);
    }
    ui->ted_chat->append(user_data["id"].toString()+":" + message_content);
}



void MainWindow::on_pbn_search_clicked()
{
    search* search_dialog = new search(client,this);
    connect(search_dialog, &search::users_found, this, &MainWindow::on_usersFound);
    search_dialog->show();
}

