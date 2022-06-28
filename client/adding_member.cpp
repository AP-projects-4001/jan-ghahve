#include "adding_member.h"
#include "ui_adding_member.h"

adding_member::adding_member(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adding_member)
{
    ui->setupUi(this);
    setFixedSize(size());


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
