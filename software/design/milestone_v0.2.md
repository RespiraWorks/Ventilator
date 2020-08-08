# This milestone is complete

_Information below only for posterity._

## How to use this page
This is a high-level overview of what we want to achieve for v0.2 (a.k.a Covent) milestone. Up-to-date state of its implementation
is [here](https://docs.google.com/document/d/1CHgz-y6wV6duTg66s1h8-wZZ6Tn22U1iL5WtjhSYQgk/edit#). Requirements are captured on Valispace.

## Date

Deliver to Covent by June 18th.

# Objectives
## Modes
 1. Pressure Assist Mode. That is mandatory ventilation together with inspiratory attempt detection.
 2. (stretch goal) High-flow nasal cannula (HFNC). Ventilator provides light continuous positive pressure, patient breathes on their own.
 3. (stretch goal) Pre-test mode (a.k.a. self-test mode).

Full description of all ventilator modes is [here](ventilation_modes.md).

## User Interaction

See [UI Requirements](UI_requirements.md).

## Alarms
We need 6 alarms that were in covent requirements:
1. Gas pipeline failure
2. Pressure exceeds
3. Apnea
4. Pressure not achieved
5. Tidal volume
6. Electricity supply failure

## GUI
Two new features have to be added for Covent:
1. Modes selection
2. Alarms settings

# Requirements

Requirements are captured in Valispace, tag v0.2

# Tasks
Specific tasks are captured as GitHub Issues in the following milestones:
 - [v0.2 GUI](https://github.com/RespiraWorks/Ventilator/milestone/12)
 - [v0.2 Alarms](https://github.com/RespiraWorks/Ventilator/milestone/11)
 - [Pressure Assist Mode](https://github.com/RespiraWorks/Ventilator/milestone/8)
 - (stretch) [Self-test mode](https://github.com/RespiraWorks/Ventilator/milestone/10)
 - (stretch) [v0.2 Controller](https://github.com/RespiraWorks/Ventilator/milestone/13)


# Brain dump, to be sorted and reorganized into sections above

We also need:
* Architecture / preliminary design review
* Calibration tools/modes for subsystems
* Validation testing plans for specific subsystems (this might be software or additional documentation overhead)
* Listing and managing the software-specific risks (note: this work is underway [here](https://docs.google.com/spreadsheets/d/1qlTV5HqxnhlJXuhbSsEIfU-YwnN6PzgASv9_2dc5-cM/edit?usp=sharing) by jam@)
* We need a software architecture review
* We need 1 or 2 people to help physical design team publish their content on github and document things in a colleague- and people-friendly way

Specific software designs needed:
* A design for alarm management ([Slack thread](https://respiraworks.slack.com/archives/C0100SF3N5T/p1589153203185000))
* A design for controller self-test mode ([Slack thread](https://respiraworks.slack.com/archives/C0100SF3N5T/p1589153203185000))
* Persistence (what to do if controller resets - start from blank slate? recover control state?)

## Robust testing and debugging
* A debugging interface - ability for developers to debug what's going on in more detail than the user-facing GUI provides
* Continuous Integration that runs tests on a real device:
  * Running unit tests against a device - which is also critical for validating the correctness of the HAL
  * Running integration tests against a full ventilator assembly
* Continuous Integration that builds a GUI image for RPi

## Simulated testing

We need simulated integration testing of physical scenarios such as "patient is hooked up" etc.

We have a sub-team of folks from Modelon who hang out on [#modelica](https://app.slack.com/client/T0102KRCH5M/C011HUP75EJ) and build a model of the system (ventilator + patient + controller) in Modelica, a language for simulating physical systems. We want to integrate our Controller with that model so we can simulate what would happen if the real ventilator was being controlled by it. See [these meeting notes](https://docs.google.com/document/d/14lrTVcYuuUii7QTvuJJneex-_uKSHQ33Hg9CR5pOARo/edit#heading=h.cpwphjtw2hj8) for more.

Can we check in regularly against Modelica to gain confidence that we're meeting functional requirements?

Contact persons:
* On Modelica side, John Batteh and Elizabeth Hillstrom.
* On Software side, Eugene Kirpichov
