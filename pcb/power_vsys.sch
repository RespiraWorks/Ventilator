EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 34 37
Title "12V System Power Supply"
Date ""
Rev "A"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	4700 2350 5000 2350
Wire Wire Line
	4200 2750 4550 2750
Wire Wire Line
	4050 3150 4550 3150
Wire Wire Line
	4550 2550 4550 2750
Connection ~ 4550 2750
Wire Wire Line
	4550 2750 5000 2750
Wire Wire Line
	5000 2950 4850 2950
$Comp
L Device:R R?
U 1 1 601EA74E
P 4700 2950
AR Path="/601A64F8/601EA74E" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/601EA74E" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/601EA74E" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601EA74E" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/601EA74E" Ref="R?"  Part="1" 
F 0 "R?" V 4650 2700 50  0000 L CNN
F 1 "10" V 4650 3100 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 4630 2950 50  0001 C CNN
F 3 "~" H 4700 2950 50  0001 C CNN
	1    4700 2950
	0    1    1    0   
$EndComp
Wire Wire Line
	4550 2950 4550 3150
Connection ~ 4550 3150
Wire Wire Line
	4550 3150 5000 3150
Wire Wire Line
	6450 2350 6900 2350
$Comp
L power:GND #PWR?
U 1 1 601F3081
P 6650 2550
AR Path="/601A64F8/601F3081" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/601F3081" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/601F3081" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601F3081" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/601F3081" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6650 2300 50  0001 C CNN
F 1 "GND" H 6655 2377 50  0000 C CNN
F 2 "" H 6650 2550 50  0001 C CNN
F 3 "" H 6650 2550 50  0001 C CNN
	1    6650 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 2550 6650 2550
$Comp
L Device:R R?
U 1 1 601F467D
P 7250 2800
AR Path="/601A64F8/601F467D" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/601F467D" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/601F467D" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601F467D" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/601F467D" Ref="R?"  Part="1" 
F 0 "R?" H 6950 2850 50  0000 L CNN
F 1 "90.9k" H 6950 2750 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 7180 2800 50  0001 C CNN
F 3 "~" H 7250 2800 50  0001 C CNN
	1    7250 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 601F52A9
P 7250 3100
AR Path="/601A64F8/601F52A9" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/601F52A9" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/601F52A9" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601F52A9" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/601F52A9" Ref="R?"  Part="1" 
F 0 "R?" H 6950 3150 50  0000 L CNN
F 1 "10.1k" H 6950 3050 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 7180 3100 50  0001 C CNN
F 3 "~" H 7250 3100 50  0001 C CNN
	1    7250 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 601F6D09
P 7250 3250
AR Path="/601A64F8/601F6D09" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/601F6D09" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/601F6D09" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601F6D09" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/601F6D09" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7250 3000 50  0001 C CNN
F 1 "GND" H 7255 3077 50  0000 C CNN
F 2 "" H 7250 3250 50  0001 C CNN
F 3 "" H 7250 3250 50  0001 C CNN
	1    7250 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 2950 7250 2950
Connection ~ 7250 2950
Text Notes 2550 1250 0    50   ~ 0
12.5-16.8V IN
Text HLabel 2750 1350 0    50   Input ~ 0
VIN
Text HLabel 8700 1350 2    50   Output ~ 0
12VOUT
Wire Wire Line
	8400 1350 8700 1350
Connection ~ 8400 1350
Wire Wire Line
	8100 1350 8400 1350
Connection ~ 8100 1350
Wire Wire Line
	7800 1350 8100 1350
Connection ~ 7800 1350
Connection ~ 7500 1350
Wire Wire Line
	7500 1350 7800 1350
Wire Wire Line
	8400 1650 8400 1750
$Comp
L Device:C C?
U 1 1 602008C1
P 8400 1500
AR Path="/601A64F8/602008C1" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/602008C1" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/602008C1" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602008C1" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/602008C1" Ref="C?"  Part="1" 
F 0 "C?" H 8450 1400 50  0000 L CNN
F 1 "10uF" H 8450 1300 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 8438 1350 50  0001 C CNN
F 3 "~" H 8400 1500 50  0001 C CNN
F 4 "35V" H 8450 1200 50  0000 L CNN "Voltage"
F 5 "X5R" H 8400 1500 50  0001 C CNN "Dielectric"
	1    8400 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 602008B9
