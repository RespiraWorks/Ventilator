# Integration tests

This path contains code and relevant documentation specific to integration testing.
Instructions for running specific tests are in the respective files for those tests. You may also want to
scroll down to the section below on [computing](#Computing).

Integration tests check that the low-level layers of the software integrate properly with the hardware.
For this reason they cannot be run without the hardware. These are basically unit tests,
but with hardware in the loop: They need to use the actual low-level layers, but the high level
layers can be test-specific. The goal is to make tests as simple and granular as possible and to make them
automated.

Because of the complexity of electronics, achieving extremely granular tests may require
labor intensive customization of hardware and means of interpreting complex electrical signals.
It may not be cost effective to develop such testing infrastructure, so in scope some of these tests
may appear to be more like functional tests and system tests. For a broader view,
please read
[page](../../design/embedded_software_testing.md)
on strategy for embedded system testing.

In the end, the goal is to test some subsets of our system with hardware on a regular basis and
in a systematic way, to increase the likelihood of catching at least some problems early.
At whatever level of granularity, these tests are vertical integration tests, i.e. they consider
some cross-section of the hardware-software stack. To avoid the "fox watching the hen-house" situation,
we have aimed at using an alternative hardware-software stack to confirm the performance of our system.

## Testing hardware

Currently testing is performed on "Pizza build" prototypes. Hardware design is tracked [here](../../../design).

There are currently 3 prototypes accessible on the server at the following ports
```
/dev/ttyACM0
/dev/ttyACM1
/dev/ttyACM2
```
These are all v0.2 pizza builds with DIY test lungs, but no oxygen limb.

Some components in the testing prototypes will be modified to facilitate testing of subsystems.
Additional or redundant sensors may also be required to confirm that the lower layers of software
correctly interpret sensor signals and actuate subsystems as intended.

### Pinch valve
3d printed components for the pinch valve may be slightly modified to confirm homing
and calibration. Please check latest CAD in
[OnShape](https://cad.onshape.com/documents/3fe0c1f79c482144c267173d/w/2ad1c08071a25185f9c78c68/e/9acbcff0021f6442a8116619).

Other components being considered for pinch valve testing:
* [Optical endstop](https://www.amazon.com/MakerHawk-Optical-Endstop-Photoelectric-Control/dp/B07PMW2QMT)
* [Rotary encoder](https://www.cuidevices.com/product/motion/rotary-encoders/absolute/modular/amt20-v-kit/amt203s-v)

It may also be possible to detect some issues with stepper performance by tapping into hall effect
sensor signals on the stepper board.

### Pressure sensing
A redundant, more precise pressure sensor will be used to help validate
blower, on-board pressure sensors and venturi flow sensors:
[AMS5915-0100-D Amplified Low Pressure Sensor](https://www.analog-micro.com/products/pressure-sensors/board-mount-pressure-sensors/ams5915/ams5915-datasheet.pdf)

The sensor can deliver data using the i2c protocol, so it will require a capable data acquisition
system separate from the ventilator controller itself.

To tap into arbitrary points in the tubing, the following components might be useful:
* [Luer lock hypodermic needles](https://www.amazon.com/gp/product/B07TT3KNXP)
* [Male luer with lock ring x 1/8" hose barb](https://www.amazon.com/gp/product/B003NV2T34)

### Flow sensing

**TBD**

The following sensors are are in our possession and hopefully soon to be connected to the system:
* [Posifa PMF4103V](https://posifatech.com/wp-content/uploads/2019/10/DataSheet_PMF4000_MassAirFlow_RevE_C2.pdf)
* [Siargo FS6122](https://www.servoflo.com/sensors/mass-flow-sensors/siargo/fs6122)

### Blower

Blower function might be confirmed by:
 * measuring pressure or flow output
 * measuring current on power line
 * tapping into hall sensors on driver board

### Power cycling

It may be necessary to power cycle prototype ventilators or specific components externally.
To this end, we will be using a simple
[8-channel relay](https://www.amazon.com/SainSmart-Eight-Channel-Relay-Automation/dp/B0093Y89DE).

The relay is confirmed to work and currently we have a powerful shop light to illuminate the testing rig.

Instructions for how to operate the relay are [here](relay8).

### Data acquisition

Analog and digital signal acquisition will be handled using
* [Labjack T7](https://labjack.com/products/t7), possibly with optional
* [CB37 terminal expander board](https://labjack.com/products/cb37-terminal-board).

There is a bit of code for this under [/labjack](labjack).

### Visual observation

Two webcams are connected to server and each may be streamed to Youtube.

Instructions for how to use them are [here](webcam).

### Serial communications testing

AS more "honest" test would be to check communication on the
actual serial pins of the Nucleo. To do so, you can use a serial dongle such as
[this](https://www.amazon.com/gp/product/B00QT7LQ88),
or, for a more scalable setup, [this](https://www.amazon.com/dp/B07XF2SLQ1).

These are available at "the farm" upon request but no tests have been attempted thus far.

## Computing

There is a networked desktop PC connected to all test hardware via USB that will coordinate all integration tests. The
machine will run a Jenkins server that will perform nightly, on-demand and pull-request-initiated tests. A ventilator
prototype might be made available for remote experimentation. The server is already accessible to team
members via ssh. Please contact Martin on slack for secure access.

Tests will be scripted using a combination of Python and bash. Python scripts will mainly communicate with redundant
data acquisition hardware to confirm the functioning of our integrated components. Experimental python code
resides in subdirectories for [labjack](labjack) and [relay](relay8).

Controller code will be tested by running as light and shallow a subset of code as possible. Dependency decoupling
is never ideal, so some higher layers might still be involved. To avoid unnecessary complexity, alternative executable
with minimal `main()` loops are used. Debug and communications functions are not provided unless they are being
explicitly tested. The goal of each test executable in this directory is to exercise one type of interaction with
hardware. A corresponding Python script will confirm the performance of this test.

Results might be of the `PASS`/`FAIL` variety, but more likely will involve comparing collected and expected datasets
to be within some degree of error from each other. In addition to reporting acceptance, they should report some aggregate
measure that was used to judge the acceptance, and should also save the relevant raw data for later examination.

## Hardware-in-the-loop tests

The integration tests can be run using `controller.sh integrate`, which is self-documented via the `--help` option. Positional parameters may be passed to these tests, which are then compiled into the code as constants. See each implementation for what those positional parameters are used for.

### How to create a new test

Take something like [buzzer_test.h](buzzer_test.h) as a template. Create a new header in this directory ending in `_test.h`. It should contain a function `void run_test()`.

If you want to use parameters, they will be provided as pre-processor macros following the pattern of `TEST_PARAM_1`, `TEST_PARAM_2`, etc..

If you need more parameters, you may have to modify [../controller.sh](../controller.sh) as well as the`[env:integration-test]` entry in [../platformio.ini](../platformio.ini).

Once your new test is meaningful and functional, you should also add it to the sequence of tests run by the `run_all_integration_tests` function in [../controller.sh](../controller.sh).
