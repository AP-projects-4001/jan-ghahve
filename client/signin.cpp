//signin.cpp


#include <QJsonObject>
#include <QJsonDocument>
#include <qbytearray.h>
#include <qmessagebox.h>
#include "signin.h"
#include "ui_signin.h"
#include "myclient.h"
#include "mainwindow.h"
#include <QLineEdit>
#include "loading.h"

signin::signin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signin)
{
    ui->setupUi(this);
    ui->led_pass->setEchoMode(QLineEdit::Password);
    ui->pbn_ok->setDefault(true);
    ui->pbn_ok->setFocus();
}

signin::~signin()
{
    delete ui;
}

//Checking validation of inputs
bool signin::validate_signin_data(QString id,QString pass)
{
    if(id.length() == 0){
        QMessageBox::warning(this, "Invalid input", "Username field cannot be empty!");
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
    //sending data to "myclient" to send to the server and compare with Database(chaeck validation)
    client = new MyClient("login",&user_b);
}

void signin::on_response_recieved(QByteArray response)
{
    QString msg = QString(response);
    //Check response(Successful or not)
    if(msg == "accepted login")
    {
        this->close();
        client->disconnect();
        MainWindow* main_window = new MainWindow();
        main_window->show();
    }
    else
    {
        QMessageBox::warning(this, "signin error", msg);
        if(msg=="incorrect username")
        {
            ui->led_name->setStyleSheet("border: 1px solid red");
        }
        else if(msg=="incorrect password")
        {
            ui->led_pass->setStyleSheet("border: 1px solid red");
        }
    }
}


void signin::on_pbn_cancel_clicked()
{
    loading* loading_page = new loading();
    loading_page->setFixedSize(205,239);
    loading_page->show();
    this->close();
}

