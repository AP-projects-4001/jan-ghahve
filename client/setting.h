#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include "myclient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

namespace Ui {
class setting;
}

class setting : public QWidget
{
    Q_OBJECT

public:
    explicit setting(QJsonObject user_data ,QWidget *parent = nullptr);
    ~setting();

private slots:
    void on_pbn_edit_clicked();
    void on_permissionsmodified(QString permissions);
    void on_pbn_cancel_clicked();

    void on_pbn_save_clicked();

    void on_pbn_permissions_clicked();

private:
    Ui::setting *ui;
    QJsonObject user_data;
};

#endif // SETTING_H
