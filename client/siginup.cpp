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
    ui->pbn_submit->setDefault(true);
    ui->pbn_submit->setFocus();
}

siginup::~siginup()
{
    delete ui;
}

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
        QMessageBox::warning(this, "Invalid input", "Email field cannot be empty!");
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
    QString id, name, pass, email,phone_num, birthdate, conf_pass;
    name = ui->lineEdit->text();
    id = ui->led_name->text();
    pass = ui->led_pass->text();
    email = ui->led_email->text();
    phone_num = ui->led_phonenumb->text();
    birthdate =  ui->dateEdit->text();
    conf_pass = ui->conf_pass->text();

    //Check validation of inputs
    if(!validate_signup_data(name, id, email,phone_num, birthdate, pass, conf_pass))
        return;

    QJsonObject user;
    user["status"] = "register";
    user["id"] = id;
    user["name"] = name;
    user["password"] = pass;
    user["email"]=email;
    user["birthdate"] = birthdate;

    QJsonDocument user_d(user);
    QByteArray user_b = user_d.toJson();
    //sending data to "myclient" to send to the server and write to the file
    client = new MyClient("register",&user_b);
}

void siginup::on_response_recieved(QByteArray response)
{
    QString msg = QString(response);
    //Check response(Successful or not)
    if(msg == "accepted"){
        this->close();
        client->disconnect();  
        MainWindow* main_window = new MainWindow();
        main_window->show();
    }
    else
    {
        QMessageBox::warning(this, "signup error", msg);
    }
}

void siginup::on_pbn_cancel_clicked()
{
    loading* loading_page = new loading();
    loading_page->setFixedSize(205,239);
    loading_page->show();
    this->close();
}

