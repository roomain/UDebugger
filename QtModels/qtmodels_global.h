#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTMODELS_LIB)
#  define QTMODELS_EXPORT Q_DECL_EXPORT
# else
#  define QTMODELS_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTMODELS_EXPORT
#endif
