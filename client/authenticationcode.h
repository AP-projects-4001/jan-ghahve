#ifndef AUTHENTICATIONCODE_H
#define AUTHENTICATIONCODE_H

#include <QDialog>
#include <QJsonObject>
#include "myclient.h"

namespace Ui {
class AuthenticationCode;
}

class AuthenticationCode : public QDialog
{
    Q_OBJECT

public:
    explicit AuthenticationCode(QJsonObject user, MyClient* client, QWidget *parent = nullptr);
    ~AuthenticationCode();

private slots:
    void on_pbn_ok_clicked();

    void on_pbn_cancel_clicked();

signals:
    void user_authenticated();

private:
    Ui::AuthenticationCode *ui;

    MyClient* client;
    QJsonObject user;
};

#endif // AUTHENTICATIONCODE_H
