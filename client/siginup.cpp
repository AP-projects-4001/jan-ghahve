#include <QJsonObject>
#include <QJsonDocument>
#include <qbytearray.h>
#include <qmessagebox.h>
#include "siginup.h"
#include "ui_siginup.h"
#include "myclient.h"
#include "loading.h"
#include "authenticationcode.h"
#include "image_convertation.h"

siginup::siginup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::siginup)
{
    ui->setupUi(this);
    setFixedSize(size());
    client = new MyClient();
    ui->pbn_submit->setDefault(true);
    ui->pbn_submit->setFocus();
    ui->led_name->setFocus();
    setTabOrder(ui->led_name,ui->led_id);
    setTabOrder(ui->led_id,ui->led_phonenumb);
    setTabOrder(ui->led_phonenumb,ui->dateEdit);
    setTabOrder(ui->dateEdit,ui->led_email);
    setTabOrder(ui->led_email,ui->led_pass);
    setTabOrder(ui->led_pass,ui->conf_pass);

    ui->led_phonenumb->setValidator(new QIntValidator(this));
}

siginup::~siginup()
{
    delete ui;
}


//Checking validation of inputs
bool siginup::validate_signup_data(QString name, QString id, QString email, QString pass, QString conf_pass, QString number)
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
    else if(number.length() == 0){
        QMessageBox::warning(this, "Invalid input", "Phone number field cannot be empty!");
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
    QString id, name, pass, email,conf_pass, number;
    QDate birthdate;
    name = ui->led_name->text();
    id = ui->led_id->text();
    pass = ui->led_pass->text();
    email = ui->led_email->text();
    birthdate =  ui->dateEdit->date();
    conf_pass = ui->conf_pass->text();
    number = ui->led_phonenumb->text();

    //Checking validation of inputs
    if(!validate_signup_data(name, id, email, pass, conf_pass, number))
        return;

    //Creating an instance to set data on it, then sending it to the server
    QJsonObject user;
    user["status"] = "register";
    user["id"] = id;
    user["name"] = name;
    user["password"] = pass;
    user["email"]=email;
    user["birthdate"] = birthdate.toString("dd/MM/yyyy");
    QImage image(":/images/resourses/default_profile.png");
    auto pix = QPixmap::fromImage(image);
    ImageConvertation *imageConvertor = new ImageConvertation();
    QJsonValue val = imageConvertor->jsonValFromPixmap(pix);
    user["img"] = val;
    user["number"] = number;
    user["state"] = "signup";
    QJsonDocument user_d(user);
    QByteArray user_b = user_d.toJson();

    if(client->connect_to_server()){
        //Sending data to the server and waiting for getting a response from it
        QByteArray response = client->request_to_server(&user_b);
        QString msg = QString(response);
        //Check response(Successful or not)
        if(msg == "accepted"){
            //qDebug()<<"msg accept oomad!";
            AuthenticationCode *auth = new AuthenticationCode(user, client);
            connect(auth, &AuthenticationCode::user_authenticated, this, &siginup::on_userauthenticated);
            auth->show();
        }else{
            QMessageBox::warning(this, "signup error", msg);
        }
    }
}

void siginup::on_userauthenticated()
{
    this->close();
}


void siginup::on_pbn_cancel_clicked()
{
    //Back to the loading page
    loading* loading_page = new loading();
    loading_page->setFixedSize(205,239);
    loading_page->show();
    this->close();
}

