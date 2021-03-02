# Manufacturing design

| Full assembly rendering    |  Internals   |
|:---:|:---:|
| [![](images/rendering_smaller.jpg)](images/rendering_full.jpg) | [![](internals/images/rendering_smaller.jpg)](internals/images/rendering_full.jpg) |


This section of the repository describes the physical design of the ventilator and the process of manufacturing the
components and fully assembled units.

## DISCLAIMERS

**This is an incomplete, as yet untested design that is not yet intended to be used on patients.**

**The repository contains live documents. All documentation here is on a "best effort" but no guarantee basis.
Information is incomplete. Pictures, renderings, parts lists and fabrication instructions may contradict each other.
Components may or may not connect and integrate as intended. Contents may also be out of date. If you
decide to buy or build anything, do so at your own risk.**

**FIRE HAZARD** The below design is not fundamentally safe to be used or tested with oxygen or other volatile gases.
Parts of the pneumatic design address the need for controlled pressurized oxygen supply. These aspects of the design
have only been tested with pressurized air as proof of concept. Listed components are not safe to use with
concentrated oxygen. Oxygen is a volatile substance and at high concentration is an extreme fire hazard. Do not
do anything stupid.

**No endorsements.** Materials listed in these pages include links to various suppliers. This does not constitute
any endorsement of specific products nor indication that those components are appropriate and reliable for use in
medical devices. These links are simply here for our own convenience, since we all use the repository as our common
collaboration space.

**Acknowledgements:** The following pages contain CAD models for various off-the-shelf components, such as valves,
fittings and screws. These have been provided by various suppliers on their websites and may be intellectual
property of the respective manufacturer. These models come with no guarantees and do **NOT** fall under the
[Apache license](../LICENSE) which generally applies the rest of our project.

## Contributing

