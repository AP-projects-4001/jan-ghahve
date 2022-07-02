#ifndef CHANNELPERMISSIONS_H
#define CHANNELPERMISSIONS_H

#include <QDialog>
#include <QListWidget>
#include "myclient.h"

namespace Ui {
class ChannelPermissions;
}

class ChannelPermissions : public QDialog
{
    Q_OBJECT

public:
    explicit ChannelPermissions(QString id, QWidget *parent = nullptr);
    ~ChannelPermissions();
    bool is_admin(QString id, QStringList admins_list);
private slots:
    void on_pbn_ok_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
private:
    Ui::ChannelPermissions *ui;

    QString id;
    int numbOfMembers = 0;
    MyClient* client;
};

#endif // CHANNELPERMISSIONS_H
