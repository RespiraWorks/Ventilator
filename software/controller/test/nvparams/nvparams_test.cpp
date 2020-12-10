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

// Helper function to compare two params.
static void CompareParams(NVparams ref, NVparams read) {

  // Reminder to update this function when NVparams changes size.
  static_assert(sizeof(NVparams) == 52);

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

static constexpr uint32_t kMemSize{32768};

TEST(NVparams, FirstInitEver) {
  // initialize simulated EEPROM with FF
  uint8_t memory[kMemSize];
  for (int i = 0; i < kMemSize; ++i) {
    memory[i] = 0xFF;
  }
  eeprom.WriteBytes(0, kMemSize, &memory, nullptr);
  nv_params.Init();

  // initialize a blank param to compare against flip side
  NVparams ref_params;
  char *ref_ptr = reinterpret_cast<char *>(&ref_params);
  ref_params.crc = soft_crc32(ref_ptr + 4, sizeof(NVparams) - 4);

  // Increment power_cycles
  ref_params.power_cycles++;
  // This should equate the flip param - Read those and compare
  NVparams read_params;
  eeprom.ReadBytes(static_cast<uint16_t>(NVParamsAddress::kFlip),
                   sizeof(NVparams), &read_params, nullptr);
  SCOPED_TRACE("flip side check");
  CompareParams(ref_params, read_params);

  // Increment count, then compute crc, this should get us in the state of flop
  // and current params
  ref_params.count++;
  ref_params.crc = soft_crc32(ref_ptr + 4, sizeof(NVparams) - 4);

  // read flop params and compare with ref
  eeprom.ReadBytes(static_cast<uint16_t>(NVParamsAddress::kFlop),
                   sizeof(NVparams), &read_params, nullptr);
  SCOPED_TRACE("flop side check");
  CompareParams(ref_params, read_params);

  // get current params and compare with ref
  nv_params.Get(0, &read_params, sizeof(NVparams));
  SCOPED_TRACE("nv_param_ check");
  CompareParams(ref_params, read_params);
}

TEST(NVparams, Update) {
  NVparams ref_params;
  // We are reusing the resulting nv_params and eeprom state from previous test
  nv_params.Get(0, &ref_params, sizeof(NVparams));
  ref_params.last_settings.mode = VentMode::VentMode_PRESSURE_CONTROL;

  // Update nv_params with new time and check that the write affected the flip
  // and flop sides with different crc
  nv_params.Update(microsSinceStartup(60E6 + 1), ref_params.last_settings);

  ref_params.count++;
  ref_params.cumulated_service = 60;

  // flip side's crc and counter do not take mode change into account
  ref_params.last_settings.mode = VentMode::VentMode_OFF;
  char *ref_ptr = reinterpret_cast<char *>(&ref_params);
  ref_params.crc = soft_crc32(ref_ptr + 4, sizeof(NVparams) - 4);
  ref_params.last_settings.mode = VentMode::VentMode_PRESSURE_CONTROL;

  NVparams read_params;
  eeprom.ReadBytes(static_cast<uint16_t>(NVParamsAddress::kFlip),
                   sizeof(NVparams), &read_params, nullptr);
  SCOPED_TRACE("flip side check after Update");
  CompareParams(ref_params, read_params);

  // nvparams and flop have the valid crc and counter
  ref_params.count++;
  ref_params.crc = soft_crc32(ref_ptr + 4, sizeof(NVparams) - 4);

  eeprom.ReadBytes(static_cast<uint16_t>(NVParamsAddress::kFlop),
                   sizeof(NVparams), &read_params, nullptr);
  SCOPED_TRACE("flop side check after Update");
  CompareParams(ref_params, read_params);

  nv_params.Get(0, &read_params, sizeof(NVparams));
  SCOPED_TRACE("nv_param_ check after Update");
  CompareParams(ref_params, read_params);

  // Set vent serial number and check resulting params
  ref_params.count++;
  ref_params.vent_serial_number = 789;
  ref_params.crc = soft_crc32(ref_ptr + 4, sizeof(NVparams) - 4);

  nv_params.Set(8, &ref_params.vent_serial_number, 4);

  nv_params.Get(0, &read_params, sizeof(NVparams));
  SCOPED_TRACE("nv_param_ check after Set");
  CompareParams(ref_params, read_params);

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
  ref_params.crc = soft_crc32(ref_ptr + 4, sizeof(NVparams) - 4);

  NVparamsUpdate(last_settings, &ref_params.last_settings);

  nv_params.Get(0, &read_params, sizeof(NVparams));
  SCOPED_TRACE("nv_param_ check after MACRO");
  CompareParams(ref_params, read_params);
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
  // reset simulated EEPROM with FF
  uint8_t memory[kMemSize];
  for (int i = 0; i < kMemSize; ++i) {
    memory[i] = 0xFF;
  }
  eeprom.WriteBytes(0, kMemSize, &memory, nullptr);

  // Init with valid Flip and invalid flop.
  NVparams valid_params = {
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
  char *ptr = reinterpret_cast<char *>(&valid_params);
  valid_params.crc = soft_crc32(ptr + 4, sizeof(NVparams) - 4);
  eeprom.WriteBytes(static_cast<uint16_t>(NVParamsAddress::kFlip),
                    sizeof(NVparams), &valid_params, nullptr);

  NVparams invalid_params = {
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
                    sizeof(NVparams), &invalid_params, nullptr);

  nv_params.Init();
  // update count, power cycles and crc: this is done by the init function and
  // written to flop side
  valid_params.count++;
  valid_params.power_cycles++;
  valid_params.crc = soft_crc32(ptr + 4, sizeof(NVparams) - 4);

  NVparams read_params;
  nv_params.Get(0, &read_params, sizeof(NVparams));
  SCOPED_TRACE("nv_param_ check after Init");
  CompareParams(valid_params, read_params);

  // update nv_params with new settings and check that the write affects the
  // flip side
  valid_params.last_settings = invalid_params.last_settings;
  nv_params.Update(microsSinceStartup(0), valid_params.last_settings);
  valid_params.count++;
  valid_params.crc = soft_crc32(ptr + 4, sizeof(NVparams) - 4);

  eeprom.ReadBytes(static_cast<uint16_t>(NVParamsAddress::kFlip),
                   sizeof(NVparams), &read_params, nullptr);
  SCOPED_TRACE("flip check after Update");
  CompareParams(valid_params, read_params);
}

TEST(NVparams, InitValidFlop) {
  // reset simulated EEPROM with FF
  uint8_t memory[kMemSize];
  for (int i = 0; i < kMemSize; ++i) {
    memory[i] = 0xFF;
  }
  eeprom.WriteBytes(0, kMemSize, &memory, nullptr);

  // Init with valid Flip and invalid flop.
  NVparams valid_params = {
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
  char *ptr = reinterpret_cast<char *>(&valid_params);
  valid_params.crc = soft_crc32(ptr + 4, sizeof(NVparams) - 4);
  eeprom.WriteBytes(static_cast<uint16_t>(NVParamsAddress::kFlop),
                    sizeof(NVparams), &valid_params, nullptr);

  NVparams invalid_params = {
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
                    sizeof(NVparams), &invalid_params, nullptr);

  nv_params.Init();
  // update count, power cycles and crc: this is done by the init function and
  // written to flip side
  valid_params.count++;
  valid_params.power_cycles++;
  valid_params.crc = soft_crc32(ptr + 4, sizeof(NVparams) - 4);

  NVparams read_params;
  nv_params.Get(0, &read_params, sizeof(NVparams));
  CompareParams(valid_params, read_params);

  // update nv_params with new settings and check that the write affects the
  // flop side
  valid_params.last_settings = invalid_params.last_settings;
  nv_params.Update(microsSinceStartup(0), valid_params.last_settings);
  valid_params.count++;
  valid_params.crc = soft_crc32(ptr + 4, sizeof(NVparams) - 4);

  eeprom.ReadBytes(static_cast<uint16_t>(NVParamsAddress::kFlop),
                   sizeof(NVparams), &read_params, nullptr);
  CompareParams(valid_params, read_params);
}
