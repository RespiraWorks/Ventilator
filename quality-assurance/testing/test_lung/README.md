# DIY Test Lung

![Completed](assets/test-lung-4.jpg)

True test lungs are expensive and hard to come by, so we must do with what's available.

This page explains how to adapt off-the-shelf parts into a basic test lung for our ventilator prototype.

To test basic function a ventilator check lung or a breathing bag is sufficient, but can really only work up to 10cmH2O.

To be able to test higher PIP values, you need to reduce the compliance of the lung so you don't just inflate it like a
balloon (or pop it).  These are partial instructions, and there is currently a shortage of test lungs, so what model you
get can vary, so expect to do some tinkering to these instructions if you are going to build you own.

This is completely uncalibrated, so it is not intended for instrumentation, just for functional observation.

## Tubing adapter

A "tee" or "wye" adapter is needed to connect both the inhale and the exhale tubes to the single port on the test lung.
Some are available for sale, and indeed some come included with various test lungs, but it has been hard to find
a consistent source. Thus we have designed a 22mm patient tubing adapter that can be 3d printed. Simple placement
without scaffolding works well and the part has been successfully printed with Siraya Fast.

**Note: Please also see general advice in the [manufacturing methods guide](../../../manufacturing/methods).**

Inventor part models is within this repo directory.

Recently exported STL files for 3d printing can be found on our Google Drive at
[Manufacturing/Output - testing equipment](https://tinyurl.com/bdfnnznf).

![](assets/patient_tubing_adapter.jpg)

## Parts

**Note: If you are a member of the RespiraWorks team, review the [part purchasing guidelines][ppg]
BEFORE purchasing any parts.**

[ppg]: ../../../manufacturing/purchasing_guidelines.md

| Item   | Quantity | Manufacturer    | Part #                 | Price (USD) | Sources[*][ppg] | Notes                                                                                                          |
|--------|---------:|-----------------|------------------------|------------:|-----------------|----------------------------------------------------------------------------------------------------------------|
| **A1** |        1 | South Pacific   | SPB-6110-5PK           |  129.00 / 5 | [Z][a1amzn]     | Test lung                                                                                                      |
| **A2** |        2 | Mars Wellness   | 4332593260             |   16.99 / 2 | [Z][a2amzn]     | CPAP tubing                                                                                                    |
| **A3** |        1 | RespiraWorks    | patient_tubing_adapter |       ~$1.0 | [R][a3rw]       | Y-adapter for 22mm tubing                                                                                      |
| **A4** |       ~1 | BXI or whatever | CA-BX-AP-PF            |   24.95 / 6 | [Z][a4amzn]     | Just some sturdy material. Aluminum, wood, whatevery you have around. 1x 16x12 sheet needed, or 2x 8x12 sheets |
| **A5** |       ~2 | Grifiti         | 41010                  |   10.98 / 5 | [Z][a5amzn]     | just a big rubber band. Several jumbo rubber bands or resistance tubing from an exercise device will work.     |
| **A6** |       ~3 | Alliance Rubber | 08997                  |  6.48  / 18 | [Z][a6aamzn]    | just some more big rubber bands. Good for tuning the response.                                                 |
| **A7** |        3 | NewMainOne      | 4336300183             | 13.98 / 500 | [Z][a7amzn]     | hopefully you have some zip ties at home already but if not, here's a link                                     |
| **A8** |        - | GRIFITI         | 41192                  |       17.98 | [Z][a8amzn]     | more rubber bands, optional or alternative to 3 and 3a                                                         |

[a1amzn]: https://www.amazon.com/FlexLung-Biomedical-Ventilator-Testing-Demonstration/dp/B07B876P9C
[a2amzn]: https://www.amazon.com/gp/product/B01N14F1MV
[a3rw]:   #tubing-adapter
[a4amzn]:  https://www.amazon.com/gp/product/B077Q2HYMW
[a5amzn]:  https://www.amazon.com/Wrapping-Exercise-Chemical-Resistant-Silicone/dp/B0070EEGTK
[a6aamzn]: https://www.amazon.com/gp/product/B0017D16PW
[a7amzn]:  https://www.amazon.com/gp/product/B0777LWBD9
[a8amzn]:  https://www.amazon.com/gp/product/B018WPZCSO

## Assembly Instructions

* For a basic test lung, with no variability in compliance, all you need is to connect 1, 2, and 3:

![Basic test lung](assets/basic_test_lung_assembly.jpg)

* Cut a 16x12 panel of sturdy material into two 8x12 panels. (the sound absorber panels above work fine, or any wood or
aluminum you have around.  Cardboard is probably not sturdy enough.)

![Panels](assets/test-lung-1.jpg)

* Drill 2 holes at one end of each panel to form a zip tie hinge.  Drill two additional holes in one of the panels as a
  securing point for the test lung.  Measure with the rubber band and cut 2 notches at the opposite end of both panels
  to accommodate the rubber band.
* If your lung does not have an attachment loop, make a fold of duct tape attached to the end of the lung and poke a
  hole in it for the zip tie to secure.  Secure the lung to the panel.
* Put the zip ties into the end of the panel for the hinge. Make sure the ties are loose enough to allow the panels to
  fold over completely.

![Fold hinge](assets/test-lung-2.jpg)

* Put the rubber band around the notches.

![Rubber Band](assets/test-lung-3.jpg)

* Mate the tee-junction to the port on the lung to allow attachment of your hoses.

![Completed](assets/test-lung-4.jpg)

## Pricing analysis

26 (A1) + 17 (A2) + 1 (A3) + 4.16 (A4) + 4.39 (A5) + 1.08 (A6) + 0.08 (A7) = 53.71 USD
