#ifndef APP_CORE_LOG_H
#define APP_CORE_LOG_H

#include <QLoggingCategory>
#include "appcore_global.h"

APPCORE_EXPORT Q_DECLARE_LOGGING_CATEGORY(appcore)

#define acDebug() qCDebug(appcore, )
#define acInfo() qCInfo(appcore, )
#define acWarning() qCWarning(appcore, )
#define acCritical() qCCritical(appcore, )

#endif // APP_CORE_LOG_H
