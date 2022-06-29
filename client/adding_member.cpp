#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "adding_member.h"
#include "ui_adding_member.h"

adding_member::adding_member(QString id, MyClient* client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adding_member)
{
    ui->setupUi(this);
    setFixedSize(size());
    user_id = id;
    this->client = client;
    QFile file(id + "%contacts.txt");
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        QStringList all_data = stream.readLine().split(',');

        QListWidget* list = ui->listWidget;
        for(QString id:all_data){
            if(id == "")
                continue;
            if(id.length() > 1 && id[1] == '%')
                continue;
            QListWidgetItem* item = new QListWidgetItem(id);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Unchecked);
            list->addItem(item);
            numbOfContacts++;
        }
    }
}

adding_member::~adding_member()
{
    delete ui;
}

void adding_member::on_pbn_ok_clicked()
{
    QString team_name = "aaa";
    if(team_name.isEmpty()){
        QMessageBox::warning(this, "Invalid input", "You must choose a groupe name!");
        return;
    }
    QListWidget* list = ui->listWidget;
    QJsonObject req_obj;
    req_obj["status"] = "createGroupe";
    req_obj["name"] = team_name;
    req_obj["1"] = user_id;
    int counter =0;
    for(int i=0; i< numbOfContacts; i++){
        QListWidgetItem* item = list->item(i);
        if(item->checkState()){
            req_obj[QString::number(counter+2)] = item->text();
            counter++;
        }
    }
    if(counter==0){
        QMessageBox::warning(this, "Invalid input", "You must choose at least one contact!");
        return;
    }
    req_obj["max"] = counter + 1;
    QJsonDocument req_doc(req_obj);
    QByteArray req_b = req_doc.toJson();
    QByteArray response = client->request_to_server(&req_b);
    QString response_str = QString::fromUtf8(response);
    if(response_str == "not accepted"){
        QMessageBox::warning(this, "Invalid input", "The groupe name has been taken!");
        return;
    }
    emit groupe_created(team_name);
    this->close();
}

