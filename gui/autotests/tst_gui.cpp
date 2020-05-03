#include <QCoreApplication>
#include <QtTest>

// add necessary includes here

class gui : public QObject {
  Q_OBJECT

public:
  gui();
  ~gui();

private slots:
  void initTestCase();
  void cleanupTestCase();
  void test_case1();
};

gui::gui() {}

gui::~gui() {}

void gui::initTestCase() {}

void gui::cleanupTestCase() {}

void gui::test_case1() {}

QTEST_MAIN(gui)

#include "tst_gui.moc"
