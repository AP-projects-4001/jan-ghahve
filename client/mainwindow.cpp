#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(MyClient* client, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

