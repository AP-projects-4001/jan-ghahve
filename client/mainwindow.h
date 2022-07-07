#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>
#include <QVBoxLayout>
#include <QListWidget>
#include <QThread>
#include <QSystemTrayIcon>
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
    void get_user_info(QString id);
    void get_all_users();
    void add_safebar();
    void get_user_contacts();
    bool is_already_added(QString id);
    QStringList get_user_contacts_from_file();
    void add_item_to_listwidget(QString name);
    bool is_admin(QString id, QStringList admins_list);
    void add_message(bool flag, QString sender, QString message);
    void append_message_in_database(QString message, QString senderId);
    void get_chat();

private slots:
    void set_msesage_widget_to_default(int i);
    void on_usersFound(QStringList users);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_messagerecievd(QString senderId, QString message, QString chatId);
    void on_pbn_send_clicked();
    void on_newgroup_clicked();
    void on_graph_clicked();
    void on_pbn_search_clicked();
    void on_groupcreated(QString id);
    void on_newchannel_clicked();
    void on_setting_clicked();
    void on_userauthenticated(QString id);
    void on_userunauthenticated(QString id);
    void on_chat_message_clicked(QListWidgetItem* item);
    void on_pbn_search_2_clicked();
    void on_message_edited();
    void on_pbn_contact_name_clicked();
    void on_logout_clicked();

private:
    Ui::MainWindow *ui;
    void pain();
    MyClient *client;
    QJsonObject user_data;
    QJsonObject contact_info;
    QJsonObject all_users;
    QSystemTrayIcon* tray;
};
#endif // MAINWINDOW_H
