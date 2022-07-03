#include "setting.h"
#include "ui_setting.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDate>

setting::setting(QJsonObject user_data ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
    setFixedSize(size());
    qDebug()<<user_data;
    ui->led_email->setText(user_data["email"].toString());
    ui->led_email->setReadOnly(true);

    ui->led_password->setText(user_data["password"].toString());
    ui->led_password->setReadOnly(true);

    ui->led_phonenum->setText(user_data["number"].toString());
    ui->led_phonenum->setReadOnly(true);

    ui->led_username->setText(user_data["id"].toString());
    ui->led_username->setReadOnly(true);
    qDebug()<<"---------------------";
    qDebug()<<user_data["birthdate"].toString();
    QDate date = QDate::fromString(user_data["birthdate"].toString(),"dd/MM/yyyy");
    ui->dateEdit->setDate(date);
    ui->dateEdit->setReadOnly(true);

    //------- TEST -------
//    QGraphicsScene scene;
//    QPixmap pixmap(":/images/resourses/profile_image.jpg");
//    scene.addPixmap(pixmap);
//    ui->image_graphicsView->setScene(&scene);

    ui->pbn_cancel->hide();
    ui->pbn_save->hide();
    this->user_data = user_data;

}

setting::~setting()
{
    delete ui;
}

void setting::on_pbn_edit_clicked()
{
    ui->pbn_edit->hide();
    ui->pbn_cancel->show();
    ui->pbn_save->show();
    ui->led_username->setReadOnly(true);
    ui->led_email->setReadOnly(false);
    ui->led_password->setReadOnly(false);
    ui->led_phonenum->setReadOnly(false);
    ui->dateEdit->setReadOnly(false);
}


void setting::on_pbn_cancel_clicked()
{
    ui->led_email->setText(this->user_data["email"].toString());
    ui->led_email->setReadOnly(true);

    ui->led_password->setText(this->user_data["password"].toString());
    ui->led_password->setReadOnly(true);

    ui->led_phonenum->setText(this->user_data["number"].toString());
    ui->led_phonenum->setReadOnly(true);

    ui->led_username->setText(this->user_data["id"].toString());
    ui->led_username->setReadOnly(true);

    //ui->dateEdit->setDate(this.user_data["birthDate"].toString());
    ui->dateEdit->setReadOnly(true);

    ui->pbn_cancel->hide();
    ui->pbn_save->hide();
    ui->pbn_edit->show();
}


void setting::on_pbn_save_clicked()
{
    QString id,password,phonenum,email;
    QDate birthdate;
    id = ui->led_username->text();
    password = ui->led_password->text();
    phonenum = ui->led_phonenum->text();
    email = ui->led_email->text();
    birthdate = ui->dateEdit->date();
    ui->dateEdit->date();

    QJsonObject user;
    user["status"] = "edit_profile";
    user["id"] = id;
    user["password"] = password;
    user["email"]=email;
    user["birthdate"] = birthdate.toString("dd/MM/yyyy");
    user["number"] = phonenum;

    MyClient client;
    QJsonDocument user_d(user);
    QByteArray user_b = user_d.toJson();

    if(client.connect_to_server()){
        //Sending data to the server and waiting for getting a response from it
        QByteArray response = client.request_to_server(&user_b);
        QString msg = QString(response);
        //Check response(Successful or not)
        if(msg == "accepted"){
            qDebug()<<"profile editing finished, continue to mainWindow";
            this->close();
        }
        else
        {
            QMessageBox::warning(this, "signup error", msg);
        }
    }
}

