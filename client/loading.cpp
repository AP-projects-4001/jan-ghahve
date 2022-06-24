#include "loading.h"
#include "ui_loading.h"
#include "siginup.h"

loading::loading(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loading)
{
    ui->setupUi(this);
}

loading::~loading()
{
    delete ui;
}

void loading::on_pbn_siginup_clicked()
{
    siginup* signup = new siginup();
    signup->setFixedSize(450,400);
    signup->show();
    this->close();
}

