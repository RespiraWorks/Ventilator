#include "logs.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

void logOutputToFile(QtMsgType type, const QMessageLogContext &context,
                     const QString &msg) {
  if (log_path.isEmpty()) {
    // log_path assignment needs to be delayed until first usage of this
    // function, so it can find the right app folder.
    log_path =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
#ifdef QT_DEBUG
    printf("saving logs in %s\n", log_path.toLatin1().data());
#endif
    printf("log folder created? %d\n", QDir().mkpath(log_path));
  }
  QFile outFile(log_path + "/gui.log");
  outFile.open(QIODevice::WriteOnly | QIODevice::Append);

  QTextStream ts(&outFile);

  const char *file = context.file ? context.file : "";
  const char *function = context.function ? context.function : "";

  switch (type) {
  case QtDebugMsg:
    ts << "[DBUG]";
    break;
  case QtInfoMsg:
    ts << "[INFO]";
    break;
  case QtWarningMsg:
    ts << "[WARN]";
    break;
  case QtCriticalMsg:
    ts << "[CRIT]";
    break;
  case QtFatalMsg:
    ts << "[FTAL]";
    break;
  }
  ts << QString(" %1 (%2:%3, %4)")
            .arg(msg)
            .arg(file)
            .arg(context.line)
            .arg(function)
     << endl;
}
