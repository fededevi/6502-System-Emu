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
    branchtest.h \
    comparetest.h \
    cputest.h \
    flagstest.h \
    incdectest.h \
    loadtest.h \
    logicaltest.h \
    misctest.h \
    shiftstest.h \
    stacktest.h \
    storetest.h \
    subtracttest.h \
    transfertest.h
