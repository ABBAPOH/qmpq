#ifndef QMPQFILEENGINE_GLOBAL_H
#define QMPQFILEENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QMPQFILEENGINE_LIBRARY)
#  define QMPQFILEENGINESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QMPQFILEENGINESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QMPQFILEENGINE_GLOBAL_H
