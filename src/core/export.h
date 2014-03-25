#ifndef EXPORT_H
#define EXPORT_H
#if defined(TSUNAMI_MAKEDLL)
# define TSUNAMI_EXPORT Q_DECL_EXPORT
#else
# define TSUNAMI_EXPORT Q_DECL_IMPORT
#endif
#endif // EXPORT_H
