/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef GREENHOUSE_GLOBAL_H
#define GREENHOUSE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GREENHOUSE_LIBRARY)
#define GREENHOUSESHARED_EXPORT Q_DECL_EXPORT
#else
#define GREENHOUSESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GREENHOUSE_GLOBAL_H
