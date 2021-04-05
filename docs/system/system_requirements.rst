.. system_requirements:

System Requirements
===================

This document defines the system requirements for |PROJECT_NAME|.

Assembly
^^^^^^^^

.. item:: RW-SYS-12
   :fulfills: SR-17

The device shall filter intake and exhaust to below 0.3 um (HEPA) or finer in order to limit the spread of virus from contaminated patients and to prevent the contamination of the device from patients.

.. item:: RW-SYS-16
   :fulfills: SR-28

The device must be able to work continuously (100% duty cycle) for a minimum of 48 hours and a target of 2 weeks without pause for maintenance, downtime, or repair.The device must operate after 2000 hours of storage time.

.. item:: RW-SYS-54
   :fulfills: SR-15

The system components that come into contact with the patient airway circuit shall be compatible with ISO 18562-1 thru ISO 18562-4.

.. item:: RW-SYS-63
   :fulfills: SR-29

The device shall be have the ability to integrate a hot water humidifier, the humidifiers that meet ISO 80601-2-74:2017 are compatible with 22mm Respiratory Tubing per ISO 5356 standard. (Note that the fitting for standard breathing tubes and humidifiers is the same)

.. item:: RW-SYS-124
   :fulfills:

Ventilator shall have ingress protection level of IP21.IP2x = No penetration into ventilator by solid objects > 12.5mm (e.g. fingers)IPx1 = Dripping water (vertically falling drops) shall have no harmful effect.Ref: www.dsmt.com/resources/ip-rating-chart/

.. item:: RW-SYS-136
   :fulfills: SR-34

Gas inlet shall be clearly labelled with gas type, color, and specified pressure ranges along with direction of flow

.. item:: RW-SYS-137
   :fulfills: SR-27

Ventilator shall operate at altitudes below 10k ft

.. item:: RW-SYS-140
   :fulfills: SR-18

The ventilator construction shall continue to operate while being disinfected by standard cleaning agents (e.g. bleach solution of X%)

.. item:: RW-SYS-141
   :fulfills: SR-16

The ventilator design shall include antiviral filters on the patient inhale and exhale limbs.

.. item:: RW-SYS-144
   :fulfills:

Patient shall to be protected against both asphyxiation and barotrauma in the event of a power, mechanical, or computing failure.

.. item:: RW-SYS-150
   :fulfills:

The hardware shall not compromise basic safety and essential performance while stress tested per IEC 60068-2-78 (damp heat).

.. item:: RW-SYS-155
   :fulfills: SR-33

The device, when fully assembled and tested against IEC 60601-1 section 9.6.2, shall not exceed 80 dBa. Test specifications in 80601-2-12 201.9

.. item:: RW-SYS-156
   :fulfills: SR-30

The assembly shall conform to transit-operable (80601-2-12 201.15.3.5.101.2) shock tests per IEC 60068-2-27:5g for 6ms, half-sine, 3x per axis

.. item:: RW-SYS-157
   :fulfills: SR-30

The assembly shall conform to transit-operable (80601-2-12 201.15.3.5.101.2) vibration tests per IEC 60068-2-24. Basic safety and essential performance shall not be compromised during or after this test.10 Hz - 100 Hz: 0.33 (m/s2)2/Hz, 30 min per axis100 Hz - 500 Hz: 6 dB per octave, 30 min per axis

.. item:: RW-SYS-158
   :fulfills: SR-30

The assembly shall conform to transit-operable (80601-2-12 201.15.3.5.101.2) drop tests per IEC 60068-2-31. Basic safety and essential performance shall not be compromised during or after this test.10 cm vertical drop, twice per specified attitude (TBD)NOTE: as of writing it is assumed the ventilator shall weigh between 1-10 kg


Pneumatics
^^^^^^^^^^

.. item:: RW-SYS-9
   :fulfills:

The device shall prevent pressure from dropping below -4 cm H20 and limit the resistance to 3 cmH20/l/s.

.. item:: RW-SYS-10
   :fulfills: SR-20

The device must, through inherently safe features, prevent patient pressures from exceeding 60 cmH2O in any operating cases or failure modes.

