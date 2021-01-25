# 2021-01-24 CA Enclosed Vent Baseline Test - AIR

* **Date of test:** 2021-01-24

* **Tester:** Edwin Chiu edwin@respira.works

* **Software version**: [ba9d365](https://github.com/RespiraWorks/Ventilator/commit/ba9d36569508827b770cb7a36474aeba5220151e)

* **Test data:** /RespiraWorks/Ventilator/tree/master/software/sample-data/2021-01-24-CaEnclosedVent-Baseline-AIR

* **Summarize test setup's main components:**
  * This the first test of the Enclosed Ventilator build in California, using unmodified and untuned pizzabuild software
  * The main objective of this test is to establish baseline of perfromance before pursuing any changes or tuning.
  * Oxygen PSOL is UFlow IBV19M
  * Blower is WS7040
  * Oxygen Regulator IR2010-N02B
  * 4x filters are present: Inhale, Exhale, and both patient isolation filters.

* **User settings (specify here or in test file):**
    * **PIP (cmH20):** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **PEEP (cmH20):** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **RR (bpm):** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **IE Ratio:** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **FiO2 (%):** Tests 1-8 run with 100% FiO2 (pure oxygen)
    * **Ventilator Mode**: Pressure Assist
    * Alarm settings if relevant: N/A

* **Lung values:**
    * **Type of test lung used:** IngMar QuickLung
    * **Compliance (ml/cmH20):** CoVent tests 1-8 as defined in 80601 Table 201.105
    * **Resistance (cmH20/L/s):** CoVent tests 1-8 as defined in 80601 Table 201.105
* **Are sensor readings included in the file?**: Patient pressure and net flow are included

* **Are controller commands to each of the actuators included in the file?**: No

* **Open loop state actuators - provide info here or specify that they are contained in the file:**
    * Unmodified from master at commit listed - see commit link at top.

* **Controller gains used (or other relevant controller information)**
    * Unmodified from master at commit listed - see commit link at top.

* **Controller actuator limits:**
	* Unmodified from master at commit listed - see commit link at top.

* **Information about mappings to/from controller (either provide mapping or pont to where in the code this is done):**
    * Unmodified from master at commit listed - see commit link at top.

* **Any other hardware settings that have been variable or might change** *(e.g. the bit resolution for the raw sensor valves):*


* **Images**

![Setup Photo 1](SetupPhoto1.jpg)

![Setup Photo 2](SetupPhoto2.jpg)
