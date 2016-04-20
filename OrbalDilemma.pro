#-------------------------------------------------
#
# Project created by QtCreator 2016-03-24T19:50:09
#
#-------------------------------------------------

QT       += core gui \
         multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OrbalDilemma
TEMPLATE = app


SOURCES += main.cpp \
    orb.cpp \
    playerorb.cpp \
    aiorb.cpp \
    game.cpp \
    feederOrb.cpp \
    startscreen.cpp \
    button.cpp

HEADERS  += \
    orb.h \
    playerorb.h \
    aiorb.h \
    game.h \
    feederOrb.h \
    startscreen.h \
    button.h

RESOURCES += \
    res.qrc

DISTFILES +=
