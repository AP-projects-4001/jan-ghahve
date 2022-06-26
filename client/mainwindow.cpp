#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->test,&QPushButton::clicked,this,&MainWindow::add_safebar);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::add_safebar()
{
    QVBoxLayout* layout = qobject_cast<QVBoxLayout *>(ui->verticalLayout->layout());

    QPushButton *Button = new QPushButton("chat",ui->groupBox_3);
    Button->setStyleSheet("border: 1px solid; border-radius:10px; background-color: palette(base); ");
    layout->insertWidget(0,Button);


}
void MainWindow::on_test_clicked()
{

}