P 8400 1750
AR Path="/601A64F8/602008B9" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/602008B9" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/602008B9" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602008B9" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/602008B9" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8400 1500 50  0001 C CNN
F 1 "GND" H 8405 1577 50  0000 C CNN
F 2 "" H 8400 1750 50  0001 C CNN
F 3 "" H 8400 1750 50  0001 C CNN
	1    8400 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 1650 8100 1750
$Comp
L Device:C C?
U 1 1 602008B2
P 8100 1500
AR Path="/601A64F8/602008B2" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/602008B2" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/602008B2" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602008B2" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/602008B2" Ref="C?"  Part="1" 
F 0 "C?" H 8150 1400 50  0000 L CNN
F 1 "10uF" H 8150 1300 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 8138 1350 50  0001 C CNN
F 3 "~" H 8100 1500 50  0001 C CNN
F 4 "35V" H 8150 1200 50  0000 L CNN "Voltage"
F 5 "X5R" H 8100 1500 50  0001 C CNN "Dielectric"
	1    8100 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 602008AA
P 8100 1750
AR Path="/601A64F8/602008AA" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/602008AA" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/602008AA" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602008AA" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/602008AA" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8100 1500 50  0001 C CNN
F 1 "GND" H 8105 1577 50  0000 C CNN
F 2 "" H 8100 1750 50  0001 C CNN
F 3 "" H 8100 1750 50  0001 C CNN
	1    8100 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 1650 7800 1750
$Comp
L Device:C C?
U 1 1 601FFCEE
P 7800 1500
AR Path="/601A64F8/601FFCEE" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/601FFCEE" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/601FFCEE" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601FFCEE" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/601FFCEE" Ref="C?"  Part="1" 
F 0 "C?" H 7850 1400 50  0000 L CNN
F 1 "10uF" H 7850 1300 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 7838 1350 50  0001 C CNN
F 3 "~" H 7800 1500 50  0001 C CNN
F 4 "35V" H 7850 1200 50  0000 L CNN "Voltage"
F 5 "X5R" H 7800 1500 50  0001 C CNN "Dielectric"
	1    7800 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 601FFCE6
P 7800 1750
AR Path="/601A64F8/601FFCE6" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/601FFCE6" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/601FFCE6" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601FFCE6" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/601FFCE6" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7800 1500 50  0001 C CNN
F 1 "GND" H 7805 1577 50  0000 C CNN
F 2 "" H 7800 1750 50  0001 C CNN
F 3 "" H 7800 1750 50  0001 C CNN
	1    7800 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 1650 7500 1750
$Comp
L Device:C C?
U 1 1 601FFCDF
P 7500 1500
AR Path="/601A64F8/601FFCDF" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/601FFCDF" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/601FFCDF" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601FFCDF" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/601FFCDF" Ref="C?"  Part="1" 
F 0 "C?" H 7550 1400 50  0000 L CNN
F 1 "10uF" H 7550 1300 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 7538 1350 50  0001 C CNN
F 3 "~" H 7500 1500 50  0001 C CNN
F 4 "35V" H 7550 1200 50  0000 L CNN "Voltage"
F 5 "X5R" H 7500 1500 50  0001 C CNN "Dielectric"
	1    7500 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 601FFCD7
P 7500 1750
AR Path="/601A64F8/601FFCD7" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/601FFCD7" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/601FFCD7" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601FFCD7" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/601FFCD7" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7500 1500 50  0001 C CNN
F 1 "GND" H 7505 1577 50  0000 C CNN
F 2 "" H 7500 1750 50  0001 C CNN
F 3 "" H 7500 1750 50  0001 C CNN
	1    7500 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2150 6600 1650
Wire Wire Line
	6450 2150 6600 2150
Wire Wire Line
	7250 1350 7500 1350
Wire Wire Line
	7250 1350 7250 2650
