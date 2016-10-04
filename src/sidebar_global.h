#pragma once

#include <QtGlobal>

#if defined(SIDEBAR_LIBRARY)
#  define SIDEBARSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SIDEBARSHARED_EXPORT Q_DECL_IMPORT
#endif
