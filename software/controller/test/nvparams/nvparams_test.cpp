/* Copyright 2020, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "nvparams.h"

#include "checksum.h"
#include "gtest/gtest.h"

using namespace NVParams;

static constexpr uint32_t kMemSize{8192};

// Helper function to compare params in memory (in RAM if address is negative)
static void CompareParams(int16_t address, const Structure &ref, NVParams::Handler &nv_params_,
                          TestEeprom &eeprom_) {
  // Reminder to update this function when Structure changes size.
  static_assert(sizeof(Structure) == 228);
  Structure read;
  if (address < 0) {
    nv_params_.Get(0, &read, sizeof(Structure));
  } else {
    eeprom_.ReadBytes(static_cast<uint16_t>(address), sizeof(Structure), &read, nullptr);
  }

  // expect all members in both structs to be equal
  EXPECT_EQ(read.crc, ref.crc);
  EXPECT_EQ(read.count, ref.count);
  EXPECT_EQ(read.version, ref.version);
  EXPECT_EQ(read.reinit, ref.reinit);
  EXPECT_EQ(read.reserved, ref.reserved);
  EXPECT_EQ(read.vent_serial_number, ref.vent_serial_number);
  EXPECT_EQ(read.power_cycles, ref.power_cycles);
  EXPECT_EQ(read.cumulated_service, ref.cumulated_service);

  // Reminder to update this function when VentParams changes size.
  static_assert(sizeof(VentParams) == 32);
  EXPECT_EQ(read.last_settings.mode, ref.last_settings.mode);
  EXPECT_EQ(read.last_settings.peep_cm_h2o, ref.last_settings.peep_cm_h2o);
  EXPECT_EQ(read.last_settings.pip_cm_h2o, ref.last_settings.pip_cm_h2o);
  EXPECT_EQ(read.last_settings.breaths_per_min, ref.last_settings.breaths_per_min);
  EXPECT_EQ(read.last_settings.fio2, ref.last_settings.fio2);
  EXPECT_EQ(read.last_settings.inspiratory_expiratory_ratio,
            ref.last_settings.inspiratory_expiratory_ratio);
  EXPECT_EQ(read.last_settings.inspiratory_trigger_cm_h2o,
            ref.last_settings.inspiratory_trigger_cm_h2o);
  EXPECT_EQ(read.last_settings.expiratory_trigger_ml_per_min,
            ref.last_settings.expiratory_trigger_ml_per_min);

  ASSERT_EQ(read.blower_pinch_cal.size(), ref.blower_pinch_cal.size());
  for (size_t i = 0; i < ref.blower_pinch_cal.size(); ++i) {
    EXPECT_EQ(read.blower_pinch_cal[i], ref.blower_pinch_cal[i]);
  }
  ASSERT_EQ(read.exhale_pinch_cal.size(), ref.exhale_pinch_cal.size());
  for (size_t i = 0; i < ref.exhale_pinch_cal.size(); ++i) {
    EXPECT_EQ(read.exhale_pinch_cal[i], ref.exhale_pinch_cal[i]);
  }
  ASSERT_EQ(read.blower_cal.size(), ref.blower_cal.size());
  for (size_t i = 0; i < ref.blower_cal.size(); ++i) {
    EXPECT_EQ(read.blower_cal[i], ref.blower_cal[i]);
  }
  ASSERT_EQ(read.psol_cal.size(), ref.psol_cal.size());
  for (size_t i = 0; i < ref.psol_cal.size(); ++i) {
    EXPECT_EQ(read.psol_cal[i], ref.psol_cal[i]);
  }
}

uint32_t ParamsCRC(Structure *params) {
  uint8_t *ptr = reinterpret_cast<uint8_t *>(params);
  return soft_crc32(ptr + 4, sizeof(Structure) - 4);
}

class NVparamsTest : public ::testing::Test {
 public:
  NVparamsTest() : eeprom_(0x50, 64, kMemSize) {
    // Simulated EEPROM is initialized with FF, representative of a new chip
    nv_params_.Init(&eeprom_);
  }

  NVParams::Handler nv_params_;
  TestEeprom eeprom_;  // = TestEeprom(0x50, 64, kMemSize)
};

TEST_F(NVparamsTest, FirstInitEver) {
  // initialize a blank param to compare against flip side
  Structure ref_params;
  ref_params.crc = ParamsCRC(&ref_params);

  // Increment power_cycles (done at the end of Init method through Set)
  ref_params.power_cycles++;
  // This should equate the flip param because Set was called while current
  // params are in flip, so power cycles are updated but not crc and counter.
  SCOPED_TRACE("Flip side check");
  CompareParams(static_cast<uint16_t>(Address::Flip), ref_params, nv_params_, eeprom_);

  // Increment count, then compute crc, this should get us in the state of
  // flop and current params
  ref_params.count++;
  ref_params.crc = ParamsCRC(&ref_params);
  SCOPED_TRACE("Flop side check");
  CompareParams(static_cast<uint16_t>(Address::Flop), ref_params, nv_params_, eeprom_);

  SCOPED_TRACE("nv_param_ check");
  CompareParams(-1, ref_params, nv_params_, eeprom_);
}

TEST_F(NVparamsTest, Update) {
  Structure ref_params;
  nv_params_.Get(0, &ref_params, sizeof(Structure));
  ref_params.last_settings.mode = VentMode::VentMode_PRESSURE_CONTROL;

  // Update nv_params_ with new time and check that the write affected the flip
  // and flop sides with different crc
  nv_params_.Update(microsSinceStartup(60E6 + 1), &ref_params.last_settings);

  ref_params.count++;
  ref_params.cumulated_service = 60;

  // flip side's crc and counter do not take mode change into account
  ref_params.last_settings.mode = VentMode::VentMode_OFF;
  ref_params.crc = ParamsCRC(&ref_params);
  ref_params.last_settings.mode = VentMode::VentMode_PRESSURE_CONTROL;

  Structure read_params;
  eeprom_.ReadBytes(static_cast<uint16_t>(Address::Flip), sizeof(Structure), &read_params, nullptr);
  SCOPED_TRACE("Flip side check after Update");
  CompareParams(static_cast<uint16_t>(Address::Flip), ref_params, nv_params_, eeprom_);
  // Keep flip params for reuse later on
  Structure flip_params = ref_params;

  // nvparams and flop have the valid crc and counter
  ref_params.count++;
  ref_params.crc = ParamsCRC(&ref_params);

  SCOPED_TRACE("Flop side check after Update");
  CompareParams(static_cast<uint16_t>(Address::Flop), ref_params, nv_params_, eeprom_);

  SCOPED_TRACE("nv_param_ check after Update");
  CompareParams(-1, ref_params, nv_params_, eeprom_);

  // Call to update with nothing to change changes nothing
  nv_params_.Update(microsSinceStartup(119E6), &ref_params.last_settings);
  SCOPED_TRACE("Flip side check after Update");
  CompareParams(static_cast<uint16_t>(Address::Flip), flip_params, nv_params_, eeprom_);

  SCOPED_TRACE("Flop side check after useless Update");
  CompareParams(static_cast<uint16_t>(Address::Flop), ref_params, nv_params_, eeprom_);

  SCOPED_TRACE("nv_param_ check after useless Update");
  CompareParams(-1, ref_params, nv_params_, eeprom_);

  // Set vent serial number and check resulting params
  ref_params.count++;
  ref_params.vent_serial_number = 789;
  ref_params.crc = ParamsCRC(&ref_params);

  nv_params_.Set(8, &ref_params.vent_serial_number, 4);

  SCOPED_TRACE("nv_param_ check after Set");
  CompareParams(-1, ref_params, nv_params_, eeprom_);

  // Set last_settings using macro and check resulting params
  ref_params.count++;
  ref_params.last_settings = {
      .mode = VentMode::VentMode_PRESSURE_ASSIST,
      .peep_cm_h2o = 20,
      .breaths_per_min = 15,
      .pip_cm_h2o = 5,
      .inspiratory_expiratory_ratio = 0.5f,
      .inspiratory_trigger_cm_h2o = 6,
      .expiratory_trigger_ml_per_min = 200,
      .fio2 = 0.21f,
  };
  ref_params.crc = ParamsCRC(&ref_params);

  nv_params_.NV_PARAMS_UPDATE(last_settings, &ref_params.last_settings);

  SCOPED_TRACE("nv_param_ check after MACRO");
  CompareParams(-1, ref_params, nv_params_, eeprom_);

  ASSERT_FALSE(nv_params_.Set(4, &ref_params.count + 1, 1));
}

TEST_F(NVparamsTest, GetAndReadMacro) {
  Structure ref_params;
  ref_params.count++;
  ref_params.last_settings = {
      .mode = VentMode::VentMode_PRESSURE_ASSIST,
      .peep_cm_h2o = 20,
      .breaths_per_min = 15,
      .pip_cm_h2o = 5,
      .inspiratory_expiratory_ratio = 0.5f,
      .inspiratory_trigger_cm_h2o = 6,
      .expiratory_trigger_ml_per_min = 200,
      .fio2 = 0.21f,
  };
  ref_params.crc = ParamsCRC(&ref_params);

  nv_params_.NV_PARAMS_UPDATE(last_settings, &ref_params.last_settings);

  uint32_t var_32b{0};
  // power_cycles should be equal to 1 after previous test
  nv_params_.Get(12, &var_32b, 4);
  EXPECT_EQ(var_32b, 1);

  VentParams settings = VentParams_init_zero;
  // get last_settings through macro and check first and last members
  nv_params_.NV_PARAMS_READ(last_settings, &settings);
  EXPECT_EQ(settings.mode, VentMode::VentMode_PRESSURE_ASSIST);
  EXPECT_EQ(settings.fio2, 0.21f);
}

TEST_F(NVparamsTest, InitValidFlip) {
  // Init with valid Flip and invalid Flop.
  Structure flip_params = {
      .crc = 0,
      .count = 12,
      .version = 0,
      .reinit = 0,
      .reserved = 0,
      .vent_serial_number = 1234,
      .power_cycles = 6,
      .cumulated_service = 456780,
      .last_settings =
          {
              .mode = VentMode::VentMode_PRESSURE_ASSIST,
              .peep_cm_h2o = 20,
              .breaths_per_min = 15,
              .pip_cm_h2o = 5,
              .inspiratory_expiratory_ratio = 0.5f,
              .inspiratory_trigger_cm_h2o = 6,
              .expiratory_trigger_ml_per_min = 200,
              .fio2 = 0.21f,
          },
  };
  flip_params.crc = ParamsCRC(&flip_params);
  eeprom_.WriteBytes(static_cast<uint16_t>(Address::Flip), sizeof(Structure), &flip_params,
                     nullptr);

  Structure flop_params = {
      .crc = 0,
      .count = 10,
      .version = 0,
      .reinit = 0,
      .reserved = 0,
      .vent_serial_number = 2345,
      .power_cycles = 6,
      .cumulated_service = 784560,
      .last_settings =
          {
              .mode = VentMode::VentMode_PRESSURE_CONTROL,
              .peep_cm_h2o = 15,
              .breaths_per_min = 20,
              .pip_cm_h2o = 3,
              .inspiratory_expiratory_ratio = 0.8f,
              .inspiratory_trigger_cm_h2o = 4,
              .expiratory_trigger_ml_per_min = 100,
              .fio2 = 0.5f,
          },
  };
  // leave crc = 0 to make them invalid
  eeprom_.WriteBytes(static_cast<uint16_t>(Address::Flop), sizeof(Structure), &flop_params,
                     nullptr);

  nv_params_.Init(&eeprom_);
  // update count, power cycles and crc: this is done by the init function and
  // written to flop side, while only power cycle is written to the flip side
  Structure valid_params = flip_params;
  valid_params.count++;
  valid_params.power_cycles++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Init");
  CompareParams(-1, valid_params, nv_params_, eeprom_);

  SCOPED_TRACE("Flip check after Init");
  flip_params.power_cycles = valid_params.power_cycles;
  CompareParams(static_cast<uint16_t>(Address::Flip), flip_params, nv_params_, eeprom_);

  SCOPED_TRACE("Flop check after Init");
  flop_params.count = valid_params.count;
  flop_params.power_cycles = valid_params.power_cycles;
  flop_params.crc = valid_params.crc;
  CompareParams(static_cast<uint16_t>(Address::Flop), flop_params, nv_params_, eeprom_);

  // update nv_params_ with new settings and check that the write affects the
  // flip side, so the flip side is now identical to what is in RAM
  valid_params.last_settings = flop_params.last_settings;
  nv_params_.Update(microsSinceStartup(0), &valid_params.last_settings);
  valid_params.count++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Update");
  CompareParams(-1, valid_params, nv_params_, eeprom_);

  SCOPED_TRACE("Flip check after Update");
  CompareParams(static_cast<uint16_t>(Address::Flip), valid_params, nv_params_, eeprom_);
}

TEST_F(NVparamsTest, InitValidFlop) {
  // Init EEPROM with invalid Flip and valid Flop.
  Structure flip_params = {
      .crc = 0,
      .count = 10,
      .version = 0,
      .reinit = 0,
      .reserved = 0,
      .vent_serial_number = 2345,
      .power_cycles = 6,
      .cumulated_service = 784560,
      .last_settings =
          {
              .mode = VentMode::VentMode_PRESSURE_CONTROL,
              .peep_cm_h2o = 15,
              .breaths_per_min = 20,
              .pip_cm_h2o = 3,
              .inspiratory_expiratory_ratio = 0.8f,
              .inspiratory_trigger_cm_h2o = 4,
              .expiratory_trigger_ml_per_min = 100,
              .fio2 = 0.5f,
          },
  };
  // leave crc = 0 to render them invalid
  eeprom_.WriteBytes(static_cast<uint16_t>(Address::Flip), sizeof(Structure), &flip_params,
                     nullptr);

  Structure flop_params = {
      .crc = 0,
      .count = 12,
      .version = 0,
      .reinit = 0,
      .reserved = 0,
      .vent_serial_number = 1234,
      .power_cycles = 6,
      .cumulated_service = 456780,
      .last_settings =
          {
              .mode = VentMode::VentMode_PRESSURE_ASSIST,
              .peep_cm_h2o = 20,
              .breaths_per_min = 15,
              .pip_cm_h2o = 5,
              .inspiratory_expiratory_ratio = 0.5f,
              .inspiratory_trigger_cm_h2o = 6,
              .expiratory_trigger_ml_per_min = 200,
              .fio2 = 0.21f,
          },
  };
  flop_params.crc = ParamsCRC(&flop_params);
  eeprom_.WriteBytes(static_cast<uint16_t>(Address::Flop), sizeof(Structure), &flop_params,
                     nullptr);

  nv_params_.Init(&eeprom_);
  // update count, power cycles and crc: this is done by the init function and
  // written to flip side
  Structure valid_params = flop_params;
  valid_params.count++;
  valid_params.power_cycles++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Init");
  CompareParams(-1, valid_params, nv_params_, eeprom_);

  SCOPED_TRACE("Flip check after Init");
  flip_params.count = valid_params.count;
  flip_params.power_cycles = valid_params.power_cycles;
  flip_params.crc = valid_params.crc;
  CompareParams(static_cast<uint16_t>(Address::Flip), flip_params, nv_params_, eeprom_);

  SCOPED_TRACE("Flop check after Init");
  flop_params.power_cycles = valid_params.power_cycles;
  CompareParams(static_cast<uint16_t>(Address::Flop), flop_params, nv_params_, eeprom_);

  // update nv_params_ with new settings and check that the write affects the
  // flop side
  valid_params.last_settings = flip_params.last_settings;
  nv_params_.Update(microsSinceStartup(0), &valid_params.last_settings);
  valid_params.count++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Update");
  CompareParams(-1, valid_params, nv_params_, eeprom_);

  SCOPED_TRACE("Flop check after Update");
  CompareParams(static_cast<uint16_t>(Address::Flop), valid_params, nv_params_, eeprom_);
}

TEST_F(NVparamsTest, InitBothValid) {
  // Init EEPROM with valid Flip and Flop.
  Structure flip_params = {
      .crc = 0,
      .count = 11,
      .version = 0,
      .reinit = 0,
      .reserved = 0,
      .vent_serial_number = 1234,
      .power_cycles = 6,
      .cumulated_service = 456780,
      .last_settings =
          {
              .mode = VentMode::VentMode_PRESSURE_ASSIST,
              .peep_cm_h2o = 20,
              .breaths_per_min = 15,
              .pip_cm_h2o = 5,
              .inspiratory_expiratory_ratio = 0.5f,
              .inspiratory_trigger_cm_h2o = 6,
              .expiratory_trigger_ml_per_min = 200,
              .fio2 = 0.21f,
          },
  };
  flip_params.crc = ParamsCRC(&flip_params);
  eeprom_.WriteBytes(static_cast<uint16_t>(Address::Flip), sizeof(Structure), &flip_params,
                     nullptr);

  Structure flop_params = {
      .crc = 0,
      .count = 12,
      .version = 0,
      .reinit = 0,
      .reserved = 0,
      .vent_serial_number = 2345,
      .power_cycles = 6,
      .cumulated_service = 784560,
      .last_settings =
          {
              .mode = VentMode::VentMode_PRESSURE_CONTROL,
              .peep_cm_h2o = 15,
              .breaths_per_min = 20,
              .pip_cm_h2o = 3,
              .inspiratory_expiratory_ratio = 0.8f,
              .inspiratory_trigger_cm_h2o = 4,
              .expiratory_trigger_ml_per_min = 100,
              .fio2 = 0.5f,
          },
  };
  flop_params.crc = ParamsCRC(&flop_params);
  eeprom_.WriteBytes(static_cast<uint16_t>(Address::Flop), sizeof(Structure), &flop_params,
                     nullptr);

  nv_params_.Init(&eeprom_);
  // Should result in using flop params since count is higher.
  // Update count, power cycles and crc: this is done by the init function and
  // written to flip side
  Structure valid_params = flop_params;
  valid_params.count++;
  valid_params.power_cycles++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Init");
  CompareParams(-1, valid_params, nv_params_, eeprom_);

  // update those values in flip_params, the rest was kept in its previous
  // state
  flip_params.count = valid_params.count;
  flip_params.power_cycles = valid_params.power_cycles;
  flip_params.crc = valid_params.crc;
  SCOPED_TRACE("Flip check after Init");
  CompareParams(static_cast<uint16_t>(Address::Flip), flip_params, nv_params_, eeprom_);

  // only update power_cycles in flop params
  flop_params.power_cycles = valid_params.power_cycles;
  SCOPED_TRACE("Flop check after Init");
  CompareParams(static_cast<uint16_t>(Address::Flop), flop_params, nv_params_, eeprom_);
}
