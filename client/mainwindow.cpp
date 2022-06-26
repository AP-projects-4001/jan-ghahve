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
    QListWidget* list = ui->listWidget;
    QListWidgetItem* item = new QListWidgetItem("pain");

    list->addItem(item);

}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //mitoni itemi ke click shode estefade koni (to vorodi tabe ferestade)
    ui->textEdit->insertPlainText("pain");
}