.. item:: RW-SYS-24
   :fulfills: SR-12

The device shall be compatible with oxygen (DISS) up to 440 kPa

.. item:: RW-SYS-92
   :fulfills: SR-7

The device shall be capable of a peak inspiratory flow rate of at least 60L/min for at least 30 seconds.NB This supports a minute volume of at least 30 L.

.. item:: RW-SYS-148
   :fulfills:

The pneumatic circuit shall not leak more than 1-2 L/min at 60 cmH2O


Electronics
^^^^^^^^^^^

.. item:: RW-SYS-50
   :fulfills: SR-35

The device battery shall be sized to provide 1 hour of operation at 12 volts nominal.

.. item:: RW-SYS-57
   :fulfills:

The system shall be compatible with 110V/60Hz and 220V/50HZ AC mains.
RW-SYS-069,The ventilator shall provide non-volatile data storage of at least 1GB.,,HW-039

.. item:: RW-SYS-69
   :fulfills:

The ventilator shall provide non-volatile data storage of at least 1GB.

.. item:: RW-SYS-74
   :fulfills:

The ventilator shall be capable of being run from a 12V power supply

.. item:: RW-SYS-149
   :fulfills: SR-24

The device must detect through physical means the disconnection of the oxygen supply from the o2 input line.

.. item:: RW-SYS-153
   :fulfills: SR-33

The 12 volt power supply shall be compatible or certified against IEC 60601-1

.. item:: RW-SYS-154
   :fulfills: SR-22

The hardware shall feature specific sensors for detecting tube dis/connection

.. item:: RW-SYS-159
   :fulfills:

The ventilator shall provide a Real Time Clock for timestamping all event log data


HMI
^^^

.. item:: RW-SYS-3
   :fulfills: SR-26 SR-4

The system shall control the target peak inspiratory pressure (PIP) from 5-55 cmH2O in increments of 1 cm H2O.The system shall measure and numerically display patient pressure in the range of 0 - 60 cmH2O with resolution 1 cmH2O.The system shall display the measured pressure as a graph with duration of 30 seconds updated at a rate >= 20Hz and units of cmH2O.

.. item:: RW-SYS-5
   :fulfills: SR-26 SR-8

The system shall control the target PEEP from 0 - 20 cm H2O in increments of 1 cmH2O.The system shall measure and numerically display the PEEP in the range of 0 - 20 cmH2O in increments of 1 cmH2O.

.. item:: RW-SYS-20
   :fulfills: SR-26 SR-10

The system shall control the respiratory rate (RR) from 5-30 breaths per minute (bpm) in increments of 1 bpm.The system shall measure and numerically display RR in the range of 0 - 30 bpm with resolution 1 bpm.

.. item:: RW-SYS-21
   :fulfills: SR-9

The device shall control the time ratio of inspiration and expiration (I:E ratio) from 1:1 to 1:4 (Inhale to exhale). Increments of 1: 0.1The system shall measure and numerically display I:E ratio in the range of 1:1 to 1:4 with resolution 1:0.1.

.. item:: RW-SYS-32
   :fulfills: SR-26 SR-11

The system shall control inspiratory tidal volumes ranging from 200-800ml with increments of 10mLThe system shall measure and numerically display the inspired tidal volume in range of 0 - 1000 mL with resolution of 10mLThe system shall display the inspired tidal volume as a graph with duration of 30 seconds updated at a rate >= 20Hz and units of mL.

.. item:: RW-SYS-72
   :fulfills: SR-13

The device shall control the fraction of inspired O2 (FiO2) in supplied air from 0.21 to as close to 1 as possible with a minimum of 0.9, adjustable in increments of 0.05.The system shall measure and numerically display the patient FiO2 in range of 21-100% with resolution of 1%.

.. item:: RW-SYS-87
   :fulfills: SR-26

The system shall display the flow rate provided to the patient as a graph with duration of 30 seconds updated at a rate >= 20Hz and units of liters / minute.

.. item:: RW-SYS-143
   :fulfills: SR-11

