SOLUTION_DIR=$$PWD

OBJECTS_DIR = $$SOLUTION_DIR/tmp/$$TARGET
MOC_DIR = $$SOLUTION_DIR/tmp/$$TARGET

QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder

INCLUDEPATH += $$SOLUTION_DIR/include \
               $$SOLUTION_DIR/src/core

QT += core sql gui
