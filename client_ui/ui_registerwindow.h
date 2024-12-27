/********************************************************************************
** Form generated from reading UI file 'registerwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERWINDOW_H
#define UI_REGISTERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLineEdit *passwordLineEdit;
    QLineEdit *usernameLineEdit;
    QLabel *label_2;
    QPushButton *loginButton_2;
    QLabel *label_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(488, 577);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(90, 160, 67, 17));
        passwordLineEdit = new QLineEdit(centralwidget);
        passwordLineEdit->setObjectName(QStringLiteral("passwordLineEdit"));
        passwordLineEdit->setGeometry(QRect(170, 220, 191, 41));
        usernameLineEdit = new QLineEdit(centralwidget);
        usernameLineEdit->setObjectName(QStringLiteral("usernameLineEdit"));
        usernameLineEdit->setGeometry(QRect(170, 150, 191, 41));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(90, 230, 67, 17));
        loginButton_2 = new QPushButton(centralwidget);
        loginButton_2->setObjectName(QStringLiteral("loginButton_2"));
        loginButton_2->setGeometry(QRect(200, 330, 121, 51));
        loginButton_2->setStyleSheet(QStringLiteral(""));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(180, 60, 171, 31));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\345\220\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\345\257\206\347\240\201", Q_NULLPTR));
        loginButton_2->setText(QApplication::translate("MainWindow", "\346\263\250\345\206\214", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", " \350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215\345\222\214\345\257\206\347\240\201\357\274\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERWINDOW_H
