# 2020-06-21 Performance Evaluation Run for Record 1 - AIR

- Test Hardware: SN 01 (Edwin)
- PID Gains: kp = 0.04 ki = std::clamp(1.0f * (desired_state.pip - desired_state.peep).cmH2O() - 5.0f, 10.0f, 20.0f);
- Exhale Valve Settings: exhale = 1.0f - 0.55f * blower_valve - 0.4f
- Pinch Valve Linearization: unchanged
- Test #8 calls for R10 but this is not a QuickLung setting, so R20 was used.
- Notes: This dataset includes CoVent AD Instruments data, setup photos

Run for record using ambient blower-driven air only.

[Test Video link](https://youtu.be/tyYFPTh4At0)

Setup Photos:

![Setup Photo 1](SetupPhoto1.jpeg)

![Setup Photo 2](SetupPhoto2.jpeg)
