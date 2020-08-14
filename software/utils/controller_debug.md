# Debug serial interface

[software/utils/controller_debug.py](controller_debug.py)

This Python script can be used to interface with the controller via the virtual serial
port that is created when the USB cable to the Nucleo board is plugged in.

To use this debug tool you will need to have Python 3.x installed on your development computer.  Several additional Python modules will also be needed:
- pyserial.  This is a Python module that allows serial port access
- matplotlib.  This is a graphing module used to display data

**TODO:** have script install dependencies

The debug tool was developed and tested using Linux, but it should be possible to use it with other operating systems.

To run the debug tool you will first need to connect your development computer to the Nucleo board using a USB cable.  When the Nucleo board is connected to the computer in this way it should create a virtual serial port.  Under Linux the default device file is named `/dev/ttyACM0`.  Make note of the device file name if the default one is not used.

After loading the controller software and running it via Platform IO you should be able to start the debug tool using the following command:
```
utils/controller_debug.py
```
That command will start the debugger using the default serial port name `/dev/ttyACM0`.

If the virtual serial port created by the Nucleo board does not have that default name, you can supply the name of the serial port device file on the command line when you start the debug program.  For example:
```
utils/controller_debug.py /dev/ttyACM1
```

If the debug tool is successful in connecting to the Nucleo board, you should see a command prompt consisting of a single square bracket:
```
]
```

The debug tool consists of a simple command line interface.  To get a list of possible commands, enter `help` on the command line.  To get help on a specific command, enter help _command name_.  For example, to get a description of the `get` command:
```
] help get
```

Several commands are currently supported by the debug tool:

**get**
**set**
These commands are used to read and write the values of debug variables.  Debug variables are objects that allow the controller to be instrumented for remote monitoring or control.  For example, there is a debug variable (named `pressure`) which allows the current patient pressure to be displayed.
```
get pressure
```
Debug variables are very easy to add to the controller software, so it’s common to add some temporary debug variables while working on a particular feature.  Typically such temporary debug variables are removed before the changes are merged into the master branch of the repository.  Only debug variables that are of general interest should be left in the master branch.

The details of adding debug variables are beyond the scope of this document, but interested readers should check the vars.h header file in the debug library.

**console**
This command can be used to display messages written using the `debug.Print` function in the console.

`debug.Print` allows messages to be output from the controller code using sprintf style formatting.  When this function is called by the controller code the string is formatted and copied to a circular buffer in the controller.  Running the console command causes the debug program to continuously read data from this circular buffer and display it.

To exit console mode, enter control-C

**peek**
**poke**
These commands allow memory locations to be read (peek) or written (poke).  They are primarily useful when developing hardware drivers as part of the HAL and should generally be used with caution.

**trace**
One of the most useful features of the debug utilities is the trace buffer.  The trace buffer is a large block of RAM into which debug variables can be saved periodically.  That data can then be download using the trace command and either saved to a file or displayed graphically.

To use the trace, several debug variables must be set to configure it.
- trace_var1 … trace_var4.  These four variables define what will be saved to the trace buffer.  The value you set them to is the name of another debug variable that you wish to save.  Up to 4 debug variables can be stored in the trace at any time using these four variables.
- trace_period.  This sets the period (in units of loop cycles) at which the debug variables will be stored to the trace buffer.  Setting trace_period to 1 will cause the data to be stored every cycle.  Setting it to 2 will cause the data to be stored every other cycle, etc.
- trace_ctrl.  This is used to start and stop the trace feature.  Setting this variable to 1 starts the collection of trace data.  Setting it to 0 stops the collection of trace data.  If the trace buffer fills up, then this will be automatically set to 0 by the controller software.
- trace_samples.  This variable can be read to see how many samples of trace data have been collected.

Once the trace is set up and started, the collected data can be graphed using the command:
```
trace graph
```
or stored to a file
```
trace download <filename>
```
For example, to capture patient pressure every loop cycle, the following commands would be used:
```
set trace_var1 pressure
set trace_period 1
set trace_ctrl 1
```
pause for a few seconds while the data is captured, then
```
trace graph
```
This will download and graph the data.

**run**
This command takes the name of a python script and executes it in the same environment that the debug program runs in.  This allows the script to use functions defined in the debug tool to do things like get or set variables.
The debug tool will search for scripts in the current working directory, and also in a sub-directory named ‘scripts’.
