# System Architecture

-------------

This document contains description of ventilator design implementation and closure of issues which
apply at the system level, such as architecture configuration.

## Overview

A ventilator is a device which supports the breathing of a patient who cannot generate
sufficient respiratory effort to oxygenate their blood. In order to do that, the device
must generate sufficient pressure to overcome the resistance of the lung tissue and the
machine itself. It must vary this pressure over time as the lungs fill and then deflate.
In order to improve oxygenation, ventilators also almost universally apply some
back-pressure to the lungs (PEEP).

There are dozens of ways to design a ventilator and each has trade-offs. This document
describes the series of design decisions that led to the final design implementation
as a means of gaining a finer understanding of the final solution and why it resulted the
way that it did.

The first question is where the ventilator is generating pressure to drive into the lungs.

Displacement type devices provide a mechanical compression of a fixed volume to drive air
into the lungs. These are commonly employed in existing ventilators, and they provide a
high degree of volumetric accuracy and precise control of flow. There are two challenges
here. First is a question of supply chain, the volume of air which must be delivered into
a lung is relatively large, as far as traditional cylinders go. The lubricant and gasket
material must be oxygen compatible, and relatively expensive and sensitive drive
mechanisms are required. The designs end up rather heavy and expensive. Bellows-type
devices can be helpful in addressing the volume issue, but are very challenging to design
for the millions of cycles required, in addition to being difficult to source.

![Ventilator Design Overview](assets/functional-block-diagram.png)

[Source](https://docs.google.com/presentation/d/1ye96itSLfdbO9PQT0MSiq6xR2ArqD2MpV1Wj8ORfHeo/edit#slide=id.g7330d84036_1_0)

## Overview Documents

* [Concept of Operations](conop.md)

## Subsystems

**#TODO:** Introduce each subsystem properly

   * [Pneumatic design](pneumatic-system)
   * [Electrical design](electrical-system)
   * **Mechanical design: #TODO**
