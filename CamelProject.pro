QT       += core gui widgets gamepad

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

HEADERS += \
    SceneStates.h \
    backgrounditem.h \
    cactus.h \
    floor.h \
    gamemanager.h \
    gamescene.h \
    highscorescene.h \
    losingscene.h \
    menuscene.h \
    platform.h \
    player.h \
    settingsscene.h

SOURCES += \
    backgrounditem.cpp \
    cactus.cpp \
    floor.cpp \
    gamemanager.cpp \
    gamescene.cpp \
    highscorescene.cpp \
    losingscene.cpp \
    main.cpp \
    menuscene.cpp \
    platform.cpp \
    player.cpp \
    settingsscene.cpp

RESOURCES += \
    resource.qrc
