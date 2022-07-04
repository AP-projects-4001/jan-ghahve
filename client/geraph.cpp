#include "geraph.h"
#include "ui_geraph.h"

geraph::geraph(QWidget *parent) :
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
        contact_name = contact["id"].toString();
        QListWidgetItem* item = new QListWidgetItem(contact_name);
        list_1->addItem(item);
    }
}

geraph::~geraph()
{
    delete ui;
}
