#include "parameters.h"
#include "gtest/gtest.h"

TEST(Parameters, SetAndGetRR) {
  parameters_setRR(RR_MAX + 1);
  ASSERT_FLOAT_EQ(RR_MAX, parameters_getRR());

  parameters_setRR(RR_MIN - 1);
  ASSERT_FLOAT_EQ(RR_MIN, parameters_getRR());

  float mid_range_rr = RR_MIN + (RR_MAX - RR_MIN) / 2;
  parameters_setRR(mid_range_rr);
  ASSERT_FLOAT_EQ(mid_range_rr, parameters_getRR());
}

TEST(Parameters, SetAndGetTV) {
  parameters_setTV(TV_MAX + 1);
  ASSERT_FLOAT_EQ(TV_MAX, parameters_getTV());

  parameters_setTV(TV_MIN - 1);
  ASSERT_FLOAT_EQ(TV_MIN, parameters_getTV());

  float mid_range_tv = TV_MIN + (TV_MAX - TV_MIN) / 2;
  parameters_setTV(mid_range_tv);
  ASSERT_FLOAT_EQ(mid_range_tv, parameters_getTV());
}

TEST(Parameters, SetAndGetPEEP) {
  parameters_setPEEP(PEEP_MAX + 1);
  ASSERT_FLOAT_EQ(PEEP_MAX, parameters_getPEEP());

  parameters_setPEEP(PEEP_MIN - 1);
  ASSERT_FLOAT_EQ(PEEP_MIN, parameters_getPEEP());

  float mid_range_peep = PEEP_MIN + (PEEP_MAX - PEEP_MIN) / 2;
  parameters_setPEEP(mid_range_peep);
  ASSERT_FLOAT_EQ(mid_range_peep, parameters_getPEEP());
}

TEST(Parameters, SetAndGetIER) {
  parameters_setInspireExpireRatio(IER_MAX + .1);
  ASSERT_FLOAT_EQ(IER_MAX, parameters_getInspireExpireRatio());

  parameters_setInspireExpireRatio(IER_MIN - .1);
  ASSERT_FLOAT_EQ(IER_MIN, parameters_getInspireExpireRatio());

  float mid_range_ier = IER_MIN + (IER_MAX - IER_MIN) / 2;
  parameters_setInspireExpireRatio(mid_range_ier);
  ASSERT_FLOAT_EQ(mid_range_ier, parameters_getInspireExpireRatio());
}

TEST(Parameters, SetAndGetPip) {
  parameters_setPIP(PIP_MAX + 1);
  ASSERT_FLOAT_EQ(PIP_MAX, parameters_getPIP());

  parameters_setPIP(PIP_MIN - 1);
  ASSERT_FLOAT_EQ(PIP_MIN, parameters_getPIP());

  float mid_range_pip = PIP_MIN + (PIP_MAX - PIP_MIN) / 2;
  parameters_setPIP(mid_range_pip);
  ASSERT_FLOAT_EQ(mid_range_pip, parameters_getPIP());
}

TEST(Parameters, SetAndGetDwell) {
  parameters_setDwell(DWELL_MAX + 1);
  ASSERT_FLOAT_EQ(parameters_getDwell(), DWELL_MAX);

  parameters_setDwell(DWELL_MIN - 1);
  ASSERT_FLOAT_EQ(parameters_getDwell(), DWELL_MIN);

  float mid_range_dwell = DWELL_MIN + (DWELL_MAX - DWELL_MIN) / 2;
  parameters_setDwell(mid_range_dwell);
  ASSERT_FLOAT_EQ(parameters_getDwell(), mid_range_dwell);
}
