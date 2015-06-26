#-------------------------------------------------
#
# Project created by QtCreator 2015-06-01T01:20:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test3
TEMPLATE = app
CONFIG += c++11

LIBS += -lCGAL -lgmp
QMAKE_CXXFLAGS += -frounding-math

SOURCES += main.cpp\
        mainwindow.cpp \
    zonedessin.cpp \
    site.cpp \
    voronoidiscretmodule.cpp

HEADERS  += mainwindow.h \
    Transitions.h \
    zonedessin.h \
    site.h \
    voronoidiscretmodule.h

RESOURCES += \
    ressources.qrc
