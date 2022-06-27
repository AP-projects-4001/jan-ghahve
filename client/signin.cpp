//signin.cpp

#include <QJsonObject>
#include <QJsonDocument>
#include <qbytearray.h>
#include <qmessagebox.h>
#include "signin.h"
#include "ui_signin.h"
#include "myclient.h"
#include "mainwindow.h"
#include "loading.h"
#include <QLineEdit>

signin::signin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signin)
{
    ui->setupUi(this);
    client = new MyClient();
    ui->pbn_ok->setDefault(true);
    ui->pbn_ok->setFocus();
    ui->led_pass->setEchoMode(QLineEdit::Password);
}

signin::~signin()
{
    delete ui;
}

//Checking validation of inputs
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

    //Creating an instance to set data on it, then sending it to the server
    QJsonObject user;
    user["status"] = "login";
    user["id"] = id;
    user["password"] = pass;
    QJsonDocument user_d(user);
    QByteArray user_b = user_d.toJson();

    if(client->connect_to_server()){
        //Sending data to the server and waiting for getting a response from it
        QByteArray response = client->request_to_server(&user_b);
        QString msg = QString(response);
        //Check response(Successful or not)
        if(msg == "accepted"){
            //disconnect the client and close signin page
            client->disconnect_from_server();
            this->close();
            this->destroy(true, true);
            this->deleteLater();
            //Go to the main window(chat window)
            MainWindow* main_window = new MainWindow(id);
            main_window->show();
        }
        else
        {
            QMessageBox::warning(this, "signin error", msg);
        }
    }
}




void signin::on_pbn_cancel_clicked()
{
    //Back to the loading page
    loading* loading_page = new loading();
    loading_page->setFixedSize(205,239);
    loading_page->show();
    this->close();
}

