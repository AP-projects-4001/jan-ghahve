#ifndef GERAPH_H
#define GERAPH_H

#include <QDialog>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>
#include <QListWidget>
#include <QListWidgetItem>

namespace Ui {
class geraph;
}

class geraph : public QDialog
{
    Q_OBJECT

public:
    explicit geraph(QString myUsername ,QJsonObject inp_allUsers_contacts ,QWidget *parent = nullptr);
    ~geraph();

private slots:
    void on_pushButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_2_itemClicked(QListWidgetItem *item);

private:
    Ui::geraph *ui;
    QJsonObject allUsers_contacts;
    QString my_username;
};

#endif // GERAPH_H
