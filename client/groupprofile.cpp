#include "groupprofile.h"
#include "ui_groupprofile.h"
#include "channelpermissions.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QJsonValue>
#include "image_convertation.h"
#include <QFileDialog>
#include <QMessageBox>

GroupProfile::GroupProfile(QString id,QString visitor, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupProfile)
{
    ui->setupUi(this);
    setFixedSize(size());

    this->id = id;
    ui->lbl_name->setText(id);
    ui->pbn_save->hide();
    ui->pbn_cancel->hide();
    //---------- GET PROFILE OF GROUP OR CHANNEL -------------
    MyClient *client = new MyClient();
    QJsonObject req;
    req["status"] = "getProfileImage";
    req["id"] = id;
    QJsonDocument req_doc(req);
    QByteArray req_b = req_doc.toJson();
    QJsonValue img_val;
    if(client->connect_to_server())
    {
        QByteArray resp_b = client->request_to_server(&req_b);
        QJsonDocument resp_d = QJsonDocument::fromJson(resp_b);
        QJsonObject resp_obj = resp_d.object();
        img_val = resp_obj[id];
    }
    ImageConvertation *imageConvertor = new ImageConvertation();
    QPixmap pix = imageConvertor->pixmapFrom(img_val);
    this->profile_pix = pix;
    this->new_profile_pix = pix;
    ui->lbl_img->setPixmap(pix);
    ui->lbl_img->setScaledContents( true );
    ui->lbl_img->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    //----------------------------channel info  ------------------------------------
    QJsonObject json_obj;
    json_obj["status"] = "channelInfo";
    json_obj["id"] = id;
    QJsonDocument json_doc(json_obj);
    QByteArray json_b = json_doc.toJson();
    json_b = client->request_to_server(&json_b);
    json_doc = QJsonDocument::fromJson(json_b);
    json_obj = json_doc.object();
    if(!json_obj.isEmpty())
    {
        QString creator_of_group = json_obj["1"].toString();
        if(visitor!=creator_of_group)
        {
            ui->pbn_permission->hide();
        }

    }

}

GroupProfile::~GroupProfile()
{
    delete ui;
}

void GroupProfile::on_pbn_permission_clicked()
{
    ChannelPermissions *chanelPermissions = new ChannelPermissions(id, this);
    chanelPermissions->show();
}

void GroupProfile::on_pbn_change_image_clicked()
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
        delete image_convertor;
        ui->pbn_save->show();
        ui->pbn_cancel->show();
        //ui->pbn_change_image->setEnabled(false);
    }
    else
    {
        this->new_profile_pix = this->profile_pix;
    }
}



void GroupProfile::on_pbn_save_clicked()
{
    QJsonObject req;
    req["status"] = "Channel_Group_Profile_Edited";
    req["id"] = id;

    ImageConvertation *imageConvertor = new ImageConvertation;
    QJsonValue val = imageConvertor->jsonValFromPixmap(this->new_profile_pix);
    req["img"] = val;
    MyClient* client = new MyClient();
    QJsonDocument req_d(req);
    QByteArray req_b = req_d.toJson();

    if(client->is_client_connectd()){
        //Sending data to the server and waiting for getting a response from it
        QByteArray response = client->request_to_server(&req_b);
        QString msg = QString(response);
        //Check response(Successful or not)
        if(msg == "accepted"){
            qDebug()<<"profile editing finished, continue to mainWindow";
            client->disconnect_from_server();
            this->close();
        }
        else
        {
            QMessageBox::warning(this, "editing error", msg);
        }
    }
}


void GroupProfile::on_pbn_cancel_clicked()
{
    ui->lbl_img->setPixmap(this->profile_pix);
    ui->lbl_img->setScaledContents( true );
    ui->lbl_img->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->pbn_save->hide();
    ui->pbn_cancel->hide();
    //ui->pbn_change_image->setEnabled(true);
}

