#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:38:06
#
#-------------------------------------------------

QT       += core gui

QT += svgwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WorldBuildr
TEMPLATE = app

#DEFINES += DEBUG_SPRITE_COUNT
#DEFINES += DEBUG_BRECT
#DEFINES += DEBUG_SHAPE
#DEFINES += DEPLOY # Pour une compilation dans un but de déploiement

SOURCES += main.cpp\
    mainfrm.cpp \
    gamescene.cpp \
    sprite.cpp \
    gamecore.cpp \
    resources.cpp \
    gameview.cpp \
    utilities.cpp \
    gamecanvas.cpp \
    spritetickhandler.cpp

HEADERS  += mainfrm.h \
    gamescene.h \
    sprite.h \
    gamecore.h \
    resources.h \
    gameview.h \
    utilities.h \
    gamecanvas.h \
    spritetickhandler.h

FORMS    += mainfrm.ui

