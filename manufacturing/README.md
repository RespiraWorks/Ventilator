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

If you are a pneumatics or mechanical engineer and intend to contribute to our design, please read
our [wiki](https://github.com/RespiraWorks/Ventilator/wiki), by which we mean **all of it**.

If you intend to make any physical components yourself, please become familiar with the
[manufacturing methods guide](methods). And please help maintain it.

## Assembly

| Full assembly rendering    |  Internals   |
|:---:|:---:|
| [![](images/rendering_smaller.jpg)](images/rendering_full.jpg) | [![](internals/images/rendering_smaller.jpg)](internals/images/rendering_full.jpg) |


Ventilator full assembly CAD model: [.iam](ventilator_assembly.iam)

>*Include GA/Sectional Here*

The following is a general outline of the sub-assemblies comprising a full ventilator build. Some smaller
sub-assemblies that do not contain custom components are not listed in this tree, but are linked and explained in the
pages for the various sub-assemblies.

* [Enclosure](enclosure) - rigid frame and enclosure components
* [**Internals**](internals) - full internals assembly, which includes some instances of the following:
  * [Pneumatics tray](internals/tray) - for mounting the main pneumatics assembly
  * [Filter panel](internals/filter_panel) - mechanical support for air filter assemblies
    * [Air filter holders](internals/filter_panel/filter_holder) - custom housings for air filters
  * [Blower](internals/blower) - this provides pressurized air
  * [Air check valve](internals/air_check_valve) - prevents concentrated oxygen back-flow
  * [Pinch valves](internals/pinch_valve) - air influx and exhale control valves
  * [Oxygen regulator](internals/regulator) - oxygen pressure regulator sub-assembly
  * [Proportional solenoid](internals/PSOL) - oxygen control valve
  * [Manifold](internals/manifold) - for oxygen sensor and overpressure relief valve
  * [Oxygen mixer](internals/mixer) - for mixing air and oxygen before delivery to patient
  * [Venturi flow sensors](internals/venturi) - in-house design of affordable flow sensors
  * [Display panel](internals/display_panel) - touchscreen, electronics and oxygen input
    * [Electronics assembly](internals/display_panel/electronics) - encompasses the electrical and computing components
  * Various tubing, manifolds and adapters
* [Wiring guide](wiring) - custom wiring harnesses and connectors
* [Peripherals](peripherals.md) - indispensable components that are strictly outside of the assembly itself

## Additional notes

While this page is not comprehensive yet, some aspects of connecting sensor tubing and electronics may still be
better cover in the (somewhat obsolete) "pizza build" [guide](../quality-assurance/testing/pizza_build).

Some aspects of the production packaged design are detailed in the first section of the (somewhat obsolete)
[assembly video](https://youtu.be/2hdV5CWcLb4).

### India build

Preliminary enclosed build was produced by our partners, Foundry-M in Visakhapatnam, India.

|     |     |
|:---:|:---:|
|![](images/India_build_yellow.jpeg) | ![](images/India_build_open_back.jpeg)  |


### Test equipment

To test a prototype you will need some additional components that are not strictly part of the ventilator itself, such
as test lungs. There is additional information on the [testing](../quality-assurance/testing) page under the quality
assurance section of the repository.
