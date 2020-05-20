# Design Rationales

## Overview

This file is meant to document the rationales for the design decisions we're making - or things we considered and rejected.

Caution: this document has not been formally reviewed and is a work in progress. Be responsible to 
understand the provenance of documentation.

Also note: right now we are collecting initial information here since it is easier - we may split 
this up to have the different sections live with the different component folders, but we'll figure that out later.

## Guiding Principles

1. **Low cost but robust and safe** - we want to create something that is affordable, but also that can perform the necessary
duties reliably with high confidence over extended periods of time. The goal is to satisfy external specifications for ventilator requirements - but also assess the cost of the requirements and not blindly follow them if it drives big decisions in the system.

2. **Keep It Simple (Stupid)** - remove complexity wherever possible to reduce failure modes (and cost), without sacrificing key functionality (though certain functionality may not be achieved)

3. **Make it something that doctors will use** - it won't necessarily do everything, but we want the ventilator to provide the key needs of doctors. Therefore, we keep doctors extremely in the loop and constantly get feedback from them on what we're doing.

4. **Amenable to developing world supply chains and maitenance** - don't presume things that may not be the case (e.g. 100% O2 source).

## System CONOPS/System Design:

More detailed thoughts on this [here](https://docs.google.com/document/d/1_2f-MABkjC65XBJjWOoxpzJW7co3L-7IdkNQfdwZbGU/edit).

* Targeting pressure control for two reasons: 
    1. We got early feedback that this was most important to doctors - though we have gotten feedback that volume control is also important.
    2. A lower cost pressure controlled system seemed more achievable - and theoretically a good pressure controlled system should be able to do volume control. Currently we are pursuing pressure control and will be evaluating how accurate volume measurements we can get to determine if the design can also support volume control.

* Doctors need fine control of FiO2 -> this drives the need for an oxygen supply, the need to support oxygen mixing and sensing, and the need for a variable state valve on the O2 supply to adjust the flow rate dynamically. #todo Need more info on the ox system design we pick.

* Multiple closed loop (feedback control) systems create complexity by creating interacting dynamics -> we're attempting to only have a single closed loop controlled part of the system at a time and to target one key feature (e.g. pressure with FiO2 achieved across cycles). We're also working to determine which components (e.g., valves, blower) need to be controlled and how in order to figure out what will actually support the system requirements. 

* Anything that will be exposed to high oxygen ratios needs to be safe and ox-clean. This drives a desire to reduce the number of items directly in the flow as much as possible.

* Air valve after the fan/blower with air valve being closed loop controlled and fan being open loop - this is driven by an issue ramping up the fan fast enough to achieve peak pressures without going to high performance fans, which were either too expensive or needed too much power.

* Bypass leg from in front of the blower to the end of the exhale circuit - meant to ensure at least some flow for the blower even when holding pressure in order to limit stall/surge when the system is opened back up.

* The goal is to maintain some flow through the system at all times, both to help the fan and to avoid big surges from completely opening and closing different parts of the circuit.

## Valves

* **Pinch Valve (Custom design)**
    * Low cost, robust design for achieving variable flow
    * Valve is not inline with the flow - which means it is not exposed to high O2 concentrations directly
    * Still evaluating fatigue and lifetime / sealing
    * Originally designed as the exhale valve to give control for maintaining PEEP - under considering to use for both the air inlet and for the oxygen inlet

* **Idle Control Valve (IAV/IAC)**
    * Common automotive part that is cheap and widely available
    * Unclear how these valves perform in high O2 environments
    
* **One way valve (check valve)**
    * Needs to work in an O2 environment
    * Needs to be reliable and relatively low cost
    * Still figuring out the exact one way valve design

## Tubing
* Pick peristaltic tubing so that it can withstand many cycles
* Needs to be O2 safe

## Sensors

* **DP sensors / Venturi**
    * The goal of the venturi design is to measure the flow through the system.
    * Venturi's are nice because they are cheap and yet can provide reliable flow measurements. See [this ticket](https://github.com/RespiraWorks/SystemDesign/issues/9) for info about evaluating the venturi's.
    * Currently we are using the difference between the two flow measurements (one for inhale and one for exhale) to determine the tidal volume delivered to the patient. We are still evaluating how to do this - see [this ticket](https://github.com/RespiraWorks/SystemDesign/issues/57) for more information.
    * Another open questions is manufacturing of the venturis - currently the goal is to design a venturi that can be injection molded.
    
* **Patient Pressure sensors**
    * Needs to be reliable, handle many cycles, and meet the required ranges and accuracies of the requirements (#todo flesh this out more).
    
* **O2 sensor**
    * The O2 sensor needs to be able to measure up to 100% O2
    * Right now we are using a galvanic cell O2 sensor - but those are known to degrade quickly. [This ticket](https://github.com/RespiraWorks/SystemDesign/issues/56) is tracking our discussions on this topic.

* **Mass flow sensor***
    * We have shied away from mass flow sensors because they are very expensive when compared to the DP sensor/venturi design. However, they are accurate and used on other system so they are not completely ruled out.

## Filters
* Filters on inlet/outlet to prevent contaminents from the surrounding environment getting into the device
* Filter on patient circuit to avoid device contamination and spreading particles between patients
* We're currently looking at HEPA filters since they are commonly avaible and cheap - but we're also evaluating other options
* 2 part mold for holding the filters is meant to be easily manufacturable and to allow easy changing of the filters over time

## Blower
* Current blower is not rated to 100% O2, which drives the lack of a blower on the O2 system and a one way valve in front of the blower to keep it separate from the high O2 part of the system
* #todo why did we pick the current blower we have?

## Enclosure
* Looking at custom folder sheet metal so that it is easily manufacturable
* Enclosed to protect the internals and keep out dust
* Has an external fan for cooling the internal electronics

## Humidifier
* Currently planning to use an off the shelf humidifier—see [this ticket](https://github.com/RespiraWorks/SystemDesign/issues/10) for more details.

## PCB/Electronics
* **Microcontroller**
    * See [this ticket](https://github.com/RespiraWorks/SystemDesign/issues/1) for more details on why a STM32 microcontroller was chosen.
* **Power Supply**
    * 12V standard chosen to ensure flexibility of power supplies (e.g. battery or UPC)
    * Open question on internal alarms battery
* #todo - need more here.

## Software
* #todo - should figure out some stuff for this

## GUI
* #todo - should figure out some stuff for this
