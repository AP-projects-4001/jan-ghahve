#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTcpSocket>
#include "mainwindow.h"
#include "mythread.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
      pain();
//    QObject::connect(ui->test,&QPushButton::clicked,this,&MainWindow::add_safebar);
      QObject::connect(ui->actionNew_Group,&QAction::triggered,this,&MainWindow::on_newgroup_clicked);

    client = new MyClient();
    get_user_info(id);
    get_all_users();
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


void MainWindow::on_messagerecievd1(QString senderId, QString message)
{
    if(senderId == contact_info["id"].toString()){
        ui->ted_chat->append(message);
    }
}


void MainWindow::on_pbn_send_clicked()
{
    //    send message
    QJsonObject message;
    message["status"] = "message";
    message["id1"] = user_data["id"];
    message["id2"] = contact_info["id"];
    message["message"] = ui->ted_message->toPlainText();
    QJsonDocument message_d(message);
    QByteArray message_b = message_d.toJson();

    if(client->is_client_connectd()){
        client->request_to_server(&message_b);
    }

}

void MainWindow::on_newgroup_clicked()
{
 ui->ted_chat->append("PAin");
}


