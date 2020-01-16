QT       += core gui widgets multimedia concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

HEADERS += \
    backgrounditem.h \
    cactus.h \
    coin.h \
    floor.h \
    gamemanager.h \
    gamescene.h \
    highscorescene.h \
    losingscene.h \
    menuscene.h \
    platform.h \
    player.h \
    scorpio.h \
    settingsscene.h \
    tree.h \
    winningscene.h

SOURCES += \
    backgrounditem.cpp \
    cactus.cpp \
    coin.cpp \
    floor.cpp \
    gamemanager.cpp \
    gamescene.cpp \
    highscorescene.cpp \
    losingscene.cpp \
    main.cpp \
    menuscene.cpp \
    platform.cpp \
    player.cpp \
    scorpio.cpp \
    settingsscene.cpp \
    tree.cpp \
    winningscene.cpp

RESOURCES += \
    resource.qrc
