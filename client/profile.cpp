#include "profile.h"
#include "ui_profile.h"

Profile::Profile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Profile)
{
    ui->setupUi(this);
    setFixedSize(size());
}

Profile::~Profile()
{
    delete ui;
}
