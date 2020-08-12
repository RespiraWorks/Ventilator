#pragma once

#include <QFile>
#include <QMessageLogContext>
#include <QSharedPointer>
#include <QString>
#include <QTextStream>

class FileLogger {
public:
  static FileLogger &singleton() {
    static FileLogger singleton_instance;
    return singleton_instance;
  }

  void set_path(QString path);
  QTextStream &stream();

private:
  QSharedPointer<QFile> out_file_;
  QSharedPointer<QTextStream> stream_;

  // singleton assurance
  FileLogger() {}
  FileLogger(FileLogger const &);
  void operator=(FileLogger const &);
};

void logOutputToFile(QtMsgType type, const QMessageLogContext &context,
                     const QString &msg);
