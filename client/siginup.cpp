#include <QJsonObject>
#include <QJsonDocument>
#include <qbytearray.h>
#include <qmessagebox.h>
#include "siginup.h"
#include "ui_siginup.h"
#include "myclient.h"
#include "mainwindow.h"
#include "loading.h"

siginup::siginup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::siginup)
{
    ui->setupUi(this);
    client = new MyClient();
}

siginup::~siginup()
{
    delete ui;
}

//Checking validation of inputs
bool siginup::validate_signup_data(QString name, QString id, QString email, QString phone_number, QString birthdate, QString pass, QString conf_pass)
{
    if(id.length() == 0){
        QMessageBox::warning(this, "Invalid input", "User name field cannot be empty!");
        return false;
    }
    else if(name.length() == 0){
        QMessageBox::warning(this, "Invalid input", "Name field cannot be empty!");
        return false;
    }
    else if(pass.length() == 0){
        QMessageBox::warning(this, "Invalid input", "Password field cannot be empty!");
        return false;
    }
    else if(email.length() == 0){
        QMessageBox::warning(this, "Invalid input", "Email field cannot be empty!");
        return false;
    }
    else if(phone_number.length() == 0){
        QMessageBox::warning(this, "Invalid input", "Phone Number field cannot be empty!");
        return false;
    }
    else if(birthdate.length() == 0){
        QMessageBox::warning(this, "Invalid input", "Birh date field cannot be empty!");
        return false;
    }
    else if(pass != conf_pass){
        QMessageBox::warning(this, "Invalid input", "Password and confirm password are different!");
        return false;
    }
    return true;
}

void siginup::on_pbn_submit_clicked()
{
    QString id, name, pass, email, phone_number, birthdate, conf_pass;
    name = ui->lineEdit->text();
    id = ui->led_name->text();
    pass = ui->led_pass->text();
    email = ui->led_email->text();
    phone_number = ui->led_phonenumb->text();
    birthdate =  ui->dateEdit->text();
    conf_pass = ui->conf_pass->text();

    //Checking validation of inputs
    if(!validate_signup_data(name, id, email, phone_number, birthdate, pass, conf_pass))
        return;

    //Creating an instance to set data on it, then sending it to the server
    QJsonObject user;
    user["status"] = "register";
    user["id"] = id;
    user["name"] = name;
    user["password"] = pass;
    user["email"]=email;
    user["birthdate"] = birthdate;
    QJsonDocument user_d(user);
    QByteArray user_b = user_d.toJson();

    if(client->connect_to_server()){
        //Sending data to the server and waiting for getting a response from it
        QByteArray response = client->request_to_server(&user_b);
        QString msg = QString(response);
        //Check response(Successful or not)
        if(msg == "accepted"){
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
            QMessageBox::warning(this, "signup error", msg);
        }
    }
}


void siginup::on_pbn_cancel_clicked()
{
    //Back to the loading page
    loading* loading_page = new loading();
    loading_page->setFixedSize(205,239);
    loading_page->show();
    this->close();
}

