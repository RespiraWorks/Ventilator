## Example README to use for test folders

Test setup description for listed test (include enough information that we can determine the design this was associated with):

List of tests:

**Test Number:**
* Date of test:
* Person who did test:
* Software repo and branch used:
* File name containing test data:
* User settings (specify here or in test file)
    * PIP:
    * PEEP:
    * RR:
    * IE Ratio:
    * FiO2: 
    * Ventilator Mode (PC = pressure control (transitions triggered by time), PA = pressure assist (inhale triggered by patient), PAC = pressure assist/control, PS = pressure support):
    * Alarm settings if relevant:
* Lung values
    * Type of test lung used:
    * Compliance:
    * Resistance: 
* Are sensor readings included in the file (Y/N)?
* Are controller commands to each of the actuators included in the file (Y/N)?
* Open loop state actuators - provide info here or specify that they are contained in the file:
    * Blower speed on inhale:
    * Blower speed on exhale:
    * Exhale Valve state on inhale:
    * Exhale Valve state on exhale:
* Controller gains used (or other relevant controller information)
    * Kp:
    * Ki:
    * Kd:
* Information about mappings to/from controller (either provide mapping or pont to where in the code this is done):
    * Pressure sensor voltage to controller value mapping:
    * Air pinch valve command to voltage mapping:
    * Ox PSOL valve command to voltage mapping:

* Any other hardware settings that have been variable or might change (e.g. the bit resolution for the raw sensor valves)
