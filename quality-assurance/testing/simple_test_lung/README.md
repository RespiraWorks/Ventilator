# Simple test lung

True test lungs are expensive and hard to come by, so we must do with what's available.

This page explains how to adapt off-the-shelf parts into a basic test lung for our ventilator prototype.

Note: You can optionally add-on to this simple test lung to make a reduced-compliance test lung; [instructions for that are here](../compliance_test_lung).

## Parts List (BOM)

**Note: If you are a member of the RespiraWorks team, review the
[part purchasing guidelines](../../../manufacturing/README.md#part-purchasing-guidelines)
BEFORE purchasing any parts.**

### Purchasing Source Key

* **Z** = Amazon
* **R** = Made by RespiraWorks

### Parts

| Item | Quantity | Manufacturer  | Part #                 | Price (USD)  | Sources        | Notes |
| ---- |---------:| ------------- | ---------------------- | ------------:|----------------| ----- |
| 1    |        1 | South Pacific | SPB-6110-5PK           | 129.00 / 5   | [Z][1amzn]     | Test lung |
| 2    |        2 | Mars Wellness | 4332593260             | 16.99 / 2    | [Z][2amzn]     | CPAP tubing |
| 3    |        1 | RespiraWorks  | patient_tubing_adapter | ~$1.0        | [R][3rw]       | Y-adapter for 22mm tubing |


[1amzn]:  https://www.amazon.com/FlexLung-Biomedical-Ventilator-Testing-Demonstration/dp/B07B876P9C
[2amzn]:  https://www.amazon.com/gp/product/B01N14F1MV
[3rw]: #tubing-adapter

#### Tubing adapter

A "tee" or "wye" adapter is needed to connect both the inhale and the exhale tubes to the single port on the test lung.
Some are available for sale, and indeed some come included with various test lungs, but it has been hard to find
a consistent source. Thus we have designed a 22mm patient tubing adapter that can be 3d printed. Simple placement
without scaffolding works well and the part has been successfully printed with Siraya Fast.
Please see the [3d printing guidelines](../../../manufacturing/3d_printing) for general advice.

[STL file for 3d printing](assets/patient_tubing_adapter.stl)

![](assets/patient_tubing_adapter.jpg)

#### Pricing analysis

Lung $26 + Tubing $17 + Adapter $1 = $44

## Assembly

![](assets/test_lung_assembly.jpg)
