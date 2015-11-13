QT += core network
QT -= gui

TARGET = data-manager
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    util.cpp \
    manager.cpp \
    download-manager.cpp

HEADERS += \
    util.h \
    manager.h \
    download-manager.h

DISTFILES += \
    data-manager.conf \
    init.sh \
    README.md

