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

3. **Make it something that doctors will use** - it won't necessarily do everything, but we want the ventilator to provide the key needs of doctors. Therefore, we must keep doctors extremely in the loop and constantly get feedback from them on what we're doing.

4. **Amenable to developing world supply chains and maitenance** - don't presume that the medical context we might be used to is the same in our target market. The only way to get this information is contact with doctors and workers familiar with the facts on the ground. 

5. **Open source** - we want this project to be something that anyone who wants can build off of and leverage. Therefore, all designs, rationales behind those designs, requirements, processes, etc. will be documented in a common repo that everyone has access to. **#todo** - go through and make sure all of the links in here are in git so that people can access them (some slack links exist temporarily before the information is fully resolved and moved to git)

## System CONOPS/System Design

More detailed thoughts on this [here](conop.md).

* Targeting **pressure control** for two reasons: 
    1. We got early feedback that this was most important to doctors - though we have gotten feedback that volume control is also important.
    2. A lower cost pressure controlled system seemed more achievable - and theoretically a good pressure controlled system should be able to do volume control. Currently we are pursuing pressure control and will be evaluating how accurate volume measurements we can get to determine if the design can also support volume control.

* **Doctors need fine control of FiO2** -> this drives the need for an oxygen supply, the need to support oxygen mixing and sensing, and the need for a variable state valve on the O2 supply to adjust the flow rate dynamically. **#todo** Need more info on the ox system design we pick.

* **Multiple closed loop (feedback control) systems create complexity by creating interacting dynamics** -> we're attempting to only have a single closed loop controlled part of the system at a time and to target one key feature (e.g. pressure with FiO2 achieved across cycles). We're also working to determine which components (e.g., valves, blower) need to be controlled and how in order to figure out what will actually support the system requirements. 

* Anything that will be exposed to high oxygen contrations and pressures needs to be safe and ox-clean. This drives a desire to **reduce the number of items directly in the flow as much as possible**, especially for high-pressure (greater than 1 bar O2)

* **Air valve after the fan/blower with air valve being closed loop controlled and fan being open loop** -> The ventilator requires a high rise time of the pressure when transition from PEEP to PIP. A motor which can change the fan inertia this quickly against increasing pressure ends up being the fan for some high-performance ventilators, or expensive. We have solved this by the use of a proportional valve after the fan. The fan runs at constant power, and the pinch valve is used to modelate the flow. For refernce, one fan we got quoted was $500 and we were never able to get one delivered. A fan capable of meeting the pressure spec + an air valve capable of the rise time spec was less than $75.

* The goal is to maintain **some bias flow through the system at all times**, both to help the fan and to avoid big surges from completely opening and closing different parts of the circuit. This also helps ensure a 'one way' flow of particles and aids with sterilization.

* **Trigger breath transitions off of changes in flow instead of pressure** -> Because the ventilator is pressure controlled, it adapts to small changes in pressure by adjusting the flow rate (by adjusting the valve states) in order to maintain pressure. Therefore, changes in flow rate are a better sign that the patient is attempting to inhale or exhale. #todo need to confirm this/flesh this out as we go

