#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QDEBUGGERSERVER_LIB)
#  define QDEBUGGERSERVER_EXPORT Q_DECL_EXPORT
# else
#  define QDEBUGGERSERVER_EXPORT Q_DECL_IMPORT
# endif
#else
# define QDEBUGGERSERVER_EXPORT
#endif
