#include "profile.h"
#include "ui_profile.h"
#include <QJsonArray>
Profile::Profile(QString contact_id, QJsonObject user_data,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Profile),
    user_data(user_data),
    contact_id(contact_id)
{
    ui->setupUi(this);
    setFixedSize(size());
    show_informations();
}

Profile::~Profile()
{
    delete ui;
}

void Profile::show_informations()
{
    QStringList permissions = user_data["permissions"].toString().split('%');
    if(permissions.contains(contact_id)){
        ui->led_phonenum->hide();
        ui->lbl_phonenum->hide();
    }else{
        ui->led_phonenum->setText(user_data["number"].toString());
    }
    ui->led_username->setText(user_data["id"].toString());
    ui->led_name->setText(user_data["name"].toString());
    ui->led_email->setText(user_data["email"].toString());
    QDate date = QDate::fromString(user_data["birthdate"].toString(),"dd/MM/yyyy");
    ui->dateEdit->setDate(date);

    ui->led_username->setReadOnly(true);
    ui->led_name->setReadOnly(true);
    ui->dateEdit->setReadOnly(true);
    ui->led_email->setReadOnly(true);
    ui->led_phonenum->setReadOnly(true);

}

void Profile::on_pbn_close_clicked()
{
    this->close();
}


