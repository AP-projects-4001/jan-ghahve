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

}

geraph::~geraph()
{
    delete ui;
}