* **Anti-asphyxiation design** (**#todo** need to confirm all this)
    * **Pneumatic** If the ventilator loses power, it is important that the patient is able to breathe and is not given dangerous amounts of pressure/flow. The oxygen system is fed by an external pressure source that the ventilator doesn't control; therefore, in the loss of power it is important to shut off flow from the oxygen system to keep the patient safe. We accomplish this by using a normally closed solenoid on the oxygen limb. The air/blower valve and the exhale valve are made to be normally open so that those paths are unobstructed to allow the patient to breathe.
    * **Electrical** To support the air/blower valves and exhale valves being normally open, the Cycle Controller watchdog timer is connected to the stepper motor drivers, which will default them to an unpowered state in the event of a Cycle Controller computing failure, causing the valves to snap open.
    

* **Use of stepper motors** - **#todo** need to fill this out

* **Metric vs imperial** - **#todo** fill this out when we make a decision

## Valves

* **Pinch Valve (Custom design)**
    * Low cost, robust design for achieving variable flow based on peristaltic tubing. 
    * The valve mechanical components are not exposed to the flow, which means it is not exposed to high O2 concentrations directly.
    * Normally open, which is important for anti-asphyxiation in the case of power loss
    * Still evaluating fatigue and lifetime / sealing.
    * Originally designed as the exhale valve to give control for maintaining PEEP - under considering to use for both the air inlet and for the oxygen inlet
    * For more informatin about the design of this valve, see [here](../research-development/project-pinch-valve)

* **Idle Air Control Valve (IAC)**
    * Common automotive part that is cheap and widely available. 
    * A linear stepper motor drives a pintle up and down to create a variable orifice.
    * Unclear how these valves perform in high O2 environments or if they could be sufficiently rated/cleaned.
    * Significantly cheaper, more robust, and more widely avaiable than the pinch valve.
    
* **Proportional Solenoid (PSOL)**
    * Relatively uncommon part but used in traditional ventilators. 
    * Provides precise, reliable control of high-pressure gasses. 
    * May be cost effective but requires a high-pressure source of oxygen (can not use low pressure sources like oxygen conentrators without a booster pump).
    * O2 cleaned PSOLs are relatively hard to find, though are already in use on ventilators. We may be able to clean automotive PSOLs.    
    * Normally closed, which is important for anti-asphyxiation. **#todo** need to confirm this
    
* **One way valve (check valve)**
    * Needs to work in an O2 environment
    * Needs to be reliable and relatively low cost
    * Still figuring out the exact one way valve design
    * Two possibilities are a normal umbrella seat check valve, problem is these have a high cracking pressure. This could be OK on the intake side where the fan can generate this but would be a problem on exhale. 
    * duckbill valves have also been explored, but these can chatter at low flow rates. 
    * Could possibly modify a normal check valve by cutting the spring down or out. 

* **Relief Valve**
    * **#todo** when we figure this out

## Tubing
* Pick peristaltic tubing in the pinch valves so that it can withstand many cycles
* Otherwise any biocompatible tubing is fine, gneerally Tygon for most of the testing. 
* Needs to be O2 safe
* 3/4 inch standard allows relatively cheap fittings. Need to transition to metric for int'l.

## Sensors

* **DP sensors / Venturi**
    * The goal of the venturi design is to measure the flow through the system at a limited pressure drop and with a large signal.
    * Venturi's are nice because they are cheap and yet can provide reliable flow measurements. See [this ticket](https://github.com/RespiraWorks/SystemDesign/issues/9) for info about evaluating the venturi's.
    * Currently we are using the difference between the two flow measurements (one for inhale and one for exhale) to determine the tidal volume delivered to the patient. We are still evaluating how to do this - see [this ticket](https://github.com/RespiraWorks/SystemDesign/issues/57) for more information.
    * Another open questions is manufacturing of the venturis - currently the goal is to design a venturi that can be injection molded.
    * For more informatni about the venturi design, see [here](../research-development/project-venturi).
    
* **Patient Pressure sensors**
    * Needs to be reliable, handle many cycles, and meet the required ranges and accuracies of the requirements (#todo flesh this out more).
    * Currently only one sensor is used, but maybe a second one is required for reliability.
    
* **O2 sensor**
    * The O2 sensor needs to be able to measure up to 100% O2
    * Right now we are using a galvanic cell O2 sensor - but those are known to degrade quickly. [This ticket](https://github.com/RespiraWorks/SystemDesign/issues/56) is tracking our discussions on this topic.

* **Mass flow sensor***
    * We have shied away from mass flow sensors because they are very expensive when compared to the DP sensor/venturi design. However, they are accurate and used on other system so they are not completely ruled out.

## Filters
* Filters on inlet/outlet to prevent contaminents from the surrounding environment getting into the device
* Filter on patient circuit to avoid device contamination and spreading particles between patients
* We're currently looking at HEPA filters since they are commonly avaible and cheap - but we're also evaluating other options. HEPA filters are appropriate because they are designed to filter small particles. [More info on HEPA filters](https://ntrs.nasa.gov/archive/nasa/casi.ntrs.nasa.gov/20170005166.pdf)
* 2 part mold for holding the filters is meant to be easily manufacturable and to allow easy changing of the filters over time
* Filters are placed physically on the outside of the box so they can be replaced without the box being opened

## Blower
* Current blower is not rated to 100% O2, which drives the lack of a blower on the O2 system and a one way valve in front of the blower to keep it separate from the high O2 part of the system
* We have evaluated a large number of blowers. When balancing supply chain, ubiquity of specification, and future avaialability, we settled on this one. However, there is an issue with the pressure spec for the fan. If we needed to reach 60 cm H2O, this fan might be under powered. We are currently leaning to not support those high pressures 
* **#todo:** link to the fan we picked

## Enclosure
* Must be sterilizable
* Must be manufacturable with commonly available fabrication equipment in the markets we are assessing (currently targeting custom folded sheet metal)
* Enclosed to protect the internals and keep out dust
* Has an external fan for cooling the internal electronics
* Ruggedized and compatible with the hospital environment
* Must support easy assembly and maitenance of the various components

## Humidifier
* Currently planning to use an off the shelf external humidifier—see [this ticket](https://github.com/RespiraWorks/SystemDesign/issues/10) for more details.

## PCB/Electronics

* **SRM32 Microcontroller For the Cycle Controller**
    * More powerful than the simpler arduino and Raspberry pis, but still cheap and widely available
    * Used in other medical devices
    * Comes with more powerful/extensive debugging tools
    * Familiar to many of the members of our team
    * See [this ticket](https://github.com/RespiraWorks/SystemDesign/issues/1) for more details on why a STM32 microcontroller was chosen.
* **Rasperry PI and Rasperry PI Touch Screen for GUI**
    * Cheap and widely available
    * Existing libraries for touch screen interactions that can be leveraged
    * Has lower performance requirements than the cycle controller
* **Power Supply**
    * 12V standard chosen to ensure flexibility of power supplies (e.g. battery or UPC)
    * Open question on internal alarms battery
* **#todo** - need more here.
* **#todo** - add diagrams from design reviews and such

## Software

* **#todo** - add diagrams from design reviews and such
### Separation of UI Controller and Cycle Controller
* To maximize safety, it is important to minimize the amount of software that is immediately hazardous to the patient (can cause harm in <1s, making human intervention useless), and expose any hazardous software to high levels of verification and testing. By moving the UI code to a separate porcess, it reduces risk and saves time because the complicated UI code doesn't have to be exposed to the same verification and testing as the cycle controller.

* NOTE: Neither of the controllers are a complete backup for the other - this is not a redundant system.  If either fails in a way that is not fixed through the watchdog reset, an immediate intervention will be needed to keep the patient alive.  The objective in the case of either failure is to sound the alarm so that the intervention can happen - and to design the system such that in the event of a failure there is time to make an intervention.  Making a truly redundant system will add cost and time, and so has been deferred for now.

* **Watchdog Timers**
    * In the event of a computing failure in either the UI Computer or the Cycle Controller, a watchdog timer monitors each and will reset the offending computer, which should then be able to regain control of the valves. 
    * **#todo** it will need to be confirmed in the final design that the UI Computer cannot command any modes that will cause the Cycle Controller to hold these valves closed. 

### GUI / UI Controller
* **#todo** - needs more
* Items that fall within the UI Controller:
    * Cycle Controller Watchdog/Alarm
    * User Parameter Setting
    * Ventilation Mode Setting
    * Sensor Data Reporting
    * Respiratory Cycle Plot Display
    * Alarm Information Display
    * Alarm-specific alert sounds

### Cycle Controller
* **#todo** - needs more
* Items that fall within the cycle controller:
    * UI Computer Watchdog/Alarm
    * Closed-Loop Valve Control
    * Open-Loop Blower/Valve Control
    * Respiratory cycle Control
    * Sensor Data Reading
    * Sensor Failure Alarm
    * Overpressure Alarm
    * Respiration Rate Alarm
    * Broken Loop Alarm
