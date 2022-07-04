#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include "channelpermissions.h"
#include "ui_channelpermissions.h"
#include "myclient.h"

ChannelPermissions::ChannelPermissions(QString id,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChannelPermissions)
{
    ui->setupUi(this);
    setFixedSize(size());
    this->id = id;

    client = new MyClient();
    client->connect_to_server();
    QJsonObject json_obj;
    json_obj["status"] = "channelInfo";
    json_obj["id"] = id;
    QJsonDocument json_doc(json_obj);
    QByteArray json_b = json_doc.toJson();
    json_b = client->request_to_server(&json_b);
    json_doc = QJsonDocument::fromJson(json_b);
    json_obj = json_doc.object();
    QString admins_str = json_obj["admins"].toString();
    QStringList admins_list = admins_str.split('%');

    QString user_id;
    int max = json_obj["max"].toInt();
    numbOfMembers = max;
    QListWidget* list = ui->listWidget;
    for(int i=2; i<= max;i++){
        user_id = json_obj[QString::number(i)].toString();
        QListWidgetItem* item = new QListWidgetItem(user_id);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        if(is_admin(user_id, admins_list))
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        list->addItem(item);
    }
}

ChannelPermissions::~ChannelPermissions()
{
    delete ui;
}

bool ChannelPermissions::is_admin(QString id, QStringList admins_list)
{
    for(QString admin:admins_list){
        if(admin == id)
            return true;
    }
    return false;
}

void ChannelPermissions::on_pbn_ok_clicked()
{
    QListWidget* list = ui->listWidget;
    QJsonObject req_obj;
    req_obj["status"] = "modifyAdmins";
    req_obj["id"] = id;

    QString admins = "";
    for(int i=0; i< numbOfMembers; i++){
        QListWidgetItem* item = list->item(i);
        if(item->checkState()){
            admins += (item->text()+"%");
        }
    }
    req_obj["admins"] = admins;
    QJsonDocument req_doc(req_obj);
    QByteArray req_b = req_doc.toJson();
    client->request_to_server(&req_b);
    client->disconnect_from_server();
    this->close();
}

void ChannelPermissions::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->checkState())
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}

