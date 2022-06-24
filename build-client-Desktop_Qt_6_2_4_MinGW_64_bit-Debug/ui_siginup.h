/********************************************************************************
** Form generated from reading UI file 'siginup.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGINUP_H
#define UI_SIGINUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_siginup
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *led_name;
    QLineEdit *led_phonenumb;
    QLineEdit *led_email;
    QLineEdit *led_pass;
    QPushButton *pbn_submit;
    QPushButton *pbn_cancel;
    QDateEdit *dateEdit;

    void setupUi(QWidget *siginup)
    {
        if (siginup->objectName().isEmpty())
            siginup->setObjectName(QString::fromUtf8("siginup"));
        siginup->resize(451, 359);
        label = new QLabel(siginup);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 60, 63, 20));
        label_2 = new QLabel(siginup);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(100, 100, 111, 20));
        label_3 = new QLabel(siginup);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(110, 140, 81, 20));
        label_4 = new QLabel(siginup);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(110, 180, 63, 20));
        label_5 = new QLabel(siginup);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(110, 220, 81, 20));
        led_name = new QLineEdit(siginup);
        led_name->setObjectName(QString::fromUtf8("led_name"));
        led_name->setGeometry(QRect(240, 60, 161, 26));
        led_phonenumb = new QLineEdit(siginup);
        led_phonenumb->setObjectName(QString::fromUtf8("led_phonenumb"));
        led_phonenumb->setGeometry(QRect(240, 100, 161, 26));
        led_email = new QLineEdit(siginup);
        led_email->setObjectName(QString::fromUtf8("led_email"));
        led_email->setGeometry(QRect(240, 180, 161, 26));
        led_pass = new QLineEdit(siginup);
        led_pass->setObjectName(QString::fromUtf8("led_pass"));
        led_pass->setGeometry(QRect(240, 220, 161, 26));
        pbn_submit = new QPushButton(siginup);
        pbn_submit->setObjectName(QString::fromUtf8("pbn_submit"));
        pbn_submit->setGeometry(QRect(300, 280, 93, 29));
        pbn_cancel = new QPushButton(siginup);
        pbn_cancel->setObjectName(QString::fromUtf8("pbn_cancel"));
        pbn_cancel->setGeometry(QRect(170, 280, 93, 29));
        dateEdit = new QDateEdit(siginup);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setGeometry(QRect(240, 140, 161, 26));

        retranslateUi(siginup);

        QMetaObject::connectSlotsByName(siginup);
    } // setupUi

    void retranslateUi(QWidget *siginup)
    {
        siginup->setWindowTitle(QCoreApplication::translate("siginup", "Form", nullptr));
        label->setText(QCoreApplication::translate("siginup", "Name :", nullptr));
        label_2->setText(QCoreApplication::translate("siginup", "PhoneNumber :", nullptr));
        label_3->setText(QCoreApplication::translate("siginup", "BirthDate :", nullptr));
        label_4->setText(QCoreApplication::translate("siginup", "Email :", nullptr));
        label_5->setText(QCoreApplication::translate("siginup", "Password :", nullptr));
        pbn_submit->setText(QCoreApplication::translate("siginup", "Submit", nullptr));
        pbn_cancel->setText(QCoreApplication::translate("siginup", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class siginup: public Ui_siginup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGINUP_H
