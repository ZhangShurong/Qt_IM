#-------------------------------------------------
#
# Project created by QtCreator 2015-02-07T19:59:32
#
#-------------------------------------------------

QT       += core gui
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
    IM/connection.cpp \
    IM/conversation.cpp \
    IM/friend.cpp \
    IM/im_socket.cpp \
    IM/imclient.cpp \
    IM/imserver_local.cpp \
    IM/user.cpp \
    protocol/jspp.cpp \
    src/ui/chat.cpp \
    src/ui/slidewindow.cpp \
    utils/jsoncpp/json_reader.cpp \
    utils/jsoncpp/json_value.cpp \
    utils/jsoncpp/json_valueiterator.inl \
    utils/jsoncpp/json_writer.cpp \
    utils/utils.cpp \
    test.cpp

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
    IM/connection.h \
    IM/conversation.h \
    IM/friend.h \
    IM/im_socket.h \
    IM/imclient.h \
    IM/imserver_local.h \
    IM/user.h \
    protocol/jspp.h \
    src/ui/chat.h \
    src/ui/slidewindow.h \
    utils/jsoncpp/allocator.h \
    utils/jsoncpp/assertions.h \
    utils/jsoncpp/autolink.h \
    utils/jsoncpp/config.h \
    utils/jsoncpp/features.h \
    utils/jsoncpp/forwards.h \
    utils/jsoncpp/json.h \
    utils/jsoncpp/json_tool.h \
    utils/jsoncpp/reader.h \
    utils/jsoncpp/value.h \
    utils/jsoncpp/version.h \
    utils/jsoncpp/version.h.in \
    utils/jsoncpp/writer.h \
    utils/thread_pool.h \
    utils/utils.h \
    test.h

FORMS    += \
    src/ui/loginform.ui \
    src/ui/collapseview.ui \
    src/ui/collapseviewitem.ui \
    src/ui/litteriem.ui \
    src/ui/mainform.ui \
    src/ui/colorpickform.ui \
    src/ui/chat.ui \
    src/ui/slidewindow.ui

RESOURCES += \
    rs/rs.qrc

DISTFILES += \
    LICENSE
