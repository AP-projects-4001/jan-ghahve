#ifndef SEARCH_H
#define SEARCH_H

#include <QWidget>
#include <QDialog>
#include <QJsonObject>
#include "myclient.h"
namespace Ui {
class search;
}

class search : public QDialog
{
    Q_OBJECT

public:
    explicit search(MyClient* client, QWidget *parent = nullptr);
    ~search();

private slots:
    void on_pbn_ok_clicked();

    void on_pbn_cancel_clicked();

signals:
    void users_found(QStringList);

private:
    Ui::search *ui;
    MyClient* client;
    QJsonObject all_users;
};

#endif // SEARCH_H
