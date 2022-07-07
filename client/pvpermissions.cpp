#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include "pvpermissions.h"
#include "ui_pvpermissions.h"
PvPermissions::PvPermissions(QString id, QString permissions, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PvPermissions),
    id(id)
{
    ui->setupUi(this);
    setFixedSize(size());

    //open database of user contacts and show
    QFile file(id + "%contacts.txt");
    if(file.open(QIODevice::ReadOnly)){
        QTextStream stream(&file);
        QStringList all_data = stream.readLine().split(',');
        QStringList permissoinsList = permissions.split('%');
        QListWidget* list = ui->listWidget;
        for(QString id:all_data){
            if(id == "")
                continue;
            if(id.length() > 1 && id[1] == '%')
                continue;
            QListWidgetItem* item = new QListWidgetItem(id);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            if(permissoinsList.contains(id))
                item->setCheckState(Qt::Checked);
            else
                item->setCheckState(Qt::Unchecked);

            list->addItem(item);
            numbOfContacts++;
        }
    }
}

PvPermissions::~PvPermissions()
{
    delete ui;
}

void PvPermissions::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->checkState())
        item->setCheckState(Qt::Unchecked);
    else
        item->setCheckState(Qt::Checked);
}

void PvPermissions::on_pbn_ok_clicked()
{
    //add selected contacts to restricted list for permission
    QListWidget* list = ui->listWidget;
    QString admins = "";
    for(int i=0; i< numbOfContacts; i++){
        QListWidgetItem* item = list->item(i);
        if(item->checkState()){
            admins += (item->text()+"%");
        }
    }
    emit permissions_modified(admins);
    this->close();
}


void PvPermissions::on_pushButton_2_clicked()
{

}

