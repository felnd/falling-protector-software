#-------------------------------------------------
#
# Project created by QtCreator 2016-03-24T09:39:13
#
#-------------------------------------------------

QT       += core gui
QT      +=sql
QT      += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
TARGET = FQZ
TEMPLATE = app

CONFIG += qaxcontainer

SOURCES += main.cpp\
        mainwindow.cpp \
    mainpage.cpp \
    addnew.cpp \
    test.cpp \
    qcustomplot.cpp \
    test2.cpp \
    selections.cpp \
    setparameters.cpp \
    datamanager.cpp \
    setenvironment.cpp \
    datetime.cpp \
    data_capturethread.cpp \
    spcomm.cpp \
    supercode.cpp \
    writeword.cpp


HEADERS  += mainwindow.h \
    mainpage.h \
    addnew.h \
    test.h \
    painxy.h \
    painxy2.h \
    qcustomplot.h \
    test2.h \
    selections.h \
    setparameters.h \
    datamanager.h \
    setenvironment.h \
    datetime.h \
    data_capturethread.h \
    spcomm.h \
    supercode.h \
    writeword.h

FORMS    += mainwindow.ui \
    mainpage.ui \
    addnew.ui \
    test.ui \
    test2.ui \
    selections.ui \
    setparameters.ui \
    setenvironment.ui \
    datetime.ui \
    supercode.ui

LIBS+= CH375DLL.dll
