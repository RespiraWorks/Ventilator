# Cooling fan

|         Outside         |         Inside         |
|:-----------------------:|:----------------------:|
| ![](images/outside.jpg) | ![](images/inside.jpg) |

> TODO: update pictures

## Design rationale

Computing electronics and stepper motors generate heat. This heat may accumulate in a closed space. Excess heat
can negatively impact the performance of computer processors and may also pose a fire hazard. A cooling fan
is required to drive hot air out of the enclosure.

**WARNING:** This sub-assembly is current **not functional**.

The first version of the mainboard did not have any provisions for powering and controlling a cooling fan.
Thus, installing one in the current design is simply a proof of concept for the mechanical/manufacturing
design.

Some questions yet to be systematically addressed:
* noise levels
* component lifetime
* air outflow vent
* airflow across the most heat-prone components

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

> TODO: update BOM to reflect CAD

[ppg]: ../../purchasing_guidelines.md

| Item   | Quantity | Manufacturer   | Part #       | Price (USD) | Sources[*][ppg] | Notes                     |
|--------|---------:|----------------|--------------|------------:|:---------------:|:--------------------------|
| **A1** |        1 | Noiseblocker   | B0083A0BIA   |       21.86 |   [Z][a1amzn]   | Cooling fan + guard, 80mm |
| **A2** |        1 | IMSurQltyPrise | 80mm-black-4 |        2.99 |   [Z][a2amzn]   | Cooling fan filter, 80mm  |
| **A3** |        4 | McMaster-Carr  | 97763A429    |        1.13 |   [C][a3mcmc]   | M4 screw, 35mm            |
| **A4** |        4 | McMaster-Carr  | 93625A150    |        0.06 |   [C][a4mcmc]   | M4 lock nuts              |
| **A5** |        4 | McMaster-Carr  | 93475A230    |        0.02 |   [C][a5mcmc]   | M4 washers, 9mm OD        |

**Total assembly price:** USD 29.69

[a1amzn]: https://www.amazon.com/Noiseblocker-NB-BlackSilentPro-PC-P-Ultra-Silent/dp/B0083A0BIA
[a2amzn]: https://www.amazon.com/Computer-Filter-Grills-Aluminum-Stainelss/dp/B07LG41F1K
[a3mcmc]: https://www.mcmaster.com/97763A429/
[a4mcmc]: https://www.mcmaster.com/93625A150/
[a5mcmc]: https://www.mcmaster.com/93475A230/

## Assembly

**Note: Please see general advice in the [manufacturing methods guide](../../methods).**

Recently exported drawings for sub-assemblies can be found on our Google Drive at
[Manufacturing/Output files v0.4/Subassemblies](https://tinyurl.com/4d3s7zmw).

All Inventor part models, assembly models and drawings are within this repo directory.
