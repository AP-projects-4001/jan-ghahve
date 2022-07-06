#include "forwardbox.h"
#include "ui_forwardbox.h"

ForwardBox::ForwardBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForwardBox)
{
    ui->setupUi(this);
    setFixedSize(size());

}

ForwardBox::~ForwardBox()
{
    delete ui;
}
