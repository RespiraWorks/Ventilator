# 2020-06-19 Improved AIR tuning: New Exhale Valve Limits, New Ki Gain Schedule

- Test Hardware: Pizza board SN 01 (Edwin)
- PID Gains: kp = 0.05 ki = std::clamp(0.5f * (desired_state.pip - desired_state.peep).cmH2O() + 5.0f, 10.0f, 20.0f);
- Exhale Valve Settings: exhale = 1.0f - 0.55 * blower_valve - 0.35
- Pinch Valve Linearization: unchanged
- Test #8 calls for R10 but this is not a QuickLung setting, so R20 was used.
- Notes: This dataset includes CoVent AD Instruments data, setup photos

Further tuning of the AIR control values trades some exhale fall time performance for better overshoot characteristics.

Stablity in test 5, 7, and 8 is improved, but 5 still possibly an issue.  Problematic undershoot in Test 6 is reduced.
