#include "loading.h"
#include "ui_loading.h"
#include "siginup.h"
#include "signin.h"

loading::loading(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loading)
{
    ui->setupUi(this);
    setFixedSize(size());
}

loading::~loading()
{
    delete ui;
}

void loading::on_pbn_siginup_clicked()
{
    //Go to the sign-up page
    siginup* signup = new siginup();
    signup->setFixedSize(450,400);
    signup->show();
    this->close();
}

void loading::on_pbn_signin_clicked()
{
    //Go to the sign-in page
    signin* signin_window = new signin();
    signin_window->setFixedSize(450,400);
    signin_window->show();
    this->close();
}
