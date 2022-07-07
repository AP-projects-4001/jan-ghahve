#include "forgetpass.h"
#include "ui_forgetpass.h"
#include "myclient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QMessageBox>

ForgetPass::ForgetPass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForgetPass)
{
    ui->setupUi(this);
    setFixedSize(size());


}

ForgetPass::~ForgetPass()
{
    delete ui;
}

void ForgetPass::on_pushButton_2_clicked()
{
    MyClient *client = new MyClient();
    QJsonObject req;
    req["status"] = "forgetpassword";
    req["email"] = ui->lineEdit->text();
    QJsonDocument req_doc(req);
    QByteArray req_b = req_doc.toJson();
    QString resp;
    if(client->connect_to_server())
    {
        QByteArray resp_b = client->request_to_server(&req_b);
        resp = QString(resp_b);

    }
    if(resp=="accepted")
    {
        QMessageBox::information(this, "Successfull!", "your password sent to your email address!");
    }
    else
    {
        QMessageBox::warning(this, "Error!", "incorrect email address!");
    }
    this->close();
    //QMessageBox::information(this, "Invalid input", "Phone number field cannot be empty!");

}