Wire Wire Line
	4200 1550 4200 2750
Wire Wire Line
	4500 1750 4500 1850
Wire Wire Line
	6900 2350 6900 1650
$Comp
L Device:C C?
U 1 1 601ED324
P 6750 1650
AR Path="/601A64F8/601ED324" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/601ED324" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/601ED324" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601ED324" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/601ED324" Ref="C?"  Part="1" 
F 0 "C?" V 6600 1700 50  0000 L CNN
F 1 "33pF" V 6500 1600 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6788 1500 50  0001 C CNN
F 3 "~" H 6750 1650 50  0001 C CNN
	1    6750 1650
	0    -1   -1   0   
$EndComp
Connection ~ 3450 1350
Wire Wire Line
	3450 1650 3450 1750
$Comp
L Device:C C?
U 1 1 601E646C
P 3450 1500
AR Path="/601A64F8/601E646C" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/601E646C" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/601E646C" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601E646C" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/601E646C" Ref="C?"  Part="1" 
F 0 "C?" H 3500 1400 50  0000 L CNN
F 1 "10uF" H 3500 1300 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 3488 1350 50  0001 C CNN
F 3 "~" H 3450 1500 50  0001 C CNN
F 4 "35V" H 3500 1200 50  0000 L CNN "Voltage"
F 5 "X5R" H 3450 1500 50  0001 C CNN "Dielectric"
	1    3450 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 601E6464
P 3450 1750
AR Path="/601A64F8/601E6464" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/601E6464" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/601E6464" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601E6464" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/601E6464" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3450 1500 50  0001 C CNN
F 1 "GND" H 3455 1577 50  0000 C CNN
F 2 "" H 3450 1750 50  0001 C CNN
F 3 "" H 3450 1750 50  0001 C CNN
	1    3450 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 1650 3150 1750
Connection ~ 3150 1350
Wire Wire Line
	2750 1350 3150 1350
$Comp
L Device:C C?
U 1 1 601E645A
P 3150 1500
AR Path="/601A64F8/601E645A" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/601E645A" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/601E645A" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601E645A" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/601E645A" Ref="C?"  Part="1" 
F 0 "C?" H 3200 1400 50  0000 L CNN
F 1 "10uF" H 3200 1300 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 3188 1350 50  0001 C CNN
F 3 "~" H 3150 1500 50  0001 C CNN
F 4 "35V" H 3200 1200 50  0000 L CNN "Voltage"
F 5 "X5R" H 3150 1500 50  0001 C CNN "Dielectric"
	1    3150 1500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 601E6452
P 3150 1750
AR Path="/601A64F8/601E6452" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/601E6452" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/601E6452" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601E6452" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/601E6452" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3150 1500 50  0001 C CNN
F 1 "GND" H 3155 1577 50  0000 C CNN
F 2 "" H 3150 1750 50  0001 C CNN
F 3 "" H 3150 1750 50  0001 C CNN
	1    3150 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 1650 4050 3150
Wire Wire Line
	5900 1350 6600 1350
Connection ~ 4950 1350
$Comp
L RespiraWorks_Std:CSD17304Q3 Q?
U 1 1 601DFE01
P 4400 1550
AR Path="/601A64F8/601DFE01" Ref="Q?"  Part="1" 
AR Path="/601A64F8/60567144/601DFE01" Ref="Q?"  Part="1" 
AR Path="/601A64F8/60567D6E/601DFE01" Ref="Q?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601DFE01" Ref="Q?"  Part="1" 
AR Path="/6041384A/60567D6E/601DFE01" Ref="Q?"  Part="1" 
F 0 "Q?" H 4450 1950 50  0000 L CNN
F 1 "CSD17304Q3" H 4300 1850 50  0000 L CNN
F 2 "RespiraWorks_Std:VSON-8_3.3x3.3mm_P0.65mm_NexFET" H 4600 1475 50  0001 L CIN
F 3 "https://www.ti.com/lit/ds/symlink/csd17304q3.pdf?ts=1608773956037" V 4400 1550 50  0001 L CNN
	1    4400 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 1350 4700 1350
Wire Wire Line
	4250 1350 4500 1350
