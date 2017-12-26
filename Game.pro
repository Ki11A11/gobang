TEMPLATE = app
TARGET = Game

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets \
                                        network

SOURCES += \
    main.cpp \
    game.cpp \
    ai.cpp \
    remote.cpp

HEADERS += \
    game.h \
    ai.h \
    remote.h
