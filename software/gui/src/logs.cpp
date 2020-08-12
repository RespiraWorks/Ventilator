#include "logs.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

void FileLogger::set_path(QString path) {
  out_file_ = QSharedPointer<QFile>(new QFile(path));
  out_file_->open(QIODevice::WriteOnly | QIODevice::Append);
  stream_ = QSharedPointer<QTextStream>(new QTextStream(out_file_.data()));
}

QTextStream &FileLogger::stream() { return *stream_; }

void logOutputToFile(QtMsgType type, const QMessageLogContext &context,
                     const QString &msg) {

  const char *file = context.file ? context.file : "";
  const char *function = context.function ? context.function : "";

  auto &ts = FileLogger::singleton().stream();

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
