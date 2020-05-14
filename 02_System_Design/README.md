# System Architecture

-------------

This document contains description of design implementation and closure of issues which
apply at the system level, such as architecture configuration.

## Overview

A ventilator is a device which supports the breathing of a patient who cannot generate sufficient respiratory effort to oxyginate their blood. In order to do that, the device must generate sufficient pressure to overcome the resistance of the lung tissue and the machine itself. It must vary this pressure over time as the lungs full and then deflate. In order to improve oxygenination, ventilators also almost universally apply some backpressure to the longs (PEEP). 

There are dozens of ways to design a ventilator, each has tradeoffs. This document describes the series of design decisions that led to the final design implementation as a means of gaining a finer understandign of the final solution and why it resulted the way that it did. 

The first question is where the ventilator is generating pressure to drive into the lungs. 

Displacement type devices provide a mechanical compression of a fixed volume to drive air into the lungs. These are commonly employed in existing ventilators, and they provide a high degree of volumetric accuracy and precise control of flow. There are two challenges here. First is a question of supply chain, the volume of air which must be delivered into a lung is relatively large, as far as traditional cylinders go. The lubricant and gasket material must be oxygen compatible, and relatively expensivie and sensitive drive mechanisms are required. The designs end up rather heavy and expensive. Bellows-type devices can be helpful in addressing the volume issue, but are very challenging to design for the millions of cycles required, in addition to being difficult to source. 

![Ventilator Design Overview](system_block_diagram.png)

## Subsystems

**TODO:** Introduce each subsystem properly

**TODO:** Why no flowchart?

   * [01_Pneumatic_Design](pneumatic_design/README.md)
   * [02_Electrical_Design](electrical_design/README.md)
   * [03_Software_Design](https://github.com/RespiraWorks/VentilatorSoftware)
   * [04_Mechanical_Design]
