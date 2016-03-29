#-------------------------------------------------
#
# Project created by QtCreator 2016-03-24T19:50:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OrbalDilemma
TEMPLATE = app


SOURCES += main.cpp \
    orb.cpp \
    playerorb.cpp \
    aiorb.cpp \
    game.cpp

HEADERS  += \
    orb.h \
    playerorb.h \
    aiorb.h \
    game.h

RESOURCES += \
    res.qrc
