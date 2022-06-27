#ifndef SIGINUP_H
#define SIGINUP_H

#include <QWidget>
#include "myclient.h"

namespace Ui {
class siginup;
}

class siginup : public QWidget
{
    Q_OBJECT

public:
    explicit siginup(QWidget *parent = nullptr);
    ~siginup();
    bool validate_signup_data(QString name, QString id, QString email, QString birthdate, QString pass, QString conf_pass, QString number);

private slots:
    void on_pbn_submit_clicked();

private:
    Ui::siginup *ui;
    MyClient* client;
};

#endif // SIGINUP_H
