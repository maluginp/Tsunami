SOLUTION_DIR=$$PWD

OBJECTS_DIR = $$SOLUTION_DIR/tmp/$$TARGET
MOC_DIR = $$SOLUTION_DIR/tmp/$$TARGET

QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder

INCLUDEPATH += $$SOLUTION_DIR/include \
               $$SOLUTION_DIR/src/core

macx:QMAKE_CC = gcc
macx:QMAKE_CXX = g++
macx:QMAKE_LINK = g++

DEFINES += DEBUG
QT += core sql gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport
