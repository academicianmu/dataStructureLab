/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.15.16
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QPushButton *linkedListButton;
    QPushButton *stackButton;
    QPushButton *queueButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        linkedListButton = new QPushButton(centralwidget);
        linkedListButton->setObjectName(QString::fromUtf8("linkedListButton"));

        verticalLayout->addWidget(linkedListButton);

        stackButton = new QPushButton(centralwidget);
        stackButton->setObjectName(QString::fromUtf8("stackButton"));

        verticalLayout->addWidget(stackButton);

        queueButton = new QPushButton(centralwidget);
        queueButton->setObjectName(QString::fromUtf8("queueButton"));

        verticalLayout->addWidget(queueButton);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\347\273\223\346\236\204\345\256\236\351\252\214\345\256\244", nullptr));
        linkedListButton->setText(QCoreApplication::translate("MainWindow", "\351\223\276\350\241\250\346\223\215\344\275\234", nullptr));
        stackButton->setText(QCoreApplication::translate("MainWindow", "\346\240\210\346\223\215\344\275\234", nullptr));
        queueButton->setText(QCoreApplication::translate("MainWindow", "\351\230\237\345\210\227\346\223\215\344\275\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