Connection ~ 4500 1350
Wire Wire Line
	5000 2150 4950 2150
$Comp
L Device:C C?
U 1 1 601DFDEE
P 4950 1500
AR Path="/601A64F8/601DFDEE" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/601DFDEE" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/601DFDEE" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601DFDEE" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/601DFDEE" Ref="C?"  Part="1" 
F 0 "C?" H 5065 1546 50  0000 L CNN
F 1 "0.1uF" H 5065 1455 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 4988 1350 50  0001 C CNN
F 3 "~" H 4950 1500 50  0001 C CNN
	1    4950 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 1350 4950 1350
$Comp
L power:GND #PWR?
U 1 1 601DFDE7
P 4500 1850
AR Path="/601A64F8/601DFDE7" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/601DFDE7" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/601DFDE7" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601DFDE7" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/601DFDE7" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 4500 1600 50  0001 C CNN
F 1 "GND" H 4505 1677 50  0000 C CNN
F 2 "" H 4500 1850 50  0001 C CNN
F 3 "" H 4500 1850 50  0001 C CNN
	1    4500 1850
	1    0    0    -1  
$EndComp
Connection ~ 4700 1350
$Comp
L RespiraWorks_Std:CSD17304Q3 Q?
U 1 1 601DFDDC
P 4050 1450
AR Path="/601A64F8/601DFDDC" Ref="Q?"  Part="1" 
AR Path="/601A64F8/60567144/601DFDDC" Ref="Q?"  Part="1" 
AR Path="/601A64F8/60567D6E/601DFDDC" Ref="Q?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601DFDDC" Ref="Q?"  Part="1" 
AR Path="/6041384A/60567D6E/601DFDDC" Ref="Q?"  Part="1" 
F 0 "Q?" V 4450 1450 50  0000 C CNN
F 1 "CSD17304Q3" V 4350 1500 50  0000 C CNN
F 2 "RespiraWorks_Std:VSON-8_3.3x3.3mm_P0.65mm_NexFET" H 4250 1375 50  0001 L CIN
F 3 "https://www.ti.com/lit/ds/symlink/csd17304q3.pdf?ts=1608773956037" V 4050 1450 50  0001 L CNN
	1    4050 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 1350 4700 2350
$Comp
L RespiraWorks_Std:SRP1038A-3R3M L?
U 1 1 601DFDD4
P 5750 1350
AR Path="/601A64F8/601DFDD4" Ref="L?"  Part="1" 
AR Path="/601A64F8/60567144/601DFDD4" Ref="L?"  Part="1" 
AR Path="/601A64F8/60567D6E/601DFDD4" Ref="L?"  Part="1" 
AR Path="/6023BF6A/60567D6E/601DFDD4" Ref="L?"  Part="1" 
AR Path="/6041384A/60567D6E/601DFDD4" Ref="L?"  Part="1" 
F 0 "L?" H 5750 1650 50  0000 C CNN
F 1 "6.8uH" H 5750 1550 50  0000 C CNN
F 2 "RespiraWorks:L_TDK_SPM12565VT" V 5750 1350 50  0001 C CNN
F 3 "https://product.tdk.com/info/en/catalog/datasheets/inductor_automotive_power_spm12565vt-d_en.pdf" V 5750 1350 50  0001 C CNN
F 4 "TDK" H 5750 1350 50  0001 C CNN "Manufacturer"
F 5 "SPM12565VT-6R8M-D" H 5800 1450 50  0000 C CNN "manufacturer PN"
	1    5750 1350
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks:LM5141 U?
U 1 1 601D6520
P 5750 3550
F 0 "U?" H 5400 1850 50  0000 C CNN
F 1 "LM5141" H 5500 1750 50  0000 C CNN
F 2 "RespiraWorks_Std:VQFN-24-1EP_4x4mm_P0.5mm_EP2.45x2.45mm_ThermalVias" H 5650 1700 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/lm5141-q1.pdf" H 8450 4600 50  0001 C CNN
F 4 "Texas Instruments" H 5750 3750 50  0000 C CNN "Manufacturer"
F 5 "LM5141RGER" H 5750 3650 50  0000 C CNN "Manufacturer PN"
	1    5750 3550
	1    0    0    -1  
