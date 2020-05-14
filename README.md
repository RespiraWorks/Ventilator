# RespiraWorks Open Source Ventilator

[RespiraWorks](https://respira.works/) is a 501(c)(3) non-profit organization (IRS status pending) with over
100 contributors worldwide working to rapidly develop, manufacture, and deploy a
low-cost and open-source ventilator for communities under-served by the global supply chain.

![Ventilator under construction](open_box.jpg)

**Design Origin**

Most low-cost ventilator designs tackle the 'bridge' - how to keep a patient alive for
6-8 hours of closely monitored care until they can be transitioned to a full ventilator.
However COVID-19 patients require lung-protective adaptive ventilation strategies, and
require ventilator support for days to weeks.

Early in the crisis, we realized that the ventilator designs that could be completed in weeks might not be necessary, and if so, a more feature complete ventilator would be needed. As we started working on that, we realized that this was a market need unmet by the established ventilator market. Despite constant developments in the disease progression, we realized that a true-open source ventilator, with transparant requirements, design history documents, and yet still optimized for low-cost and ease of manufacturing would enable expanded global access to a critical piece of medical equipment.

Our ventilator exists between the short-term ambu-bag actuating designs and the commercial ventilators offered by established manufacturers. It provides essential features such as a graphical user interface, adaptive pressure controls, and fine control of FiO2, oxygen, and PEEP, with CMV, SIMV, PSV and PRVC operating modes. Unlike tranditional ventilators, our design work is being completed as fully open-source design, yet still with an IEC-conformant quality process, and an optimized BOM for sourcing and manufacture worldwide.

Our target is not the US or Europe, but the middle- and low-income parts of the world that will deal with COVID-19 for longer, with less healthcare, and a larger deficit of medical devices. RespiraWorks provides solutions for these communities where existing manufacturers cannot.

## DISCLAIMER

**This is an incomplete, as yet untested design that is not yet intended to be used on patients.**

**This repository contains live documents.
There are duplicate and alternative items for undecided aspects of the physical design.
Components may or may not connect and integrate as intended.
Contents may also be out of date. 
RespiraWorks may, or may not compensate you for parts you purchase.
If you decide to buy or build anything, do so at your own risk.**

# Repository Overview

This repository tracks the physical design of the RespiraWorks open source medical ventilator.
The physical design encompasses everything that is not software run on the device, from the pneumatics, to the mechanical design, bill of materials, and circuit diagram. The software repository can be found below and linked from within the system design. 


## Structure

  * [Requirements](01_Requirements/README.md) -
  describes the requirement architecture, heirarchy, and process. Links to the releases of our system requirements, hardware specifications, and software requirements. 
  * [System design](02_System_Design/README.md) -
  describes the functional implementation and design theory of the system. This document also defines the hierarchy for the subsystems below.
      * [Pneumatic design](02_System_Design/pneumatic_design/README.md)
      * [Electrical design](02_System_Design/electrical_design/README.md)
      * [Software design](https://github.com/RespiraWorks/VentilatorSoftware)
      * **Mechanical design**
  * [Quality documentation](03_Quality_Documentation)
  * **Build instructions**
      * [Alpha build](04_Build_Instructions/alpha/README.md) -
         documentation for building the "Alpha" version of the device which can be used by developers to contribute to the software. Note that this is almost always out of date, by definition.
