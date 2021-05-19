.. SRS:

Software Requirements Specification
===================================

Purpose
-------

This document describe *what* |PROJECT_NAME| software must do.

This document is meant to be read and agreed-upon by the project owners and by software developers during design and construction.

.. todo:: Specify the functional requirements, performance requirements, interface requirements, safety requirements, hazard mitigations

Requirements
------------

General
^^^^^^^

.. item:: RW-SW-8
   :fulfills: RW-SYS-16

All software clocks must have high enough range to not overflow within 2 weeks

.. item:: RW-SW-9
   :fulfills: RW-SYS-16

Numeric algorithms within the software must not lose stability over 2 weeks

.. item:: RW-SW-10
   :fulfills: RW-SYS-16

Software algorithms must be runnable in faster than real time to allow stress testing of long-term continuous operation

.. item:: RW-SW-41
   :fulfills: RW-SYS-50

Software (both controller and Pi) should be power-efficient enough to continuously work from battery for 1 hour

.. item:: RW-SW-154
   :fulfills: RW-SYS-132

Controller must be able to detect patient inspiration


Alarms
^^^^^^

.. item:: RW-SW-45
   :fulfills:

GUI must have an alarm that triggers when the input air line is blocked (leading to low pressure), with severity HIGH

.. item:: RW-SW-58
   :fulfills:

GUI must have an alarm that triggers when the input O2 line is blocked (leading to low pressure), with severity HIGH

.. item:: RW-SW-60
   :fulfills: RW-SYS-9

Device and GUI must have an alarm for inspiratory pressure dropping below -4cm H2O

.. item:: RW-SW-21
   :fulfills:

GUI must have an alarm that triggers when the input O2 line pressure is too high, with severity HIGH

.. item:: RW-SW-61
   :fulfills:

Controller shall trigger an alarm in case GUI is unresponsive for 1/2 of the apnea time

.. item:: RW-SW-5
   :fulfills: RW-SYS-10

GUI displays an overpressure alarm

.. item:: RW-SW-36
   :fulfills:

GUI shall trigger an alarm in case controller is unresponsive for 1/2 of the apnea time

.. item:: RW-SW-6
   :fulfills: RW-SYS-10

Controller must implement an overpressure alarm firing above 60 cmH2O

.. item:: RW-SW-37
   :fulfills: RW-SYS-113

GUI can compute a "LOW VTI" alarm that becomes active if the VTI for the current breath is above threshold

.. item:: RW-SW-79
   :fulfills:

The LOW VTI alarm is of medium priority

.. item:: RW-SW-38
   :fulfills:

GUI has a control for setting the threshold for the LOW VTI alarm

.. item:: RW-SW-40
   :fulfills:

The control for setting the threshold for LOW VTI alarm allows setting values between X and Y (TODO what values)

.. item:: RW-SW-163
   :fulfills: RW-SYS-113

GUI can compute a "HIGH VTI" alarm that becomes active if the VTI for the current breath is above threshold

.. item:: RW-SW-164
   :fulfills:

The HIGH VTI alarm is of medium priority

.. item:: RW-SW-165
   :fulfills: RW-SYS-113

GUI can compute VTI for each breath

.. item:: RW-SW-14
   :fulfills: RW-SYS-112

GUI can compute a "LOW VTE" alarm that becomes active if the VTE for the current breath is above threshold

.. item:: RW-SW-15
   :fulfills: RW-SYS-112

GUI can compute a "HIGH VTE" alarm that becomes active if the VTE for the current breath is above threshold

.. item:: RW-SW-16
   :fulfills: RW-SYS-112

The LOW VTE alarm is of medium priority

.. item:: RW-SW-17
   :fulfills: RW-SYS-112

The HIGH VTE alarm is of medium priority

.. item:: RW-SW-18
   :fulfills: RW-SYS-112

GUI can compute VTE for each breath

.. item:: RW-SW-19
   :fulfills: RW-SYS-111

When cancelled, the "pressure high" alarm does not re-trigger for 2 minutes

