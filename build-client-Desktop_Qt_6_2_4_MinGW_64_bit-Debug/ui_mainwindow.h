/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionprofile;
    QAction *actionlvl_3_graph;
    QAction *actionchange_password;
    QWidget *centralwidget;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QScrollBar *verticalScrollBar;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_4;
    QLineEdit *lineEdit;
    QGraphicsView *graphicsView;
    QGroupBox *groupBox;
    QTextEdit *textEdit;
    QGroupBox *groupBox_2;
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QMenu *menusetting;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(986, 684);
        actionprofile = new QAction(MainWindow);
        actionprofile->setObjectName(QString::fromUtf8("actionprofile"));
        actionlvl_3_graph = new QAction(MainWindow);
        actionlvl_3_graph->setObjectName(QString::fromUtf8("actionlvl_3_graph"));
        actionchange_password = new QAction(MainWindow);
        actionchange_password->setObjectName(QString::fromUtf8("actionchange_password"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(90, 20, 811, 511));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox_3 = new QGroupBox(widget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        scrollArea = new QScrollArea(groupBox_3);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(20, 50, 201, 361));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 199, 359));
        verticalScrollBar = new QScrollBar(scrollAreaWidgetContents_2);
        verticalScrollBar->setObjectName(QString::fromUtf8("verticalScrollBar"));
        verticalScrollBar->setGeometry(QRect(180, 0, 16, 351));
        verticalScrollBar->setOrientation(Qt::Vertical);
        scrollArea->setWidget(scrollAreaWidgetContents_2);

        horizontalLayout_2->addWidget(groupBox_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_4 = new QGroupBox(widget);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setStyleSheet(QString::fromUtf8("border:hide;"));
        lineEdit = new QLineEdit(groupBox_4);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(80, 0, 491, 51));
        graphicsView = new QGraphicsView(groupBox_4);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 0, 61, 51));

        verticalLayout->addWidget(groupBox_4);

        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("border:hide;"));
        textEdit = new QTextEdit(groupBox);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(10, -50, 561, 381));

        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setStyleSheet(QString::fromUtf8("border:hide;\n"
"border-right:2px solid blue;\n"
"\n"
""));
        plainTextEdit = new QPlainTextEdit(groupBox_2);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 10, 511, 51));
        plainTextEdit->setStyleSheet(QString::fromUtf8("border:hide;"));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(520, 20, 41, 29));
        pushButton->setStyleSheet(QString::fromUtf8("border:hide;"));

        verticalLayout->addWidget(groupBox_2);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 4);
        verticalLayout->setStretch(2, 1);

        horizontalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 5);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 986, 26));
        menusetting = new QMenu(menubar);
        menusetting->setObjectName(QString::fromUtf8("menusetting"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menusetting->menuAction());
        menusetting->addSeparator();
        menusetting->addAction(actionprofile);
        menusetting->addAction(actionlvl_3_graph);
        menusetting->addAction(actionchange_password);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionprofile->setText(QCoreApplication::translate("MainWindow", "profile", nullptr));
        actionlvl_3_graph->setText(QCoreApplication::translate("MainWindow", "lvl 3 graph", nullptr));
        actionchange_password->setText(QCoreApplication::translate("MainWindow", "change password", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_4->setTitle(QString());
        groupBox->setTitle(QString());
        groupBox_2->setTitle(QString());
        pushButton->setText(QCoreApplication::translate("MainWindow", "send", nullptr));
        menusetting->setTitle(QCoreApplication::translate("MainWindow", "setting", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
