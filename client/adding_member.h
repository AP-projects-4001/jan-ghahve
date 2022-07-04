#ifndef ADDING_MEMBER_H
#define ADDING_MEMBER_H

#include <QDialog>
#include "myclient.h"
#include <QListWidget>

namespace Ui {
class adding_member;
}

class adding_member : public QDialog
{
    Q_OBJECT

public:
    explicit adding_member(QString id, MyClient* client, QString chat, QWidget *parent = nullptr);
    ~adding_member();

private slots:
    void on_pbn_ok_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_2_clicked();

signals:
    void group_created(QString);
    void channel_created(QString);

private:
    Ui::adding_member *ui;
    int numbOfContacts = 0;
    QString user_id, chat;
    MyClient *client;

};

#endif // ADDING_MEMBER_H