.. item:: RW-SW-20
   :fulfills: RW-SYS-139

GUI shall be able to compute a high-priority alarm that triggers when the O2 input line is disconnected

.. item:: RW-SW-22
   :fulfills: RW-SYS-68

GUI must be able to display all currently active alarms if there are several of them

.. item:: RW-SW-26
   :fulfills: RW-SYS-69

The event log must include all confirmed alarm settings

.. item:: RW-SW-27
   :fulfills: RW-SYS-69

The event log must include all alarm occurrences

.. item:: RW-SW-28
   :fulfills: RW-SYS-69

The event log must include all alarm inhibitions, cancellations, resets, acknowledgements

.. item:: RW-SW-31
   :fulfills: RW-SYS-69

GUI must allow inhibiting alarms

.. item:: RW-SW-32
   :fulfills: RW-SYS-69

GUI must allow acknowledging alarms

.. item:: RW-SW-33
   :fulfills: RW-SYS-111 RW-SYS-69

GUI must allow cancelling alarms

.. item:: RW-SW-39
   :fulfills: RW-SYS-113 RW-SYS-79

GUI must allow setting an alarm on inspiration tidal volume above threshold between the bounds of 80 to 3000 ml in increments of 10ml

.. item:: RW-SW-42
   :fulfills: RW-SYS-111 RW-SYS-80

GUI must have an alarm that fires if for 3 consecutive breaths PIP >user-specified threshold

.. item:: RW-SW-44
   :fulfills: RW-SYS-116 RW-SYS-81

GUI must have an alarm that fires if amount of leakage is >user-specified threshold between 5 to 150lpm

.. item:: RW-SW-66
   :fulfills: RW-SYS-17 RW-SYS-97

Controller must be able to detect and/or receive alarms from GUI

.. item:: RW-SW-67
   :fulfills: RW-SYS-17 RW-SYS-97

Controller must be able to command the hardware PCB buzzer.

.. item:: RW-SW-68
   :fulfills: RW-SYS-98

GUI/Controller enumerates alarm codes and assigns them priorities

.. item:: RW-SW-69
   :fulfills: RW-SYS-68 RW-SYS-99

GUI displays highest priority alarm on main screen

.. item:: RW-SW-70
   :fulfills: RW-SYS-68 RW-SYS-99

GUI displays list of active alarms on button press

.. item:: RW-SW-71
   :fulfills: RW-SYS-17 RW-SYS-97 RW-SYS-100

GUI displays alarm on main screen legible at distance of 1 meter.

.. item:: RW-SW-73
   :fulfills: RW-SYS-101

High severity alarms shall be indicated with a red color and a flashing frequency of 2Hz with a 50% duty cycle

.. item:: RW-SW-74
   :fulfills: RW-SYS-102

Low severity alarms shall be indicated with a yellow color which shall be constantly on.

.. item:: RW-SW-75
   :fulfills: RW-SYS-103

Medium severity alarms shall be indicated with a yellow color, and a flashing frequency of 0.5Hz with a 50% duty cycle

.. item:: RW-SW-76
   :fulfills: RW-SYS-101 RW-SYS-104

High severity alarms shall have an auditory signal consisting of 10 pulses with each pulse lasting 200ms with a pulse frequency of 500Hz (A4 note).

.. item:: RW-SW-77
   :fulfills: RW-SYS-101 RW-SYS-104

High severity alarms shall have an auditory signal consisting of 4 pulses with each pulse lasting 200ms with a pulse frequency of 500Hz. (how is this different from 77?)

.. item:: RW-SW-78
   :fulfills: RW-SYS-106

A press on the AUDIO PAUSE / ALARM PAUSE key shall initiate an auditory alarm signal pause of all active alarms during 60 seconds (Assuming that all the active alarms can be inhibited). The alarm activation symbol shall be displayed during the inactivation state with the symbol IEC 60417-5576

.. item:: RW-SW-80
   :fulfills: RW-SYS-128 RW-SYS-108

Alarm: apnea. If no breath is triggered within the specified apnea time interval, the ventilator shall respond with a medium priority alarm with the display text "APNEA ALARM"

