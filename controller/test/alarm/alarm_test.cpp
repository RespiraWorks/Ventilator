#include "alarm.h"
#include "hal.h"
#include "gtest/gtest.h"

class AlarmTest : public testing::Test {
public:
  void SetUp() override {
    alarm_init();
    // TODO: Reset Hal mock object.
  }
};

TEST_F(AlarmTest, NotAvailableAfterInit) { ASSERT_FALSE(alarm_available()); }

TEST_F(AlarmTest, AddOneAlarm) {
  char alarm_data[ALARM_DATALEN];
  memset(alarm_data, 'x', sizeof(alarm_data));

  alarm_add(dataID::alarm_1, alarm_data);
  ASSERT_TRUE(alarm_available());
}

TEST_F(AlarmTest, Timestamp) {}

// TODO(jlebar): Add more tests, checking that:
//  - Alarms get dropped after a certain point.
//  - Alarms can be popped.
//  - Alarm data gets copied into storage (not just referring to the pointer
//    passed to alarm_add).
