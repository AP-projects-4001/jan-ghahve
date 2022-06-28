#include "adding_member.h"
#include "ui_adding_member.h"

adding_member::adding_member(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adding_member)
{
    ui->setupUi(this);
    setFixedSize(size());
    QListWidgetItem* item = new QListWidgetItem("item");
    QListWidgetItem* item2 = new QListWidgetItem("item");
    QListWidgetItem* item3 = new QListWidgetItem("item");

    item->setForeground(Qt::blue);
    item->setBackground(Qt::gray);
    ui->listWidget->setStyleSheet("QListWidget:item{height: 40px;border: 2px solid red;}");
    ui->listWidget->setCursor(Qt::PointingHandCursor);
    ui->listWidget->setSpacing(5);
    ui->listWidget->addItem(item);
    ui->listWidget->addItem(item2);
    ui->listWidget->addItem(item3);




}

adding_member::~adding_member()
{
    delete ui;
}
