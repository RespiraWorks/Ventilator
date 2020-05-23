This document will track desired characterizatino tests and link to the corresponding data when the test is performed. Eventually the info from this document will likely be formalized into a record of tests that have been performed as part of the development and validation of the ventilator - integration tests with the real software are expected to be defined elsewhere, though they could be folded into this.

Caution: this document has not been formally reviewed and is a work in progress. Be responsible to understand the provenance of documentation.

NOTE: these tests are supposed to be incrementally developed. As we progress, the tests will become more granular.

## Pneumatics Component/System Tests

* **System characterization**
1. What is the maximum pressure that can be achieved with the current blower with a representative amount of tubing, a fully open air inhale valve, and a fully closed exhale valve?

2. From PEEPs of 0 to 20 cm H20 (in reasonable increments), how quickly can the current design achieve PIPs from 10 cm H20 up to 60 cm H20 (or the max value, testing reasonable increments)?

3. From PIPs from 20 cm H20 up to 60 cm H20 (or however high can be achieved, in reasonable increments), how quickly can the system vent down to PEEPs of 0 to 20 cm H20 (testing reasonable increments)?

4. For PIPs from 20 cm H20 up to 60 cm H20 (in reasonable increments, what is the maximum flow rate into the patient that can be achieved? How does this change as a function of how much the exhaust valve is open?

5. For 1-4 with a flow source that is better than the blower, what can be achieved (what becomes the next lim fac)?

6. What is the difference in pressure right after the inhale venturi (before the humidifier) and at the patient pressure sensor before the exhale venturi?

* **Pinch Valve Characterizatino**
1. How quickly can the pinch valve be fully opened? How does this change as a function of pressure (up to at least 60 cm H20)?

2. How quickly can the pinch valve be fully closed? How does this change as a function of pressure (up to at least 60 cm H20)

3. What is the leak rate past a fully closed pinch valve as a functin of pressure (up to at least 60 cm H20)?

* **Proportional Solenoid Characterizatino**
1. How quickly can the solenoid be fully opened? How does this change as a function of pressure (up to at least 60 cm H20)?

2. How quickly can the solenoid be fully closed? How does this change as a function of pressure (up to at least 60 cm H20)

3. What is the leak rate past a fully closed soleonid valve as a function of pressure (up to at least 60 cm H20)?

* **DP sensor/Venturi characterization
1. For flow rates from 1 to 80 lpm, how accurate is the flow measurement and how does this accuracy change over time (if sitting at a specific FiO2)?

2. How do the measurements for #1 change as a function of temperature and humidity?

3. How do the measurements for #1 change as a functino of the flow rate that is being started at (e.g. if you go from 1->80 or 40->80 does the accuracy change? What about 80->1 and 80->40)?

4. When there is a change in flow rate, how long does it take for the sensor to settle?

* **Pressure sensor characterization
1. For pressures from 1 to 60 cm H20, how accurate is the pressure reading and how does this accuracy change over time (if sitting at a specific FiO2)?

2. How do the measurements for #1 change as a function of temperature and humidity?

3. How do the measurements for #1 change as a function of flow rate past the sensor?

3. How do the measurements for #1 change as a function of the pressure that is being started at (e.g. if you go from 5->60 or 30->60 does the accuracy change? What about 60->5 and 60->30)?

4. When there is a change in pressure, how long does it take for the sensor to settle?

* **O2 sensor characterization
1. For FiO2s from 21% to 100%, how accurate is the o2 sensor reading and how does this accuracy change over time (if sitting at a specific FiO2)?

2. How do the measurements for #1 change as a function of temperature and humdity?

3. How do the measurements from #1 change as a function of flow rate?

4. When there is a change in FiO2, how long does it take for the sensor to settle?

## Things that need to be added