.. item:: RW-SW-82
   :fulfills: RW-SYS-128 RW-SYS-109

Alarm: breath rate. When the breath rate has exceeded the breath rate setting, the device shall trigger a medium level alarm with the display text "HIGH BREATH RATE"

.. item:: RW-SW-83
   :fulfills: RW-SYS-128 RW-SYS-109

There must be a maximum breath rate setting which acts as alarm threshold.

.. item:: RW-SW-84
   :fulfills: RW-SYS-110

Alarm: pressure low. Upon a low pressure alarm, the gui will sound and display a high severity alarm and a text shall be displayed as "LOW INSP P".

.. item:: RW-SW-85
   :fulfills: RW-SYS-110

GUI will log onset and reset of low pressure alarm.

.. item:: RW-SW-86
   :fulfills: RW-SYS-110

Alarm: pressure low. The system shall trigger a low pressure alarm with a high priority in the event of a low inspiratory pressure.This alarm shall be automatically reset upon the first breath with inspiration pressure above the threshold.

.. item:: RW-SW-88
   :fulfills: RW-SYS-111

GUI can compute a "pressure high" alarm that becomes active when pressure goes above a specified threshold for 3+ consecutive breaths AND becomes inactive on the first breath where it doesn't go above the threshold

.. item:: RW-SW-89
   :fulfills: RW-SYS-111

While the "pressure high" alarm is active, GUI emits a sound

.. item:: RW-SW-90
   :fulfills: RW-SYS-111

The "pressure high" alarm is displayed until cancelled even if inactive

.. item:: RW-SW-91
   :fulfills: RW-SYS-111

Alarm management subsystem should support an alarm state where the alarm is visible but not audible

.. item:: RW-SW-93
   :fulfills: RW-SYS-111 RW-SYS-80

The PIP too high alarm shall have high priority

.. item:: RW-SW-94
   :fulfills: RW-SYS-111 RW-SYS-80

The default value for the "PIP too high" alarm shall be the commanded PIP value + 5cm H2O

.. item:: RW-SW-103
   :fulfills: RW-SYS-114

Controller should report whether the proximal tube is connected

.. item:: RW-SW-104
   :fulfills: RW-SYS-114

GUI should compute a "proximal tube disconnect" alarm that becomes active when the proximal tube is disconnected

.. item:: RW-SW-105
   :fulfills: RW-SYS-114

The "proximal tube disconnected" alarm is of medium priority

.. item:: RW-SW-106
   :fulfills: RW-SYS-115

GUI can detect obstruction, based on the condition that VTI < 100ml

.. item:: RW-SW-107
   :fulfills: RW-SYS-115

When obstruction is detected, GUI activates an "obstruction" alarm

.. item:: RW-SW-108
   :fulfills: RW-SYS-115

The obstruction alarm is inactivated when obstruction is not detected

.. item:: RW-SW-109
   :fulfills: RW-SYS-115

The obstruction alarm has high priority

.. item:: RW-SW-110
   :fulfills: RW-SYS-116

GUI can compute amount of leakage (L/min) based on sensor readings

.. item:: RW-SW-111
   :fulfills: RW-SYS-116

GUI can compute a "high leakage" alarm that becomes active when average leakage over X seconds is > 3 L/min

.. item:: RW-SW-112
   :fulfills: RW-SYS-116

The "high leakage" alarm is high priority

.. item:: RW-SW-114
   :fulfills: RW-SYS-117

The "involuntary shutdown" alarm is a continuous-tone alarm

.. item:: RW-SW-115
   :fulfills: RW-SYS-117

The "involuntary shutdown" alarm emits a sound tone for at least 2 min

.. item:: RW-SW-116
   :fulfills: RW-SYS-117

The alarm subsystem supports continuous-tone alarms

.. item:: RW-SW-117
   :fulfills: RW-SYS-118

GUI can detect loss of AC mains

.. item:: RW-SW-118
   :fulfills: RW-SYS-118

GUI is connected to the "AC mains power loss" sensor (and we have such a sensor)

