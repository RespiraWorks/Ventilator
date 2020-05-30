# Fan Characterization Test - May 25, 2020

## Overview

* **Test Date:** May 25, 2020
* **Location:**	The Port, Oakland, CA
* **Involved:**	Ethan Chaleff, echaleff@gmail.com; Bryan Murray, brmurray@vt.edu

## Test Specifications

The blower/fan was directly in series with a venturi tube, with 3x MXPV5004DP pressure sensors attached. 
Sensor #1 measured the venturi choke point ("high" side of sensor #1) and atmospheric pressure ("low" side).
Sensors #2 & #3 of the pressure sensors measured dP across the venturi (they measured the same exact thing). 

3 tests were conducted, each at a different fan speed. Fan speed was deduced by soldering leads to a Hall sensor
("HA") and ground on the blower driver board, and read using an oscilloscope. The test files are named for nominal speed.

Data was recorded using "PCBreathe" SN07. Unfortunately the STM was programmed using "STMduino"
(using the Arduino IDE), so data was collected at just 10-bit resolution. 

Post-processing enabled by a rachety python script. Ask Bryan for more details.

## Files

* Flow Test 21818
  * [CSV file](assets/flow-test-21818rpm.csv)
  * [PKL file](assets/flow-test-21818.pkl)
* Flow Test 24000
  * [CSV file](assets/flow-test-24000rpm.csv)
  * [PKL file](assets/flow-test-24000.pkl)
* Flow Test 26666
  * [CSV file](assets/flow-test-26666rpm.csv)
  * [PKL file](assets/flow-test-26666.pkl)

## CSV File

* `t`: Time step, normalized to start of the data. Should be ~10ms/100Hz
* `p1_adc`: Raw adc counts for the "patient" pressure sensor (referenced to atmosphere)
* `patient`: Converted to dP [kPa]
* `p2_adc`: Raw adc counts from the first of two pressure sensors measuring dP in the venturi
* `p2`: dP [kPa] across the venturi
* `p3_adc`: Raw adc counts from the second of two pressure sensors measuring dP in the venturi
* `p3`: dP [kPa] across the venturi
* `Q1`: Flow rate through the venturi calculated using p2 (dP in kPa)

## PKL File

These are "pickled" Pandas dataframe objects of the raw data. Useful for post-processing with Python/Numpy