# Pizza Build: Test Lung Sub-Assembly

True test lungs are expensive and hard to come by, so we must do with what's available. 

This page explains how to adapt off-the-shelf parts into a basic test lung for our ventilator prototype.

Note: You can optionally add-on to this simple test lung to make a reduced-compliance test lung; [instructions for that are here](../pizza-compliance-test-lung).

## Parts List (BOM)

**Note: If you are a member of the RespiraWorks team, review [part purchasing guidelines](../../README.md) BEFORE purchasing any parts.**

### Purchasing Source Key

* **E** = ebay
* **Z** = Amazon
* **C** = McMaster-Carr

### Parts

| Item | Quantity | Manufacturer  | Part #         | Price (USD)     | Action | Sources         | Notes |
| ---- |---------:| ------------- | ------------------- | ------------:|-----------|----------------| ----- |
| 1    |        1 | Portex        | 370924              | 15.00        |*ask first*| [E][1ebay]     | test "lung" or breathing bag. |
| 2  |        1 | South Pacific   | SPB-6110-5PK        |   129.00 / 5 |*ask first*| [Z][2amzn]     | **ALT for item 1** |
| 3    |        2 | Mars Wellness | 4332593260          | 16.99 / 2    | buy       | [Z][3amzn]     | CPAP tubing |
| 4    |        2 | McMaster-Carr | 5463K167            | 13.16 / 10   | buy       | [C][4mcmc]     | 3/4"ID <-> 5/8"ID single barb adapter |

[1ebay]:  https://www.ebay.com/itm/PORTEX-Adult-Disposable-Anesthesia-Breathing-Circuit-3L-Breathing-Bag-370924/362943456196
[2amzn]:  https://www.amazon.com/FlexLung-Biomedical-Ventilator-Testing-Demonstration/dp/B07B876P9C
[3amzn]:  https://www.amazon.com/gp/product/B01N14F1MV
[4mcmc]:  https://www.mcmaster.com/5463K167

### Parts: Alt Tee

| Item | Quantity | Manufacturer  | Part #         | Price (USD)     | Sources        | Notes |
| ---- |---------:| ------------- | ------------------- | ------------:|----------------|----------------|
| a1   |  ~10 cm  | McMaster-Carr | 5234K57             | 21.50 / 5ft  | [C][a1mcmc]     | Soft latex tubing, 5/8" ID, 7/8" OD, optional? |
| a2   |       1  | McMaster-Carr | 9579K72             | 12.21 / 20   | [C][a2mcmc]     | Herbie clip J (20.3-23mm) for clamping 7/8"OD tubing, optional? |
| a3   |       1  | McMaster-Carr | 4880K154            | 2.82         | [C][a3mcmc]     | 1/2 NTP tee female |
| a4   |       3  | McMaster-Carr | 5372K128            | 4.89 / 10    | [C][a4mcmc]     | 1/2 NTP <-> 5/8"ID male multibarbed |
| a5   |        1 | McMaster-Carr | 5463K615            |     12.37 for pack of 10| [C][a5mcmc]     | Tee junction - *alternative to item a3* |
| a6   |       1  | McMaster-Carr | 5372K625            | 9.59 / 10    | [C][a6mcmc]     | 5/8"ID tee, multibarbed, *alternative to items a3 & a5* |

[a1mcmc]:  https://www.mcmaster.com/5234K57
[a2mcmc]:  https://www.mcmaster.com/9579K72
[a3mcmc]:  https://www.mcmaster.com/4880K154
[a4mcmc]:  https://www.mcmaster.com/5372K128
[a5mcmc]:  https://www.mcmaster.com/5463K615
[a6mcmc]:  https://www.mcmaster.com/5372K625

**TODO:** pricing analysis

## Assembly

The test lung (*item 1*) will arrive with elbow fitting, tee-connector and CPAP hoses.

| As purchased         |  without hoses       |           
|:--------------------:|:--------------------:|
![](assets/lung-purchased.jpg)|![](assets/lung-no-tubes.jpg)|

These hoses are not great for interfacing with our system, so you should replace them with the set bought separately (*item 2*).

The rubber ends work well with 3/4"ID fittings, so if interfacing with a 5/8"ID system, also attach adapters (*item 3*) on the other ends.
For a prototype with 3/4"ID tubing, these adapters should not be necessary.

![Lung-with-new-tubes](assets/lung-new-tubes.jpg)

### Alternative Configurations

Although running long respiratory limbs is the best emulation of a real clinical setup, you may be short on space in your set up. Also, the test lung you buy may not come with the same type of connectors. 

In this case, you could set up with tee connectors of your own. Two different options are possible with McMaster parts and pasts for both alternatives are listed as optional in the parts list. The assembled article below uses *item a5* as tee.

| Parts                  |  Assembled           |           
|:----------------------:|:--------------------:|
![](assets/lung-short-parts.jpg)|![](assets/lung-short-assembled.jpg)|

