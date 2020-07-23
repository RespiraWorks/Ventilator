# Alarm subsystem

This is a proposed technical design for the alarm subsystem of the ventilator GUI, in response to
[Issue #306](https://github.com/RespiraWorks/Ventilator/issues/306).

Almost all alarms will be computed and displayed in the GUI. An exception is the "GUI unresponsive" buzzer alarm
implemented on the controller. The current document focuses solely on the GUI-computed alarms.

Specifically, the following is in scope for this document:
* Terminology common across the alarm subsystem
* The internal representation of alarm subsystem state in C++
* Principles of how it can be wired up with GUI elements for reading (rendering) and writing (changing parameters,
deactivating alarms)

The following is out of scope:
* The full set of alarms and precise triggering conditions
* The look-and-feel of alarm management in the GUI

## Concepts

ISO-60601-8 ([here](https://drive.google.com/file/d/1wr9WwpoUfvIk1hiBeBqECrA6jUgHHmgL/view?usp=sharing)) defines some
very useful terminology and principles for alarm management, which we will use and slightly extend. Terms below are in
more informal language than in the standard.

**Alarm** - an entity representing the management of detecting a hazardous condition and presenting it to the operator.
Groups together the Condition, Occurrence(s) and Signals.

**Alarm condition** - the predicate determining whether a hazardous condition currently exists, e.g. "average PIP over
last 3 breaths is too high". This predicate is a pure boolean function of alarm parameters and system state history, and
is not affected by alarm suppressions.

**Alarm occurrence** - a continuous time interval in which an alarm condition is met - from the moment the condition
transitioned 0->1, to the moment it transitions back to 0. An alarm can have at most 1 occurrence at the same time.

**Alarm signal** - something the UI presents to the user that lets them know about an alarm occurrence: e.g. changing
the visuals of some UI elements, or buzzing. In the end, the UI presents all alarms via a bunch of signals.

**Signal types**: there are **Audio** and **Visual** signals. They are treated differently in several ways. A given
alarm occurrence can generate several different types of visual signals at the same time.

**Presentation of signals:** The GUI presents the union of active signals from each alarm with some rules:
* There can be only one audio signal played at the same time; highest-priority signal wins.
* If several signals should affect the state of a UI element (e.g. background color of a sensor widget), the
highest-priority one wins.

**Signals vs Occurrences**: Signals are tied to occurrences, with a few gimmicks:
* A signal may in some cases be generated after a **Signal generation delay** from the moment the Occurrence begins.
* There are **Latching** and **Non-Latching** signals. Latching signals are started by the occurrence and ended manually
by a RESET. Non-latching signals automatically stop when the condition stops.

**Suppression** - an action by the operator that makes there be fewer signals going on (eg. because they believe an
alarm occurrence is a false positive, or if they simply have taken note of it).

There are several types of suppressions:
  * **ALARM OFF** and **ALARM PAUSE** - suppresses all signals from an alarm, respectively indefinitely (until the
    suppression is lifted) and for a specified time. Applies to signals from all occurrences of this alarm.
  * **AUDIO OFF** and **AUDIO PAUSE** - suppresses just the audio signals from an alarm. Also applies to all
    occurrences.
  * **GLOBAL AUDIO PAUSE** (not in the standard, but in our system requirements) - suppresses audio signals from all
    alarms for a specified time. Ditto.
  * **ACKNOWLEDGE** and **Timed ACKNOWLEDGE** - suppresses the audio signal from the _current_ occurrence of the alarm.
    In the former case, the suppression ends when the occurrence ends. In the latter case, it ends after a specified
    time (or when the occurrence ends).
  * **RESET** - suppresses a Latching signal when its Condition is not met.

## Architecture

The alarm subsystem manages its internal state **independently for each alarm**, and the state of all alarms is finally
**joined for presentation** at the GUI level _(the presentation of the state of all alarms in the GUI is a complex
function because several alarms may affect the same widget or request to play a sound).

The suppression state associated with an alarm is also maintained and presented in the GUI.

For managing the internal state associated with one alarm, data flows from bottom (sensor readings) to top (signals) via
several layers, where the function of each layer depends on the parameters configurable in the GUI.

### Maintaining the state of an alarm

There is a global, fixed, well-known set of alarms.

**The state of each alarm is maintained as follows:**
* Its priority is known and fixed.
* Its condition function is controlled by GUI parameters and can be changed at any time.
* Keep track of _current occurrence_ by repeatedly evaluating the condition and watching when it starts and stops being
  true
* For each signal that this alarm may in principle generate:
  * Keep track of whether the alarm currently _proposes_ to generate this signal. Usually this is the same as "condition
    is met", but for latching signals it is managed differently.
  * Keep track of the suppression state of this signal (not suppressed; OFF; PAUSED until a certain time; ACKNOWLEDGEDi
    ndefinitely or until a certain time). The GUI may modify this.
  * Be able to tell whether the signal is currently _active_ - i.e. the signal is proposed but not suppressed.

### Binding alarm state to GUI elements

The rendering and actions of GUI elements are **bound to queries and updates** over the respective active signals, e.g.:
* The "list of all active alarms" widget is bound to the query "collect all alarms that have an active signal"
* The background color of the pressure sensor reading widget is bound to the query "red if the PRESSURE TOO HIGH or
  PRESSURE TOO LOW alarm is active, otherwise green"
* A component that rings the buzzer is bound to the query "there is an active audio signal from any alarm AND audio
  alarms are not currently globally suppressed"
* A widget displaying the Leak alarm has an "Acknowledge" button that is visible only if the respective audio signal is
  not currently suppressed, and which, if you press it, sets the suppression state of that signal to ACKNOWLEDGED.

## C++ representation

TBD
