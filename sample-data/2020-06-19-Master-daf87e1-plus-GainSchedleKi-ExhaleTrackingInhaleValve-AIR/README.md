# 2020-06-18 Branch /inceptionev/PR-GainScheduleKi-ExhaleTrackingInhaleValveControl CoVent Tests on QuickLung

- Test Hardware: Pizza board SN 01 (Edwin)
- PID Gains: kp = 0.05 ki = gain scheduled based on PIP-PEEP from 10.0 to 20.0
- Exhale Valve Settings: exhale = 1.0 - 0.8*inhale - 0.2
- Pinch Valve Linearization: unchanged
- Test #8 calls for R10 but this is not a QuickLung setting, so R20 was used.
- Notes: This dataset includes CoVent AD Instruments data

This branch implements an exhale valve control that is dependent on the inhale valve position.  It allows the exhale valve to close completely if the inhale valve is driven to full open.  This allows a very fast rise time response, but without the uncontrolled overshoot issues of having the exhale valve closed completely for the entire inhale phase.  This still has some overshoot and ringing problems in some conditions, so further work will be required.

This branch also implments Gain Scheduling on Ki based on (PIP - PEEP).  This improves the dynamics significantly, but again more work will be needed.
