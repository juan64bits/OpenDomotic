# -------------------------------------------------
# Project created by QtCreator 2012-02-06T17:05:39
# -------------------------------------------------
QT += xml \
    svg
TARGET = avrDomotic
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    signaldisplay.cpp \
    serialport.cpp \
    counter.cpp \
    callapp.cpp \
    simplecrypt.cpp
HEADERS += mainwindow.h \
    signaldisplay.h \
    domo.h \
    serialport.h \
    counter.h \
    callapp.h \
    simplecrypt.h \
    simplecrypt.h
FORMS += mainwindow.ui \
    passworddialog.ui
RESOURCES += icons.qrc
MOC_DIR = objects
UI_DIR = .
OBJECTS_DIR = objects
