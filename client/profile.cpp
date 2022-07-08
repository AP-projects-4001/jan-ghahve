#include "profile.h"
#include "ui_profile.h"
#include <QJsonArray>
#include "myclient.h"
#include "image_convertation.h"

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
    if(permissions.contains(contact_id))
    {
        //if user restricted,default profile show and number does'nt show
        ui->led_phonenum->hide();
        ui->lbl_phonenum->hide();
        QPixmap pix(":/images/resourses/default_profile.png");
        ui->lbl_img->setPixmap(pix);
        ui->lbl_img->setScaledContents( true );
        ui->lbl_img->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );


    }
    else
    {
        //if user has permission, show profile and number
        ui->led_phonenum->setText(user_data["number"].toString());
        MyClient *client = new MyClient();
        QJsonObject req;
        req["status"] = "getProfileImage";
        req["id"] = user_data["id"].toString();
        QJsonDocument req_doc(req);
        QByteArray req_b = req_doc.toJson();
        QJsonValue img_val;
        if(client->connect_to_server())
        {
            QByteArray resp_b = client->request_to_server(&req_b);
            QJsonDocument resp_d = QJsonDocument::fromJson(resp_b);
            QJsonObject resp_obj = resp_d.object();
            img_val = resp_obj[user_data["id"].toString()];
        }
        ImageConvertation *imageConvertor = new ImageConvertation();
        QPixmap pix = imageConvertor->pixmapFrom(img_val);
        ui->lbl_img->setPixmap(pix);
        ui->lbl_img->setScaledContents( true );
        ui->lbl_img->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
        client->disconnect_from_server();

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


