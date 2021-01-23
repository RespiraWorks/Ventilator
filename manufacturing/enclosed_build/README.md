# Enclosed build

## DISCLAIMER

**This is an incomplete, as yet untested design that is not yet intended to be used on patients.**

This page contains a running list of notes and planned updates that will be organized and added to as we get further into beta design and build.

## Concept

These components are intended to be assembled into a rugged sheet metal enclosure for the final mechanical design. The
compact version of the ventilator assembly and packaging strategy has already gone through several iterations,
including an Alpha Build device which proved out the use of a detachable internal structure for ease of assembly.

The next iteration of this build will demonstrate the packaging in the production housing, which has been designed but
not yet constructed. Based on lessons learned from the Alpha Build prototyping process, the ventilator will use a
detachable internal assembly, to which all pneumatic components can be mounted and connected. This allows for 360
degree accessibility to components during initial assembly. When complete, this internal assembly is bolted into the
enclosure, and the enclosure is screwed shut. The top-rear positioning of the removable enclosure lid provides
access to components that will need routine maintenance.

| production ventilator assembly    |  back panel removed   |
|:---:|:---:|
| ![](images/enclosed_rendering_face.png) | ![](images/enclosed_rendering_open_back.png)  |

>*Include GA/Sectional Here*

Further aspects of the production packaged design are detailed in the first section of the\
[Assembly Video](https://youtu.be/2hdV5CWcLb4).

## India build

Preliminary enclosed build was produced by our partners, Foundry-M in Visakhapatnam, India.

|     |     |
|:---:|:---:|
|![](images/India_build_yellow.jpeg) | ![](images/India_build_open_back.jpeg)  |

## Outline

**TODO:** electrical assembly

* [Enclosure assembly](enclosure) - rigid frame and enclosure components
* [Display assembly](display_panel) - Mechanical support for user interface touchscreen
* [Filter panel](filter_panel) - Mechanical support for air filter assemblies
  * [Air filter assembly](filter_panel/filter_holder) - air filters for keeping patient safe and system clean
* [**Pneumatic assembly**](pneumatics) - full pneumatic assembly, which includes some instances of the following:
  * [Blower assembly](pneumatics/blower) - this provides pressurized air
  * [Venturi flow sensors](pneumatics/venturi) - in-house design of affordable flow sensors
  * [Pinch valve](pneumatics/pinch_valve) - in-house design of affordable valve for inhale and exhale control
  * [Oxygen mixer](pneumatics/mixer) - for mixing air and oxygen before delivery to patient
  * [Manifold](pneumatics/manifold) - for oxygen sensor and overpressure relief valve
  * [Tubing reducer](pneumatics/reducer_3-4_1-4) for change in tubing diameter in oxygen supply

Here is a preliminary (obsolete) CAD model for viewing: [autodesk360 link](https://a360.co/2PTyRzh)
