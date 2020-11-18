# Oxygen subsystem

The oxygen pneumatic sub-system consists of the following components (colorized):

![](images/oxygen_diagram.jpg)

* Tubing and appropriate fittings for diameter transitions
* One-way check valve (passive)
* Oxygen pressure regulator
* Proportional solenoid valve
* Flow sensor
* Air+oxygen mixing chamber (passive)
* Oxygen sensor

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../manufacturing/purchasing_guidelines.md

## Tubing and adapters

### Materials

| Item | Quantity | Manufacturer  | Part #              | Price (USD)  | Sources[*][ppg]| Notes |
| ---- |---------:| ------------- | ------------------- | ------------:|:--------------:| ----- |
| 1    |      5ft | McMaster-Carr | 5894K34             | 12.15 / 5ft  | [C][1mcmc]     | 1/4"ID 3/8"OD Tygon tubing |
| 2    |        5 | McMaster-Carr | 5121K451            | 5.78 / 10    | [C][2mcmc]     | 1/4 NPT x 1/4" ID barbed adapter |
| 3    |        1 | RespiraWorks  |                     |              | [B][3rw]       | 1/4"ID x 3/4"ID reducer |

[1mcmc]: https://www.mcmaster.com/5894K34
[2mcmc]: https://www.mcmaster.com/5121K451
[3rw]: #tubing-reducer

### Tubing reducer

The first few components on the oxygen line will use 1/4"ID tubing, before transitioning to the 3/4" used throughout
the rest of the ventilator assembly. Unfortunately, there is no readily available off-the-shelf reducer for interfacing
these diameters. For this purpose, we have created a custom 3d-printable adapter.

|  CAD rendering    |  3d printed   |
:------------------:|:-----------------:|
![](reducer/reducer_CAD.jpg)  |  ![](reducer/reducer_printed.jpg)  |

The following files are provided:
* [STL](reducer/Reducer%201-4in%20x%203-4in.stl) - exported as high resolution for 3d printing.
* [IPT](reducer/Reducer%201-4in%20x%203-4in.ipt) - AutoDesk Inventor, original CAD with constraints.

In a future iteration, the reducer may be integrated right into the venturi.

## Check valve

We are currently testing with this brass threaded check valve:

https://www.mcmaster.com/7768K22/

It interfaces with a through-wall straight connector that will be on the enclosed ventilator's front panel:

https://www.mcmaster.com/50785K273/

On the opposite end of the fitting, a 1/4"ID tubing adapter (item 3 in [materials list above](#materials)) should be
attached. Together with this adapter, the oxygen input port should appear like this:

|  Front    |  Back   |
:------------------:|:-----------------:|
![](images/through-fitting_1.jpg)  |  ![](images/through-fitting_2.jpg)  |

It is not clear whether the check valve has the correct cracking pressure and other pneumatic properties for our system.

Related issues are being tracked here:
[#653](https://github.com/RespiraWorks/Ventilator/issues/653)
[#646](https://github.com/RespiraWorks/Ventilator/issues/646)

## Oxygen regulator

We are currently testing with

https://www.smcpneumatics.com/IR2010-N02B.html

It requires 2x of 1/4"ID tubing adapter (item 3 in [materials list above](#materials)) to interface with tubing.

## Proportional solenoid valve

We are currently testing with:

https://www.smcpneumatics.com/PVQ31-6G-40-01N.html

It requires 2x of 1/4"ID tubing adapter (item 3 in [materials list above](#materials)) to interface with tubing.

Depending on the version procured (NPT or BSPT), it may also required 2x of the following adapters:

https://www.mcmaster.com/1786N249/

In collaboration with Foundry-M in India, we are also testing with the following locally sourced solenoids:

https://www.uflowvalve.com/solenoid-proportional-flow-control-valve/product/2-2-way-miniature-high-flow-proportional-valve

https://www.uflowvalve.com/solenoid-proportional-flow-control-valve/product/2-2-way-miniature-low-flow-proportional-valve

## Gas flow sensor

For metering oxygen flow metering, we are using the differential-pressure-based venturi sensor.

This may be a "generic" variant for a pizza build, or a one of the custom "oxygen influx" configurations.
Please refer to [this page](../venturi) for details.

## Mixing chamber

The mixing chamber should passively produce a well blended stream of oxygen and ambient air. Research has not been
carried out on the best solution for this. It may very well be that a simple T- or Y-connector is sufficient for the
purpose.

Currently, two options may be employed:
* [22-mm patient tubing tee](../../quality-assurance/testing/test_lung/README.md#tubing-adapter), same as used for test lungs.
* [3/4" why with barbed fittings](https://www.mcmaster.com/5463K62/) from McMaster Carr

Further refinements to this solution are tracked in [Issue #915](https://github.com/RespiraWorks/Ventilator/issues/915).

## Oxygen sensor sub-assembly

We are currently testing our prototypes with the following sensor:

[G0-040](https://www.cablesandsensors.com/products/compatible-o2-cell-for-hamilton-medical-396008)

**TODO:** Short description of the type of sensor this fundamentally is.

The sensor port has M6-1.0 threads for interfacing with the pneumatic system. Since at this point in our pneumatic
circuit air and oxygen have been mixed, the expected diameter of tubing is 3/4". The following manifold has been
designed for connecting the sensor:

| CAD rendering  |  Sub-assembly   |
|:---------------------:|:--------------------:|
![](sensor/sensor_manifold_CAD.jpg) |![](sensor/sensor_assembly.jpg) |

The following files are provided:
* [IPT](sensor/oxygen_sensor_manifold.ipt) - AutoDesk Inventor, original CAD with constraints
* [STL](sensor/oxygen_sensor_manifold.stl) - exported as high resolution

3d printing has been successful using Siraya Fast, and no hand-tapping or chasing of threads was necessary. No
customizations were needed when slicing.
Please see the [3d printing guidelines](../3d_printing) for general advice.

**TODO:** document wiring
