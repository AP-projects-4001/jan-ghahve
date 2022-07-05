#include "groupprofile.h"
#include "ui_groupprofile.h"
#include "channelpermissions.h"

GroupProfile::GroupProfile(QString id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupProfile)
{
    ui->setupUi(this);
    setFixedSize(size());

    this->id = id;
}

GroupProfile::~GroupProfile()
{
    delete ui;
}

void GroupProfile::on_pbn_permission_clicked()
{
    ChannelPermissions *chanelPermissions = new ChannelPermissions(id, this);
    chanelPermissions->show();
}




