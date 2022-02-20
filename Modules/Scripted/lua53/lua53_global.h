#ifndef LUA53_GLOBAL_H
#define LUA53_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LUA53_LIBRARY)
#  define LUA53SHARED_EXPORT Q_DECL_EXPORT
#else
#  define LUA53SHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LUA53_GLOBAL_H
