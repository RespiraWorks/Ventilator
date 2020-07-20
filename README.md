# RespiraWorks Open Source Ventilator

[![CircleCI Build Status](https://circleci.com/gh/RespiraWorks/Ventilator.svg?style=shield)](https://circleci.com/gh/RespiraWorks/Ventilator/tree/master)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
[![Code style: clang-format](https://img.shields.io/badge/code%20style-clang--format-blue)](https://clang.llvm.org/docs/ClangFormat.html)
[![Built with: platformio](https://img.shields.io/badge/built%20with-platformio-orange)](https://platformio.org/)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/RespiraWorks/Ventilator)](https://github.com/RespiraWorks/Ventilator/pulse)

This is a common repository for all hardware and software components of the RespiraWorks open source ventilator project.

[RespiraWorks](https://respira.works/) is a 501(c)(3) non-profit organization with over 100 contributors worldwide
working to rapidly develop, manufacture, and deploy a low-cost and open-source ventilator for communities under-served
by the global supply chain.

![Ventilator under construction](manufacturing/alpha-build-instructions/assets/open_box.jpg)

## Design Origin

Most low-cost ventilator designs tackle the 'bridge' - how to keep a patient alive for 6-8 hours of closely monitored
care until they can be transitioned to a full ventilator. However COVID-19 patients require lung-protective adaptive
ventilation strategies, and require ventilator support for days to weeks.

Early in the crisis, we realized that the ventilator designs that could be completed in weeks might not be necessary,
and if so, a more feature complete ventilator would be needed. Despite constant developments in the disease progression,
we realized that a true-open source ventilator, with transparent requirements, design history documents, and yet still
optimized for low-cost and ease of manufacturing would enable expanded global access to a critical piece of medical
equipment.

Our ventilator exists between the short-term ambu-bag actuating designs and the commercial ventilators offered by
established manufacturers. It provides essential features such as a graphical user interface, adaptive pressure
controls, and fine control of FiO2, oxygen, and PEEP, with CMV, SIMV, PSV and PRVC operating modes. Unlike traditional
ventilators, our design work is being completed as fully open-source design, yet still with an IEC-conformant quality
process, and an optimized BOM for sourcing and manufacture worldwide.

Our target is not the US or Europe, but the middle- and low-income parts of the world that will deal with COVID-19 for
longer, with less healthcare, and a larger deficit of medical devices. RespiraWorks provides solutions for these
communities where existing manufacturers cannot.

## DISCLAIMER

**This is an incomplete, as yet untested design that is not yet intended to be used on patients.**

**This repository contains live documents. There are duplicate and alternative items for undecided aspects of the
physical design. Components may or may not connect and integrate as intended. Contents may also be out of date. If you
decide to buy or build anything, do so at your own risk.**

## CoVent Documentation

A set of comprehensive documents denoting the current state of the ventilator design, manufacturing and assembly specs,
was created for the CoVent competition in June 2020. Links to key documents follow. This is more detailed than the
documentation contained in the repository, but note that it is also a snapshot in time as of late June 2020.

* [Design Overview document](design/assets/covent-june-2020-design-overview-document.pdf)
* [Progress Status Report](design/assets/covent-june-2020-progress-status-report.pdf)
* [Assembly instructions](manufacturing/assets/covent-june-2020-assembly-instructions.pdf)
* [Manufacturing for custom components](manufacturing/assets/covent-june-2020-production-methods-for-custom-components.pdf)

## Repository Structure

* [System Requirements](requirements) -
  describes the requirement architecture, hierarchy, and process. Links to the releases of our system requirements, hardware specifications, and software requirements.
* [Ventilator Design](design) -
  describes the functional implementation and design theory of the system. This document also defines the hierarchy for the subsystems below.
    * [Design Rationale](design/design-rationales.md) - An explanation of the rationale that went into specific parts can be found here.
    * [Pneumatic System](design/pneumatic-system)
    * [Electrical System](design/electrical-system)
* [Manufacturing design](manufacturing)
    * [Alpha Build Instructions](manufacturing/alpha-build-instructions) - documentation for building the "Alpha" prototype of the device which can be used by contributors to test and experiment with.
    * [Beta Build Plan](manufacturing/beta-build-instructions) - Details on this page will be added as we progress further into beta build.
* [Printed Circuit Board (PCB)](pcb) - design, materials, assembly instructions, validation code
* [Ventilator Software](software) - Source code for cycle controller and user interface; debug tools, test scripts and test data
* [Research & Development](research-development)
    * [Project: Fan](research-development/project-fan)
    * [Project: IAV Air Control](research-development/project-iav-air-control)
    * [Project: Pinch Valve](research-development/project-pinch-valve)
    * [Project: Venturi](research-development/project-venturi)
    * [Characterization Test Plan](research-development/characterization-test-plan.md)
* [Quality Assurance](quality-assurance)
    * [Testing](quality-assurance/testing)
