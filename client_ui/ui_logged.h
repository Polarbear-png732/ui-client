/********************************************************************************
** Form generated from reading UI file 'logged.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGGED_H
#define UI_LOGGED_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_logged
{
public:
    QWidget *centralwidget;
    QListWidget *listWidget;
    QLabel *headimg;
    QPushButton *addfriend;
    QPushButton *friendReq;
    QPushButton *delfriend;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *logged)
    {
        if (logged->objectName().isEmpty())
            logged->setObjectName(QStringLiteral("logged"));
        logged->resize(413, 699);
        centralwidget = new QWidget(logged);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(50, 0, 361, 701));
        headimg = new QLabel(centralwidget);
        headimg->setObjectName(QStringLiteral("headimg"));
        headimg->setGeometry(QRect(0, 0, 51, 51));
        headimg->setStyleSheet(QLatin1String("\n"
"border-image: url(:/image/poalrbear.png);"));
        addfriend = new QPushButton(centralwidget);
        addfriend->setObjectName(QStringLiteral("addfriend"));
        addfriend->setGeometry(QRect(0, 51, 51, 51));
        addfriend->setStyleSheet(QStringLiteral("border-image: url(:/image/addfriend.png);"));
        friendReq = new QPushButton(centralwidget);
        friendReq->setObjectName(QStringLiteral("friendReq"));
        friendReq->setGeometry(QRect(0, 102, 51, 51));
        friendReq->setStyleSheet(QStringLiteral("border-image: url(:/image/friendReq.png);"));
        delfriend = new QPushButton(centralwidget);
        delfriend->setObjectName(QStringLiteral("delfriend"));
        delfriend->setGeometry(QRect(0, 153, 51, 51));
        delfriend->setStyleSheet(QLatin1String("image: url(:/image/delfriend.png);\n"
""));
        logged->setCentralWidget(centralwidget);
        menubar = new QMenuBar(logged);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 413, 28));
        logged->setMenuBar(menubar);
        statusbar = new QStatusBar(logged);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        logged->setStatusBar(statusbar);

        retranslateUi(logged);

        QMetaObject::connectSlotsByName(logged);
    } // setupUi

    void retranslateUi(QMainWindow *logged)
    {
        logged->setWindowTitle(QApplication::translate("logged", "MainWindow", Q_NULLPTR));
        headimg->setText(QString());
        addfriend->setText(QString());
        friendReq->setText(QString());
        delfriend->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class logged: public Ui_logged {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGGED_H
