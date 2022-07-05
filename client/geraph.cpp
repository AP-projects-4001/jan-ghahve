#include "geraph.h"
#include "ui_geraph.h"

geraph::geraph(QString myUsername ,QJsonObject inp_allUsers_contacts ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::geraph)
{

    ui->setupUi(this);
    setFixedSize(size());

    //age code kar nakard va ina, onja ke add mikoni ghablesh ina ro bezar
    ui->widget->setStyleSheet(
    "QListWidget:item"
    "{"
    "height: 30px;"
    "border: 1px solid gray;"
    "border-right:hide;"
    "border-left:hide;"
    "border-top:hide;"
    "}"
    "QListWidget:item:selected"
    "{"
    "background-color:gray;"
    "}"
    "*{"

    "}"
    );

    this->allUsers_contacts = inp_allUsers_contacts;
    this->my_username = myUsername;
    ui->label->setText(my_username);
    QJsonArray data_arr = allUsers_contacts[my_username].toArray();
    QListWidget* list_1 = ui->listWidget;
    QJsonObject contact;
    QString contact_name;
    //insert datas in first-listWidget
    for(QJsonValueRef user_ref:data_arr){
        contact = user_ref.toObject();
        if(contact["status"].toString()=="user")
        {
            contact_name = contact["id"].toString();
            QListWidgetItem* item = new QListWidgetItem(contact_name);
            list_1->addItem(item);
        }
    }
}

geraph::~geraph()
{
    delete ui;
}

void geraph::on_pushButton_clicked()
{
    this->close();
}


void geraph::on_listWidget_itemClicked(QListWidgetItem *item)
{
    ui->listWidget_2->clear();
    ui->listWidget_3->clear();
    QString clicked_name = item->text();
    QJsonArray data_arr = allUsers_contacts[clicked_name].toArray();
    QListWidget* list_2 = ui->listWidget_2;
    QJsonObject contact2;
    QString contact2_id;
    //insert selected listWidget1-contact's datas in second-listWidget
    for(QJsonValueRef user_ref:data_arr)
    {
        contact2 = user_ref.toObject();
        if(contact2["status"].toString() == "user")
        {
            contact2_id = contact2["id"].toString();
            QListWidgetItem* item = new QListWidgetItem(contact2_id);
            list_2->addItem(item);
        }

    }
}


void geraph::on_listWidget_2_itemClicked(QListWidgetItem *item)
{
    ui->listWidget_3->clear();
    QString clicked_name = item->text();
    QJsonArray data_arr = allUsers_contacts[clicked_name].toArray();
    QListWidget* list_3 = ui->listWidget_3;
    QJsonObject contact3;
    QString contact3_id;
    //insert selected listWidget2-contact's datas in third-listWidget
    for(QJsonValueRef user_ref:data_arr)
    {
        contact3 = user_ref.toObject();
        if(contact3["status"].toString()=="user")
        {
            contact3_id = contact3["id"].toString();
            QListWidgetItem* item = new QListWidgetItem(contact3_id);
            list_3->addItem(item);
        }
    }
}

