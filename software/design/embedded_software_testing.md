# Notions on software testing
When working on embedded software, there are some things that can be tested without running on the actual target machine, and some things that can not.

Anything that involves the electrical signals obviously falls into the second category. But some functional tests also need to be performed on the actual target in order to capture issues that are specific to the hardware.

We usually distinguish three types of tests for embedded software:
* Unit tests, which I think everybody is familiar with, that can usually be run outside the actual target.
* Integration tests, which are similar to units tests but involve the electrical signals and should therefore be run on the target.
* Functional tests, which are the most complete tests and can be run either on the actual target or not, depending on their scope, and on the stage of development.

Add to those the system tests, that run not only on the chip, but with the actual equipment : real sensors, real actuators, and eventually real pneumatics, and require access to physical parameters rather than electrical signals.

## Unit tests
A unit test is a check that a specific software module does what it is supposed to, in a use case that is as narrow as possible.
It should use the actual module's code, but can replace everything else with test-specific code.

Sometimes, unit tests will involve more than one module, and this is absolutely OK.

Unless there is a very good reason to run it on the actual target, it can be performed on a different environment.

## Integration tests
Integration tests check that the software low-level layers integrate properly with the hardware. For that reason they can not be run without the hardware.

These are basically unit tests, but with hardware in the loop: They need to use the actual low-level layers, but the high level layers can be test-specific.

## Functional tests
Functional tests, which check that the software actually does what it is supposed to do, and these usually need to use the actual software in its entirety, preferably in release mode, as they are the most complete tests we run on the software.
For that reason, they are preferably run on the target chip, but this rule is not set in stone.

## System tests
System tests are very similar to functional tests, but usually differ in terms of expected results: a system test no longer checks the controller's inner variables or the electrical signals, but the physical phenomenon that comes with it.
This means they can lose their deterministic nature, and results need to be interpreted with care.
They can be run on a partial system (for example replace the RPi with a laptop), but any piece of software being tested should be integrated in the full software (to capture CPU-sensitive phenomenons for example).

# Testing on the platform

Testing on the platform is usually harder than testing on a development environment, mostly because accessing the required data (electrical signal, software variable or physical parameter) is harder.

## Access to electrical signals
In order to test the embedded software on its target, we need a way to control the input signals and check the output signals of the system we are integrating.
For example, testing the controller on its hardware may require:
* reading/writing on the serial I/O line
* checking solenoid control signal
* observing blower PWM
* setting pressure sensor signal

This means you need some test equipments that allow you to read and write those signals: oscilloscopes, PWM acquisition card, voltage source, you name it.
To allow automation, those test equipments are integrated with a test PC that controls them and checks their results.

In case reading or writing a signal is too hard to do, it is OK to replace the software integration test with a system integration test where we control the physical parameter rather than the electrical one, but these are usually harder to interpret and therefore harder to automate.

For functional tests, this can be performed without using the actual hardware signals: if our integration + unit tests show that the fan power reported on the serial I/O is a reliable representation of the actual PWM signal, it is OK to use the fan power for a functional test instead of the PWM signal.
Similarly, we can use an engineering interface to replace the sensors' reading on the low-level layer output and not worry about the actual input pin.

## Access to physical parameters

The same test equipment can be used for system tests, along with additional equipment that read the physical parameter we want to check: redundant pressure sensor, speed meter for the blower, redundant flow meter, ... Those are rarely as well integrated with the test PC since the system tests that use them are not run as often and usually performed manually.

For example a system test that involves the pressure sensor would normally require placing the sensor in a pressure-controlled chamber (this is probably doable if we have the full pneumatics and a test lung, but really hard otherwise).
If that test is not primarily destined to test the pressure sensor, using an engineering interface to replace the sensors' readings with a simulated one is OK.

## Access to inner variables
If the integration tests need access to a high level module's inner variable, make it happen. Remember it is OK to hack the high level functions for an integration test.

If a functional test wants access to a module's inner variables:
* check if these variables can this be set or observed with an accessible input or output (even if it is not a 1 to 1 link)
* otherwise, these variables need to be made available on an engineering interface.

## Engineering interface
An engineering interface is a way for test engineers to access data that is otherwise not available outside the system.

I don't think we can actually get away without one of these for functional tests (but please prove me wrong! As this is usually quite a hassle to build and test).

Since the functional tests are preferably run on the release version of the software, keep in mind the engineering interface will be included in the final product, even though it is not needed by the users. It may be hard to access to a user, but it is still there.

That interface can be a specific interface (a second UART channel?) accessible only with specific instruments (instrumented PCB for testing, ...), or an extended version of the existing interface that is dormant unless some test conditions are met (I would go that route for our needs).
This interface must :
* Have unit tests of its own to make sure it gives a reliable copy of the desired data and properly writes data at the correct place
* Prevent accidental write on the write data (secure write privilege by a discrete signal, a specific sequence to enable writing, ...)
