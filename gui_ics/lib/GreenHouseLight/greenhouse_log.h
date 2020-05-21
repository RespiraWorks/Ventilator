/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef GREENHOUSE_LOG_H
#define GREENHOUSE_LOG_H

#include "greenhouse_global.h"
#include <QLoggingCategory>

GREENHOUSESHARED_EXPORT Q_DECLARE_LOGGING_CATEGORY(gh)
#define ghDebug() qCDebug(gh, )
#define ghInfo() qCInfo(gh, )
#define ghWarning() qCWarning(gh, )
#define ghCritical() qCCritical(gh, )

        GREENHOUSESHARED_EXPORT Q_DECLARE_LOGGING_CATEGORY(theming)
#define themingDebug() qCDebug(theming, )
#define themingInfo() qCInfo(theming, )
#define themingWarning() qCWarning(theming, )
#define themingCritical() qCCritical(theming, )

#endif // GREENHOUSE_LOG_H
