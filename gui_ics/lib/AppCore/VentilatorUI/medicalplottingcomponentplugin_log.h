/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#pragma once

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(mpcp)

#define mpcpDebug() qCDebug(mpcp, )
#define mpcpInfo() qCInfo(mpcp, )
#define mpcpWarning() qCWarning(mpcp, )
#define mpcpCritical() qCCritical(mpcp, )
