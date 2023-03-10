#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:38:06
#
#-------------------------------------------------

QT += core gui

QT += svgwidgets

QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorldBuildr
TEMPLATE = app

#DEFINES += DEBUG_SPRITE_COUNT
#DEFINES += DEBUG_BRECT
#DEFINES += DEBUG_SHAPE
#DEFINES += DEPLOY # Pour une compilation dans un but de déploiement

SOURCES += GameFramework/main.cpp\
    GameFramework/mainfrm.cpp \
    GameFramework/gamescene.cpp \
    GameFramework/sprite.cpp \
    GameFramework/resources.cpp \
    GameFramework/gameview.cpp \
    GameFramework/utilities.cpp \
    GameFramework/gamecanvas.cpp \
    GameFramework/spritetickhandler.cpp \
    WorldBuildrEditor/EditorHistory.cpp \
    WorldBuildrEditor/EditorManager.cpp \
    WorldBuildrEditor/EditorSprite.cpp \
    WorldBuildrEditor/gamecore.cpp \
    WorldBuildrEditor/SaveFileManager.cpp \
    WorldBuildrEditor/SelectionZone.cpp \
    WorldBuildrUi/EditorActionPanel.cpp

HEADERS  += GameFramework/mainfrm.h \
    GameFramework/gamescene.h \
    GameFramework/sprite.h \
    GameFramework/resources.h \
    GameFramework/gameview.h \
    GameFramework/utilities.h \
    GameFramework/gamecanvas.h \
    GameFramework/spritetickhandler.h \
    WorldBuildrEditor/EditorHistory.h \
    WorldBuildrEditor/EditorManager.h \
    WorldBuildrEditor/EditorSprite.h \
    WorldBuildrEditor/gamecore.h \
    WorldBuildrEditor/SaveFileManager.h \
    WorldBuildrEditor/SelectionZone.h \
    WorldBuildrUi/EditorActionPanel.h

FORMS    += GameFramework/mainfrm.ui

