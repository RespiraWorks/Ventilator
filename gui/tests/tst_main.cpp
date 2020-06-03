#include <QCoreApplication>
#include <QtTest>

#include "goodbyetest.h"
#include "hellotest.h"

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  int status = 0;
  {
    HelloTest tc;
    status = QTest::qExec(&tc, argc, argv);
  }

  {
    GoodbyeTest tc;
    status |= QTest::qExec(&tc, argc, argv);
  }
  return status;
}