Ventilator shall support user entry of patient gender and height. This data shall be used to calculate ideal body weight (IBW).Ventilator shall support the display of tidal volumes as both volume (units mL) and volume per kg IBW (units mL/kg or cc/kg)

.. item:: RW-SYS-147
   :fulfills:

The system shall display pressure/volume loops.


Manager
^^^^^^^

.. item:: RW-SYS-77
   :fulfills:

The system shall support a HFNC operational mode. In this mode there are no breath cycles. The operator defines the following control parameters:constant flow rate (up to maximum possible L/min)constant FiO2 (21-100%).Display parameters: Patient pressure (cmH2O), flow rate (L/min), FiO2 %Relevant alarms: Low/high FiO2 triggers, Low/high pressures

.. item:: RW-SYS-131
   :fulfills:

The ventilator shall have a pre-test mode where the patient inhalation port and patient exhalation port are connected and the operation of the device is checked including the capability to produce the correct pressure and flow rate, the function of valves, and the function of tidal volume measurement.

.. item:: RW-SYS-132
   :fulfills: SR-6 SR-4 SR-2 SR-1

The system shall support Pressure Assist Mode. In this mode a breath cycle is defined by the control parameters below. The patient can exceed the respiratory rate by initiating additional breaths, but the breath parameters do not change.Control parameters:PIP. Peak Inspiratory Pressure.PEEP. Positive End-Exhalatory Pressure.FiO2. Fraction of Inspired Oxygen.I-time. Inspiratory (rise + dwell) time.Inspiratory sensitivity.RR. Respiratory Rate.Display parameters: PIP/PEEP/RR/FiO2/I:E ratio/tidal volume.Relevant Alarms: * Low tidal volume * High tidal volume * High RR

.. item:: RW-SYS-133
   :fulfills: SR-5

The system shall support Pressure Support Mode. In this mode the breath cycle shall be defined by the control parameters below. The breath cycle shall be triggered exclusively by the patient.Control parameters:SIP: Support Inspiratory Pressure. Like PIP, but specified as a support pressure, not a target pressure.PEEP. Positive End-Exhalatory Pressure.FiO2. Fraction of Inspired Oxygen.I-time. Inspiratory (rise + dwell) time.Inspiratory sensitivity (change in flow rate at PEEP)Expiratory sensitivity (change in flow rate at P_plateau)Display parameters: PIP/PEEP/RR/FiO2/I:E ratio/tidal volume.Relevant Alarms: * Low tidal volume * High tidal volume * Low RR (apnea) *

.. item:: RW-SYS-134
   :fulfills:

The system shall support Pressure Control Mode. This mode shall not be featured. It is effectively a subset of Pressure Assist with zero patient inspiratory effort.

.. item:: RW-SYS-146
   :fulfills: SR-3

The system shall support Volume Assist mode. In this mode a breath cycle is defined by the control parameters below. The patient can exceed the respiratory rate by initiating additional breaths, but the breath parameters do not change.Control parameters:VT. Tidal volume.PEEP. Positive End-Exhalatory Pressure.FiO2. Fraction of Inspired Oxygen.I-time. Inspiratory (rise + dwell) time.Inspiratory sensitivity.Expiratory sensitivity.RR. Respiratory Rate.Display parameters: PIP/PEEP/RR/FiO2/I:E ratio/tidal volume.Relevant Alarms: * Low pressure * High pressure * High RR

.. item:: RW-SYS-160
   :fulfills:

The manager shall log at least 5000 events in an event log, including at least the following items:- Ventilation starts and stops- All confirmed Ventilation settings- All confirmed alarm settings- All occurrences and ends of alarms with all their related actions: Inhibitions, cancellations, resets, acknowledge button presses.

.. item:: RW-SYS-161
   :fulfills: SR-11

Measurement of tidal volume must be accurate to within 10mL

.. item:: RW-SYS-163
   :fulfills: SR-20

GUI must compute PIP over each breath

.. item:: RW-SYS-164
   :fulfills: SR-10

GUI must compute the breath rate based on controller sensor readings

.. item:: RW-SYS-165
   :fulfills: SR-9

Algs must be able to detect inspiration and expiration phase and how long it took.

