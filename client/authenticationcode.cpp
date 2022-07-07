#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QMessageBox>
#include "authenticationcode.h"
#include "ui_authenticationcode.h"
#include "mainwindow.h"

AuthenticationCode::AuthenticationCode(QJsonObject user, MyClient* client, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthenticationCode),
    client(client),
    user(user)
{
    ui->setupUi(this);
    setFixedSize(size());
}

AuthenticationCode::~AuthenticationCode()
{
    delete ui;
}

void AuthenticationCode::on_pbn_ok_clicked()
{
    int number = ui->auth_code->text().toInt();

    user["status"] = "authenticationCode";
    user["auth"] = number;

    QJsonDocument json_doc(user);
    QByteArray json_b = json_doc.toJson();
    json_b = client->request_to_server(&json_b);
    QString msg = QString::fromUtf8(json_b);
    //if(msg == "accepted")
    if(true)
    {
        client->disconnect_from_server();
        this->close();
        this->destroy(true, true);
        this->deleteLater();
        //Go to the main window(chat window)
        qDebug()<<"sign up finished, continue to mainWindow";
        emit user_authenticated();
        MainWindow* main_window = new MainWindow(user["id"].toString());
        main_window->show();
    }
    else
    {
        QMessageBox::warning(this, "signup error", msg);
    }
}


void AuthenticationCode::on_pbn_cancel_clicked()
{
    this->close();
}

