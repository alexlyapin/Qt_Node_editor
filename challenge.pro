QT       += core xml gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = challenge
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    mainwindow.h \
    state.h \
    positionable.h \
    transition.h \
    joint.h \
    machine.h \
    statediagram.h \
    machinescene.h \
    machineview.h \
    machineeditor.h \
    stateeditor.h \
    jointdiagram.h \
    weightdiagram.h \
    transitiondiagram.h \
    weighteditor.h \
    connectortype.h \
    connectordiagram.h \
    connectorpathdiagram.h \
    bezierpath.h

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    state.cpp \
    positionable.cpp \
    transition.cpp \
    joint.cpp \
    machine.cpp \
    statediagram.cpp \
    machinescene.cpp \
    machineview.cpp \
    machineeditor.cpp \
    stateeditor.cpp \
    jointdiagram.cpp \
    weightdiagram.cpp \
    transitiondiagram.cpp \
    weighteditor.cpp \
    connectortype.cpp \
    connectordiagram.cpp \
    connectorpathdiagram.cpp \
    bezierpath.cpp


QMAKE_CXXFLAGS += -Wall
