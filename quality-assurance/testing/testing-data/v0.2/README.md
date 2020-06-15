# v0.2 Testing Data

This folder stores test data from v0.2 of the ventilator.

## Instructions for Testers

For people running tests—for each test, please do the following:
1. Create a directory for your test set up within `SystemDesign/quality-assurance/testing/testing-data/v0.2`.
    * Use a format of `mmddyyyy` for the directory name (e.g., a test conducted on June 14, 2020 would have a directory name of `06142020`).
    * Add data files for a given test into that directory. 
2.  Within the dated directory for the test, create a README.md (markdown file) explaining the overall test set up as well as information about the specific test. 
    * **Please use the [README template here](test-README-template.md). For additional information on how to structure your file using markdown,see [the repository wiki here](https://github.com/RespiraWorks/SystemDesign/wiki).** 
    * At minimum between the data file and the description in the README, the following test information should be captured:
      * User settings (PIP, PEEP, RR, IE ratio, FiO2, etc.)
      * All sensor readings
      * Commands from the controller to each of the actuators
      * Commands for each of the open state actuators (blower, exhaust valve)
      * Snapshot of the controller gains used
      * Information for how the output commands from the controller are mapped to the actual voltages for the actuators
      * The lung compliance/resistance used (if there is a way to measure it, e.g., the quick lung offers)
      * Any other hardware settings that have been variable or might change (e.g., the bit resolution for the raw sensor valves)


