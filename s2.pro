#-------------------------------------------------
#
# Project created by QtCreator 2015-05-05T21:35:34
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = s2
CONFIG   += console
CONFIG   -= app_bundle

LIBS += -L"/home/hkitty/Libs/SFML-master-build/lib"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system
CONFIG(debug, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

INCLUDEPATH += "/home/hkitty/Libs/SFML-master/include"
DEPENDPATH += "/home/hkitty/Libs/SFML-master/include"


TEMPLATE = app


SOURCES += main.cpp \
    settings.cpp \
    parser.cpp \
    banlist.cpp \
    queue.cpp \
    messagecontroller.cpp \
    accounts.cpp \
    player.cpp \
    user.cpp \
    adminInterface.cpp \
    menuLogic.cpp \
    gameLogic.cpp

CONFIG += c++11

HEADERS += \
    settings.h \
    parser.h \
    banlist.h \
    queue.h \
    task.h \
    messagecontroller.h \
    accounts.h \
    player.h \
    user.h \
    adminInterface.h \
    menuLogic.h \
    gameLogic.h
