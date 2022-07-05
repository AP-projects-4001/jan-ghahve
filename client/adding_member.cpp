#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include "adding_member.h"
#include "ui_adding_member.h"
#include "image_convertation.h"

adding_member::adding_member(QString id, MyClient* client, QString chat, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adding_member)
{
    ui->setupUi(this);
    setFixedSize(size());
    user_id = id;
    this->client = client;
    this->chat = chat;
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

adding_member::~adding_member()
{
    delete ui;
}

void adding_member::on_pbn_ok_clicked()
{
    QString name = ui->led_name->text();
    if(name.isEmpty()){
        QMessageBox::warning(this, "Invalid input", "You must choose a name!");
        return;
    }
    QListWidget* list = ui->listWidget;
    QJsonObject req_obj;
    if(chat == "group"){
        req_obj["status"] = "createGroup";
    }else{
        req_obj["status"] = "createChannel";
    }
    req_obj["name"] = name;
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
    if(chat == "group")
    {
        QImage image(":/images/resourses/group_default_profile.jpg");
        auto pix = QPixmap::fromImage(image);
        ImageConvertation *imageConvertor = new ImageConvertation();
        QJsonValue val = imageConvertor->jsonValFromPixmap(pix);
        req_obj["img"] = val;
    }
    else
    {
        QImage image(":/images/resourses/channel_default_profile.jpg");
        auto pix = QPixmap::fromImage(image);
        ImageConvertation *imageConvertor = new ImageConvertation();
        QJsonValue val = imageConvertor->jsonValFromPixmap(pix);
        req_obj["img"] = val;
    }

    QJsonDocument req_doc(req_obj);
    QByteArray req_b = req_doc.toJson();
    QByteArray response = client->request_to_server(&req_b);
    QString response_str = QString::fromUtf8(response);
    if(response_str == "not accepted"){
        QMessageBox::warning(this, "Invalid input", "The name has been taken!");
        return;
    }
    if(chat == "group"){
        emit group_created(name);
    }else{
        emit group_created(name);
    }
    this->close();
}

void adding_member::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->checkState())
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}


void adding_member::on_pushButton_2_clicked()
{
    this->close();
}

