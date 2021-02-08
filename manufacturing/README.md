# Manufacturing design

This section of the repository describes the physical design of the ventilator and the process of manufacturing the
components and fully assembled units.

## DISCLAIMERS

**This is an incomplete, as yet untested design that is not yet intended to be used on patients.**

**This repository contains live documents. There are duplicate and alternative items for undecided aspects of the
physical design. Components may or may not connect and integrate as intended. Contents may also be out of date. If you
decide to buy or build anything, do so at your own risk.**

**No endorsements:** materials listed in these pages include links to various suppliers. This does not constitute
any endorsement of specific products nor indication that those components are appropriate and reliable for use in
medical devices. These links are simply here for our own convenience, since we all use the repository as our common
collaboration space.

**Acknowledgements:** The following pages contain CAD models for various off-the-shelf components, such as valves,
fittings and screws. These have been provided by various suppliers on their websites and may be intellectual
property of the respective manufacturer. These models come with no guarantees and do **NOT** fall under the
[Apache license](../LICENSE) which generally applies the rest of our project.

## Contributing

If you are a member of the team and intend to build your own prototype, please read the
[purchasing guidelines](purchasing_guidelines.md) first.

If you are 3d-printing any custom components yourself, please check out the [3d printing guidelines](3d_printing).

If you are a pneumatics or mechanical engineer and intend to contribute to our design, please read
our [wiki](https://github.com/RespiraWorks/Ventilator/wiki), by which we mean **all of it**.

## Assembly

| production ventilator assembly    |  back panel removed   |
|:---:|:---:|
| ![](images/enclosed_rendering_face.png) | ![](images/enclosed_rendering_open_back.png)  |


Ventilator full assembly CAD model: [.iam](ventilator_assembly.iam)

>*Include GA/Sectional Here*

Some aspects of the production packaged design are detailed in the first section of the (somewhat obsolete)
[assembly video](https://youtu.be/2hdV5CWcLb4).

### Sub-assemblies

The following is a general outline of the sub-assemblies comprising a full ventilator build. Some smaller
sub-assemblies that do not contain custom components are not listed in this tree, but are linked and explained in the
pages for the various sub-assemblies.

* [Enclosure](enclosure) - rigid frame and enclosure components
* [Display panel](display_panel) - Mechanical support for user interface touchscreen
* [Filter panel](filter_panel) - Mechanical support for air filter assemblies
  * [Air filter holders](filter_panel/filter_holder) - custom housings for air filters, to keep patient safe and system clean
* [**Pneumatic assembly**](pneumatics) - full pneumatic assembly, which includes some instances of the following:
  * [Blower](pneumatics/blower) - this provides pressurized air
  * [Venturi flow sensors](pneumatics/venturi) - in-house design of affordable flow sensors
  * [Pinch valves](pneumatics/pinch_valve) - in-house design of affordable valve for inhale and exhale control
  * [Oxygen mixer](pneumatics/mixer) - for mixing air and oxygen before delivery to patient
  * [Manifold](pneumatics/manifold) - for oxygen sensor and overpressure relief valve
  * [Tubing reducer](pneumatics/reducer_3-4_1-4) for change in tubing diameter in oxygen supply
* [Electrical assembly](electrical) - encompasses the electrical and computing components
  * [Wiring guide](electrical/wiring) - custom wiring harnesses and connectors

## Additional notes

While this page is not comprehensive yet, some aspects of connecting sensor tubing and electronics may still be
better cover in the (somewhat obsolete) "pizza build" [guide](../quality-assurance/testing/pizza_build).

### India build

Preliminary enclosed build was produced by our partners, Foundry-M in Visakhapatnam, India.

|     |     |
|:---:|:---:|
|![](images/India_build_yellow.jpeg) | ![](images/India_build_open_back.jpeg)  |


### Test equipment

To test a prototype you will need some additional components that are not strictly part of the ventilator itself, such
as test lungs.


There is a page dedicated to [Testing](../quality-assurance/testing) under the quality assurance section of the
repository.
