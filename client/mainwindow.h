#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>
#include "myclient.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QThread>

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
    void get_user_contacts();
    void get_allUsers_contacts();

private slots:
    void on_usersFound(QStringList users);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_messagerecievd1(QString senderId, QString message);
    void on_pbn_send_clicked();
    void on_newgroup_clicked();
    void on_graph_clicked();

    void on_pbn_search_clicked();

private:
    Ui::MainWindow *ui;
    void pain();
    MyClient *client;
    QJsonObject user_data;
    QJsonObject contact_info;
    QJsonObject all_users;
    QJsonObject all_users_contacts;
    bool chat_switched;
};
#endif // MAINWINDOW_H
