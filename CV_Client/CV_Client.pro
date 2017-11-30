#-------------------------------------------------
#
# Project created by QtCreator 2015-02-07T19:59:32
#
#-------------------------------------------------

QT       += core gui network
CONFIG += console c++11

LIBS += -lws2_32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CV_IM_Client
TEMPLATE = app


SOURCES += main.cpp\
    src/ui/loginform.cpp \
    src/ui/collapseview.cpp \
    src/ui/collapseviewitem.cpp \
    src/ui/litteriem.cpp \
    src/ui/moveableframelesswindow.cpp \
    src/ui/mainform.cpp \
    src/model/singleton.cpp \
    src/ui/colorpickform.cpp \
    src/model/config.cpp \
    IM/imclient.cpp \
    IM/imserver_local.cpp \
    IM/user.cpp \
    protocol/jspp.cpp \
    src/ui/chat.cpp \
    src/ui/slidewindow.cpp \
    utils/utils.cpp \
    src/ui/dialogrec.cpp \
    src/ui/regisdialog.cpp \
    src/ui/clickablelabel.cpp \
    src/ui/forgotdia.cpp \
    utils/AES.cpp \
    utils/clsEncrypt.cpp \
    utils/TAesClass.cpp

HEADERS  += \
    src/ui/loginform.h \
    src/ui/collapseview.h \
    src/ui/collapseviewitem.h \
    src/ui/litteriem.h \
    src/ui/moveableframelesswindow.h \
    src/ui/mainform.h \
    src/model/singleton.h \
    src/ui/colorpickform.h \
    src/model/config.h \
    IM/imclient.h \
    IM/imserver_local.h \
    IM/user.h \
    protocol/jspp.h \
    src/ui/chat.h \
    src/ui/slidewindow.h \
    utils/utils.h \
    config.h \
    src/ui/dialogrec.h \
    src/ui/regisdialog.h \
    src/ui/clickablelabel.h \
    src/ui/forgotdia.h \
    utils/AES.h \
    utils/clsEncrypt.h \
    utils/TAesClass.h \
    utils/thread_pool.h

FORMS    += \
    src/ui/loginform.ui \
    src/ui/collapseview.ui \
    src/ui/collapseviewitem.ui \
    src/ui/litteriem.ui \
    src/ui/mainform.ui \
    src/ui/colorpickform.ui \
    src/ui/chat.ui \
    src/ui/slidewindow.ui \
    src/ui/dialogrec.ui \
    src/ui/regisdialog.ui \
    src/ui/forgotdia.ui

RESOURCES += \
    rs/rs.qrc

DISTFILES += \
    LICENSE
