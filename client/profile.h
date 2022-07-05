#ifndef PROFILE_H
#define PROFILE_H

#include <QDialog>
#include <QString>
#include "myclient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>

namespace Ui {
class Profile;
}

class Profile : public QDialog
{
    Q_OBJECT

public:
    explicit Profile(QString contact_id, QJsonObject user_data, QWidget *parent = nullptr);
    ~Profile();
    void show_informations();

private slots:
    void on_pbn_close_clicked();

private:
    Ui::Profile *ui;
    QJsonObject user_data;
    QString contact_id;
};

#endif // PROFILE_H
