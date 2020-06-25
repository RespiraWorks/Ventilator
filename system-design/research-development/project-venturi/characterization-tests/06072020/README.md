# Fan Characterization Test - May 25, 2020

## Overview

* **Test Date:** June 8, 2020
* **Location:**	Oakland, CA
* **Involved:**	Ethan Chaleff (echaleff@gmail.com); Bryan Murray (brmurray@vt.edu)

## Test Specifications

A fan drove air past two flow-sensing elements in series: a 3/4" Venturi (with its sensing ports branched to **both** an MXP5004D analog and an AMS5916 I2C sensor), and an AD Instruments pneumotachograph (PTG). The fan was ramped from 0 RPM - max speed over ~20 seconds.

| Pneumotachograph  |  Venturi   | Blower Driver     |
|:---------------------:|:--------------------:|:-----------------------:|
![](assets/adinstruments-pneumotachograph.jpg) |![](assets/blower-venturi.jpg)  |![](assets/pcb-blower-driver.jpg)  |

This test serves two purposes:
1. Compare the sensitivity of two pressure sensors
2. Compare the venturi's calculated flow rate against a trusted reference (the ADI PTG)

Data was collected using a modified version of 'pcbreathe-bringup.ino'

## Data, Collection, and Analysis Files

* "ptg_venturi_i2c_0607_x1"
  * [ptg_venturi_i2c_0607_x1_labchart.txt](assets/ptg-venturi-i2c-0607-x1-labchart.txt)
  * [ptg_venturi_i2c_0607_x1_pcb.txt](assets/ptg-venturi-i2c-0607-x1-pcb.txt)
  * [pcbreathe-bringup_Venturi_char.ino](assets/pcbreathe-bringup-venturi-char.ino)
  * [Venturi_Characterization.py](assets/venturi-characterization.py)

## Results

First, the two pressure senors (MXP5004DP and AMS5915) were compared. The time trace shows "good" agreement, comfirmed by a linear regression coefficeient of 0.97.

![pressure sensor compare](assets/pressure-sensor-compare.PNG)

More important is the effect of these sensors' accuracies on flow calculation. First, a qualitative comparison with the PTG. Three notes on this plot:
* the disagreement before t=17s is a very low flow regime, where the static voltage offset and noise dominates the flow calculation
* The PTG data was aligned with the pressure sensor data using a empirical offset of 0.35 second. The PTG data was downsampled from 100 Hz to 20Hz to match the pressure sensor data.

![flow compare](assets/flow-compare.PNG)

A quadratic function was fit to a curve of dP vs Flow for each dataset. (The PTG does not provide dP, so the MXP dP points were used for that data.) These curves show a qualitative comparison of the pressure sensors/venturi's prediction of flow.

![quad fit compare](assets/quad-fit-compare.PNG)

Finally, a linear regression to compare each sensor/venturi pair's prediction accuracy against the PTG.

![flow regression](assets/flow-regression.PNG)
