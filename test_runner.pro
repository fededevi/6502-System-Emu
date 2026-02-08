QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

SOURCES += \
    test_runner.cpp \
    cpu.cpp \
    memory.cpp \
    load.cpp \
    store.cpp \
    addcarry.cpp \
    subtract.cpp \
    transfer.cpp \
    stack.cpp \
    logical.cpp \
    branch.cpp \
    incdec.cpp \
    compare.cpp \
    shifts.cpp \
    flags.cpp \
    misc.cpp

HEADERS += \
    cpu.h \
    memory.h \
    types.h
