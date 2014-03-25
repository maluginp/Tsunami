TEMPLATE = subdirs

SUBDIRS += \
    src/logger \
    src/core  \
    src/Tsunami \
    test \
    src/experiments

OTHER_FILES += \
    Tsunami.pri

#CONFIG += sql
