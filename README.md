# RespiraWorks Open Source Ventilator

[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.png?v=103)](open_source.md)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![CircleCI Build Status](https://circleci.com/gh/RespiraWorks/Ventilator.svg?style=shield)](https://circleci.com/gh/RespiraWorks/Ventilator/tree/master)
[![codecov](https://codecov.io/gh/RespiraWorks/Ventilator/branch/master/graph/badge.svg)](https://codecov.io/gh/RespiraWorks/Ventilator)
[![pre-commit](https://img.shields.io/badge/pre--commit-enabled-brightgreen?logo=pre-commit&logoColor=white)](https://github.com/pre-commit/pre-commit)
[![Code style: clang-format](https://img.shields.io/badge/code%20style-clang--format-blue)](https://clang.llvm.org/docs/ClangFormat.html)
[![Code style: black](https://img.shields.io/badge/code%20style-black-000000.svg)](https://github.com/psf/black)
[![Built with: platformio](https://img.shields.io/badge/built%20with-platformio-orange)](https://platformio.org/)
[![GitHub commit activity](https://img.shields.io/github/commit-activity/m/RespiraWorks/Ventilator)](https://github.com/RespiraWorks/Ventilator/pulse)
[![Donate](https://img.shields.io/badge/donate-gofundme-blueviolet)](https://www.gofundme.com/f/RespiraWorks)

This is a common repository for all hardware and software components of the RespiraWorks open source ventilator.

**If you intend to contribute, please read our [wiki](https://github.com/RespiraWorks/Ventilator/wiki).**

[RespiraWorks](https://respira.works/) is a 501(c)(3) non-profit organization with over 200 contributors worldwide
working to rapidly design a low-cost and [open-source](open_source.md) ventilator for communities underserved by the
global supply chain. Our objective is to provide the know-how and technology to enable others to manufacture these
systems and put them in the hands of those who need them most.

| Rendering of upcoming build  | Assembled v0.3 prototype |
|:---:|:---:|
| [![](manufacturing/images/rendering_smaller.jpg)](manufacturing/images/rendering_full.jpg) | [![](manufacturing/images/assembled_smaller.jpg)](manufacturing/images/assembled_full.jpg) |

## Design Goals

We are dedicated to making this ventilator:
* [fully open source (software and hardware)](open_source.md)
* affordable (materials cost under US$2000)
* fully featured for extended clinical use through weaning and recovery
* vetted and filing-ready for regulatory review around the world
* optimized bill of materials to account for constrained (medical) supply chains
* designed for long-term clinical use beyond the pandemic

Most low-cost ventilator designs that have emerged in light of the COVID-19 crisis tackle the 'bridge' problem —
keeping a patient alive for 6-8 hours of closely monitored care until they can be transitioned to a full
ventilator. We are designing that full ventilator, a highly valued medical device critical to care beyond the
current crisis.

Our design shall provide essential features such as an intuitive, localized user interface, adaptive pressure- and
volume-based operating modes, fine control of oxygen concentration, High Flow Nasal Cannula and other advanced features.
What sets us apart from other similarly ambitious projects is our dedication to being fully open source. This repository
embodies that promise.

We are targeting middle- and low-income parts of the world that will deal with COVID-19 for longer, with less access
to healthcare, and a larger deficit of medical devices. We aim to deliver a design which meets the same operational
rigors of current commercial ventilators, while addressing the high barrier to entry for such devices.

The maturity of our design is attested to by our collaboration with [Foundry-M](http://www.foundrym.in/), a startup
in Visakhapatnam, India. We are working closely with our colleagues in India to adapt this design to their local
manufacturing capabilities and regulatory needs.

If you can lend your skills towards achieving this goal, please contact us at
[join-us@respira.works](mailto:join-us@respira.works?subject=[GitHub]%20Join%20request). A list of roles we are looking
to fill is also on our [website](https://respira.works/join-us).

## DISCLAIMERS

**This is an incomplete, as yet untested design that is not yet intended to be used on patients.**

**The repository contains live documents. There may be incomplete or contradictory information about various aspects
of the design. Components may or may not connect and integrate as intended. Contents may also be out of date. If you
decide to buy or build anything, do so at your own risk.**

## Repository Structure

* [**System Requirements**](requirements) -
  describes the requirement architecture, hierarchy, and process. Links to the releases of our system requirements,
  hardware specifications, and software requirements
* [**System Design**](design) -
  describes the functional implementation and design theory of the system
* [**Manufacturing Design**](manufacturing) - describes the physical design of the ventilator and its components.
  Includes CAD models and notes on the manufacturing process
* [**Main Circuit Board**](pcb) - design, materials, assembly instructions for the ventilator main circuit board
* [**Software**](software) - Source code for cycle controller and user interface, debug tools, test scripts, and test data
* [**Quality Assurance**](quality-assurance) - quality assurance strategy, testing plans, data