$EndComp
Connection ~ 7250 1350
Wire Wire Line
	5250 1800 5750 1800
Text Label 5750 1800 2    50   ~ 0
LM5141_VCC
Text Label 7700 4200 2    50   ~ 0
LM5141_VCC
Wire Wire Line
	6450 4200 8050 4200
$Comp
L Device:C C?
U 1 1 6022668B
P 8050 4350
AR Path="/601A64F8/6022668B" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/6022668B" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/6022668B" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/6022668B" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/6022668B" Ref="C?"  Part="1" 
F 0 "C?" H 7850 4350 50  0000 L CNN
F 1 "2.2uF" H 7700 4450 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 8088 4200 50  0001 C CNN
F 3 "~" H 8050 4350 50  0001 C CNN
	1    8050 4350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 602277DB
P 8050 4500
AR Path="/601A64F8/602277DB" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/602277DB" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/602277DB" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602277DB" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/602277DB" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 8050 4250 50  0001 C CNN
F 1 "GND" H 8055 4327 50  0000 C CNN
F 2 "" H 8050 4500 50  0001 C CNN
F 3 "" H 8050 4500 50  0001 C CNN
	1    8050 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 602289D7
P 7750 3750
AR Path="/601A64F8/602289D7" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/602289D7" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/602289D7" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602289D7" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/602289D7" Ref="R?"  Part="1" 
F 0 "R?" H 7450 3800 50  0000 L CNN
F 1 "10k" H 7450 3700 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 7680 3750 50  0001 C CNN
F 3 "~" H 7750 3750 50  0001 C CNN
	1    7750 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 3900 7750 3900
Wire Wire Line
	7750 3600 8100 3600
Text HLabel 8100 3600 2    50   Input ~ 0
VLOGIC
Wire Wire Line
	7750 3900 8100 3900
Connection ~ 7750 3900
Text HLabel 8100 3900 2    50   Output ~ 0
PG
$Comp
L Device:C C?
U 1 1 602371E3
P 7450 4550
AR Path="/601A64F8/602371E3" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/602371E3" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/602371E3" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602371E3" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/602371E3" Ref="C?"  Part="1" 
F 0 "C?" H 7565 4596 50  0000 L CNN
F 1 "0.1uF" H 7565 4505 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 7488 4400 50  0001 C CNN
F 3 "~" H 7450 4550 50  0001 C CNN
	1    7450 4550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 6023DFE2
P 7150 5250
AR Path="/601A64F8/6023DFE2" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/6023DFE2" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/6023DFE2" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/6023DFE2" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/6023DFE2" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7150 5000 50  0001 C CNN
F 1 "GND" H 7155 5077 50  0000 C CNN
F 2 "" H 7150 5250 50  0001 C CNN
F 3 "" H 7150 5250 50  0001 C CNN
	1    7150 5250
	1    0    0    -1  
$EndComp
Connection ~ 7150 5100
Wire Wire Line
	7150 5100 7150 5250
Connection ~ 7150 4900
Wire Wire Line
	7150 4900 7150 5100
Text Label 7250 4400 2    50   ~ 0
LM5141_VDDA
$Comp
L Device:R R?
U 1 1 60263CF0
P 4600 5250
AR Path="/601A64F8/60263CF0" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/60263CF0" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/60263CF0" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/60263CF0" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/60263CF0" Ref="R?"  Part="1" 
F 0 "R?" H 4700 5350 50  0000 L CNN
F 1 "56.2k" H 4700 5250 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 4530 5250 50  0001 C CNN
F 3 "~" H 4600 5250 50  0001 C CNN
	1    4600 5250
	1    0    0    -1  
$EndComp
$Comp
L Device:Net-Tie_2 NT?
U 1 1 6026B185
P 7150 4800
F 0 "NT?" V 7150 4900 50  0000 C CNN
F 1 "Net-Tie_2" H 7100 4850 50  0001 C CNN
F 2 "" H 7150 4800 50  0001 C CNN
F 3 "~" H 7150 4800 50  0001 C CNN
	1    7150 4800
	0    1    1    0   
