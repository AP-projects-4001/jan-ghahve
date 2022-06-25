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
    client = new MyClient("login",&user_b);
}

void signin::on_response_recieved(QByteArray response)
{
    QString msg = QString(response);
    if(msg == "accepted login")
    {
        client->disconnect();
        this->close();
        MainWindow* main_window = new MainWindow();
        main_window->show();
    }
    else
    {
        QMessageBox::warning(this, "signin error", msg);
    }
}