.. item:: RW-SYS-167
   :fulfills: SR-30

GUI needs to automatically invoke a safety action when blower motor temperature is above threshold

.. item:: RW-SYS-168
   :fulfills:

GUI should be able to compute the rise time for each breath

.. item:: RW-SYS-169
   :fulfills:

Controller must keep track of #cycles of wear of various components and issue alarms if they're nearing their lifespan


Controller
^^^^^^^^^^

.. item:: RW-SYS-42
   :fulfills:

The device shall reach the commanded patient pressure within 100 ms of detecting respiratory effort.

.. item:: RW-SYS-89
   :fulfills: SR-5

The minimum detectable inspiratory effort shall be 5 L/min.

.. item:: RW-SYS-94
   :fulfills: SR-20

The pressure overshoot during the pressure fall phase shall not exceed 0.8*PEEP-1 cmH2O

.. item:: RW-SYS-95
   :fulfills: SR-20

The pressure overshoot during the pressure rise phase shall not exceed 2 cmH2O.

.. item:: RW-SYS-142
   :fulfills: SR-31

The device shall ensure control parameters are within 10% of commanded values in each of the test conditions specified in 80601-2-12 table 201.104.

.. item:: RW-SYS-152
   :fulfills:

The device needs to be able to achieve the FiO2 target within 30-60s

.. item:: RW-SYS-162
   :fulfills:

Controller must detect onset of respiratory effort within X ms

.. item:: RW-SYS-166
   :fulfills:

Controller shall report to the Manager the moments when each breath begins (patient-triggered or automatic)


Alarms
^^^^^^

.. item:: RW-SYS-17
   :fulfills: SR-25

The device parameters and alarms shall be readable at a distance of 1 meter.The device alarms shall be audible (60-80 dBa) at a distance of 1m

.. item:: RW-SYS-68
   :fulfills:

The device shall be allow the user to view all alarms if multiple alarms are simultaneously present.

.. item:: RW-SYS-98
   :fulfills:

The system shall have multiple priority alarms indicating the severity of the condition. These modes shall be:High - Likely to cause PATIENT injury or death within seconds to several minutes if uncorrectedMedium - Do not cause PATIENT injury or death until at least several to many minutes have elapsedLow - May cause PATIENT injury only after many minutes to hours have passed

.. item:: RW-SYS-101
   :fulfills:

High severity visual alarms shall be indicated with a red color and a flashing frequency of 1.4-2.8 Hz with a 20-60% duty cycle.High severity audio alarms shall have the following characteristics defined by 60601-1-8:an auditory burst of 10 pulsespulse tone frequency shall be 150-1000Hz.There shall be at least 4 harmonic components of the pulse tone in the range 300-4000 Hz.The effective pulse duration shall be 75-200 ms.See attached table for burst description.

.. item:: RW-SYS-102
   :fulfills:

Low severity alarms shall be indicated with a cyan or yellow color which shall be constantly on.

.. item:: RW-SYS-103
   :fulfills:

Medium severity alarms shall be visually indicated with a yellow color, and a flashing frequency of 0.4-0.8Hz with a 20-60% duty cycleMedium severity alarms shall have the following characteristics defined by 60601-1-8:pulse tone frequency shall be 150-1000Hz.There shall be at least 4 harmonic components of the pulse tone in the range 300-4000 Hz.The effective pulse duration shall be 125-250 ms.See attached table for burst description.

.. item:: RW-SYS-106
   :fulfills:

A press on the AUDIO PAUSE / ALARM PAUSE key shall initiate an auditory alarm signal pause of all active alarms up to 120 seconds. The alarm activation symbol shall be displayed during the inactivation state with the symbol IEC 60417-5576

.. item:: RW-SYS-107
   :fulfills:

The system shall have a means to dismiss alarms which have been previously triggered.

.. item:: RW-SYS-110
   :fulfills: SR-22

The operator can set a threshold for a low patient pressure alarm in the following ranges:Low Patient Pressure alarm: 0-20 cmH2O, 1 cmH2O incrementsWhen the alarm thresholds are triggered the system shall initiate a high priority alarm.

