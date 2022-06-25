#include <QJsonDocument>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QJsonObject request;
    request["status"] = "getInfo";
    request["id"] = id;
    QJsonDocument request_d(request);
    QByteArray request_b = request_d.toJson();

    client = new MyClient();
    if(client->connect_to_server()){
        QByteArray response = client->request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        data = response_d.object();
        qDebug() << data["id"] << ' ' << data["name"];
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

