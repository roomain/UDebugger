#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DEBUGPROTOCOL_LIB)
#  define DEBUGPROTOCOL_EXPORT Q_DECL_EXPORT
# else
#  define DEBUGPROTOCOL_EXPORT Q_DECL_IMPORT
# endif
#else
# define DEBUGPROTOCOL_EXPORT
#endif
