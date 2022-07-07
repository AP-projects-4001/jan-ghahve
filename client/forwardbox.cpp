#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "forwardbox.h"
#include "ui_forwardbox.h"

ForwardBox::ForwardBox(QString sender, QString message, MyClient* client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForwardBox),
    sender(sender),
    message(message),
    client(client)
{
    ui->setupUi(this);
    setFixedSize(size());

    QFile file(sender + "%contacts.txt");
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


    //age code kar nakard va ina, onja ke add mikoni ghablesh ina ro bezar
    ui->listWidget->setStyleSheet(
                "QListWidget:item"
                "{"
                "height: 30px;"
                "border: 1px solid gray;"
                "border-radius: 6px;"
                "}"
                "QListWidget:item:selected"
                "{"
                "background-color:gray;"
                "}"
                "*{"
                "background-color:rgb(253, 240, 213);"
                "border-radius: 10%;"
                "font-size:18px;"
                "font:url(:/new/prefix1/Lato-Italic.ttf)"
                "}");
    ui->listWidget->setCursor(Qt::PointingHandCursor);
    ui->listWidget->setSpacing(0);
}

ForwardBox::~ForwardBox()
{
    delete ui;
}

void ForwardBox::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->checkState())
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}

void ForwardBox::on_pbn_ok_clicked()
{
    QListWidget* list = ui->listWidget;
    QJsonObject req_obj;
    req_obj["status"] = "forwardMessage";
    req_obj["sender"] = sender;
    req_obj["message"] = message;
    int counter =0;
    for(int i=0; i< numbOfContacts; i++){
        QListWidgetItem* item = list->item(i);
        if(item->checkState()){
            counter++;
            req_obj[QString::number(counter)] = item->text();
        }
    }
    if(counter==0){
        QMessageBox::warning(this, "Invalid input", "You must choose at least one contact!");
        return;
    }
    req_obj["max"] = counter;
    QJsonDocument req_doc(req_obj);
    QByteArray req_b = req_doc.toJson();
    client->request_to_server(&req_b);
    this->close();
}


void ForwardBox::on_pushButton_2_clicked()
{
    this->close();
}

