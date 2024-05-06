.. hardware_requirements:

Hardware Requirements Specification
===================================

Purpose
-------

This document describe *what* |PROJECT_NAME| hardware must do.

This document is meant to be read and agreed-upon by the project owners and by software developers during design and construction.

.. todo:: Specify the functional requirements, performance requirements, interface requirements, safety requirements, hazard mitigations

Requirements
------------

Assembly
^^^^^^^^

.. item:: RW-HW-6
   :fulfills: RW-SYS-16

All subcomponents shall pass accelerated life testing equivalent to at least 336 hours (2 weeks continuous runtime)

.. item:: RW-HW-8
   :fulfills:

The construction, assembly, and maintenance of the device shall be compatible with metric tooling/set ups.

.. item:: RW-HW-10
   :fulfills: RW-SYS-16

The device must last 2000 hours if the maintenance schedule is followed.

.. item:: RW-HW-23
   :fulfills:

The ventilator shall be operable by a user wearing standard PPE (e.g. latex or nitrile gloves)

.. item:: RW-HW-30
   :fulfills: RW-SYS-63 RW-SYS-77

The patient circuit shall interface with the vent thru 22mm ID tubing.The patient circuit shall interface with nasal cannulas or pillows through a standard 15mm ET tube adapter interface.

.. item:: RW-HW-33
   :fulfills: RW-SYS-12

The assembly shall feature HEPA filters on the blower intake and exhaust pathways

.. item:: RW-HW-43
   :fulfills: RW-SYS-124

Enclosure openings shall not exceed 12.5mm diameter and shall feature no openings on the top face .

.. item:: RW-HW-44
   :fulfills: RW-SYS-124

Ventilation openings on the enclosure shall not be adjacent to electronics or other sensitive components targeted by IEC 61000-4-1 series of test.

.. item:: RW-HW-45
   :fulfills: RW-SYS-137

All hardware components shall operate without adverse effect at altitudes up to 10000 ft.

.. item:: RW-HW-46
   :fulfills: RW-SYS-140

Enclosure does not degrade when cleaned with X% bleach solution

.. item:: RW-HW-47
   :fulfills: RW-SYS-136

Gas inlet ports are clearly labeled per X


Pneumatics
^^^^^^^^^^

.. item:: RW-HW-3
   :fulfills: RW-SYS-92

The blower shall provide a flow rate of at least 60 L/min.(Stretch goal) the blower shall provide a flow rate of at least 130 L/min.

.. item:: RW-HW-11
   :fulfills: RW-SYS-10

The overpressure relief device shall be set to automatically relieve pressure at or above 60 cm H2O.

.. item:: RW-HW-12
   :fulfills: RW-SYS-144 RW-SYS-92

The device must not let the patient asphyxiate if they are attempting to breathe, even if the device fails. This can be achieved by preventing pressure from dropping below -4 cm H20 and limiting the resistance to 3 cmH20/l/s

.. item:: RW-HW-15
   :fulfills: RW-SYS-42

The pneumatic circuit shall reach maximum flow rate (e.g. 60 L/min) in 100 ms.

.. item:: RW-HW-16
   :fulfills: RW-SYS-89

The device must be able to detect that a patient is trying to trigger an inhale or exhale. This requires being able to measure changes in flow rate to a resolution of 5 L/min during inspiratory and expiratory holds.

.. item:: RW-HW-35
   :fulfills: RW-SYS-76 RW-SYS-24

The oxygen input port shall be DISS compatible up to 440 kPa


Electronics
^^^^^^^^^^^

.. item:: RW-HW-27
   :fulfills: RW-SYS-121 RW-SYS-119 RW-SYS-120

The device shall provide a battery monitor for assessing battery presence, charge, and failure.

.. item:: RW-HW-28
   :fulfills: RW-SYS-121 RW-SYS-120 RW-SYS-119 RW-SYS-117 RW-SYS-118

The device shall provide a sensor indicating whether power is coming from AC or battery and whether AC is available

.. item:: RW-HW-32
   :fulfills: RW-SYS-103 RW-SYS-102 RW-SYS-101 RW-SYS-32 RW-SYS-5 RW-SYS-3

The display shall support touch input, 8-bit color and a minimum refresh rate of 30 fps.

.. item:: RW-HW-34
   :fulfills: RW-SYS-16

The electronic non-volatile memory shall be rated for at least 10e6 read/write cycles.

.. item:: RW-HW-36
   :fulfills: RW-SYS-74 RW-SYS-117 RW-SYS-50

The device battery shall have a charge capacity of X Ah at 12 volts nominal.

.. item:: RW-HW-38
   :fulfills: RW-SYS-69

The electronics shall include a real-time clock with independent battery (e.g. coin cell) supply.

.. item:: RW-HW-39
   :fulfills: RW-SYS-69

The electronics shall include non-volatile memory of at least 1GB


Sensors and Actuators
^^^^^^^^^^^^^^^^^^^^^

.. item:: RW-HW-2
   :fulfills: RW-SYS-111 RW-SYS-110 RW-SYS-115 RW-SYS-3 RW-SYS-5

The pressure sensor shall have a resolution of 1 cmH2O and a range of 0 - 60 cmH2O

.. item:: RW-HW-4
   :fulfills: RW-SYS-113 RW-SYS-112 RW-SYS-116 RW-SYS-115 RW-SYS-89 RW-SYS-32

The inhale and exhale flow sensors shall have a resolution of X L/min and a range of Y L/min.

.. item:: RW-HW-21
   :fulfills: RW-SYS-72

The O2 sensor shall measure oxygen fraction from 21% to 100% with an accuracy of 5%.

.. item:: RW-HW-41
   :fulfills: RW-SYS-122

The temperature sensors shall have range -40 to +125 C with a minimum resolution of 1 C

Traceability Matrix
===================
.. item-matrix:: System Requirements to Hardware Requirements
   :target: RW-SYS-
   :source: RW-HW-
   :sourcetitle: Hardware Requirements
   :targettitle: System Requirements
   :type: fulfills
