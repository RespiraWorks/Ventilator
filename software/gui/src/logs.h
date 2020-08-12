#pragma once

#include <QString>
#include <QMessageLogContext>

static QString log_path {""};

void logOutputToFile(QtMsgType type, const QMessageLogContext &context,
                     const QString &msg);
