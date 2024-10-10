TEMPLATE = app
TARGET = qtdemo

HEADERS += src/*.h
SOURCES += src/*.cpp

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets