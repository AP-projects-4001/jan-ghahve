#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>
#include "myclient.h"

namespace Ui {
class signin;
}

class signin : public QDialog
{
    Q_OBJECT

public:
    explicit signin(QWidget *parent = nullptr);
    ~signin();
    bool validate_signin_data(QString id,QString pass);

private slots:
    void on_pbn_ok_clicked();
    void on_userauthenticated();
    void on_pbn_cancel_clicked();

private:
    Ui::signin *ui;
    MyClient* client;
};

#endif // SIGNIN_H
