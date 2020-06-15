## This folder is going to store test data from v0.2 of the ventilator

For each test, please do the following:
1. Create a folder for your test set up that has a README explaining the overall test set up
2. Place the data for a given test in that folder and update the README in that folder with the information about the specific test. At minimum between the data file and the description in the README, the following should be captured:
    * User settings (PIP, PEEP, RR, IE ratio, FiO2, etc.)
    * All sensor readings
    * Commands from the controller to each of the actuators
    * Commands for each of the open state actuators (blower, exhaust valve)
    * Snapshot of the controller gains used
    * Information for how the output commands from the controller are mapped to the actual voltages for the actuators
    * The lung compliance/resistance used (if there is a way to measure it, e.g. like the quick lung offers)
    * Any other hardware settings that have been variable or might change (e.g. the bit resolution for the raw sensor valves)
