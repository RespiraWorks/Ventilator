#ifndef HELLOTEST_H
#define HELLOTEST_H

#include <QObject>

class HelloTest : public QObject {
  Q_OBJECT
public:
  explicit HelloTest(QObject *parent = nullptr);

private slots:
  void initTestCase();
  void cleanupTestCase();
  void test_case1();
};

#endif // HELLOTEST_H
