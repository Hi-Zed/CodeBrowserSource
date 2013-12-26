#-------------------------------------------------
#
# Project created by QtCreator 2011-05-15T20:46:22
#
#-------------------------------------------------

QT       += core gui

TARGET = CodeBrowser
TEMPLATE = app


SOURCES += main.cpp\
        codebrowser.cpp \
    data.cpp \
    bbmodel.cpp \
    srcmodel.cpp \
    datamanager.cpp \
    specificiconprovider.cpp

HEADERS  += codebrowser.h \
    data.h \
    bbmodel.h \
    srcmodel.h \
    datamanager.h \
    specificiconprovider.h

FORMS    += codebrowser.ui

RESOURCES += \
    resource.qrc
