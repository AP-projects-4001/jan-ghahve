#ifndef PVPERMISSIONS_H
#define PVPERMISSIONS_H

#include <QDialog>
#include <QListWidgetItem>
#include "myclient.h"
namespace Ui {
class PvPermissions;
}

class PvPermissions : public QDialog
{
    Q_OBJECT

public:
    explicit PvPermissions(QString id, QString permissions, QWidget *parent = nullptr);
    ~PvPermissions();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pbn_ok_clicked();

    void on_pushButton_2_clicked();

signals:
    void permissions_modified(QString);

private:
    Ui::PvPermissions *ui;
    QString id;
    MyClient* client;
    int numbOfContacts = 0;
};

#endif // PVPERMISSIONS_H
