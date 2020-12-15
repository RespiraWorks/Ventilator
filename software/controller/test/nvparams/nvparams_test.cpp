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

#include "checksum.h"
#include "eeprom.h"
#include "nvparams.h"
#include "gtest/gtest.h"

// Helper function to compare params in memory (in RAM if address is negative)
static void CompareParams(int16_t address, const NVparams_struct &ref) {

  // Reminder to update this function when NVparams_struct changes size.
  static_assert(sizeof(NVparams_struct) == 52);
  NVparams_struct read;
  if (address < 0) {
    nv_params.Get(0, &read, sizeof(NVparams_struct));
  } else {
    eeprom.ReadBytes(static_cast<uint16_t>(address), sizeof(NVparams_struct),
                     &read, nullptr);
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
  EXPECT_EQ(read.last_settings.breaths_per_min,
            ref.last_settings.breaths_per_min);
  EXPECT_EQ(read.last_settings.fio2, ref.last_settings.fio2);
  EXPECT_EQ(read.last_settings.inspiratory_expiratory_ratio,
            ref.last_settings.inspiratory_expiratory_ratio);
  EXPECT_EQ(read.last_settings.inspiratory_trigger_cm_h2o,
            ref.last_settings.inspiratory_trigger_cm_h2o);
  EXPECT_EQ(read.last_settings.expiratory_trigger_ml_per_min,
            ref.last_settings.expiratory_trigger_ml_per_min);
}

uint32_t ParamsCRC(NVparams_struct *params) {
  char *ptr = reinterpret_cast<char *>(params);
  return soft_crc32(ptr + 4, sizeof(NVparams_struct) - 4);
}

static constexpr uint32_t kMemSize{32768};

TEST(NVparams, FirstInitEver) {
  // Simulated EEPROM is initialized with FF, representative of a new chip
  nv_params.Init();

  // initialize a blank param to compare against flip side
  NVparams_struct ref_params;
  ref_params.crc = ParamsCRC(&ref_params);

  // Increment power_cycles (done at the end of Init method through Set)
  ref_params.power_cycles++;
  // This should equate the flip param because Set was called while current
  // params are in flip, so power cycles are updated but not crc and counter.
  SCOPED_TRACE("Flip side check");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlip), ref_params);

  // Increment count, then compute crc, this should get us in the state of flop
  // and current params
  ref_params.count++;
  ref_params.crc = ParamsCRC(&ref_params);
  SCOPED_TRACE("Flop side check");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlop), ref_params);

  SCOPED_TRACE("nv_param_ check");
  CompareParams(-1, ref_params);
}

TEST(NVparams, Update) {
  NVparams_struct ref_params;
  // We are reusing the resulting nv_params and eeprom state from previous test
  nv_params.Get(0, &ref_params, sizeof(NVparams_struct));
  ref_params.last_settings.mode = VentMode::VentMode_PRESSURE_CONTROL;

  // Update nv_params with new time and check that the write affected the flip
  // and flop sides with different crc
  nv_params.Update(microsSinceStartup(60E6 + 1), &ref_params.last_settings);

  ref_params.count++;
  ref_params.cumulated_service = 60;

  // flip side's crc and counter do not take mode change into account
  ref_params.last_settings.mode = VentMode::VentMode_OFF;
  ref_params.crc = ParamsCRC(&ref_params);
  ref_params.last_settings.mode = VentMode::VentMode_PRESSURE_CONTROL;

  NVparams_struct read_params;
  eeprom.ReadBytes(static_cast<uint16_t>(NVParamsAddress::kFlip),
                   sizeof(NVparams_struct), &read_params, nullptr);
  SCOPED_TRACE("Flip side check after Update");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlip), ref_params);
  // Keep flip params for reuse later on
  NVparams_struct flip_params = ref_params;

  // nvparams and flop have the valid crc and counter
  ref_params.count++;
  ref_params.crc = ParamsCRC(&ref_params);

  SCOPED_TRACE("Flop side check after Update");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlop), ref_params);

  SCOPED_TRACE("nv_param_ check after Update");
  CompareParams(-1, ref_params);

  // Call to update with nothing to change changes nothing
  nv_params.Update(microsSinceStartup(119E6), &ref_params.last_settings);
  SCOPED_TRACE("Flip side check after Update");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlip), flip_params);

  SCOPED_TRACE("Flop side check after useless Update");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlop), ref_params);

  SCOPED_TRACE("nv_param_ check after useless Update");
  CompareParams(-1, ref_params);

  // Set vent serial number and check resulting params
  ref_params.count++;
  ref_params.vent_serial_number = 789;
  ref_params.crc = ParamsCRC(&ref_params);

  nv_params.Set(8, &ref_params.vent_serial_number, 4);

  SCOPED_TRACE("nv_param_ check after Set");
  CompareParams(-1, ref_params);

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

  NVparamsUpdate(last_settings, &ref_params.last_settings);

  SCOPED_TRACE("nv_param_ check after MACRO");
  CompareParams(-1, ref_params);

  ASSERT_FALSE(nv_params.Set(4, &ref_params.count + 1, 1));
}

