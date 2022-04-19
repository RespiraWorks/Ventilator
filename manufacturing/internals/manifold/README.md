# Oxygen sensor and overpressure relief valve manifold

## Design rationale

> **TODO:** Short description of the type of oxygen sensor this fundamentally is.

The need for this manifold was outlined in [#780](https://github.com/RespiraWorks/Ventilator/issues/780).

Since at this point in our pneumatic circuit air and oxygen have been mixed, the expected diameter of tubing is 3/4".

Overpressure release valve has been selected based on latest conclusions in
[#656](https://github.com/RespiraWorks/Ventilator/issues/656).
This valve has 1/4 NPT threads.

Oxygen sensor was selected in [#637](https://github.com/RespiraWorks/Ventilator/issues/637).
The oxygen sensor has M16-1.0 threads for interfacing with a pneumatic system.

The manifold also requires an elbow to route the pneumatic circuit to the underside of the equipment tray, where
it will follow towards the patient tubing interface.

**Fire hazard:** The current design has the overpressure relief valve venting into the ventilator enclosure.
In case of an overpressure event with concentrated oxygen, this might pose a fire hazard. Do not test this
version of the system with oxygen. Future versions of the design will address this issue by redirecting overflow
out of the ventilator enclosure.

## Design implementation

All Inventor part models, assembly models and drawings are within this repo directory.

Recently exported files for 3d printing can be found on our Google Drive at
[Manufacturing/Output files v0.4/3d printing](https://tinyurl.com/2kuwps9h).

The part was designed in Autodesk Inventor, along with assembly to geometrically test interface fit
before manufacture. The design has been tested by 3d printing the manifold in resin.

There are some nuances to modeling threads correctly for 3d printing so that no tapping would be required.
Notes on modeling threads have been published in our
[mechanical engineering wiki page](https://github.com/RespiraWorks/Ventilator/wiki/Mechanical-engineering-notes).

|                  CAD rendering                   |                Drawings                |                   3d Printed                   |
|:------------------------------------------------:|:--------------------------------------:|:----------------------------------------------:|
| ![](images/oxygen_relief_manifold_rendering.jpg) | ![](images/oxygen_relief_manifold.jpg) | ![](images/oxygen_relief_manifold_printed.jpg) |

### Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../purchasing_guidelines.md

| Item   | Quantity | Manufacturer   | Part #                  | Price (USD) | Sources[*][ppg] | Notes                     |
|--------|---------:|----------------|-------------------------|------------:|:---------------:|---------------------------|
| **A1** |        1 | RespiraWorks   | [Custom manifold][a1rw] |        1.00 |       Rw        | Custom manifold           |
| **A2** |        1 | Cables&Sensors | G0-040                  |       76.00 |   [Cs][a2cns]   | Oxygen sensor             |
| **A3** |        1 | Generant       | VRVI-250B-V-0.8         |       54.00 |  [Gn][a3gene]   | Overpressure relief valve |

**Total assembly price:** USD 131.00

[a1rw]:   #design-implementation
[a2cns]:  https://www.cablesandsensors.com/products/compatible-o2-cell-for-hamilton-medical-396008
[a3gene]: https://www.generant.com/product/vent-relief-valve/

## Manufacturing

**Note: Please also see general advice in the [manufacturing methods guide](../../methods).**

|                 Sub-assembly CAD                 |                   Assembled                    |
|:------------------------------------------------:|:----------------------------------------------:|
| ![](images/oxygen_relief_assembly_rendering.jpg) | ![](images/oxygen_relief_assembly_printed.jpg) |

3d printing has been successful using Siraya Fast.

When slicing the STL file, it is advisable to lay the manifold on its side. If printing with threaded adapters
downwards, a membrane-like artifact may form on the M16 oxygen interface. This prints ready to use, and no
hand-tapping or chasing of threads is necessary.

The NPT interface should accept the valve without any tapping and allows for about 4 threads to engage
when using a wrench. This should be acceptable, but if in the future it is determined to not be so, the manifold
sketch has a parametrized tapered hole that may be expanded. Alternatively, a tap may be used.
