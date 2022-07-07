#ifndef FORGETPASS_H
#define FORGETPASS_H

#include <QDialog>
#include "myclient.h"
namespace Ui {
class ForgetPass;
}

class ForgetPass : public QDialog
{
    Q_OBJECT

public:
    explicit ForgetPass(QWidget *parent = nullptr);
    ~ForgetPass();

public slots:
    void on_pushButton_2_clicked();
    void on_user_authenticated();

private:
    Ui::ForgetPass *ui;
    MyClient* client;
};

#endif // FORGETPASS_H
