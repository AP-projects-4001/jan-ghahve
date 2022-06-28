#include "geraph.h"
#include "ui_geraph.h"

geraph::geraph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::geraph)
{
    ui->setupUi(this);
    setFixedSize(size());
}

geraph::~geraph()
{
    delete ui;
}
