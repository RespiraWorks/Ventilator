# Mechanical Design

## General Assembly Approach

The ventilator system is designed such that the internal pneumatic
assembly can be almost entirely completed with the system removed from
the enclosure. This has proved useful during prototyping, because it
allows nearly 360 degree access to the components which enables a
compact arrangement of components that is otherwise difficult to
achieve. To accomplish this, the front panels of the ventilator are
mounted to a light internal structure for assembly of the pneumatic
system. The final step of assembly is to mount this internal structure
into the enclosure, connect the wiring for the enclosure cooling fan,
and seal the enclosure.

The design basis for the functional-critical components of the pneumatic
system are called out above in the [Pneumatic Design](../pneumatic-system) section. When selecting the remaining
components (e.g. plumbing fittings, tubing), the guiding principle was
to select components that are widely available, so as to avoid
constrained supply chains. Therefore, rigid plumbing connections use NPT
standard fittings and connectors, usually in
PVC plastic, and most connections
between components are made using flexible plastic tubing and hose barb
and clamp terminations. The determination to use the NPT standard is
based on reports from our manufacturing partners in India and Guatemala,
which indicated that NPT components are commonly available in both
markets, as well as in the US where most of our prototyping activities
take place. Additionally, these connections are sized such that either
¾" ID tubing or 19mm ID tubing can be used, allowing for flexibility in
use of metric or imperial components according to availability. In all
other cases (e.g. fasteners, stock thicknesses, custom part
dimensioning), metric components are used, due to their broader
availability globally.

## Enclosure

The following requirements drive the design of the ventilator system
enclosure:

-   Must be sterilizable
-   Must be manufacturable with commonly available fabrication equipment
    in the markets we are assessing
-   Enclosed to protect the internals and keep out dust, liquid
    splashes, and fingers
-   Has an external fan (with dust filter) for cooling the internal
    electronics, sufficient to provide cooling even in unconditioned
    outdoor environments in emergency use cases
-   Ruggedized and compatible with the hospital
    environment, including capacity to mount on a rolling cart
-   Must support easy assembly and maintenance of the internal
    components
-   Acoustic damping to minimize auditory disruption of
    the machine


The current concept uses basic sheet metal folding processes, which are
widely available globally. Design details of this assembly can be found
in 05-01 Production Methods for Custom Components.

## Maintenance

Several components have known lifetimes that necessitate periodic
replacement, specifically, filter cartridges, pinch valve tubing, and
the O2 sensor.

Filter cartridges should at minimum be changed between patients. Further
testing will determine whether more regular changes should be carried
out, especially for the filters that separate the patient breathing
circuit from the ventilator device. Because this is a frequent
requirement, the filters are positioned on the front panel of the
ventilator with easily accessible screw-down
connections. The plastic hold-down components can be
quickly removed, wiped down with disinfectant, and a new filter
cartridge can be swapped in.

The pinch valve tubing is designed for peristaltic pumps, and is
therefore rated for a high number of cycles. However, it has a lifetime
shorter than the intended life of the ventilator, likely losing
performance after several weeks of use, and therefore will need to be
periodically replaced. To enable this, the pinch valves are positioned
in the pneumatic assembly such that they are directly accessible by way
of removing the back cover plate of the enclosure. This enables
replacement of the tubing with minimal disassembly of
the device.

Similarly, the oxygen sensor currently specified has a lifetime shorter
than the design life of the ventilator device as a whole. It also is
positioned in the ventilator internal assembly such that it is
accessible for replacement.
