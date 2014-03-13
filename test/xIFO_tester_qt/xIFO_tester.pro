#-------------------------------------------------
#
# Project created by QtCreator 2014-03-13T19:35:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = xIFO_tester
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ../../xIFO_8/xIFO_8.c \
    ../../xIFO_16/xIFO_16.c \
    ../../xIFO_32/xIFO_32.c \
    ../../xIFO_64/xIFO_64.c \
    ../../xIFO_type/xIFO_SizeType.c \
    main.cpp

HEADERS += \
    ../../xIFO_8/xIFO_8.h \
    ../../xIFO_16/xIFO_16.h \
    ../../xIFO_32/xIFO_32.h \
    ../../xIFO_64/xIFO_64.h \
    ../../xIFO_type/xIFO_SizeType.h
