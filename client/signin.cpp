//signin.cpp

#include <QJsonObject>
#include <QJsonDocument>
#include <qbytearray.h>
#include <qmessagebox.h>
#include "signin.h"
#include "ui_signin.h"
#include "myclient.h"
#include "mainwindow.h"

signin::signin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signin)
{
    ui->setupUi(this);
    client = new MyClient();
}

signin::~signin()
{
    delete ui;
}

bool signin::validate_signin_data(QString id,QString pass)
{
    if(id.length() == 0){
        QMessageBox::warning(this, "Invalid input", "User name field cannot be empty!");
        return false;
    }
    else if(pass.length() == 0){
        QMessageBox::warning(this, "Invalid input", "Password field cannot be empty!");
        return false;
    }
    return true;
}

void signin::on_pbn_ok_clicked()
{
    QString id,pass;
    id = ui->led_name->text();
    pass = ui->led_pass->text();

    if(!validate_signin_data(id,pass))
        return;

    QJsonObject user;
    user["status"] = "login";
    user["id"] = id;
    user["password"] = pass;

    QJsonDocument user_d(user);
    QByteArray user_b = user_d.toJson();

    if(client->connect_to_server()){
        QByteArray response = client->request_to_server(&user_b);
        QString msg = QString(response);
        if(msg == "accepted login"){
            client->disconnect_from_server();
            this->close();
            this->destroy(true, true);
            this->deleteLater();
            MainWindow* main_window = new MainWindow(id);
            main_window->show();
        }
        else
        {
            QMessageBox::warning(this, "signin error", msg);
        }
    }
}



