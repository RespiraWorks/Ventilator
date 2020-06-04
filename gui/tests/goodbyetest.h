#ifndef GOODBYETEST_H
#define GOODBYETEST_H

#include <QCoreApplication>
#include <QtTest>

class GoodbyeTest : public QObject {
  Q_OBJECT
public:
  GoodbyeTest();
  ~GoodbyeTest();

private slots:
  void initTestCase();
  void cleanupTestCase();
  void test_case1();
};

#endif // GOODBYETEST_H
