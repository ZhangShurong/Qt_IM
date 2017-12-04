#-------------------------------------------------
#
# Project created by QtCreator 2011-09-21T09:49:59
#
#-------------------------------------------------

QT       += core gui network sql
CONFIG += console c++11

LIBS += -lws2_32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Servers_1
TEMPLATE = app


SOURCES += main.cpp \
    cvserver.cpp \
    user.cpp \
    utils.cpp \
    jspp.cpp \
    serverdb.cpp

HEADERS += \
    cvserver.h \
    user.h \
    utils.h \
    jspp.h \
    serverdb.h

RESOURCES += \
    rs.qrc

FORMS += \
    cvserver.ui
