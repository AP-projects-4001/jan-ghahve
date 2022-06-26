#include <QFile>
#include <QJsonDocument>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //getinfo
    QJsonObject request;
    request["status"] = "getInfo";
    request["id"] = id;
    QJsonDocument request_d(request);
    QByteArray request_b = request_d.toJson();

   client = new MyClient();
    if(client->connect_to_server()){
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        user_data = response_d.object();
    }

//    send message
    QJsonObject message;
    message["status"] = "message";
    message["id1"] = user_data["id"];
    message["id2"] = "d";//contact_info["id"];
    message["message"] = "Hello!";
    QJsonDocument message_d(message);
    QByteArray message_b = message_d.toJson();

    if(client->is_client_connectd()){
        client->request_to_server(&message_b);
    }

    //get chat
//    QJsonObject chat;
//    QJsonObject chatReq;
//    chatReq["status"] = "chatInfo";
//    chatReq["id1"] = user_data["id"];
//    chatReq["id2"] = "d";//contact_info["id"];
//    QJsonDocument chatReq_d(chatReq);
//    QByteArray chatReq_b = chatReq_d.toJson();
//    if(client->is_client_connectd()){
//        QByteArray response = client->request_to_server(&chatReq_b);
//        QJsonDocument response_d = QJsonDocument::fromJson(response);
//        chat = response_d.object();
//        QFile file("chat.json");
//        file.open(QIODevice::WriteOnly);
//        file.write(response);
//        file.close();
//    }

    while (true) {
        if(client->is_client_connectd()){
            QByteArray response = client->message_recieved();
            QJsonDocument response_d = QJsonDocument::fromJson(response);
            QJsonObject response_obj = response_d.object();
            qDebug() << response_obj["message"];
        }
    }

    QObject::connect(ui->test,&QPushButton::clicked,this,&MainWindow::add_safebar);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::test_function()
{
    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender()); // retrieve the button you have clicked
    QString id = buttonSender->text(); // retrive the text from the button clicked
    QJsonObject request;
    request["status"] = "getInfo";
    request["id"] = id;
    QJsonDocument request_doc(request);
    QByteArray request_b = request_doc.toJson();

    if(client->is_client_connectd()){
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        contact_info = response_d.object();
    }


void MainWindow::add_safebar()
{
    QListWidget* list = ui->listWidget;
    QListWidgetItem* item = new QListWidgetItem("pain");

    list->addItem(item);

}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //mitoni itemi ke click shode estefade koni (to vorodi tabe ferestade)
    ui->textEdit->insertPlainText("pain");
}

