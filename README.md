# RespiraWorks Open Source Ventilator

RespiraWorks is a 501(c)(3) non-profit organization (IRS status pending) with over
100 contributors worldwide working to rapidly develop, manufacture, and deploy a
low-cost and open-source ventilator for communities under-served by the global supply chain.

![Ventilator under construction](open_box.jpg)

Most low-cost ventilator designs tackle the 'bridge' - how to keep a patient alive for
6-8 hours of closely monitored care until they can be transitioned to a full ventilator.
However COVID-19 patients require lung-protective adaptive ventilation strategies, and
require ventilator support for days to weeks.

Our product provides these essential features with an open-source design, an
IEC-conformant quality process, and an optimized BOM for sourcing and manufacture worldwide.
These parts of the world will deal with COVID-19 for longer, with less healthcare, and a
larger deficit of medical devices. RespiraWorks provides solutions for these communities
where existing manufacturers cannot.

This repository tracks the physical design of the RespiraWorks open source medical ventilator.
The physical design encompasses everything that is not software run on the device.

  * [Website](https://respira.works/)
  * [GoFundMe page](https://www.gofundme.com/f/open-source-pandemic-ventilator)

## DISCLAIMER

**This is an incomplete, untested design and is not intended to be used on patients.**

**This repository contains live documents.
There are duplicate and alternative items for undecided aspects of the physical design.
Components may or may not connect and integrate as intended.
Contents may also be out of date. 
RespiraWorks may, or may not compensate you for parts you may purchase.
If you decide to buy or build anything, do so at your own risk.**

## Design Objectives

- Design and build a ventilator that provides the level of care required for effectively treating COVID-19 patients.
- Have the right modes and controls.
- Provide the right feedback for caregivers to set those modes and controls.
- The design must be sourceable, and buildable in a repeatable fashion.
- Keep the unit material cost under $500
- Limit the waste of limited consumables (O2, filters)

## Structure

  * [Requirements](requirements/README.md)
  * [System architecture](system_architecture/README.md)
  * [Pneumatic design](pneumatic_design/README.md)
  * [Electrical design](electrical_design/README.md)
  * [Alpha build BOM + instructions](alpha/README.md)
  * [Software (separate repository)](https://github.com/RespiraWorks/VentilatorSoftware)
