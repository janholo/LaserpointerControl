#-------------------------------------------------
#
# Project created by QtCreator 2015-03-30T03:05:19
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaserpointerControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglsimulation.cpp \
    geometryengine.cpp \
    laserpointer.cpp \
    observedsubject.cpp \
    observer.cpp \
    uartinterface.cpp

HEADERS  += mainwindow.h \
    openglsimulation.h \
    geometryengine.h \
    laserpointer.h \
    observedsubject.h \
    observer.h \
    uartinterface.h

FORMS    += mainwindow.ui


RC_FILE = LaserpointerControl.rc

RESOURCES += \
    resourcelist.qrc
