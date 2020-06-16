# Test Data README Template

This is a template file to use when you're creating a README file for a ventilator test. Be sure to include enough information that we can determine the design this was associated with.

**Copy and paste everything below the line into a new `.md` file within the dated directory for your specific test, following the instructions [here](README.md).**

-----------------------------

# [Test Number]

* **Date of test:** [Date]

* **Tester:** [Full name / email address]

* **Software version** *(link to specific commit by hash so we know the state of the software you tested)*: [Commit link]

* **Test data:** [Link to file]

* **User settings (specify here or in test file):**
    * **PIP (cmH20):** [value]
    * **PEEP (cmH20):** [value]
    * **RR (bpm):** [value]
    * **IE Ratio:** [value]
    * **FiO2 (%):** [value]
    * **Ventilator Mode** *(PC = pressure control (transitions triggered by time), PA = pressure assist (inhale triggered by patient), PAC = pressure assist/control, PS = pressure support)*: [value]
    * Alarm settings if relevant: [value]

* **Lung values:**
    * **Type of test lung used:** [value]
    * **Compliance (ml/cmH20):** [value]
    * **Resistance (cmH20/L/s):** [value]
* **Are sensor readings included in the file?** [Y/N]

* **Are controller commands to each of the actuators included in the file?** [Y/N]

* **Open loop state actuators - provide info here or specify that they are contained in the file:**
    * **Blower speed on inhale:** [value]
    * **Blower speed on exhale:** [value]
    * **Exhale Valve state on inhale:** [value]
    * **Exhale Valve state on exhale:** [value]

* **Controller gains used (or other relevant controller information)**
    * **Kp:** [value]
    * **Ki:** [value]
    * **Kd:** [value]

* **Information about mappings to/from controller (either provide mapping or pont to where in the code this is done):**
    * **Pressure sensor voltage to controller value mapping:** [value]
    * **Air pinch valve command to voltage mapping:** [value]
    * **Ox PSOL valve command to voltage mapping:** [value]

* **Any other hardware settings that have been variable or might change** *(e.g. the bit resolution for the raw sensor valves):* [Details here]

* **Images** 
[Embed images of the ventilator prototype you used for testing]
