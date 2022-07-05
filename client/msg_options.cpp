#include "msg_options.h"
#include "ui_msg_options.h"

Msg_Options::Msg_Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Msg_Options)
{
    ui->setupUi(this);
    setFixedSize(size());
}

Msg_Options::~Msg_Options()
{
    delete ui;
}
