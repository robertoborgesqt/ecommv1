
QT += core sql network

TEMPLATE = app
TARGET = datasrv
DESTDIR = ./debug
CONFIG += debug console
DEFINES += _CONSOLE _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE
LIBS += -L"."
DEPENDPATH += .
MOC_DIR += .
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(datasrv.pri)
