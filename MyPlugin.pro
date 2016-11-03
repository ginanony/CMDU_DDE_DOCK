#-------------------------------------------------
#
# Project created by QtCreator 2016-09-27T20:57:24
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = MyPlugin
TEMPLATE = lib
CONFIG += plugin c++11

SOURCES += myplugin.cpp

HEADERS += myplugin.h
DISTFILES += MyPlugin.json

INCLUDEPATH += ../dde-dock/

unix {
    target.path = /usr/lib/dde-dock/plugins/
    INSTALLS += target
}
