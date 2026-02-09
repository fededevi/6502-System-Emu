QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# Only compile cpu.cpp which includes all instruction implementations
SOURCES += \
    test_runner.cpp \
    cpu.cpp \
    memory.cpp

HEADERS += \
    cpu.h \
    memory.h \
    types.h
