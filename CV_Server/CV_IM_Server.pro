QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app
CONFIG += console c++11

LIBS += -lws2_32
SOURCES += main.cpp \
    IM/user.cpp \
#    utils/jsoncpp/json_reader.cpp \
#    utils/jsoncpp/json_value.cpp \
#    utils/jsoncpp/json_valueiterator.inl \
#    utils/jsoncpp/json_writer.cpp \
#    test.cpp \
    IM/connection.cpp \
    #IM/im_socket.cpp \
    IM/imserver_local.cpp \
    #IM/friend.cpp \
    IM/imclient.cpp \
    IM/conversation.cpp \
    protocol/jspp.cpp \
    utils/utils.cpp


HEADERS += \
    IM/user.h \
#    utils/jsoncpp/allocator.h \
#    utils/jsoncpp/assertions.h \
#    utils/jsoncpp/autolink.h \
#    utils/jsoncpp/config.h \
#    utils/jsoncpp/features.h \
#    utils/jsoncpp/forwards.h \
#    utils/jsoncpp/json.h \
#    utils/jsoncpp/json_tool.h \
#    utils/jsoncpp/reader.h \
#    utils/jsoncpp/value.h \
#    utils/jsoncpp/version.h \
#    utils/jsoncpp/version.h.in \
#    utils/jsoncpp/writer.h \
    utils/thread_pool.h \
    test.h \
    IM/connection.h \
    #IM/im_socket.h \
    IM/imserver_local.h \
    #IM/friend.h \
    IM/imclient.h \
    IM/conversation.h \
    protocol/jspp.h \
    utils/utils.h

