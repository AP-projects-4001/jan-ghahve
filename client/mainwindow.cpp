#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pain();
//    QObject::connect(ui->test,&QPushButton::clicked,this,&MainWindow::add_safebar);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::add_safebar()
{
    QListWidget* list = ui->listWidget;
    QListWidgetItem* item = new QListWidgetItem("pain");
//    item->setBackground(Qt::blue);
    list->addItem(item);

}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //mitoni itemi ke click shode estefade koni (to vorodi tabe ferestade)
//    ui->textEdit->insertPlainText("pain");
}

void MainWindow::pain()
{
    ui->pbn_send->setStyleSheet("*{border-image: url(:/images/send.png);"
                                "background-color:rgb(191, 215, 234);"
                                "border-radius: 20%;"
                                "background-position:center;}"
                                "*:hover{background-color:rgb(30, 157, 230);}");
}

void MainWindow::on_test_clicked()
{

}
