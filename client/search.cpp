#include "search.h"
#include "ui_search.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

search::search(MyClient* client,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::search)
{
    ui->setupUi(this);
    this->client = client;
    if(client->is_client_connectd()){
        QJsonObject request;
        request["status"] ="allUsersInfo";
        QJsonDocument requset_doc(request);
        QByteArray request_b=requset_doc.toJson();
        QByteArray response = client->request_to_server(&request_b);

        QJsonDocument response_d = QJsonDocument::fromJson(response);
        all_users = response_d.object();
    }
    setFixedSize(size());
}

search::~search()
{
    delete ui;
}

void search::on_pbn_ok_clicked()
{
    QString name = ui->led_name->text();
    QString id = ui->led_id->text();
    QString number = ui->led_number->text();
    QStringList foundedIds;

    if(name == "" && id == "" && number == ""){
        QMessageBox::warning(this, "Invalid input", "You must fill at least one of the values!");
        return;
    }

    QJsonArray data_arr = all_users["profiles"].toArray();
    QJsonObject user;
    for(QJsonValueRef userRef:data_arr){
        user = userRef.toObject();
        if(user["id"].toString() == id || user["name"].toString() == name || user["number"].toString() == number){
            foundedIds.append(user["id"].toString());
        }
    }
    if(foundedIds.length() == 0){
        QMessageBox::warning(this, "","No user has founded!");
        return;
    }

    QListWidget* list = ui->listWidget;
    list->clear();
    numbOfUsers = 0;
    for(QString id:foundedIds){
        QListWidgetItem* item = new QListWidgetItem(id);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        list->addItem(item);
        numbOfUsers++;
    }

}

void search::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->checkState())
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}


void search::on_pbn_cancel_clicked()
{
    this->close();
}


void search::on_pbn_submit_clicked()
{
    QStringList foundedIds;
    QListWidget* list = ui->listWidget;
    int counter = 0;
    for(int i=0; i< numbOfUsers; i++){
        QListWidgetItem* item = list->item(i);
        if(item->checkState()){
            foundedIds.append(item->text());
            counter++;
        }
    }
    if(counter == 0){
        QMessageBox::warning(this, "Invalid input", "You must choose at least one user!");
        return;
    }
    emit users_found(foundedIds);
    this->close();
}

