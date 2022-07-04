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
    explicit Profile(QString id ,QWidget *parent = nullptr);
    ~Profile();
    void get_user_data(QString id);
    void show_informations();

private slots:
    void on_pbn_close_clicked();

private:
    Ui::Profile *ui;
    QJsonObject user_data;
};

#endif // PROFILE_H
