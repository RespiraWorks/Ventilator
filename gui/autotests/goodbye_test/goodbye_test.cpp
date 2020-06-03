#include <QCoreApplication>
#include <QtTest>

class GoodbyeTest : public QObject {
  Q_OBJECT

private slots:
  void test_goodbye() { QVERIFY(1 == 1); }
};

QTEST_MAIN(GoodbyeTest)

#include "goodbye_test.moc"
