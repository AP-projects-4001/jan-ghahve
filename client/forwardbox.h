#ifndef FORWARDBOX_H
#define FORWARDBOX_H

#include <QDialog>
#include <QListWidget>
#include "myclient.h"
namespace Ui {
class ForwardBox;
}

class ForwardBox : public QDialog
{
    Q_OBJECT

public:
    explicit ForwardBox(QString sender, QString message, MyClient* client, QWidget *parent = nullptr);
    ~ForwardBox();
private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pbn_ok_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ForwardBox *ui;
    QString sender;
    QString message;
    MyClient* client;
    int numbOfContacts = 0;
};

#endif // FORWARDBOX_H
