/********************************************************************************
** Form generated from reading UI file 'loading.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADING_H
#define UI_LOADING_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_loading
{
public:
    QPushButton *pbn_signin;
    QPushButton *pbn_siginup;

    void setupUi(QWidget *loading)
    {
        if (loading->objectName().isEmpty())
            loading->setObjectName(QString::fromUtf8("loading"));
        loading->resize(355, 183);
        pbn_signin = new QPushButton(loading);
        pbn_signin->setObjectName(QString::fromUtf8("pbn_signin"));
        pbn_signin->setGeometry(QRect(70, 110, 93, 41));
        pbn_siginup = new QPushButton(loading);
        pbn_siginup->setObjectName(QString::fromUtf8("pbn_siginup"));
        pbn_siginup->setGeometry(QRect(200, 110, 93, 41));

        retranslateUi(loading);

        QMetaObject::connectSlotsByName(loading);
    } // setupUi

    void retranslateUi(QWidget *loading)
    {
        loading->setWindowTitle(QCoreApplication::translate("loading", "Form", nullptr));
        pbn_signin->setText(QCoreApplication::translate("loading", "sign in", nullptr));
        pbn_siginup->setText(QCoreApplication::translate("loading", "sign up", nullptr));
    } // retranslateUi

};

namespace Ui {
    class loading: public Ui_loading {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADING_H
