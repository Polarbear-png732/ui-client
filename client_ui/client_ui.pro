#-------------------------------------------------
#
# Project created by QtCreator 2024-12-27T09:56:00
#
#-------------------------------------------------

QT       += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client_ui
TEMPLATE = app
CONFIG += c++11
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    client.c \
    registerwindow.cpp \
    responsethread.cpp \
    logged.cpp \
    frienditem.cpp \
    sendthread.cpp \
    addfrienddialog.cpp \
    handleaddfrienddialog.cpp \
    friendreqlist.cpp \
    friendchat.cpp


HEADERS += \
        mainwindow.h \
    client.h \
    registerwindow.h \
    responsethread.h \
    logged.h \
    frienditem.h \
    sendthread.h \
    messagedispatcher.h \
    addfrienddialog.h \
    handleaddfrienddialog.h \
    friendreqlist.h \
    friendchat.h


FORMS += \
        mainwindow.ui \
    registerwindow.ui \
    logged.ui \
    frienditem.ui \
    friendchat.ui

RESOURCES += \
    resources.qrc \


DISTFILES += \
    style.qss \
    img/poalrbear.png
