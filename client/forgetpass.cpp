#include "forgetpass.h"
#include "ui_forgetpass.h"

ForgetPass::ForgetPass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgetPass)
{
    ui->setupUi(this);
    setFixedSize(size());
}

ForgetPass::~ForgetPass()
{
    delete ui;
}