.. item:: RW-SW-119
   :fulfills: RW-SYS-118

The AC mains power loss alarm is medium priority

.. item:: RW-SW-120
   :fulfills: RW-SYS-120 RW-SYS-119

GUI can compute battery ETA from a history of remaining battery amounts

.. item:: RW-SW-121
   :fulfills: RW-SYS-120 RW-SYS-119

GUI is connected to a battery amount sensor

.. item:: RW-SW-122
   :fulfills: RW-SYS-119

GUI can compute a "low battery" alarm when battery ETA is <30min

.. item:: RW-SW-123
   :fulfills: RW-SYS-119

The "low battery" alarm is low priority

.. item:: RW-SW-124
   :fulfills: RW-SYS-120

GUI can compute a "critically low battery" alarm when battery ETA is <10min

.. item:: RW-SW-125
   :fulfills: RW-SYS-120

The "critically low battery" alarm is high priority

.. item:: RW-SW-128
   :fulfills: RW-SYS-121

GUI needs to be able to read hardware battery presense and chargeability sensors

.. item:: RW-SW-129
   :fulfills: RW-SYS-121

GUI needs to have a "battery failure" alarm that is activated when the battery is absent, unavailable, or unchargeable

.. item:: RW-SW-130
   :fulfills: RW-SYS-121

The "battery failure" alarm has high priority

.. item:: RW-SW-131
   :fulfills: RW-SYS-122 RW-SYS-125

(System requirement 125) "System shall be capable of sensing the temperature of heating elements (blower motor, battery cell during operation)"

.. item:: RW-SW-132
   :fulfills: RW-SYS-122

GUI needs to be able to read temperature sensors for: blower motor, blower power mgmt system, battery cell

.. item:: RW-SW-133
   :fulfills: RW-SYS-122

GUI needs to have a "blower motor temperature too high" alarm triggered by a user-specified threshold

.. item:: RW-SW-134
   :fulfills: RW-SYS-122

GUI needs to have a "blower power system temperature too high" alarm triggered by a user-specified threshold

.. item:: RW-SW-135
   :fulfills: RW-SYS-122

GUI needs to have a "battery temperature too high" alarm triggered by a user-specified threshold

.. item:: RW-SW-137
   :fulfills: RW-SYS-122

GUI needs to automatically invoke a safety action when blower power system temperature is above threshold

.. item:: RW-SW-138
   :fulfills: RW-SYS-122

GUI needs to automatically invoke a safety action when battery temperature is above threshold

.. item:: RW-SW-144
   :fulfills: RW-SYS-128

GUI has a control for setting the threshold for HIGH RR alarm

.. item:: RW-SW-145
   :fulfills: RW-SYS-128

The control for setting the threshold for HIGH RR alarm allows setting values between 5-30 in increments of 1

.. item:: RW-SW-146
   :fulfills: RW-SYS-128 RW-SYS-129

GUI has a control for setting the threshold for LOW RR alarm

.. item:: RW-SW-147
   :fulfills: RW-SYS-128 RW-SYS-129

The control for setting the threshold for LOW RR alarm allows setting values between 5-30 in increments of 1

.. item:: RW-SW-148
   :fulfills: RW-SYS-128 RW-SYS-130

GUI has a control for setting the threshold for the apnea alarm

.. item:: RW-SW-149
   :fulfills: RW-SYS-128 RW-SYS-130

The control for setting the threshold for the apnea alarm allows setting values between 1-30 seconds in increments of 1s

.. item:: RW-SW-158
   :fulfills:

Alarm when relief valve opens. Need to detect this based on an unexpected increase in flow (not pressure). Might be folded into a leak alarm - but need to ensure we account for this case and test it.


Self-Test
^^^^^^^^^

.. item:: RW-SW-1
   :fulfills: RW-SYS-131

Self-test shall include a test for accuracy of the O2 sensor

.. item:: RW-SW-2
   :fulfills: RW-SYS-131

Self-test shall include validation of blower response: commanded speed vs. pressure achieved at that speed

