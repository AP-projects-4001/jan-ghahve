#include <QJsonObject>
#include <QJsonDocument>
#include <qbytearray.h>
#include <qmessagebox.h>
#include "siginup.h"
#include "ui_siginup.h"
#include "myclient.h"

siginup::siginup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::siginup)
{
    ui->setupUi(this);
}

siginup::~siginup()
{
    delete ui;
}

bool siginup::validate_signup_data(QString name, QString id, QString email, QString birthdate, QString pass, QString conf_pass)
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
    QString id, name, pass, email, birthdate, conf_pass;
    name = ui->lineEdit->text();
    id = ui->led_name->text();
    pass = ui->led_pass->text();
    email = ui->led_email->text();
    birthdate =  ui->dateEdit->text();
    conf_pass = ui->conf_pass->text();

    if(!validate_signup_data(name, id, email, birthdate, pass, conf_pass))
        return;

    QJsonObject user;
    user["id"] = id;
    user["name"] = name;
    user["password"] = pass;
    user["email"]=email;
    user["birthdate"] = birthdate;

    QJsonDocument user_d(user);
    QByteArray user_b = user_d.toJson();
    MyClient client(&user_b);
    //client.connectingToServer();
}

