# Ventilation modes

This list is probably not complete. **Please improve.**
**TODO:** link to requirements

## Pressure Assist Mode

In this mode a breath cycle is defined by the control parameters below. The patient can exceed the respiratory rate by
initiating additional breaths, but the breath parameters do not change.

**Control parameters:**
 - PIP. Peak Inspiratory Pressure.
 - PEEP. Positive End-Exhalatory Pressure.
 - FiO2. Fraction of Inspired Oxygen.
 - I-time. Inspiratory (rise + dwell) time.
 - Inspiratory sensitivity.
 - RR. Respiratory Rate.

**Display parameters:**
 - PIP
 - PEEP
 - RR
 - FiO2
 - I:E ratio
 - Tidal Volume (TV)

**Relevant Alarms:**
 * Low tidal volume
 * High tidal volume
 * High RR

## Pressure Support Mode

In this mode the breath cycle shall be defined by the control parameters below. The breath cycle shall be triggered
exclusively by the patient.

**Control parameters:**
 - SIP: Support Inspiratory Pressure. Like PIP, but specified as a support pressure, not a target pressure.
 - PEEP. Positive End-Exhalatory Pressure.
 - FiO2. Fraction of Inspired Oxygen.
 - I-time. Inspiratory (rise + dwell) time.
 - Inspiratory sensitivity

**Display parameters:**
 - PIP
 - PEEP
 - RR
 - FiO2
 - I:E ratio
 - TV

**Relevant Alarms:**
 * Low tidal volume
 * High tidal volume
 * Low RR (apnea)

## HFNC operational mode
In this mode there are no breath cycles.

The operator defines the following control parameters:
 - constant flow rate (up to maximum possible L/min)
 - constant FiO2 (21-100%)

**Display parameters:**
 - Patient pressure
 - flow rate
 - FiO2.

**Relevant alarms:**
 - patient pressure out of bounds (user set, 2-20 cmH2O)

*jlebar*: I don't believe it's possible to control both patient pressure and flow rate.  Trying to clarify this in
this [Slack thread](https://respiraworks.slack.com/archives/C011UMNUWGZ/p1590904911451700?thread_ts=1590895687.450400&cid=C011UMNUWGZ)
and also this [Slack thread](https://respiraworks.slack.com/archives/C011UMNUWGZ/p1590938052463200).
