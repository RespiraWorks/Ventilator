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

TEST_F(AlarmTest, AddAndRemoveOneAlarm) {
  char alarm_data[ALARM_DATALEN];
  memset(alarm_data, 'x', sizeof(alarm_data));

  alarm_add(dataID::alarm_1, alarm_data);
  ASSERT_TRUE(alarm_available());
  alarm_remove();
  ASSERT_FALSE(alarm_available());
}

TEST_F(AlarmTest, OverflowAlarmQueue) {
  for (uint8_t i = 0; i <= ALARM_NODES; i++) {
    char alarm_data[ALARM_DATALEN];
    memset(alarm_data, 'x', sizeof(alarm_data));
    alarm_add(dataID::alarm_1, alarm_data);
  }
  ASSERT_TRUE(alarm_available());
  // TODO(SarahAdelaide) when logging is implemented, validate log is generated
}

TEST_F(AlarmTest, AddAndReadAlarm) {
  while (alarm_available()) {
    alarm_remove();
  }

  char alarm_data_a[ALARM_DATALEN];
  memset(alarm_data_a, 'y', sizeof(alarm_data_a));
  dataID alarm_id_a = dataID::alarm_2;
  alarm_add(alarm_id_a, alarm_data_a);
  // checking that alarm data got copied, not referenced
  memset(alarm_data_a, 'x', sizeof(alarm_data_a));
  alarm_id_a = dataID::alarm_1;

  enum dataID alarm_id_b;
  uint32_t alarm_timestamp_b;
  char alarm_data_b[ALARM_DATALEN];
  int32_t res = alarm_read(&alarm_id_b, &alarm_timestamp_b, alarm_data_b);
  ASSERT_EQ(res, VC_STATUS_SUCCESS);
  ASSERT_EQ(alarm_data_b[0], 'y');
  ASSERT_EQ(alarm_id_b, dataID::alarm_2);
  ASSERT_TRUE(alarm_available());
}

TEST_F(AlarmTest, Timestamp) {}

// TODO(jlebar): Add more tests, checking that:
//  - Alarms get dropped after a certain point.
