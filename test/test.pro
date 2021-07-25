QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    ../cpu.cpp \
    ../memory.cpp \
    main.cpp

HEADERS += \
    ../cpu.h \
    ../memory.h \
    addcarrytest.h \
    cputest.h \
    loadtest.h \
    storetest.h
