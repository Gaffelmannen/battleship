TEMPLATE = app
TARGET = battleship
VERSION = 1.0.0.0
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
COMMENT = "Beta"
CONFIG += sdk_no_version_check


HEADERS += src/*.h
SOURCES += src/*.cpp

QT = core gui

macx: ICON = res/icon.icns

QMAKE_CXXFLAGS += -Werror

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
