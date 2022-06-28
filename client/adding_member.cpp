#include "adding_member.h"
#include "ui_adding_member.h"

adding_member::adding_member(QString id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adding_member)
{
    ui->setupUi(this);
    setFixedSize(size());

}

adding_member::~adding_member()
{
    delete ui;
}
