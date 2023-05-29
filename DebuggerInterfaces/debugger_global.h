#pragma once


#ifndef BUILD_STATIC
# if defined(DEBUGGER_INTERFACE_LIB)
#  define DEBUGGER_INTERFACE_EXPORT __declspec(dllexport)
# else
#  define DEBUGGER_INTERFACE_EXPORT __declspec(dllimport)
# endif
#else
# define DEBUGGER_INTERFACE_EXPORT
#endif
