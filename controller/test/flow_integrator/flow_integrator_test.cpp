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

#include "flow_integrator.h"
#include "gtest/gtest.h"

static const Volume COMPARISON_TOLERANCE_VOLUME = ml(1);

#define EXPECT_VOLUME_NEAR(a, b)                                               \
  EXPECT_NEAR((a).ml(), (b).ml(), COMPARISON_TOLERANCE_VOLUME.ml())

static constexpr Time base = microsSinceStartup(10'000'000);
static constexpr Duration sample_period = milliseconds(10);
Time ticks(int num_ticks) { return base + num_ticks * sample_period; }

// TODO: There ought to be more tests in here, e.g. of NoteExpectedVolume.
// TODO: Convert this test into a TEST_F.
// TODO: Move EXPECT_VOLUME_NEAR into a common header, for use here, in
// sensors_test, etc?

TEST(FlowIntegrator, FlowIntegrator) {
  // this Hal.delay is needed to allow TV construction with proper init time
  Hal.delay(base - Hal.now());
  FlowIntegrator tidal_volume;
  VolumetricFlow flow = liters_per_sec(1.0f);
  int t = 0;
  tidal_volume.AddFlow(ticks(t++), flow);
  // first call to AddFlow ==> initialization and TV is 0, even if flow is not
  EXPECT_EQ(tidal_volume.GetVolume().ml(), 0.0f);
  tidal_volume.AddFlow(ticks(t++), flow);
  // integrate 1 l/s flow over 10 ms ==> 5 ml (rectangle rule with initial flow
  // set to 0)
  EXPECT_VOLUME_NEAR(tidal_volume.GetVolume(), ml(5));

  tidal_volume.AddFlow(ticks(t++), cubic_m_per_sec(2e-3f));
  // add 2 l/s flow over 10 ms ==> 20 ml ()
  EXPECT_VOLUME_NEAR(tidal_volume.GetVolume(), ml(20));

  tidal_volume.AddFlow(ticks(t++), ml_per_min(0.0f));
  // add 0 l/s flow over 10 ms ==> 30 ml (rectangle rule)
  EXPECT_VOLUME_NEAR(tidal_volume.GetVolume(), ml(30.0f));

  // integrate 0 for some time ==> still 30 ms
  while (t < 100) {
    tidal_volume.AddFlow(ticks(t++), ml_per_min(0.0f));
  }

  EXPECT_VOLUME_NEAR(tidal_volume.GetVolume(), ml(30.0f));

  // reverse flow
  flow = liters_per_sec(-1.0f);
  // this does not increment t in order to allow oversampling (following test)
  tidal_volume.AddFlow(ticks(t), flow);
  // remove 1 l/s flow over 10 ms ==> 25 ml (rectangle rule)
  EXPECT_VOLUME_NEAR(tidal_volume.GetVolume(), ml(25.0f));

  // oversampling and expect volume to not change except on multiples of 5 ms
  for (int i = 0; i < 50; i++) {
    tidal_volume.AddFlow(ticks(t) + milliseconds(i), flow);

    // remove 1l/s flow over 5 ms only when i is a multiple of 5
    int j = i / 5 * 5;
    EXPECT_VOLUME_NEAR(tidal_volume.GetVolume(),
                       ml(25.0f - static_cast<float>(j)));
  }
}
