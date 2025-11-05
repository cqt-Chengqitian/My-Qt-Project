#-------------------------------------------------
#
# Project created by QtCreator 2025-07-02T23:27:19
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp \
    mytcpserver.cpp \
    mytcpsocket.cpp \
    protocol.cpp \
    operatedb.cpp \
    reqhandler.cpp \
    clienttask.cpp

HEADERS  += server.h \
    mytcpserver.h \
    mytcpsocket.h \
    protocol.h \
    operatedb.h \
    reqhandler.h \
    clienttask.h

FORMS    += server.ui

DISTFILES +=

RESOURCES += \
    server.qrc
