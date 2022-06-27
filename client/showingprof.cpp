#include "showingprof.h"
#include "ui_showingprof.h"

showingProf::showingProf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showingProf)
{
    ui->setupUi(this);
}

showingProf::~showingProf()
{
    delete ui;
}