$EndComp
Text Label 6450 4700 0    50   ~ 0
LM5141_AGND
Wire Wire Line
	6450 4900 7150 4900
Wire Wire Line
	6450 5100 7150 5100
Connection ~ 3750 1350
Wire Wire Line
	3450 1350 3750 1350
Wire Wire Line
	3750 1350 3850 1350
Wire Wire Line
	3150 1350 3450 1350
Connection ~ 2550 5400
Wire Wire Line
	2550 5100 2550 5400
Wire Wire Line
	2150 4200 2150 5400
Text Label 1200 5400 0    50   ~ 0
LM5141_AGND
Wire Wire Line
	3150 4600 3150 5400
Wire Wire Line
	3150 5400 2800 5400
$Comp
L Device:C C?
U 1 1 6025AFE5
P 3150 4450
AR Path="/601A64F8/6025AFE5" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/6025AFE5" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/6025AFE5" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/6025AFE5" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/6025AFE5" Ref="C?"  Part="1" 
F 0 "C?" H 2950 4400 50  0000 L CNN
F 1 "0.1uF" H 2850 4550 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 3188 4300 50  0001 C CNN
F 3 "~" H 3150 4450 50  0001 C CNN
	1    3150 4450
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 6024C413
P 2550 4950
AR Path="/601A64F8/6024C413" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/6024C413" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/6024C413" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/6024C413" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/6024C413" Ref="R?"  Part="1" 
F 0 "R?" H 2300 4850 50  0000 L CNN
F 1 "6.49k" H 2300 4750 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 2480 4950 50  0001 C CNN
F 3 "~" H 2550 4950 50  0001 C CNN
	1    2550 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6024AB9B
P 2800 4850
AR Path="/601A64F8/6024AB9B" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/6024AB9B" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/6024AB9B" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/6024AB9B" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/6024AB9B" Ref="C?"  Part="1" 
F 0 "C?" H 2850 5000 50  0000 L CNN
F 1 "100pF" H 2850 4700 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 2838 4700 50  0001 C CNN
F 3 "~" H 2800 4850 50  0001 C CNN
	1    2800 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 3500 2150 3600
Text Label 1350 3500 0    50   ~ 0
LM5141_VDDA
Wire Wire Line
	2150 3500 1350 3500
Connection ~ 2150 3900
$Comp
L Device:R R?
U 1 1 6023155B
P 2150 4050
AR Path="/601A64F8/6023155B" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/6023155B" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/6023155B" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/6023155B" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/6023155B" Ref="R?"  Part="1" 
F 0 "R?" H 1850 4100 50  0000 L CNN
F 1 "10k" H 1850 4000 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 2080 4050 50  0001 C CNN
F 3 "~" H 2150 4050 50  0001 C CNN
	1    2150 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 602302A0
P 2150 3750
AR Path="/601A64F8/602302A0" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/602302A0" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/602302A0" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602302A0" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/602302A0" Ref="R?"  Part="1" 
F 0 "R?" H 1850 3850 50  0000 L CNN
F 1 "10k" H 1850 3750 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 2080 3750 50  0001 C CNN
F 3 "~" H 2150 3750 50  0001 C CNN
F 4 "DNP" H 1950 3650 50  0000 C CNN "DNP"
	1    2150 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 602493D1
P 2550 4650
AR Path="/601A64F8/602493D1" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/602493D1" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/602493D1" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602493D1" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/602493D1" Ref="C?"  Part="1" 
F 0 "C?" H 2300 4850 50  0000 L CNN
F 1 "2.7nF" H 2300 4750 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 2588 4500 50  0001 C CNN
F 3 "~" H 2550 4650 50  0001 C CNN
	1    2550 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 5000 2800 5400
Connection ~ 2800 5400
Wire Wire Line
	2800 5400 2550 5400
Wire Wire Line
	2550 4100 2800 4100
Wire Wire Line
	2550 4100 2550 4500
Wire Wire Line
	2800 4100 2800 4700
