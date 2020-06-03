#include <QCoreApplication>
#include <QtTest>

class HelloTest : public QObject {
  Q_OBJECT

private slots:
  void test_hello() { QVERIFY(1 == 1); }
};

QTEST_MAIN(HelloTest)

#include "hello_test.moc"
