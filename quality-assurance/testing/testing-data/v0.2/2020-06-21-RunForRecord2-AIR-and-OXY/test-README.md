# 2020-06-21 CoVent Run for Record (AIR and OXY)

* **Date of test:** 2020-06-21

* **Tester:** Edwin Chiu edwin@respira.works

* **Software version**: [b514f94](https://github.com/RespiraWorks/Ventilator/commit/b514f94b0de66710a43f8e3bb138cbb976c54fca)

* **Test data:** [Link to file](https://github.com/RespiraWorks/Ventilator/tree/master/sample-data/2020-06-21-RunForRecord2-AIR-and-OXY)

* **Summarize test setup's main components:**
  * Pizzabuild SN01 (Edwin) Inhale and Exhale pinch valves, oxygen PSOL

* **User settings (specify here or in test file):**
    * **PIP (cmH20):** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **PEEP (cmH20):** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **RR (bpm):** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **IE Ratio:** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **FiO2 (%):** Tests 1-8 run with 21% FiO2 (pure air) and 100% FiO2 (pure oxygen)
    * **Ventilator Mode**: Pressure Assist
    * Alarm settings if relevant: N/A

* **Lung values:**
    * **Type of test lung used:** IngMar QuickLung
    * **Compliance (ml/cmH20):** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **Resistance (cmH20/L/s):** CoVent tests 1-8 as defined in 80601 Table 201.105
* **Are sensor readings included in the file?**: Patient pressure and net flow are included

* **Are controller commands to each of the actuators included in the file?**: No

* **Open loop state actuators - provide info here or specify that they are contained in the file:**
    * **Blower speed on inhale:** 100%
    * **Blower speed on exhale:** 100%
    * **Exhale Valve state on inhale (AIR):** Closed loop 1.0f - 0.55f * blower_valve - 0.4f
    * **Exhale Valve state on exhale (AIR):** Closed loop 1.0f - 0.55f * blower_valve - 0.4f
	* **Exhale Valve state on inhale (OXY):** Closed loop 1.0f - 0.6f * psol_valve - 0.4f
    * **Exhale Valve state on exhale (OXY):** Closed loop 1.0f - 0.6f * psol_valve - 0.4f

* **Controller gains used (or other relevant controller information)**
    * **Kp:** 0.04
    * **Ki (AIR):** std::clamp(1.0f * (desired_state.pip - desired_state.peep).cmH2O() - 5.0f, 10.0f, 20.0f)
	* **Ki (OXY):** 20.0
    * **Kd:** 0.0

* **Information about mappings to/from controller (either provide mapping or pont to where in the code this is done):**
    * **Pressure sensor voltage to controller value mapping:** /controller/lib/core/sensors.cpp
    * **Air pinch valve command to voltage mapping:** /controller/lib/core/pinch_valve.cpp
    * **Ox PSOL valve command to voltage mapping:** /controller/lib/hal/psol.cpp

* **Any other hardware settings that have been variable or might change** *(e.g. the bit resolution for the raw sensor valves):*
  * [Details here]

* **Images**

![Setup Photo 1](SetupPhoto1.jpeg)

![Setup Photo 2](SetupPhoto2.jpeg)