Connection ~ 2800 4100
Wire Wire Line
	2800 4100 5000 4100
Connection ~ 2150 5400
Wire Wire Line
	2150 5400 1200 5400
Wire Wire Line
	2150 5400 2550 5400
Wire Wire Line
	3150 4300 5000 4300
$Comp
L Device:C C?
U 1 1 602DA13A
P 3600 4650
AR Path="/601A64F8/602DA13A" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/602DA13A" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/602DA13A" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602DA13A" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/602DA13A" Ref="C?"  Part="1" 
F 0 "C?" H 3400 4600 50  0000 L CNN
F 1 "220nF" H 3300 4750 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 3638 4500 50  0001 C CNN
F 3 "~" H 3600 4650 50  0001 C CNN
	1    3600 4650
	-1   0    0    1   
$EndComp
Wire Wire Line
	5000 4500 3600 4500
Wire Wire Line
	3600 5400 3150 5400
Connection ~ 3150 5400
Wire Wire Line
	3600 4800 3600 5400
Wire Wire Line
	5000 4700 4000 4700
Wire Wire Line
	4000 4700 4000 5400
Wire Wire Line
	4000 5400 3600 5400
Connection ~ 3600 5400
Wire Wire Line
	2150 3900 5000 3900
$Comp
L Device:C C?
U 1 1 602EA7E5
P 4200 5050
AR Path="/601A64F8/602EA7E5" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/602EA7E5" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567D6E/602EA7E5" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602EA7E5" Ref="C?"  Part="1" 
AR Path="/6041384A/60567D6E/602EA7E5" Ref="C?"  Part="1" 
F 0 "C?" H 4000 5000 50  0000 L CNN
F 1 "220nF" H 3900 5150 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 4238 4900 50  0001 C CNN
F 3 "~" H 4200 5050 50  0001 C CNN
	1    4200 5050
	-1   0    0    1   
$EndComp
Wire Wire Line
	5000 5100 4600 5100
Wire Wire Line
	4600 5400 4200 5400
Connection ~ 4000 5400
Wire Wire Line
	5000 4900 4200 4900
Wire Wire Line
	4200 5200 4200 5400
Connection ~ 4200 5400
Wire Wire Line
	4200 5400 4000 5400
Wire Wire Line
	6450 4700 7150 4700
Connection ~ 7150 4700
Wire Wire Line
	7150 4700 7450 4700
Wire Wire Line
	6450 4400 7450 4400
$Comp
L Diode:BAT54J D?
U 1 1 6028E91B
P 5100 1800
F 0 "D?" H 5100 1900 50  0000 C CNN
F 1 "BAT54J" H 5150 1650 50  0000 C CNN
F 2 "RespiraWorks_Std:D_SOD-323F" H 5100 1625 50  0001 C CNN
F 3 "https://assets.nexperia.com/documents/data-sheet/BAT54J.pdf" H 5100 1800 50  0001 C CNN
	1    5100 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 1650 4950 1800
Connection ~ 4950 1800
Wire Wire Line
	4950 1800 4950 2150
$Comp
L Device:R R?
U 1 1 60279FEE
P 4700 2550
AR Path="/601A64F8/60279FEE" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/60279FEE" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/60279FEE" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/60279FEE" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/60279FEE" Ref="R?"  Part="1" 
F 0 "R?" V 4650 2300 50  0000 L CNN
F 1 "10" V 4650 2700 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 4630 2550 50  0001 C CNN
F 3 "~" H 4700 2550 50  0001 C CNN
	1    4700 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	4850 2550 5000 2550
$Comp
L Device:R R?
U 1 1 60286B7B
P 6750 1350
AR Path="/601A64F8/60286B7B" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/60286B7B" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/60286B7B" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/60286B7B" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/60286B7B" Ref="R?"  Part="1" 
F 0 "R?" V 6550 1300 50  0000 L CNN
F 1 "0.006" V 6650 1250 50  0000 L CNN
F 2 "RespiraWorks_Std:R_1206_3216Metric" V 6680 1350 50  0001 C CNN
F 3 "https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=1773449-3&DocType=DS&DocLang=English" H 6750 1350 50  0001 C CNN
F 4 "TE Connectivity" V 6750 1350 50  0001 C CNN "Manufacturer"
F 5 "TLR2B10DR006FTDG" V 6750 1350 50  0001 C CNN "Manufacturer PN"
	1    6750 1350
	0    1    1    0   
