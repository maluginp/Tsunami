#ifndef SPICE_GLOBAL_H
#define SPICE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPICE_LIBRARY)
#  define SPICESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPICESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPICE_GLOBAL_H