.. item:: RW-SYS-111
   :fulfills: SR-20

The operator can set a threshold for a high patient pressure alarm in the following ranges:High Patient Pressure alarm: 20-60 cmH2O, 1 cmH2O increments. Default: PIP+5 cmH2OThis alarm shall trigger if the patient pressure exceeds the set PIP for three consecutive breath cycles.The audible indicator shall silence upon the first breath that goes below the threshold, but the visual indicator shall remain active until the alarm is cleared.

.. item:: RW-SYS-112
   :fulfills: SR-23

The operator can set thresholds for low and high exhaled tidal volume (VTE) alarms in the following ranges:High VTE alarm: 500-3000 mL, 10 mL incrementsLow VTE alarm: 0-1000 mL, 10 mL incrementsWhen the alarm thresholds are triggered the system shall initiate a medium priority alarm.This alarm does not apply in HFNC mode.

.. item:: RW-SYS-113
   :fulfills: SR-23

The operator can set thresholds for low and high inspired tidal volume (VTI) alarms in the following ranges:High VTI alarm: 500-3000 mL, 10 mL incrementsLow VTI alarm: 0-1000 mL, 10 mL incrementsWhen the alarm thresholds are triggered the system shall initiate a medium priority alarm.This alarm does not apply in HFNC mode.

.. item:: RW-SYS-114
   :fulfills: SR-22

The ventilator shall initiate a high priority alarm when the proximal tube is not connected for all ventilation modes.

.. item:: RW-SYS-115
   :fulfills: SR-23

The ventilator shall initiate a high priority alarm if no significant volume can be delivered to the patient while the target pressure is reached (e.g. patient circuit blockage detected).

.. item:: RW-SYS-116
   :fulfills:

The operator can set a maximum leak threshold as follows:* Leak threshold: 5-60 L/min, 1 L/min incrementsA high priority alarm shall be triggered when leakage above this threshold is observed in the patient circuit.

.. item:: RW-SYS-117
   :fulfills:

The ventilator shall initiate continuous tone alarm when shut down occurs while ventilating. The alarm shall last at least 2 min.

.. item:: RW-SYS-118
   :fulfills: SR-19

The ventilator shall provide a low priority alarm if an AC power source (previously available) is lost or disconnected

.. item:: RW-SYS-119
   :fulfills: SR-19

A ventilator operating on battery shall provide a medium priority alarm when the estimated remaining operating time is 30 minutes or below.

.. item:: RW-SYS-120
   :fulfills: SR-19

A ventilator operating on battery shall provide a high priority alarm when the estimated remaining operating time is 10 minutes or below.

.. item:: RW-SYS-121
   :fulfills:

The ventilator shall provide a high priority alarm if the battery is absent or not available, or if the battery is unchargeable.

.. item:: RW-SYS-122
   :fulfills:

Temperatures of critical system elements shall be monitored. A high priority alarm shall trigger if these elements exceed their recommended temperatures.BlowerElectronicsBatteryTBD: Do we cut back on ventilation as a fallback (preserve the ventilator at risk to patient)

.. item:: RW-SYS-128
   :fulfills: SR-6 SR-21

The operator can set thresholds for low and high respiratory rate (RR) alarms in the following ranges:High RR alarm: 5-30 bpm, 1 bpm incrementsLow RR alarm: 2-30 bpm, 1 bpm incrementsWhen the alarm thresholds are triggered the system shall initiate a medium priority alarm.This alarm does not apply in HFNC mode.

.. item:: RW-SYS-139
   :fulfills: SR-24

The ventilator shall issue a high priority alarm in the event of O2 disconnection

.. item:: RW-SYS-151
   :fulfills: SR-25

The device shall provide an audible alarm system that can:provide volume at 60 dBa from 150 Hz to 4000 Hzproduce volume from an electrical signal within 33 ms


Traceability Matrix
===================
.. item-matrix:: System Requirements to SRS
   :source: RW-SYS
   :target: RW-SW
   :targettitle: Software Requirements
   :sourcetitle: System Requirements
   :type: fulfilled_by
