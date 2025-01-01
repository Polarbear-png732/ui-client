/********************************************************************************
** Form generated from reading UI file 'friendchat.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDCHAT_H
#define UI_FRIENDCHAT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendChat
{
public:
    QWidget *centralwidget;
    QPushButton *sendButton;
    QTextEdit *message;
    QLabel *file;
    QLabel *delfriend;
    QTextEdit *inputmsg;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *FriendChat)
    {
        if (FriendChat->objectName().isEmpty())
            FriendChat->setObjectName(QStringLiteral("FriendChat"));
        FriendChat->resize(800, 600);
        FriendChat->setStyleSheet(QStringLiteral(""));
        centralwidget = new QWidget(FriendChat);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setGeometry(QRect(680, 530, 101, 41));
        message = new QTextEdit(centralwidget);
        message->setObjectName(QStringLiteral("message"));
        message->setGeometry(QRect(0, 0, 801, 431));
        QFont font;
        font.setPointSize(18);
        message->setFont(font);
        message->setStyleSheet(QStringLiteral(""));
        file = new QLabel(centralwidget);
        file->setObjectName(QStringLiteral("file"));
        file->setGeometry(QRect(10, 430, 31, 31));
        file->setStyleSheet(QStringLiteral("image: url(:/image/fiel.png);"));
        delfriend = new QLabel(centralwidget);
        delfriend->setObjectName(QStringLiteral("delfriend"));
        delfriend->setGeometry(QRect(60, 430, 31, 31));
        delfriend->setStyleSheet(QStringLiteral("image: url(:/image/delfriend.png);"));
        inputmsg = new QTextEdit(centralwidget);
        inputmsg->setObjectName(QStringLiteral("inputmsg"));
        inputmsg->setGeometry(QRect(0, 470, 801, 111));
        inputmsg->setFont(font);
        inputmsg->setStyleSheet(QStringLiteral(""));
        FriendChat->setCentralWidget(centralwidget);
        inputmsg->raise();
        message->raise();
        file->raise();
        delfriend->raise();
        sendButton->raise();
        statusbar = new QStatusBar(FriendChat);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        FriendChat->setStatusBar(statusbar);

        retranslateUi(FriendChat);

        QMetaObject::connectSlotsByName(FriendChat);
    } // setupUi

    void retranslateUi(QMainWindow *FriendChat)
    {
        FriendChat->setWindowTitle(QApplication::translate("FriendChat", "MainWindow", Q_NULLPTR));
        sendButton->setText(QApplication::translate("FriendChat", "\345\217\221\351\200\201", Q_NULLPTR));
        file->setText(QString());
        delfriend->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FriendChat: public Ui_FriendChat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDCHAT_H