.. item:: RW-SW-3
   :fulfills: RW-SYS-131

Self-test shall include validation of the pinch valves pressure response at a fixed blower speed

.. item:: RW-SW-159
   :fulfills: RW-SYS-131

Self-test shall include validation that all the audible alarm devices work. Eg could test that, given voltage there's current, or could ask the user to confirm that it's audible

.. item:: RW-SW-160
   :fulfills: RW-SYS-131

Self-test shall include validation that all visual alarms work (asking user to confirm that the alarm works)

.. item:: RW-SW-161
   :fulfills: RW-SYS-131

Self-test shall include validation of flow sensor accuracy (set blower to certain speed, air lines open, nothing connected to the output etc. -> gives expected flow)

.. item:: RW-SW-166
   :fulfills: RW-SYS-131

Self-test shall include validation of pressure sensor accuracy

.. item:: RW-SW-150
   :fulfills: RW-SYS-131

Controller must support a pre-test mode

.. item:: RW-SW-151
   :fulfills: RW-SYS-131

GUI must support the controller's pre-test mode

.. item:: RW-SW-152
   :fulfills: RW-SYS-131

Controller's pre-test mode must include the following checks: ...


GUI
^^^

.. item:: RW-SW-162
   :fulfills: RW-SYS-5

GUI allows user to adjust PEEP from 0-20 cmH2O with increments of 1

.. item:: RW-SW-11
   :fulfills: RW-SYS-17

GUI shall render parameters and alarms readable at a distance of 1m

.. item:: RW-SW-12
   :fulfills: RW-SYS-20

GUI must allow adjusting RR from 5-30 bpm with increments of 1

.. item:: RW-SW-13
   :fulfills: RW-SYS-21

GUI must allow adjusting IER from 1:1 to 1:4 (Inhale to exhale)

.. item:: RW-SW-24
   :fulfills: RW-SYS-69

GUI must require all ventilation and alarm setting changes to be confirmed

.. item:: RW-SW-34
   :fulfills: RW-SYS-72

GUI must allow adjusting O2 infusion (FiO2) in supplied air from 0.21 to as close to 1 as possible with a minimum of 0.96, adjustable in increments of 0.1.

.. item:: RW-SW-35
   :fulfills: RW-SYS-77

GUI must allow adjusting HFNC flow rate from 0 to 60 L/min in increments of 5(TBD) L/min

.. item:: RW-SW-43
   :fulfills: RW-SYS-116 RW-SYS-81

GUI must be able to compute amount of leakage based on controller actuation and sensor readings

.. item:: RW-SW-46
   :fulfills: RW-SYS-3 RW-SYS-83

GUI must display patient pressure measured at the gas outlet port, based on data from controller

.. item:: RW-SW-47
   :fulfills: RW-SYS-3 RW-SYS-83

GUI graph refresh rate must be >= 20 Hz

.. item:: RW-SW-48
   :fulfills: RW-SYS-3 RW-SYS-83

GUI graph duration must be 30 seconds

.. item:: RW-SW-49
   :fulfills: RW-SYS-32 RW-SYS-84

GUI must display a reading of inspiratory tidal volume in the range 0..1000mL with resolution of 1mL

.. item:: RW-SW-50
   :fulfills: RW-SYS-21 RW-SYS-85

GUI must display the ratio of inspiration time to expiration time of the most recent breath in the range  10:1 to 1:200 with display resolution of 0.1 for values under 10 and 1 for values over 10. Transition from inspire to expire is defined as being after the plateau

.. item:: RW-SW-53
   :fulfills: RW-SYS-5 RW-SYS-86

GUI must display a reading of PEEP for the most recent breath in increments of 1 cmH2O

.. item:: RW-SW-54
   :fulfills: RW-SYS-32 RW-SYS-87

GUI must display a graph of volume provided to the patient

.. item:: RW-SW-92
   :fulfills: RW-SYS-87

The GUI shall display the flow rate provided to the patient as a graph with duration of 30 seconds updated at a rate >=20Hz and units of liters / minute.

