# Debug serial interface

The controller debug interface should be run using [debug.sh](debug.sh) wrapper of from the common controller utilities script at [software/controller/controller.sh](../../controller/controller.sh)

This Python script can be used to interface with the controller via the virtual serial port that is created when you plug a USB cable into the controller's debug port. The command line interface allows you to examine and directly manipulate a number of low level variables, as well as run calibration routines and automated performance tests.

To use this debug tool you will need to have Python 3.x installed on your development computer, as well as a number of python packages. Necessary dependencies will be installed by the [controller.sh](../../controller/controller.sh) script.

The debug tool was developed and tested using Linux, but it should be possible to use it with other operating systems. The command line interface will allow you to discover available commands using `help`. Some commands provide an "autocomplete" feature you can access with `TAB`. To get help on a specific command, enter help _command name_.  For example, to get a description of the `get` command:
```
[sn:v03e2] help get
```

## Connecting

To use the debug tool you will first need to connect your development computer to the controller's debug port using a USB cable. This should create a virtual serial port.

Serial ports can be searched and connection established using the `device` command.

- `device list` - will detect STM controllers connected to machine and print them out
- `device auto` - will detect a controller and connect to it, if there is one
- `device <alias/port>` - will connect to specific device, using either its alias or port name.
- `device update` - will download the canonical RespiraWorks device manifest from Google Drive. At some point you should register your device on the spreadsheet.

Once connected, the prompt should display the hardware unit's serial number. If there is no assigned serial number, the prompt will display the serial port name.

You may also immediately connect when calling the debug script using
`./debug.sh --device /dev/ttyACM01` or `./debug.sh -d e2` or `./debug.sh -d auto`.

## Commands

Several commands are currently supported by the debug tool:

### get and set
These commands are used to read and write the values of debug variables.  Debug variables are objects that allow the controller to be instrumented for remote monitoring or control.  For example, there is a debug variable (named `pressure`) which allows the current patient pressure to be displayed.
```
get pressure
```
or you may check on the totality of system state by retrieving all variables

```
get all
```

Debug variables are very easy to add to the controller software, so it’s common to add some temporary debug variables while working on a particular feature. Typically, such temporary debug variables are removed before the changes are merged into the master branch of the repository.  Only debug variables that are of general interest should be left in the master branch.

The details of adding debug variables are beyond the scope of this document, but interested readers should check the vars.h header file in the debug library.

### debug

This command can be used to display messages written using the `debug.Print` function in the console.

```
debug on
```

`debug_print()` allows messages to be output from the controller code using sprintf style formatting.  When this function is called by the controller code the string is formatted and copied to a circular buffer in the controller.  Running the console command causes the debug program to continuously read data from this circular buffer and display it.

To exit console mode, enter control-C

### peek and poke
These commands allow memory locations to be read (peek) or written (poke).  They are primarily useful when developing hardware drivers as part of the HAL and should generally be used with caution.

### eeprom
This command allows the user to `read` and `write` in non-volatile memory.
To read the user provides the data's address and number of bytes to read:
```
eeprom read 4096 52
```
To write the user provides the data's address and the bytes to write:
```
eeprom write 128 255 255 255 255 255 255 255
```

### trace
One of the most useful features of the debug utilities is the trace buffer.  The trace buffer is a large block of RAM into which debug variables can be saved periodically.  That data can then be downloaded using the trace command and either saved to a file or displayed graphically.

To start the trace, one needs to provide a list of traced vars and (optionally) a trace period:
```
trace start [--period <period>] <var_name1 ... var_name4>
```
where period is given in loop cycles

Once the trace is set up and started, the collected data can be saved to disk using:
```
trace save
```
For example, to capture patient pressure every loop cycle, the following commands would be used:
```
trace start pressure
```
pause for a few seconds while the data is captured, then
```
trace save --plot
```
This will download and plot the data.

### test
The test interface augments the trace interface by acquiring data in a more structured way. each "test" is a well-defined experimental scenario that identifies variables of interest. Such performance tests can be reproduced by other engineers and testers and easily compared. For anything other than "on the fly" experimenting, you should prefer this approach.

You can view available test scenarios using
```
test list
```

To see the full definition of a test named "iso_pc_1", use
```
test show iso_pc_1
```

To run the test and display acquired data in plots, do
```
test run iso_pc_1 --plot
```

Each time you run a test scenario, all prescribed variables will be applied. You will also be given the opportunity to adjust manual (physically manipulable) settings. All available metadata about the state of the ventilator at the time of this experiment will be saved along with the acquired data.

To retrieve save data and view it, you may type:
```
test read 2021-08-15-23-23-23_john_test.json --plot
```

Test scenarios are defined in `json` files in the [test_scenarios](test_scenarios) directory. All files in that directory should be loaded when the script starts. You may also see additional commands provided by this interface with `help test`.

### run
This command takes the name of a python script and executes it in the same environment that the debug program runs in.  This allows the script to use functions defined in the debug tool to do things like get or set variables. The debug tool will search for scripts in the [scripts](scripts) subdirectory. These scripts may take additional parameters.

## Unit Tests
### Local Setup
The unit tests for the debug interface were created using pytest. Necessary dependencies will be installed by the [controller.sh](../../controller/controller.sh) script.

### Running Unit Tests Locally
To run the unit tests, run the following command in Bash (not in the debug interface) within this working directory (`Ventilator/software/utils/debug`):
```bash
python3 -m pytest --verbose unit_test.py
```
Omit the verbose flag if you only need to see failed tests.
