QT       -= core gui

VERSION = 1.0.0.3
TARGET = HtmlFile
TEMPLATE = lib

CONFIG += shared
CONFIG += plugin

CONFIG += core_static_link_libstd

DEFINES += HTMLFILE_USE_DYNAMIC_LIBRARY
DEFINES += UNICODECONVERTER_USE_DYNAMIC_LIBRARY


CORE_ROOT_DIR = $$PWD/..
PWD_ROOT_DIR = $$PWD
include(../Common/base.pri)

LIBS += -L$$CORE_BUILDS_LIBRARIES_PATH -lUnicodeConverter

include(../DesktopEditor/xml/build/qt/libxml2.pri)

core_linux {
    DEFINES += asc_static_link_libstd
}

core_windows {
    DEFINES += NOMINMAX
}

SOURCES += HtmlFile.cpp
HEADERS += HtmlFile.h
