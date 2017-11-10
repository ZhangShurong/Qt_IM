#-------------------------------------------------
#
# Project created by QtCreator 2017-11-05T13:59:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CV_IM
TEMPLATE = app


SOURCES += main.cpp\
            interface/login_window.cpp \
    interface/click_label.cpp \
    communication/protocal.cpp \
    interface/main_window.cpp \
    interface/friends_list.cpp \
    interface/message_list.cpp \
    interface/friend_item.cpp \
    interface/message_item.cpp \
    interface/im_btn.cpp \
    interface/chat_widget.cpp

HEADERS  += interface/login_window.h \
    interface/click_label.h \
    communication/protocal.h \
    interface/main_window.h \
    interface/friends_list.h \
    interface/message_list.h \
    interface/friend_item.h \
    interface/message_item.h \
    interface/im_btn.h \
    interface/chat_widget.h

RESOURCES += \
    images.qrc
