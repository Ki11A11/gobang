TEMPLATE = app
TARGET = Game

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    game.cpp

HEADERS += \
    game.h
