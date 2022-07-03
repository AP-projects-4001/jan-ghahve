#include "profile.h"
#include "ui_profile.h"
#include <QJsonArray>
Profile::Profile(QString id ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Profile)
{
    ui->setupUi(this);
    setFixedSize(size());
    //Get user info from server
    get_user_data(id);
    show_informations();

}

Profile::~Profile()
{
    delete ui;
}

void Profile::get_user_data(QString id)
{
    QJsonObject request;
    request["status"] = "userInfo";
    request["id"] = id;
    QJsonDocument request_d(request);
    QByteArray request_b = request_d.toJson();
    MyClient client;
    if(client.connect_to_server()){
        QByteArray response = client.request_to_server(&request_b);
        QJsonDocument response_d = QJsonDocument::fromJson(response);
        this->user_data = response_d.object();
    }
}

void Profile::show_informations()
{
    ui->led_username->setText(user_data["id"].toString());
    ui->led_email->setText(user_data["email"].toString());
    ui->led_phonenum->setText(user_data["number"].toString());
    QDate date = QDate::fromString(user_data["birthdate"].toString(),"dd/MM/yyyy");
    ui->dateEdit->setDate(date);
    ui->dateEdit->setReadOnly(true);
    ui->led_email->setReadOnly(true);
    ui->led_phonenum->setReadOnly(true);
    ui->led_username->setReadOnly(true);
}

void Profile::on_pbn_close_clicked()
{
    this->close();
}

