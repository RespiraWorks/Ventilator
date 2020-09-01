# Software Design

The RespiraWorks ventilator includes software running on two computers:
an STM32 microcontroller and a Raspberry Pi.

![Module diagram](module_diagram.png)

The microcontroller runs bare-metal and controls the sensors and
actuators (valves and blower).  Its main loop runs every
10ms on a hardware timer interrupt,
and the code running in the main loop contains no loops of variable
length, ensuring minimal jitter. The microcontroller’s software is
written in C++.

The Raspberry Pi runs Linux and is used for displaying the GUI. The
software we wrote is a Qt app, also written in C++. The app displays a
GUI on the ventilator’s touch screen, showing sensor data received from
the controller. Users set high-level parameters via the GUI (e.g. "PIP
20 cmH2O"), and then the GUI software commands the controller to meet
these parameters. The GUI software is also responsible for calculating
alarms and readings derived from raw sensor values (such as measured
PIP/PEEP).

We chose to use two separate computers for three reasons.
1. Two computers reduces the amount of software that is immediately hazardous
to the patient. Since our UI code runs on a separate device with no
direct access to the ventilator’s actuators, a bug or crash in it is
less likely to cause immediate harm; the ventilator will just keep
running with the most recent parameters (and, in the future, sound an alarm).
2. This design saves time and money because it allows us to use
large libraries like Linux and Qt, which make it easier to develop the
GUI, but would likely be unacceptable in a patient-critical module like
the cycle controller.
3. Having two modules adds a level of
redundancy. If the microcontroller crashes, it will immediately receive
new commands from the Raspberry Pi, and the Pi can raise an alarm.
Similarly, if the Pi crashes, the ventilator continues running with the
last set of parameters, and the cycle controller can raise an alarm.
(Disclosure: Neither of these alarms is currently implemented, and we
have not performed extensive testing of this failure model.)

Like the rest of the RespiraWorks project, our software is developed in
the open on GitHub. We have an extensive continuous integration
infrastructure running unit tests on x86, and we are building
infrastructure to run automated tests on the full device. We are also
nearly ready to start testing our software against a Modelica simulation
of the device. We hope this will make it easier for developers who do
not have access to hardware to contribute to the project. We also
believe having a robust simulation environment will allow us to run
tests that would otherwise be prohibitively time-consuming, for instance
running each commit against many combinations of ventilator parameters
plus lung compliances.

Besides a detailed user manual and maintenance plan, the device will also
come with a robust self-test mode.  This ensures that produced
ventilators achieve necessary safety levels before they are used with
patients and will allow users to check the health of the device over its
lifetime. A cryptographic hash will be released with the approved
software, and allow local manufacturers to be confident in the software
they are loading to the device.


## Communication Between Pi and Microcontroller

The microcontroller and Raspberry Pi communicate over a serial bus using
a simple, predictable protocol. Every ~50ms, the microcontroller sends
its current state to the Raspberry Pi, and every ~50ms, the Raspberry
Pi sends its full state to the microcontroller.  There are no ACKs,
resends, detection of missed packets, etc.

This simplified communication protocol avoids failure modes present in
other protocols.  For instance, imagine a different, stateful protocol
where one command the GUI can send is `set PIP to X`.  Suppose the GUI
sends two packets, `set PIP to 15` and `set PIP to 20`, and imagine that
the first one gets corrupted or lost on the wire.  The controller must
not accept the `PIP 20` command until it receives and applies the `PIP
15` command, otherwise the final PIP will be 15!  This requires buffers,
resends, and complex logic that, in the limit, looks much like a full
implementation of TCP.

Our design sidesteps this whole class of problems. If a message gets
missed or corrupted, we can just wait for the next one.

The state is encoded for transport on the wire using protocol
buffers (specifically, nanopb) and
wrapped in a frame with a checksum for detecting packet corruption. As
compared to a fully custom binary format, this makes it far easier to
change the protocol and potentially interoperate with other systems.

## Software component architecture

[Controller architecture](controller_architecture.md)

[GUI architecture](GUI_architecture.md)

## Other documents

For an overview of the software modules, see the Ventilator software architecture
[document](https://docs.google.com/document/d/1FPB31V72r_keu1_xjUfYCUXGLxBC5hoyCT1naPNNNTA).

**TODO:** Migrate contents of the above document to github.

There is a document defining the [ventilation modes](ventilation_modes.md) we intend to support.

## Testing considerations

There is a strategy defined for [Embedded_software_testing](embedded_software_testing.md).

## Milestones

We have completed and successfully demonstrated the
[Alpha Milestone](milestone_alpha.md) and the [v0.2 milestone](milestone_v0.2.md).

## Design Reviews

**TODO:** Links to the design reviews