$EndComp
Wire Wire Line
	4950 1350 5600 1350
Wire Wire Line
	6900 1350 7250 1350
Wire Wire Line
	6600 1650 6600 1350
Connection ~ 6600 1650
Connection ~ 6600 1350
Wire Wire Line
	6900 1650 6900 1350
Connection ~ 6900 1650
Connection ~ 6900 1350
Text Notes 6100 1050 0    50   ~ 0
Cycle-by-cycle current limit: 75mA -> 12.5A
Wire Wire Line
	3750 3500 3750 2500
$Comp
L Transistor_BJT:MMBT3904 Q?
U 1 1 602A3DA2
P 2900 3100
F 0 "Q?" H 3091 3146 50  0000 L CNN
F 1 "MMBT3904" H 3091 3055 50  0000 L CNN
F 2 "RespiraWorks_Std:SOT-23" H 3100 3025 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/2N3903-D.PDF" H 2900 3100 50  0001 L CNN
	1    2900 3100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 602A5044
P 3000 3300
AR Path="/601A64F8/602A5044" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/602A5044" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567D6E/602A5044" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602A5044" Ref="#PWR?"  Part="1" 
AR Path="/6041384A/60567D6E/602A5044" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3000 3050 50  0001 C CNN
F 1 "GND" H 3005 3127 50  0000 C CNN
F 2 "" H 3000 3300 50  0001 C CNN
F 3 "" H 3000 3300 50  0001 C CNN
	1    3000 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 602A8288
P 3000 2650
AR Path="/601A64F8/602A8288" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/602A8288" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/602A8288" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602A8288" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/602A8288" Ref="R?"  Part="1" 
F 0 "R?" H 2700 2700 50  0000 L CNN
F 1 "10k" H 2700 2600 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2930 2650 50  0001 C CNN
F 3 "~" H 3000 2650 50  0001 C CNN
	1    3000 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2500 3750 2500
Connection ~ 3750 2500
Wire Wire Line
	3750 2500 3750 1350
Wire Wire Line
	3000 2800 3000 2850
Wire Wire Line
	3000 2850 3600 2850
Wire Wire Line
	3600 2850 3600 3700
Connection ~ 3000 2850
Wire Wire Line
	3000 2850 3000 2900
Wire Wire Line
	3600 3700 5000 3700
Wire Wire Line
	3750 3500 5000 3500
Wire Wire Line
	2700 3100 2300 3100
Text HLabel 1350 3100 0    50   Input ~ 0
VSYS_EN
$Comp
L Device:R R?
U 1 1 602BAB86
P 2150 3100
AR Path="/601A64F8/602BAB86" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/602BAB86" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567D6E/602BAB86" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567D6E/602BAB86" Ref="R?"  Part="1" 
AR Path="/6041384A/60567D6E/602BAB86" Ref="R?"  Part="1" 
F 0 "R?" V 1950 3050 50  0000 L CNN
F 1 "10k" V 2050 3050 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" V 2080 3100 50  0001 C CNN
F 3 "~" H 2150 3100 50  0001 C CNN
	1    2150 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	2000 3100 1350 3100
Text Notes 1750 3800 2    50   ~ 0
Forced-PWM CCM mode\n@ light load
Text Notes 1750 4150 2    50   ~ 0
Diode emulation mode\n@ light load
Text Notes 4000 4700 0    50   ~ 0
440 kHz fsw nominal
Text Notes 4100 5700 0    50   ~ 0
220nF modulates fsw +/- 5% @ ~~380 Hz
Text Notes 4500 5550 0    50   ~ 0
56.2K fine-tunes fsw = 436 kHz nominal
Text Notes 3100 4300 0    50   ~ 0
Soft-start time = 1.2V*100nF/20uA = 6 ms
$EndSCHEMATC
