TEMPLATE = subdirs

SUBDIRS += \
    src/core  \
    src/Tsunami \
    src/MeasureData \
    src/logger

   # src/freeda


OTHER_FILES += \
    Tsunami.pri
CONFIG += sql
