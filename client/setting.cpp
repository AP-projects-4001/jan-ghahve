#include "setting.h"
#include "ui_setting.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDate>
#include "image_convertation.h"
#include "myclient.h"

setting::setting(QJsonValue img_val ,QJsonObject user_data ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting)
{
    ImageConvertation *imageConvertor = new ImageConvertation();
    QPixmap pix = imageConvertor->pixmapFrom(img_val);
    this->profile_pix = pix;
    qDebug()<<pix;
    ui->setupUi(this);
    setFixedSize(size());
    ui->led_email->setText(user_data["email"].toString());
    ui->led_email->setReadOnly(true);

    ui->led_password->setText(user_data["password"].toString());
    ui->led_password->setReadOnly(true);

    ui->led_phonenum->setText(user_data["number"].toString());
    ui->led_phonenum->setReadOnly(true);

    ui->led_username->setText(user_data["id"].toString());
    ui->led_username->setReadOnly(true);

    ui->led_name->setText(user_data["name"].toString());
    ui->led_name->setReadOnly(true);

    QDate date = QDate::fromString(user_data["birthdate"].toString(),"dd/MM/yyyy");
    ui->dateEdit->setDate(date);
    ui->dateEdit->setReadOnly(true);

    this->new_profile_pix = profile_pix;

    ui->lbl_img->setPixmap(this->profile_pix);
    ui->lbl_img->setScaledContents( true );
    ui->lbl_img->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );

//    QIcon ButtonIcon(pix);
//    QSize iconSize(QSize(151,151));
//    ui->pbn_profile->setIconSize(iconSize);
//    ui->pbn_profile->setIcon(ButtonIcon);

    ui->pbn_changeImage->hide();
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
    ui->pbn_changeImage->show();
    ui->led_username->setReadOnly(true);
    ui->led_name->setReadOnly(false);
    ui->led_email->setReadOnly(false);
    ui->led_password->setReadOnly(false);
    ui->led_phonenum->setReadOnly(false);
    ui->led_name->setReadOnly(false);
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

    ui->led_name->setText(this->user_data["name"].toString());
    ui->led_name->setReadOnly(true);


    QDate date = QDate::fromString(user_data["birthdate"].toString(),"dd/MM/yyyy");
    ui->dateEdit->setDate(date);
    ui->dateEdit->setReadOnly(true);

    ui->lbl_img->setPixmap(this->profile_pix);

    ui->pbn_cancel->hide();
    ui->pbn_save->hide();
    ui->pbn_changeImage->hide();
    ui->pbn_edit->show();

}


void setting::on_pbn_save_clicked()
{
    QString id,name,password,phonenum,email;
    QDate birthdate;
    id = ui->led_username->text();
    name = ui->led_name->text();
    password = ui->led_password->text();
    phonenum = ui->led_phonenum->text();
    email = ui->led_email->text();
    birthdate = ui->dateEdit->date();
    ui->dateEdit->date();

    QJsonObject user;
    user["status"] = "edit_profile";
    user["id"] = id;
    user["name"] = name;
    user["password"] = password;
    user["email"]=email;
    user["birthdate"] = birthdate.toString("dd/MM/yyyy");
    user["number"] = phonenum;

    ImageConvertation *imageConvertor = new ImageConvertation;
    QJsonValue val = imageConvertor->jsonValFromPixmap(this->new_profile_pix);
    user["img"] = val;
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
            QMessageBox::warning(this, "editing error", msg);
        }
    }
}

void setting::on_pbn_changeImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("open image"), QDir::homePath(),tr("JPG (*.jpg)"));
    if(fileName.size())
    {
        QImage image(fileName);


        auto pix = QPixmap::fromImage(image);
        ImageConvertation *image_convertor = new ImageConvertation();
        ui->lbl_img->setPixmap(pix);
        ui->lbl_img->setScaledContents( true );
        ui->lbl_img->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
        this->new_profile_pix = pix;
        //    QIcon ButtonIcon(pix);
    //    QSize iconSize(QSize(151,151));
    //    ui->pbn_profile->setIconSize(iconSize);
    //    ui->pbn_profile->setIcon(ButtonIcon);
        delete image_convertor;
    }
    else
    {
        this->new_profile_pix = this->profile_pix;
    }

    //auto val = image_convertor->jsonValFromPixmap(pix);


//    QFile file("image_file.json");
//    if(file.open(QIODevice::WriteOnly))
//    {
//       auto pix = QPixmap::fromImage(image);
//       auto val = jsonValFromPixmap(pix);
//       QJsonObject image;
//       image["img"] = val;
//       QJsonDocument img_d(image);
//       QByteArray img_b = img_d.toJson();
//       file.write(img_b);
////       auto pix2 = pixmapFrom(val);
////       auto img2 = pix2.toImage();
//    }

}

