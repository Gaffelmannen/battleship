TEMPLATE = app
TARGET = battleship

HEADERS += src/*.h
SOURCES += src/*.cpp

QT = core gui

macx: ICON = res/icon.icns

QMAKE_CXXFLAGS += -Werror

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets