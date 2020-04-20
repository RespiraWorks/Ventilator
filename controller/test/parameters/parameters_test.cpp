#include "parameters.h"
#include "gtest/gtest.h"

TEST(Parameters, SetAndGetRR) {
  parameters_setRR(RR_MAX + 1);
  float rr = parameters_getRR();
  ASSERT_FLOAT_EQ(RR_MAX, rr);

  parameters_setRR(RR_MIN - 1);
  rr = parameters_getRR();
  ASSERT_FLOAT_EQ(RR_MIN, rr);

  parameters_setRR(RR_MIN + (RR_MAX - RR_MIN) / 2);
  rr = parameters_getRR();
  ASSERT_FLOAT_EQ(RR_MIN + (RR_MAX - RR_MIN) / 2, rr);
}

TEST(Parameters, SetAndGetTV) {
  parameters_setTV(TV_MAX + 1);
  float tv = parameters_getTV();
  ASSERT_FLOAT_EQ(TV_MAX, tv);

  parameters_setTV(TV_MIN - 1);
  tv = parameters_getTV();
  ASSERT_FLOAT_EQ(TV_MIN, tv);

  parameters_setTV(TV_MIN + (TV_MAX - TV_MIN) / 2);
  tv = parameters_getTV();
  ASSERT_FLOAT_EQ(TV_MIN + (TV_MAX - TV_MIN) / 2, tv);
}

TEST(Parameters, SetAndGetPEEP) {
  parameters_setPEEP(PEEP_MAX + 1);
  float peep = parameters_getPEEP();
  ASSERT_FLOAT_EQ(PEEP_MAX, peep);

  parameters_setPEEP(PEEP_MIN - 1);
  peep = parameters_getPEEP();
  ASSERT_FLOAT_EQ(PEEP_MIN, peep);

  parameters_setPEEP(PEEP_MIN + (PEEP_MAX - PEEP_MIN) / 2);
  peep = parameters_getPEEP();
  ASSERT_FLOAT_EQ(PEEP_MIN + (PEEP_MAX - PEEP_MIN) / 2, peep);
}

TEST(Parameters, SetAndGetIER) {
  parameters_setInspireExpireRatio(IER_MAX + .1);
  float ier = parameters_getInspireExpireRatio();
  ASSERT_FLOAT_EQ(IER_MAX, ier);

  parameters_setInspireExpireRatio(IER_MIN - .1);
  ier = parameters_getInspireExpireRatio();
  ASSERT_FLOAT_EQ(IER_MIN, ier);

  parameters_setInspireExpireRatio(IER_MIN + (IER_MAX - IER_MIN) / 2);
  ier = parameters_getInspireExpireRatio();
  ASSERT_FLOAT_EQ(IER_MIN + (IER_MAX - IER_MIN) / 2, ier);
}

TEST(Parameters, SetAndGetPip) {
  parameters_setPIP(PIP_MAX + 1);
  float pip = parameters_getPIP();
  ASSERT_FLOAT_EQ(PIP_MAX, pip);

  parameters_setPIP(PIP_MIN - 1);
  pip = parameters_getPIP();
  ASSERT_FLOAT_EQ(PIP_MIN, pip);

  parameters_setPIP(PIP_MIN + (PIP_MAX - PIP_MIN) / 2);
  pip = parameters_getPIP();
  ASSERT_FLOAT_EQ(pip, PIP_MIN + (PIP_MAX - PIP_MIN) / 2);
}

TEST(Parameters, SetAndGetDwell) {
  parameters_setDwell(DWELL_MAX + 1);
  float dw = parameters_getDwell();
  ASSERT_FLOAT_EQ(dw, DWELL_MAX);

  parameters_setDwell(DWELL_MIN - 1);
  dw = parameters_getDwell();
  ASSERT_FLOAT_EQ(dw, DWELL_MIN);

  parameters_setDwell(DWELL_MIN + (DWELL_MAX - DWELL_MIN) / 2);
  dw = parameters_getDwell();
  ASSERT_FLOAT_EQ(dw, DWELL_MIN + (DWELL_MAX - DWELL_MIN) / 2);
}
