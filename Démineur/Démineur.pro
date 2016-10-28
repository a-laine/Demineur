#-------------------------------------------------
#
# Project created by QtCreator 2013-06-24T17:28:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Demineur
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    grille/grille.cpp \
    popups/fenoptions.cpp \
    grille/options.cpp \
    grille/case.cpp \
    compteurs.cpp \
    popups/popupfin.cpp \
    popups/fenstats.cpp \
    popups/fenstyle.cpp

HEADERS  += \
    grille/case.hpp \
    grille/grille.hpp \
    grille/options.hpp \
    mainwindow.hpp \
    popups/fenoptions.hpp \
    compteurs.hpp \
    popups/popupfin.hpp \
    popups/fenstats.hpp \
    popups/fenstyle.hpp
