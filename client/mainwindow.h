#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include "myclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString id, QWidget *parent = nullptr);
    ~MainWindow();
    void test_function();

private:
    Ui::MainWindow *ui;
    MyClient *client;
    QJsonObject user_data;
    QJsonObject contact_info;
};
#endif // MAINWINDOW_H
