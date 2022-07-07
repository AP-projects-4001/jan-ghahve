/********************************************************************************
** Form generated from reading UI file 'forgetpass.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORGETPASS_H
#define UI_FORGETPASS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ForgetPass
{
public:
    QPushButton *pushButton_2;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QDialog *ForgetPass)
    {
        if (ForgetPass->objectName().isEmpty())
            ForgetPass->setObjectName(QString::fromUtf8("ForgetPass"));
        ForgetPass->resize(345, 180);
        ForgetPass->setStyleSheet(QString::fromUtf8("background-color:rgb(191, 215, 234);"));
        pushButton_2 = new QPushButton(ForgetPass);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(110, 110, 111, 31));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 48, 73);\n"
"border-radius: 10%;\n"
"color:rgb(34, 119, 255)"));
        lineEdit = new QLineEdit(ForgetPass);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(70, 50, 191, 31));
        lineEdit->setStyleSheet(QString::fromUtf8("background-color:rgb(253, 240, 213);\n"
"border-radius: 10%;"));
        label = new QLabel(ForgetPass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 10, 131, 20));

        retranslateUi(ForgetPass);

        QMetaObject::connectSlotsByName(ForgetPass);
    } // setupUi

    void retranslateUi(QDialog *ForgetPass)
    {
        ForgetPass->setWindowTitle(QCoreApplication::translate("ForgetPass", "Dialog", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ForgetPass", "Send Password", nullptr));
        label->setText(QCoreApplication::translate("ForgetPass", "Enter your email :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ForgetPass: public Ui_ForgetPass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORGETPASS_H
