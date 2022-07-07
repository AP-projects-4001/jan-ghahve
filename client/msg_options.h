#ifndef MSG_OPTIONS_H
#define MSG_OPTIONS_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QDialog>
#include "myclient.h"
namespace Ui {
class Msg_Options;
}

class Msg_Options : public QDialog
{
    Q_OBJECT

public:
    explicit Msg_Options(MyClient* client, QJsonObject data, QWidget *parent = nullptr);
    ~Msg_Options();

private slots:
    void on_pbn_edit_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void message_edited();

private:
    Ui::Msg_Options *ui;
    QJsonObject data;
    MyClient* client;
};

#endif // MSG_OPTIONS_H
