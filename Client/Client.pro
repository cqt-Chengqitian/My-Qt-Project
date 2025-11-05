#-------------------------------------------------
#
# Project created by QtCreator 2025-07-02T19:02:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    protocol.cpp \
    index.cpp \
    friend.cpp \
    file.cpp \
    reshandler.cpp \
    chat.cpp \
    uploader.cpp

HEADERS  += client.h \
    protocol.h \
    index.h \
    friend.h \
    file.h \
    reshandler.h \
    chat.h \
    uploader.h

FORMS    += client.ui \
    index.ui \
    friend.ui \
    file.ui \
    chat.ui

RESOURCES += \
    config.qrc \
    icon.qrc
