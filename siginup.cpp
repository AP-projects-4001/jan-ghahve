#include "siginup.h"
#include "ui_siginup.h"

siginup::siginup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::siginup)
{
    ui->setupUi(this);
}

siginup::~siginup()
{
    delete ui;
}
