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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_registerwindow
{
public:
    QLineEdit *passwordLineEdit;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *usernameLineEdit;
    QPushButton *register_2;
    QLabel *label_3;

    void setupUi(QWidget *registerwindow)
    {
        if (registerwindow->objectName().isEmpty())
            registerwindow->setObjectName(QStringLiteral("registerwindow"));
        registerwindow->resize(403, 564);
        passwordLineEdit = new QLineEdit(registerwindow);
        passwordLineEdit->setObjectName(QStringLiteral("passwordLineEdit"));
        passwordLineEdit->setGeometry(QRect(110, 220, 191, 41));
        label = new QLabel(registerwindow);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 160, 67, 17));
        label_2 = new QLabel(registerwindow);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 230, 67, 17));
        usernameLineEdit = new QLineEdit(registerwindow);
        usernameLineEdit->setObjectName(QStringLiteral("usernameLineEdit"));
        usernameLineEdit->setGeometry(QRect(110, 150, 191, 41));
        register_2 = new QPushButton(registerwindow);
        register_2->setObjectName(QStringLiteral("register_2"));
        register_2->setGeometry(QRect(150, 330, 121, 51));
        register_2->setStyleSheet(QStringLiteral(""));
        label_3 = new QLabel(registerwindow);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(120, 80, 161, 41));

        retranslateUi(registerwindow);

        QMetaObject::connectSlotsByName(registerwindow);
    } // setupUi

    void retranslateUi(QWidget *registerwindow)
    {
        registerwindow->setWindowTitle(QApplication::translate("registerwindow", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("registerwindow", "\347\224\250\346\210\267\345\220\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("registerwindow", "\345\257\206\347\240\201", Q_NULLPTR));
        register_2->setText(QApplication::translate("registerwindow", "\346\263\250\345\206\214", Q_NULLPTR));
        label_3->setText(QApplication::translate("registerwindow", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215\344\270\216\345\257\206\347\240\201", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class registerwindow: public Ui_registerwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERWINDOW_H
