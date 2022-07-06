/********************************************************************************
** Form generated from reading UI file 'forwardbox.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORWARDBOX_H
#define UI_FORWARDBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ForwardBox
{
public:
    QListWidget *listWidget;
    QPushButton *pushButton;

    void setupUi(QDialog *ForwardBox)
    {
        if (ForwardBox->objectName().isEmpty())
            ForwardBox->setObjectName(QString::fromUtf8("ForwardBox"));
        ForwardBox->resize(355, 391);
        ForwardBox->setStyleSheet(QString::fromUtf8("background-color:rgb(191, 215, 234);"));
        listWidget = new QListWidget(ForwardBox);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(140, 20, 191, 351));
        listWidget->viewport()->setProperty("cursor", QVariant(QCursor(Qt::PointingHandCursor)));
        listWidget->setStyleSheet(QString::fromUtf8("background-color:rgb(253, 240, 213);\n"
"border-radius: 10%;"));
        pushButton = new QPushButton(ForwardBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 300, 93, 29));
        pushButton->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton->setStyleSheet(QString::fromUtf8("background-color:rgb(0, 48, 73);\n"
"border-radius: 10px;\n"
"color:rgb(245, 223, 187);"));

        retranslateUi(ForwardBox);

        QMetaObject::connectSlotsByName(ForwardBox);
    } // setupUi

    void retranslateUi(QDialog *ForwardBox)
    {
        ForwardBox->setWindowTitle(QCoreApplication::translate("ForwardBox", "Dialog", nullptr));
        pushButton->setText(QCoreApplication::translate("ForwardBox", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ForwardBox: public Ui_ForwardBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORWARDBOX_H
