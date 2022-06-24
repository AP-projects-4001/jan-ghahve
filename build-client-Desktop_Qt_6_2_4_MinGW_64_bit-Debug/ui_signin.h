/********************************************************************************
** Form generated from reading UI file 'signin.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNIN_H
#define UI_SIGNIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_signin
{
public:
    QDialogButtonBox *Login;
    QLineEdit *led_name;
    QLineEdit *led_pass;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pbn_forgetpass;

    void setupUi(QDialog *signin)
    {
        if (signin->objectName().isEmpty())
            signin->setObjectName(QString::fromUtf8("signin"));
        signin->resize(400, 300);
        Login = new QDialogButtonBox(signin);
        Login->setObjectName(QString::fromUtf8("Login"));
        Login->setGeometry(QRect(40, 240, 341, 32));
        Login->setOrientation(Qt::Horizontal);
        Login->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        led_name = new QLineEdit(signin);
        led_name->setObjectName(QString::fromUtf8("led_name"));
        led_name->setGeometry(QRect(162, 80, 161, 26));
        led_pass = new QLineEdit(signin);
        led_pass->setObjectName(QString::fromUtf8("led_pass"));
        led_pass->setGeometry(QRect(162, 120, 161, 26));
        label = new QLabel(signin);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(80, 80, 63, 20));
        label_2 = new QLabel(signin);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(70, 120, 81, 20));
        pbn_forgetpass = new QPushButton(signin);
        pbn_forgetpass->setObjectName(QString::fromUtf8("pbn_forgetpass"));
        pbn_forgetpass->setGeometry(QRect(70, 170, 131, 29));

        retranslateUi(signin);
        QObject::connect(Login, &QDialogButtonBox::accepted, signin, qOverload<>(&QDialog::accept));
        QObject::connect(Login, &QDialogButtonBox::rejected, signin, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(signin);
    } // setupUi

    void retranslateUi(QDialog *signin)
    {
        signin->setWindowTitle(QCoreApplication::translate("signin", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("signin", "Name :", nullptr));
        label_2->setText(QCoreApplication::translate("signin", "PassWord :", nullptr));
        pbn_forgetpass->setText(QCoreApplication::translate("signin", "Forget PassWord?", nullptr));
    } // retranslateUi

};

namespace Ui {
    class signin: public Ui_signin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNIN_H
