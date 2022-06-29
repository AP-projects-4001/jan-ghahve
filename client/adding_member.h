#ifndef ADDING_MEMBER_H
#define ADDING_MEMBER_H

#include <QDialog>
#include "myclient.h"

namespace Ui {
class adding_member;
}

class adding_member : public QDialog
{
    Q_OBJECT

public:
    explicit adding_member(QString id, MyClient* client, QWidget *parent = nullptr);
    ~adding_member();

private slots:
    void on_pbn_ok_clicked();

signals:
    void groupe_created(QString);

private:
    Ui::adding_member *ui;
    int numbOfContacts = 0;
    QString user_id;
    MyClient *client;
};

#endif // ADDING_MEMBER_H