.. item:: RW-SW-96
   :fulfills: RW-SYS-72 RW-SYS-138

GUI shall display FiO2 as measured prior to the patient inhale limb

.. item:: RW-SW-99
   :fulfills: RW-SYS-3

GUI shall allow the user to set PIP from 5-55 cmH2O with increments of 1 cm H2O.

.. item:: RW-SW-113
   :fulfills: RW-SYS-117

GUI can detect controller shutdown

.. item:: RW-SW-139
   :fulfills:

In [what modes?] , GUI should have a "rise time" parameter controlling how long it takes from PEEP to PIP

.. item:: RW-SW-141
   :fulfills:

In  GUI should allow setting the "rise parameter" in the range of 0..20% of breath cycle time

.. item:: RW-SW-155
   :fulfills: RW-SYS-132

GUI must allow specifying the assist/control with pressure controlled ventilation mode


Embedded
^^^^^^^^

.. item:: RW-SW-4
   :fulfills: RW-SYS-9

Controller should respond fast enough to prevent pressure dropping below -4cm under ? (under what conditions)

.. item:: RW-SW-7
   :fulfills: RW-SYS-10

Controller should never command pressure to go above 60

.. item:: RW-SW-52
   :fulfills: RW-SYS-5 RW-SYS-86

SW must measure PEEP over each breath (pressure at the end of the breath)

.. item:: RW-SW-55
   :fulfills: RW-SYS-142 RW-SYS-88

For parameters that do not have a different performance target specified, controller shall be able to hit commanded values within 10%

.. item:: RW-SW-56
   :fulfills: RW-SYS-89

Controller should trigger an inspiration cycle when (NEED TO CLARIFY)

.. item:: RW-SW-57
   :fulfills:

Controller should trigger an expiration cycle when

.. item:: RW-SW-59
   :fulfills: RW-SYS-3

Controller should be able to meet PIP targets in the range 5-55cmH2O +/- 5cm

.. item:: RW-SW-62
   :fulfills: RW-SYS-93

Controller should be able to meet commanded breath rate within +- 1 breath

.. item:: RW-SW-63
   :fulfills: RW-SYS-94

Controller should be able to meet commanded pressure during the pressure fall phase within an overshoot of 0.8*PEEP-1 cmH2O

.. item:: RW-SW-64
   :fulfills: RW-SYS-142 RW-SYS-95

Controller should be able to meet commanded PEEP within +-1 H2O + 10% (of what?)

.. item:: RW-SW-95
   :fulfills: RW-SYS-72

Controller shall be able to achieve target FiO2 value within 0.05

.. item:: RW-SW-97
   :fulfills: RW-SYS-72 RW-SYS-138

Controller shall report FiO2 as measured prior to patient inhale limb

.. item:: RW-SW-98
   :fulfills: RW-SYS-137 RW-SYS-63

Controller shall be able to achieve commanded values under different air conditions affecting air density, such as temperature, altitude and humidity

.. item:: RW-SW-153
   :fulfills: RW-SYS-132

Controller must implement assist/control with pressure controlled ventilation mode, where inspiration is triggered by detecting patient inspiration

.. item:: RW-SW-156
   :fulfills:

Controller must have safety measures to avoid acting on absurd commands even in case of data corruption between GUI and controller communication


Logging
^^^^^^^

.. item:: RW-SW-23
   :fulfills: RW-SYS-68

GUI must store an event log in non-volatile memory

.. item:: RW-SW-25
   :fulfills: RW-SYS-69

The event log must include all confirmed ventilation settings

.. item:: RW-SW-29
   :fulfills: RW-SYS-69

The event log must include a date and timestamp

.. item:: RW-SW-30
   :fulfills: RW-SYS-69

The event log must include changes to the system's real time clock by logging the Current Date/Time followed by the new Date/Time


Traceability Matrix
===================
.. item-matrix:: System Requirements to SRS
   :target: RW-SYS-
   :source: RW-SW-
   :sourcetitle: Software Requirements
   :targettitle: System Requirements
   :type: fulfills