TEST(NVparams, GetAndReadMacro) {
  // We are reusing the resulting nv_params from previous test.
  // Make sure to update expected values if the previous test changes
  uint32_t var_32b{0};
  // power_cycles should be equal to 1 after previous test
  nv_params.Get(12, &var_32b, 4);
  EXPECT_EQ(var_32b, 1);

  VentParams settings = VentParams_init_zero;
  // get last_settings through macro and check first and last members
  NVparamsRead(last_settings, &settings);
  EXPECT_EQ(settings.mode, VentMode::VentMode_PRESSURE_ASSIST);
  EXPECT_EQ(settings.fio2, 0.21f);
}

TEST(NVparams, InitValidFlip) {
  // Init with valid Flip and invalid Flop.
  NVparams_struct flip_params = {
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
  eeprom.WriteBytes(static_cast<uint16_t>(NVParamsAddress::kFlip),
                    sizeof(NVparams_struct), &flip_params, nullptr);

  NVparams_struct flop_params = {
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
  eeprom.WriteBytes(static_cast<uint16_t>(NVParamsAddress::kFlop),
                    sizeof(NVparams_struct), &flop_params, nullptr);

  nv_params.Init();
  // update count, power cycles and crc: this is done by the init function and
  // written to flop side, while only power cycle is written to the flip side
  NVparams_struct valid_params = flip_params;
  valid_params.count++;
  valid_params.power_cycles++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Init");
  CompareParams(-1, valid_params);

  SCOPED_TRACE("Flip check after Init");
  flip_params.power_cycles = valid_params.power_cycles;
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlip), flip_params);

  SCOPED_TRACE("Flop check after Init");
  flop_params.count = valid_params.count;
  flop_params.power_cycles = valid_params.power_cycles;
  flop_params.crc = valid_params.crc;
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlop), flop_params);

  // update nv_params with new settings and check that the write affects the
  // flip side, so the flip side is now identical to what is in RAM
  valid_params.last_settings = flop_params.last_settings;
  nv_params.Update(microsSinceStartup(0), &valid_params.last_settings);
  valid_params.count++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Update");
  CompareParams(-1, valid_params);

  SCOPED_TRACE("Flip check after Update");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlip), valid_params);
}

TEST(NVparams, InitValidFlop) {
  // Init EEPROM with invalid Flip and valid Flop.
  NVparams_struct flip_params = {
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
  eeprom.WriteBytes(static_cast<uint16_t>(NVParamsAddress::kFlip),
                    sizeof(NVparams_struct), &flip_params, nullptr);

  NVparams_struct flop_params = {
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
  eeprom.WriteBytes(static_cast<uint16_t>(NVParamsAddress::kFlop),
                    sizeof(NVparams_struct), &flop_params, nullptr);

  nv_params.Init();
  // update count, power cycles and crc: this is done by the init function and
  // written to flip side
  NVparams_struct valid_params = flop_params;
  valid_params.count++;
  valid_params.power_cycles++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Init");
  CompareParams(-1, valid_params);

  SCOPED_TRACE("Flip check after Init");
  flip_params.count = valid_params.count;
  flip_params.power_cycles = valid_params.power_cycles;
  flip_params.crc = valid_params.crc;
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlip), flip_params);

  SCOPED_TRACE("Flop check after Init");
  flop_params.power_cycles = valid_params.power_cycles;
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlop), flop_params);

  // update nv_params with new settings and check that the write affects the
  // flop side
  valid_params.last_settings = flip_params.last_settings;
  nv_params.Update(microsSinceStartup(0), &valid_params.last_settings);
  valid_params.count++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Update");
  CompareParams(-1, valid_params);

  SCOPED_TRACE("Flop check after Update");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlop), valid_params);
}

TEST(NVparams, InitBothValid) {
  // Init EEPROM with valid Flip and Flop.
  NVparams_struct flip_params = {
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
  eeprom.WriteBytes(static_cast<uint16_t>(NVParamsAddress::kFlip),
                    sizeof(NVparams_struct), &flip_params, nullptr);

  NVparams_struct flop_params = {
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
  eeprom.WriteBytes(static_cast<uint16_t>(NVParamsAddress::kFlop),
                    sizeof(NVparams_struct), &flop_params, nullptr);

  nv_params.Init();
  // Should result in using flop params since count is higher.
  // Update count, power cycles and crc: this is done by the init function and
  // written to flip side
  NVparams_struct valid_params = flop_params;
  valid_params.count++;
  valid_params.power_cycles++;
  valid_params.crc = ParamsCRC(&valid_params);

  SCOPED_TRACE("nv_param_ check after Init");
  CompareParams(-1, valid_params);

  // update those values in flip_params, the rest was kept in its previous state
  flip_params.count = valid_params.count;
  flip_params.power_cycles = valid_params.power_cycles;
  flip_params.crc = valid_params.crc;
  SCOPED_TRACE("Flip check after Init");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlip), flip_params);

  // only update power_cycles in flop params
  flop_params.power_cycles = valid_params.power_cycles;
  SCOPED_TRACE("Flop check after Init");
  CompareParams(static_cast<uint16_t>(NVParamsAddress::kFlop), flop_params);
}
