/********************************************************************************
** Form generated from reading UI file 'frienditem.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDITEM_H
#define UI_FRIENDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_friendItem
{
public:
    QLabel *headimg;
    QLabel *nameLabel;
    QLabel *onoffLabel;
    QLabel *msgLabel;

    void setupUi(QWidget *friendItem)
    {
        if (friendItem->objectName().isEmpty())
            friendItem->setObjectName(QStringLiteral("friendItem"));
        friendItem->resize(361, 80);
        friendItem->setStyleSheet(QStringLiteral("selection-background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));"));
        headimg = new QLabel(friendItem);
        headimg->setObjectName(QStringLiteral("headimg"));
        headimg->setGeometry(QRect(20, 10, 65, 65));
        headimg->setStyleSheet(QLatin1String("QLabel {\n"
"	border-image: url(:/image/poalrbear.png);\n"
"background-color: rgb(138, 241, 244);\n"
" border-radius: 30px;\n"
" }"));
        nameLabel = new QLabel(friendItem);
        nameLabel->setObjectName(QStringLiteral("nameLabel"));
        nameLabel->setGeometry(QRect(110, 10, 71, 21));
        QFont font;
        font.setFamily(QStringLiteral("Ubuntu Light"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        nameLabel->setFont(font);
        nameLabel->setStyleSheet(QStringLiteral(""));
        onoffLabel = new QLabel(friendItem);
        onoffLabel->setObjectName(QStringLiteral("onoffLabel"));
        onoffLabel->setGeometry(QRect(246, 10, 51, 20));
        onoffLabel->setStyleSheet(QStringLiteral(""));
        msgLabel = new QLabel(friendItem);
        msgLabel->setObjectName(QStringLiteral("msgLabel"));
        msgLabel->setGeometry(QRect(120, 40, 171, 21));

        retranslateUi(friendItem);

        QMetaObject::connectSlotsByName(friendItem);
    } // setupUi

    void retranslateUi(QWidget *friendItem)
    {
        friendItem->setWindowTitle(QApplication::translate("friendItem", "Form", Q_NULLPTR));
        headimg->setText(QString());
        nameLabel->setText(QString());
        onoffLabel->setText(QString());
        msgLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class friendItem: public Ui_friendItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDITEM_H
