# Manufacturing design

This section of the repository describes the physical design of the ventilator and the process of manufacturing the
components and fully assembled units.

If you are a member of the team and intend to build your own prototype, please read the
[purchasing guidelines](purchasing_guidelines.md) first.

If you are 3d-printing any custom components yourself, please check out the [3d printing guidelines](3d_printing).

## Development prototypes

Broadly speaking there are two types of ventilator prototypes we are currently testing with:

[**Pizza build**](pizza_build)
A fundamentally functional ventilator that that includes sensors, drivers and pneumatics.
It is not enclosed like the final product and is meant to be operated in an open layout on your table, like a pizza.

[**Enclosed build**](enclosed_build)
This is our work-in-progress towards a "beta" version of the ventilator, which should be rather close to the final
product. This is currently more expensive to make and slower to iterate on.

| Pizza build                           | Enclosed build                                     |
|:-------------------------------------:|:--------------------------------------------------:|
| ![](pizza_build/images/assembled.jpg) | ![](enclosed_build/images/India_build_yellow.jpeg) |

## Sub-assemblies

Components and sub-assemblies have firstly been tested and documented in the context of the pizza build. They should
generally be similar for the enclosed build, but documentation might be lagging.

One of the foundational principles of the ventilator design was to avoid, where possible, custom components. However,
for several parts, it was unavoidable to design a new component if price, availability, and functional performance
requirements could not be achieved with already-available components. Some of the following contain components that
have been custom designed for this application. The manufacturing process for the prototypes is discussed, as well as
plans and features for production at larger volumes, with an approximate scale-point of 1,000 units. For smaller orders
of less than 100 units production, some of the prototype production methods may prove more cost- and time-effective.

For each component, the goal is to document:
* the reasoning for the design
* summary of design history
* files or links to original CAD, derived step files and possibly 3d-printer files with scaffolding
* prototype fabrication process and common problems to watch out for
* tentative plans or implications for fabrication at scale
* link to characterization tests and data
* link to software that interfaces with component
* link to any validation tests that are available to evaluate individual assemblies

Some of the above may not be done for each component. Feel free to help us fill this out.

### Components

* [Brain build](brain) - constitutes just the computing components
* [Blower assembly](blower) - this provides pressurized air
* [Venturi flow sensors](venturi) - in-house design of affordable flow sensors
* [Pinch valve](pinch_valve) - in-house design of affordable valve for inhale and exhale control
* [Air filter assembly](filter_holder) - air filters for keeping patient safe and system clean
* [Oxygen subsystem](oxygen) - pneumatics and sensing components for controlled oxygen delivery

### Characterization Testing

For details on component characterization tests, see the [Characterization Test Plan](characterization-test-plan.md).

## Test equipment

To test a prototype you will need some additional components that are not strictly part of the ventilator itself, such
as test lungs.

There is a page dedicated to [Testing](../quality-assurance/testing) under the quality assurance section of the
repository.
