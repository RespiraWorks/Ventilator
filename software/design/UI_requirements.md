# User Interface Requirements

The UI runs on a 1024x600 touchscreen. There are no external buttons or anything.

The UI requirements below come from several sources:
* Original mock-ups (the black-and-white mockups included
  [here](https://www.ics.com/blog/ics-joins-respiraworks-on-ventilator-project))
* [ICS prototype](http://demo.ics.com/ventilatorui-demo/) that was built based on the mock-ups
* [This paper](https://ccforum.biomedcentral.com/articles/10.1186/s13054-016-1431-1) and
  [this demo](https://www.youtube.com/watch?v=BT3snJ1Hm2s)
* [These notes](https://docs.google.com/document/d/1QFFcg2gL3PXXzyqgSBz3XZQm_oobZ2osFuErh492UEA/edit) from a
  conversation with a respiratory therapist

The list below is incomplete and possibly wrong in some ways.
Please consult Valispace and the main [Requirements page](../../requirements).
However, Valispace also does not have complete information at the time of writing. Proceed with caution.

## Self-test mode

On startup, ventilator is in self-test mode. It performs a set of checks and reports on the status.

Some checks are **automatic** _(eg that a certain tube is not blocked: turn on the blower, check that output flow
sensor detects some flow)_, Some are **manual** _(eg for "the buzzer works" it needs to turn on the buzzer and ask user
whether it's audible)_

The **full set of checks** is not yet known and generally open-ended.
Checks should include **guidance on how to fix** if it failed.
The self-test mode should be easy to use _(the hospital where the therapist above works decided not to purchase vents
where it was too cumbersome)_.
Results of self-test should be retained in memory.

**Open questions:**
* Should you be able to perform self-test only on startup or at any time?
* Should you confirm that patient is not connected?
* Should device refuse to operate if a check fails?

See also https://github.com/RespiraWorks/Ventilator/issues/307.

## Ventilation modes

We intend to support **Command Pressure** _(device commands all breaths)_, **Pressure Assist** _(device detects inhale
effort and helps that breath, but also initiates breaths if patient doesn't)_, **CPAP with O2 therapy aka HFNC** _(light
constant positive O2-saturated air pressure, non-invasive)_.

_Note: Different sources seem to indicate that pressure-controlled modes are going out of favor and volume control is
preferable (regular - not PRVC or VC+) - eg see
[notes](https://docs.google.com/document/d/1QFFcg2gL3PXXzyqgSBz3XZQm_oobZ2osFuErh492UEA/edit). But for v0.2 we're doing
pressure control._

**Mode parameters**: Each mode has several parameters defining it, e.g. Respiratory Rate, PIP (Peak Inspiratory
Pressure), PEEP (Positive End-Expiratory Pressure), FiO2 (O2 percentage). Parameter changes must be confirmed before
taking effect.

**Displayed measurements**: In each mode we display real-time graphs and sensor readings of several measured values:
pressure, flow, volume.

**Measured vs. commanded values**: All or almost all values have a "commanded" and "measured" value. E.g. ventilator
tries to achieve a certain PIP - this is the "commanded" value - but what it actually achieves is the "measured" value.
If they're too far off, this is bad and usually triggers an alarm.

## Alarms
**Alarm formats:**
* https://docs.google.com/document/d/1bMZfkq_p8dkg7F0ZGVIFhityv-s3GVRMsm_Nd1zNlww/edit#heading=h.yvoratpyj4xl
* Specifies visual flashing frequency / duty cycle
* Specifies auditory pulse frequency / pattern
* Derived from 60601-1-8

**Alarm conditions:**
The main alarm types for v0.2 are **"measured value too far from commanded value"** and **"the device is in a bad
state"** _(eg some tube disconnected, battery low etc)_.

Consider a **"check settings"** alarm. Per
[notes](https://docs.google.com/document/d/1QFFcg2gL3PXXzyqgSBz3XZQm_oobZ2osFuErh492UEA/edit) drs check settings every
3-6 hours, so maybe we should issue a low-prio alarm if settings haven't been confirmed in longer than that.

**Alarm presentation:**
* Alarms are indicated visually and sometimes audially (buzzer, continuous tone).
* A good way to display the "measured value off" alarm is to color the wrong value's background.
* At any moment the GUI must display the highest-priority alarm.
* You should be able to see all active alarms

**Alarm priorities:** low, medium, high (buzzer), and one or two alarms emit a continuous beep.

**Alarm lifecycle:**
* The alarm becomes active when its condition is met.
* Open question: We haven't figured out a general rule for when an alarm stops being active.
* Alarms can be suppressed in several ways: eg. cancel a particular alarm _(we don't know exactly what this means)_,
suspend all audial alarms for 2 minutes, etc. (see `01_Alarms` in Valispace)

**General advice:**
* Alarm fatigue is real. Need to make alarm management simple and conductive towards minimizing fatigue.

**Additional alarms per ISO 80601-2-12:**
(See [Slack thread](https://respiraworks.slack.com/archives/C011UMNUWGZ/p1591500042061800?thread_ts=1591207530.031400&cid=C011UMNUWGZ))
![](https://i.imgur.com/DJqVFaL.png)

## Event logs (not in v0.2)

GUI must include a way to view the event log. It must include:
* All alarm-related events (activated, cancelled etc)
* Confirmed parameter changes and mode changes

## Non-functional requirements
The screen resolution is 1024x600.

**Remote observation/control**
* Things must be **visible from a distance of N meters**. The Covent reqs say from 1 meter, but per
[notes](https://docs.google.com/document/d/1QFFcg2gL3PXXzyqgSBz3XZQm_oobZ2osFuErh492UEA/edit) drs often want to observe
the screen from outside the patient's room, to decrease exposure to infection.
* Nice-to-have: remote control - also to decrease exposure.

**Usability:**
* Errors can kill a patient, and the UI is used by overworked staff. It is very important to design for minimizing
  chance of human error and minimizing dr fatigue.
* Staff in target markets where there's no vents will be inexperienced with vents. UI should help them, eg understand
  the numbers, guide towards configuring things right.
  * Example: in volume-controlled modes, there's an ideal volume of 4-8mL per kg of ideal body weight (based on
    gender/height). Could set default volume by that and color-code if setting outside this range.

Consider the likely working conditions - need a dark theme (optionally light theme too).

The UI should be colorblind-friendly, at least for elements where discerning the color is safety-critical.
