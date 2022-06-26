#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include "myclient.h"
#include <QVBoxLayout>
#include <QListWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString id, QWidget *parent = nullptr);
    ~MainWindow();
    void get_user_info(QString id);
    void get_all_users();
    void add_safebar();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    MyClient *client;
    QJsonObject user_data;
    QJsonObject contact_info;
    QJsonObject all_users;
};
#endif // MAINWINDOW_H
