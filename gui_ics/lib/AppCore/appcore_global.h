#ifndef APPCORE_GLOBAL_H
#define APPCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(APPCORE_LIBRARY)
#define APPCORE_EXPORT Q_DECL_EXPORT
#else
#define APPCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // APPCORE_GLOBAL_H
