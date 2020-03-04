QT       += core gui widgets multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

HEADERS += \
    Header/backgrounditem.h \
    Header/cactus.h \
    Header/coin.h \
    Header/floor.h \
    Header/gamemanager.h \
    Header/gamescene.h \
    Header/highscorescene.h \
    Header/losingscene.h \
    Header/menuscene.h \
    Header/platform.h \
    Header/player.h \
    Header/scorpio.h \
    Header/settingsscene.h \
    Header/tree.h \
    Header/winningscene.h

SOURCES += \
    Source/backgrounditem.cpp \
    Source/cactus.cpp \
    Source/coin.cpp \
    Source/floor.cpp \
    Source/gamemanager.cpp \
    Source/gamescene.cpp \
    Source/highscorescene.cpp \
    Source/losingscene.cpp \
    Source/main.cpp \
    Source/menuscene.cpp \
    Source/platform.cpp \
    Source/player.cpp \
    Source/scorpio.cpp \
    Source/settingsscene.cpp \
    Source/tree.cpp \
    Source/winningscene.cpp

RESOURCES += \
    Resource/resource.qrc
