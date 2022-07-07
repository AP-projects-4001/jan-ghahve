#include "msg_options.h"
#include "ui_msg_options.h"

Msg_Options::Msg_Options(MyClient* client, QJsonObject data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Msg_Options),
    data(data),
    client(client)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->led_message->setText(data["message"].toString());
    ui->led_message->hide();
}

Msg_Options::~Msg_Options()
{
    delete ui;
}

void Msg_Options::on_pbn_edit_clicked()
{
    if(ui->led_message->isHidden()){
        ui->led_message->show();
        return;
    }
    QString new_msg = ui->led_message->text();
    if(new_msg == data["message"].toString() || new_msg.isEmpty())
        return;

    data["status"] = "editMessage";
    data["message"] = new_msg;
    QJsonDocument data_doc(data);
    QByteArray data_b = data_doc.toJson();
    if(client->is_client_connectd())
        client->request_to_server(&data_b);
    emit message_edited();
    this->close();
}


void Msg_Options::on_pushButton_clicked()
{
    this->close();
}


void Msg_Options::on_pushButton_2_clicked()
{
    data["status"] = "editMessage";
    data.remove("message");
    QJsonDocument data_doc(data);
    QByteArray data_b = data_doc.toJson();
    if(client->is_client_connectd())
        client->request_to_server(&data_b);
    emit message_edited();
    this->close();
}

