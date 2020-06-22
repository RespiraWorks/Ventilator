# 2020-06-17 Master CoVent Tests on QuickLung with Adaptive Exhale Valve Control

- Test Hardware: Pizza board SN 01 (Edwin)
- PID Gains: kp = 0.05 ki = 6.0
- Exhale Valve Settings: exhale = 1.0 - 0.8*inhale - 0.2
- Pinch Valve Linearization: unchanged
- Test #8 calls for R10 but this is not a QuickLung setting, so R20 was used.
- Notes: Pinch valve replacement helped overshoot issues, but overshoot is still an issue

This branch implements an exhale valve control that is dependent on the inhale valve position.  It allows the exhale valve to close completely if the inhale valve is driven to full open.  This allows a very fast rise time response, but without the uncontrolled overshoot issues of having the exhale valve closed completely for the entire inhale phase.  This still has some overshoot and ringing problems in some conditions, but this may be solved by integrating a blower power control such as Justin's reduce blower power design.