If you are a pneumatics or mechanical engineer and intend to contribute to our design, please read
our [wiki](https://github.com/RespiraWorks/Ventilator/wiki), by which we mean **all of it**.

If you intend to make any physical components yourself, please become familiar with the
[manufacturing methods guide](methods). And please help maintain it.

## Design principles

### Logical structure

The following pages and CAD models are structured according to the sub-assembly hierarchy that should be followed
in building the ventilator. In general, each subassembly CAD model resides in its own directory and should be
accompanied by a README file, any renderings, drawings or photos of the physical implementation, exported
files for 3d printing or whatever other manufacturing process. The readme file is there to explain the design rationale
and any non-trivial manufacturing or quality control steps necessary to produce the part.

There are directories for each custom-designed component or custom assembly involving off-the-shelf components
arranged in a special way, or some mix thereof. Off-the-shelf CAD models all reside in the [third_party](third_party)
directory with some categorization. If a component requires custom manufacturing, you can safely assume that the
necessary files will be in its own directory.

This is a on a best effort basis, so some hardware may reside with CAD assemblies of a scope higher or lower from where
they are listed in the parts tables. You should always consult both CAD and verbal documentation before attempting to
build anything. It is highly advised that you have your parts and measure them to confirm expected dimensions match,
as models may be imprecise.

### Metal frame and enclosure

The current version of the ventilator assembly is meant to demonstrate its packaging in "production" housing. Based
on lessons learned from previous iterations, the ventilator uses a detachable internal assembly, to which all pneumatic
components can be mounted and connected. This allows for 360 degree accessibility to components during initial
assembly. When complete, this internal assembly is bolted into the enclosure, and the enclosure is screwed shut. The
top-rear positioning of the removable enclosure lid provides access to components that will need routine maintenance.

We chose a sheet metal construction because:
* it is cheap and quick - sheet metal is cheap to fabricate. Unlike plastic parts, it doesn't require any expensive
  molds. This is great for prototyping stages and should also be an accessible manufacture process in many countries.
* strength and durability - stainless steel and other highly durable materials can be used. The frame and enclosure
  should be rigid and withstand mechanical stresses. The simple two-piece enclosure should prove to be quite rugged.
* staged assembly - we wanted the ability to assemble all internals outside the enclosure. The design is built around
  a strong bracket upon which all pneumatics and electrical equipment is installed. This makes assembly and maintenance
  easier. Also useful when prototyping and making design modifications.

### Choice of hardware

* **metric** - preference has been given to metric hardware. Metric standard hardware is available worldwide, including
  countries that prefer imperial standard, but generally not vice versa.
* **hex drive** - screws with hex drive interface have been chosen. Phillips drive screws are prone to cam-out. Torx
  and other interfaces may be too exotic and supply chains may be limited in some parts of the world. Hex drive is
  practical and ubiquitous enough.
* **button head** - preference is given to button head screws. These are smaller and more flush with the surface than
  pan head or socket head screws and thus less likely to catch on clothing or other materials, and are also the least
  visually obtrusive.
* **predominantly M4** - where possible, we are standardizing to M4 mounting components. This is to minimize the variety
  of components to keep in inventory.

## Parts

If you are a member of the team and intend to build your own prototype, please read the
[purchasing guidelines](purchasing_guidelines.md) first.

[ppg]: purchasing_guidelines.md

| Item  | Quantity | Manufacturer  | Part #                       | Price (USD) | Sources[*][ppg]| Notes |
| ----- |---------:| ------------- | ---------------------------- | -----------:|:--------------:|:------|
|**A1** | 1        | RespiraWorks  | [Wiring harnesses][a1rw]     | (0.0)       | Rw             | Wiring harnesses: these get integrated into the below assemblies |
|**A2** | 1        | RespiraWorks  | [Internals][a2rw]            | 1327.82     | Rw             | Internals assembly - includes most pneumatics, electrical and computing components |
|**A3** | 1        | RespiraWorks  | [Enclosure][a3rw]            | 430.90      | Rw             | Metal enclosure that contains and protects the ventilator internals |
|**A4** | 1        | RespiraWorks  | [Cooling fan assembly][a4rw] | 29.69       | Rw             | For removing heat generated by electrical equipment |
|**A5** | 1        | RespiraWorks  | [Closure panel][a5rw]        | 119.01      | Rw             | Completes the enclosure and provides power input interface |
|**A6** | 12       | McMaster-Carr | 92095A188                    | 0.10        | [C][a6mcmc]    | M4 screws, 6mm: main bracket and closure fastening |
|**A7** | 1        | RespiraWorks  | [Peripherals][a7rw]          | 72.98       | Rw             | Indispensable components that are strictly outside of the assembly itself |

**Total price estimate:** USD 1981.60

[a1rw]:   wiring
[a2rw]:   internals
[a3rw]:   enclosure
[a4rw]:   cooling_fan
[a5rw]:   closure_panel
[a6mcmc]: https://www.mcmaster.com/92095A188/
[a7rw]:   peripherals.md

## General assembly

Ventilator full assembly CAD model: [.iam](ventilator_assembly.iam)

>**TODO:** drawings

### Pre-fabrication

A number of custom components may have to be pre-fabricated by contract manufacturer:

* Main board / PCB and daughter board (in [electronics assembly](internals/display_panel/electronics))
* Sheet metal components: [equipment bracket](internals/tray), [enclosure](enclosure) and [closure panel](closure_panel)
* Acrylic components: [pneumatics tray](internals/tray), [display panel](internals/display_panel) and
  [filter panel](internals/filter_panel).

There are also various 3d printed components throughout the sub-assemblies. They will not be listed here explicitly.
Familiarize yourself with the entire hierarchy of assemblies and print what is needed ahead of time.

### Order of assembly:

* Prepare all custom wiring components `[A1]`
* Complete the internals assembly with all its sub-assemblies `[A2]`
* Insert internals assembly into enclosure `[A3]` and fasten it to the sides with 6x M4 screws `[A6]`
* Install cooling fan `[A4]` (optional)
* Attach main power harness to the PCB and install the closure panel `[A5]` using the remaining screws `[A6]`
* Attach power cables and/or human input devices listed in [peripherals page](peripherals.md)

Once the assembly is complete, you may proceed to the [software](../software) section of the repository
and learn how to get its brain working.

## Additional notes

While this page is not comprehensive yet, some aspects of connecting sensor tubing and electronics may still be
better covered in the (somewhat obsolete) "pizza build" [guide](../quality-assurance/testing/pizza_build).

Some aspects of the production packaged design are detailed in the first section of the (somewhat obsolete)
[assembly video](https://youtu.be/2hdV5CWcLb4).

## Living prototypes

There are currently at least 3 prototypes being worked on in various locations around the world:

|     |     |     |
|:---:|:---:|:---:|
| Germantown Maryland  | [![](images/assembled_smaller.jpg)](images/assembled_full.jpg) | [![](internals/images/top_smaller.jpg)](internals/images/top_smaller.jpg) |
| Sunnyvale California | [![](images/California_front_smaller.jpg)](images/California_front.jpg) | [![](images/California_back_smaller.jpg)](images/California_back.jpg) |
| Visakhapatnam India  | [![](images/India_full_smaller.jpg)](images/India_full.jpg) | [![](images/India_back_smaller.jpg)](images/India_back.jpg) |

### Test equipment

To test a prototype you will need some additional components that are not strictly part of the ventilator itself, such
as test lungs. There is additional information on the [testing](../quality-assurance/testing) page under the quality
assurance section of the repository.
