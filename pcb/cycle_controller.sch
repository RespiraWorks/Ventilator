EESchema Schematic File Version 4
LIBS:Ventilator-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 7 30
Title "Cycle Controller"
Date ""
Rev "2.0"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_ST_STM32L4:STM32L496RGTx U?
U 1 1 5FEB9E53
P 5800 3200
F 0 "U?" H 5250 4950 50  0000 C CNN
F 1 "STM32L496RGTx" H 6300 1350 50  0000 C CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 5200 1500 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00284211.pdf" H 5800 3200 50  0001 C CNN
	1    5800 3200
	1    0    0    -1  
$EndComp
$Comp
L Ventilator-rescue:+3.3V_Ctrl-RespiraWorks #PWR?
U 1 1 5FEBC752
P 5700 700
F 0 "#PWR?" H 5700 700 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 5685 873 50  0000 C CNN
F 2 "" H 5700 700 50  0001 C CNN
F 3 "" H 5700 700 50  0001 C CNN
	1    5700 700 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 700  5800 700 
Wire Wire Line
	5800 1400 5800 700 
Connection ~ 5800 700 
Wire Wire Line
	5800 700  5700 700 
Wire Wire Line
	5700 700  5600 700 
$Comp
L Ventilator-rescue:+3.3V_Ctrl-RespiraWorks #PWR?
U 1 1 5FEBC80C
P 7100 800
F 0 "#PWR?" H 7100 800 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 7085 973 50  0000 C CNN
F 2 "" H 7100 800 50  0001 C CNN
F 3 "" H 7100 800 50  0001 C CNN
	1    7100 800 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEBC8E3
P 6450 950
F 0 "C?" H 6565 976 50  0000 L CNN
F 1 "0.1uF, 10V" H 6500 850 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 6488 800 50  0001 C CNN
F 3 "~" H 6450 950 50  0001 C CNN
	1    6450 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEBC9C4
P 6750 950
F 0 "C?" H 6865 976 50  0000 L CNN
F 1 "0.1uF, 10V" H 6800 850 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 6788 800 50  0001 C CNN
F 3 "~" H 6750 950 50  0001 C CNN
	1    6750 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEBC9E0
P 7100 950
F 0 "C?" H 7215 976 50  0000 L CNN
F 1 "0.1uF, 10V" H 7150 850 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 7138 800 50  0001 C CNN
F 3 "~" H 7100 950 50  0001 C CNN
	1    7100 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEBC9FE
P 7400 950
F 0 "C?" H 7515 976 50  0000 L CNN
F 1 "0.1uF, 10V" H 7450 850 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 7438 800 50  0001 C CNN
F 3 "~" H 7400 950 50  0001 C CNN
	1    7400 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEBCA35
P 7700 950
F 0 "C?" H 7815 976 50  0000 L CNN
F 1 "4.7uF, 10V" H 7750 850 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 7738 800 50  0001 C CNN
F 3 "~" H 7700 950 50  0001 C CNN
	1    7700 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 800  7400 800 
Connection ~ 7100 800 
Connection ~ 7400 800 
Wire Wire Line
	7400 800  7700 800 
Wire Wire Line
	7100 800  6750 800 
Connection ~ 6750 800 
Wire Wire Line
	6750 800  6450 800 
$Comp
L power:GND #PWR?
U 1 1 5FEBCB86
P 7100 1200
F 0 "#PWR?" H 7100 950 50  0001 C CNN
F 1 "GND" H 7105 1027 50  0000 C CNN
F 2 "" H 7100 1200 50  0001 C CNN
F 3 "" H 7100 1200 50  0001 C CNN
	1    7100 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 1050 7700 1100
Wire Wire Line
	7700 1200 7400 1200
Connection ~ 7700 1100
Wire Wire Line
	7700 1100 7700 1200
Wire Wire Line
	7400 1100 7400 1200
Connection ~ 7400 1200
Wire Wire Line
	7400 1200 7100 1200
Wire Wire Line
	7100 1100 7100 1200
Connection ~ 7100 1200
Wire Wire Line
	6750 1100 6750 1200
Wire Wire Line
	6750 1200 7100 1200
Wire Wire Line
	6750 1200 6450 1200
Wire Wire Line
	6450 1200 6450 1100
Connection ~ 6750 1200
Text Notes 7200 550  2    50   ~ 0
VDD pins
Text Notes 8400 550  2    50   ~ 0
VDDA pins
$Comp
L Device:C C?
U 1 1 5FEBD47E
P 8350 950
F 0 "C?" H 8465 976 50  0000 L CNN
F 1 "0.1uF, 10V" H 8400 850 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 8388 800 50  0001 C CNN
F 3 "~" H 8350 950 50  0001 C CNN
	1    8350 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEBD4CF
P 8700 950
F 0 "C?" H 8815 976 50  0000 L CNN
F 1 "1uF, 10V" H 8750 850 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 8738 800 50  0001 C CNN
F 3 "~" H 8700 950 50  0001 C CNN
	1    8700 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 1100 8550 1100
Wire Wire Line
	8550 1100 8550 1150
Wire Wire Line
	8700 1100 8550 1100
Connection ~ 8550 1100
Wire Wire Line
	5700 5350 5600 5350
Wire Wire Line
	5800 5000 5800 5350
Wire Wire Line
	5800 5350 5700 5350
Connection ~ 5700 5350
$Comp
L power:GNDA #PWR?
U 1 1 5FED43B1
P 6750 5600
F 0 "#PWR?" H 6750 5350 50  0001 C CNN
F 1 "GNDA" H 6755 5427 50  0000 C CNN
F 2 "" H 6750 5600 50  0001 C CNN
F 3 "" H 6750 5600 50  0001 C CNN
	1    6750 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FED43F4
P 6150 5600
F 0 "#PWR?" H 6150 5350 50  0001 C CNN
F 1 "GND" H 6155 5427 50  0000 C CNN
F 2 "" H 6150 5600 50  0001 C CNN
F 3 "" H 6150 5600 50  0001 C CNN
	1    6150 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper JP?
U 1 1 5FED4432
P 6450 5600
F 0 "JP?" H 6450 5864 50  0000 C CNN
F 1 "Jumper" H 6450 5773 50  0000 C CNN
F 2 "" H 6450 5600 50  0001 C CNN
F 3 "~" H 6450 5600 50  0001 C CNN
	1    6450 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FED4C8E
P 4700 1750
F 0 "C?" H 4815 1776 50  0000 L CNN
F 1 "0.1uF, 10V" H 4750 1650 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 4738 1600 50  0001 C CNN
F 3 "~" H 4700 1750 50  0001 C CNN
	1    4700 1750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5FED4D7F
P 4100 1800
F 0 "SW?" V 4054 1948 50  0000 L CNN
F 1 "SW_Push" V 4145 1948 50  0000 L CNN
F 2 "" H 4100 2000 50  0001 C CNN
F 3 "" H 4100 2000 50  0001 C CNN
	1    4100 1800
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FED4E7D
P 4100 2050
F 0 "#PWR?" H 4100 1800 50  0001 C CNN
F 1 "GND" H 4105 1877 50  0000 C CNN
F 2 "" H 4100 2050 50  0001 C CNN
F 3 "" H 4100 2050 50  0001 C CNN
	1    4100 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 1600 4700 1600
Connection ~ 4700 1600
Wire Wire Line
	4700 1600 4100 1600
Wire Wire Line
	4100 2050 4100 2000
Wire Wire Line
	4700 1900 4700 2050
Wire Wire Line
	4700 2050 4100 2050
Connection ~ 4100 2050
$Comp
L Device:R R?
U 1 1 5FED7A37
P 3700 3050
F 0 "R?" H 3550 3150 50  0000 C CNN
F 1 "10K" H 3550 3050 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 3630 3050 50  0001 C CNN
F 3 "~" H 3700 3050 50  0001 C CNN
	1    3700 3050
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FED7B93
P 3700 3200
F 0 "#PWR?" H 3700 2950 50  0001 C CNN
F 1 "GND" H 3705 3027 50  0000 C CNN
F 2 "" H 3700 3200 50  0001 C CNN
F 3 "" H 3700 3200 50  0001 C CNN
	1    3700 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2900 3700 2900
Text Label 4400 2900 2    50   ~ 0
BOOT0
Text Notes 4650 3000 2    50   ~ 0
Check Boot0 pin
Text HLabel 6700 2900 2    50   Input ~ 0
TMS
Wire Wire Line
	6400 2900 6700 2900
Text HLabel 6700 3000 2    50   Input ~ 0
TCK
Wire Wire Line
	6400 3000 6700 3000
Text HLabel 6700 3100 2    50   Input ~ 0
TDI
Wire Wire Line
	6400 3100 6700 3100
Text HLabel 6700 3600 2    50   Input ~ 0
TDO
Wire Wire Line
	6700 3600 6400 3600
Text HLabel 4850 4100 0    50   Output ~ 0
CC-LED-R
Text HLabel 4850 3900 0    50   Output ~ 0
CC-LED-G
Wire Wire Line
	5100 3900 4850 3900
Wire Wire Line
	5100 4100 4850 4100
Wire Wire Line
	5100 4200 4850 4200
$Comp
L Device:Crystal Y?
U 1 1 5FEE2B95
P 2850 2950
F 0 "Y?" V 2804 3081 50  0000 L CNN
F 1 "8MHz(16pF)" V 2895 3081 50  0000 L CNN
F 2 "" H 2850 2950 50  0001 C CNN
F 3 "~" H 2850 2950 50  0001 C CNN
	1    2850 2950
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5FEE2BA5
P 2550 2700
F 0 "C?" V 2600 2750 50  0000 L CNN
F 1 "20pF" V 2500 2750 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 2588 2550 50  0001 C CNN
F 3 "~" H 2550 2700 50  0001 C CNN
F 4 "DNP" V 2600 2500 50  0000 L CNN "DNP"
	1    2550 2700
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C?
U 1 1 5FEE2BAD
P 2550 3250
F 0 "C?" V 2600 3300 50  0000 L CNN
F 1 "20pF" V 2500 3300 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 2588 3100 50  0001 C CNN
F 3 "~" H 2550 3250 50  0001 C CNN
F 4 "DNP" V 2600 3050 50  0000 L CNN "DNP"
	1    2550 3250
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5FEE2BB4
P 3100 2700
F 0 "R?" V 3000 2650 50  0000 C CNN
F 1 "0" V 3000 2800 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 3030 2700 50  0001 C CNN
F 3 "~" H 3100 2700 50  0001 C CNN
	1    3100 2700
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5FEE2BBB
P 3150 3250
F 0 "R?" V 3050 3150 50  0000 C CNN
F 1 "0" V 3050 3350 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 3080 3250 50  0001 C CNN
F 3 "~" H 3150 3250 50  0001 C CNN
	1    3150 3250
	0    1    1    0   
$EndComp
Wire Wire Line
	3550 2800 3550 3250
Wire Wire Line
	3550 3250 3300 3250
Wire Wire Line
	2850 2800 2850 2700
Wire Wire Line
	2850 2700 2950 2700
Wire Wire Line
	2850 3100 2850 3250
Wire Wire Line
	2850 3250 3000 3250
Wire Wire Line
	2700 3250 2850 3250
Connection ~ 2850 3250
Wire Wire Line
	2700 2700 2850 2700
Connection ~ 2850 2700
$Comp
L power:GND #PWR?
U 1 1 5FEE2BCF
P 2300 3400
F 0 "#PWR?" H 2300 3150 50  0001 C CNN
F 1 "GND" H 2305 3227 50  0000 C CNN
F 2 "" H 2300 3400 50  0001 C CNN
F 3 "" H 2300 3400 50  0001 C CNN
	1    2300 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 3400 2300 3250
Wire Wire Line
	2300 3250 2400 3250
Wire Wire Line
	2400 2700 2300 2700
Wire Wire Line
	2300 2700 2300 3250
Connection ~ 2300 3250
Text HLabel 4850 4200 0    50   Output ~ 0
CC-LED-Y
Wire Wire Line
	3250 2700 5100 2700
Wire Wire Line
	3550 2800 5100 2800
$Comp
L Device:Crystal_GND23 Y?
U 1 1 5FEE6F6E
P 3950 4800
F 0 "Y?" V 3800 4650 50  0000 L CNN
F 1 "ABS25-32.768KHZ-6-T" V 4050 3700 50  0000 L CNN
F 2 "" H 3950 4800 50  0001 C CNN
F 3 "~" H 3950 4800 50  0001 C CNN
F 4 "DNP" V 4100 4900 50  0000 C CNN "DNP"
	1    3950 4800
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5FEE8703
P 4450 5050
F 0 "R?" V 4350 4950 50  0000 C CNN
F 1 "0" V 4350 5150 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 4380 5050 50  0001 C CNN
F 3 "~" H 4450 5050 50  0001 C CNN
	1    4450 5050
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5FEE8763
P 4450 4700
F 0 "R?" V 4350 4650 50  0000 C CNN
F 1 "0" V 4350 4800 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 4380 4700 50  0001 C CNN
F 3 "~" H 4450 4700 50  0001 C CNN
	1    4450 4700
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5FEE87FF
P 3600 5050
F 0 "C?" V 3700 4900 50  0000 L CNN
F 1 "6.8pF" V 3500 4850 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 3638 4900 50  0001 C CNN
F 3 "~" H 3600 5050 50  0001 C CNN
F 4 "DNP" V 3750 5050 50  0000 C CNN "DNP"
	1    3600 5050
	0    1    1    0   
$EndComp
$Comp
L Device:C C?
U 1 1 5FEE8ADE
P 3600 4600
F 0 "C?" V 3700 4450 50  0000 L CNN
F 1 "6.8pF" V 3500 4400 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 3638 4450 50  0001 C CNN
F 3 "~" H 3600 4600 50  0001 C CNN
F 4 "DNP" V 3750 4600 50  0000 C CNN "DNP"
	1    3600 4600
	0    1    1    0   
$EndComp
Wire Wire Line
	5100 4800 4600 4800
Wire Wire Line
	4600 4800 4600 5050
Wire Wire Line
	5100 4700 4600 4700
Wire Wire Line
	3950 4600 3950 4650
Wire Wire Line
	3950 4950 3950 5050
Wire Wire Line
	3950 4600 3750 4600
Wire Wire Line
	3950 5050 3750 5050
$Comp
L power:GND #PWR?
U 1 1 5FEF2575
P 3300 5150
F 0 "#PWR?" H 3300 4900 50  0001 C CNN
F 1 "GND" H 3305 4977 50  0000 C CNN
F 2 "" H 3300 5150 50  0001 C CNN
F 3 "" H 3300 5150 50  0001 C CNN
	1    3300 5150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 4600 3300 5050
Connection ~ 3300 5050
Wire Wire Line
	3300 5050 3300 5150
NoConn ~ 4150 4800
NoConn ~ 3750 4800
Text HLabel 6700 2500 2    50   Output ~ 0
USART-TX-RPI
Text HLabel 6700 2600 2    50   Input ~ 0
USART-RX-RPI
Text HLabel 6700 2700 2    50   Input ~ 0
USART-CTS-RPI
Text HLabel 6700 2800 2    50   Output ~ 0
USART-RTS-RPI
Wire Wire Line
	6700 2700 6400 2700
Wire Wire Line
	6700 2800 6400 2800
Text HLabel 6700 1800 2    50   Output ~ 0
USART-TX-STLINK
Text HLabel 6700 1900 2    50   Input ~ 0
USART-RX-STLINK
Wire Wire Line
	6400 1900 6700 1900
Text HLabel 6700 1700 2    50   Input ~ 0
INH-FLOW-OUT
Wire Wire Line
	6400 1700 6700 1700
Text Notes 3400 550  0    50   ~ 0
Note: some pinouts changed, inform sw
Text HLabel 7000 2400 2    50   Input ~ 0
EXH-FLOW-OUT
Wire Wire Line
	6400 2400 7000 2400
Text HLabel 6700 3300 2    50   Output ~ 0
SOLENOID-CTRL
Text HLabel 6700 3400 2    50   Output ~ 0
HEATER-CTRL
Wire Wire Line
	6400 3300 6700 3300
Text HLabel 6700 3500 2    50   Output ~ 0
BLOWER-CTRL
Text HLabel 4850 3300 0    50   Input ~ 0
OXYGEN-SENSOR
Wire Wire Line
	5100 3300 4850 3300
Text HLabel 4850 3400 0    50   Input ~ 0
PATIENT-PRESSURE-OUT
Text HLabel 4850 3500 0    50   Input ~ 0
MIXING-TANK-PRESSURE-OUT
Text HLabel 4850 3600 0    50   Input ~ 0
OXYGEN-PRESSURE-OUT
Wire Wire Line
	5100 3400 4850 3400
Wire Wire Line
	5100 3500 4850 3500
Wire Wire Line
	5100 3600 4850 3600
Text HLabel 4850 3700 0    50   Input ~ 0
VIN-SENSE-OUT
Text HLabel 4850 3800 0    50   Input ~ 0
POWER-SWITCH-OVER
Wire Wire Line
	4850 3700 5100 3700
Wire Wire Line
	4850 3800 5100 3800
Text HLabel 7000 3900 2    50   Input ~ 0
OXY-FLOW-OUT
Wire Wire Line
	6400 3700 6700 3700
Text HLabel 4850 4300 0    50   Input ~ 0
SWO
Wire Wire Line
	5100 4300 4850 4300
$Comp
L Switch:SW_Push SW?
U 1 1 5FF60966
P 2700 4550
F 0 "SW?" V 2654 4698 50  0000 L CNN
F 1 "SW_Push" V 2745 4698 50  0000 L CNN
F 2 "" H 2700 4750 50  0001 C CNN
F 3 "" H 2700 4750 50  0001 C CNN
	1    2700 4550
	0    1    1    0   
$EndComp
$Comp
L Device:Jumper JP?
U 1 1 5FF609B4
P 3400 4350
F 0 "JP?" H 3400 4614 50  0000 C CNN
F 1 "Jumper" H 3400 4523 50  0000 C CNN
F 2 "" H 3400 4350 50  0001 C CNN
F 3 "~" H 3400 4350 50  0001 C CNN
	1    3400 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 4550 4700 4600
Wire Wire Line
	4700 4600 5100 4600
Wire Wire Line
	3700 4550 3700 4350
Wire Wire Line
	2700 4350 3100 4350
$Comp
L power:GND #PWR?
U 1 1 5FF73D09
P 2700 4850
F 0 "#PWR?" H 2700 4600 50  0001 C CNN
F 1 "GND" H 2705 4677 50  0000 C CNN
F 2 "" H 2700 4850 50  0001 C CNN
F 3 "" H 2700 4850 50  0001 C CNN
	1    2700 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 4850 2700 4750
$Comp
L Device:R R?
U 1 1 5FF77D45
P 2700 4200
F 0 "R?" H 2550 4300 50  0000 C CNN
F 1 "4.7K" H 2550 4200 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 2630 4200 50  0001 C CNN
F 3 "~" H 2700 4200 50  0001 C CNN
	1    2700 4200
	-1   0    0    1   
$EndComp
Connection ~ 2700 4350
$Comp
L Ventilator-rescue:+3.3V_Ctrl-RespiraWorks #PWR?
U 1 1 5FF77EF3
P 2700 4050
F 0 "#PWR?" H 2700 4050 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 2685 4223 50  0000 C CNN
F 2 "" H 2700 4050 50  0001 C CNN
F 3 "" H 2700 4050 50  0001 C CNN
	1    2700 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FF77F9A
P 2100 4500
F 0 "C?" H 2215 4526 50  0000 L CNN
F 1 "0.1uF, 10V" H 2150 4400 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 2138 4350 50  0001 C CNN
F 3 "~" H 2100 4500 50  0001 C CNN
	1    2100 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5FF77FFC
P 2300 4750
F 0 "R?" V 2150 4850 50  0000 C CNN
F 1 "100" V 2150 4650 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 2230 4750 50  0001 C CNN
F 3 "~" H 2300 4750 50  0001 C CNN
	1    2300 4750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2700 4350 2100 4350
Wire Wire Line
	2150 4750 2100 4750
Wire Wire Line
	2100 4750 2100 4650
Wire Wire Line
	2700 4750 2450 4750
Connection ~ 2700 4750
Text HLabel 6700 4100 2    50   BiDi ~ 0
CC-SCL
Text HLabel 6700 4200 2    50   BiDi ~ 0
CC-SDA
$Comp
L RespiraWorks:+3.3V_Ctrl_A #PWR?
U 1 1 60010B9C
P 8550 750
F 0 "#PWR?" H 8550 750 50  0001 C CNN
F 1 "+3.3V_Ctrl_A" H 8535 923 50  0000 C CNN
F 2 "" H 8550 750 50  0001 C CNN
F 3 "" H 8550 750 50  0001 C CNN
	1    8550 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 750  8350 800 
Wire Wire Line
	8700 750  8700 800 
$Comp
L power:GND #PWR?
U 1 1 5FED05CB
P 5700 5350
F 0 "#PWR?" H 5700 5100 50  0001 C CNN
F 1 "GND" H 5705 5177 50  0000 C CNN
F 2 "" H 5700 5350 50  0001 C CNN
F 3 "" H 5700 5350 50  0001 C CNN
	1    5700 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 5050 3300 5050
Wire Wire Line
	3450 4600 3300 4600
Connection ~ 3950 5050
Connection ~ 3950 4600
Wire Wire Line
	4300 4600 4300 4700
Text HLabel 4800 4450 0    50   Output ~ 0
UART-TX-FPGA
Wire Wire Line
	3700 4550 4700 4550
Text HLabel 4800 3100 0    50   Input ~ 0
UART-RX-FPGA
Wire Wire Line
	4800 3100 5100 3100
Wire Wire Line
	6400 3900 7000 3900
Text HLabel 6700 3700 2    50   Output ~ 0
UART-RTS-FPGA
Text HLabel 6700 3800 2    50   Input ~ 0
UART-CTS-FPGA
Wire Wire Line
	6700 3800 6400 3800
Text HLabel 6700 4000 2    50   Output ~ 0
CC-GPIO
Wire Wire Line
	6400 4000 6700 4000
Text HLabel 10150 4400 0    50   BiDi ~ 0
D8
Text HLabel 10150 5850 0    50   BiDi ~ 0
D2
Text HLabel 10150 5300 0    50   BiDi ~ 0
D15
Text HLabel 10150 5200 0    50   BiDi ~ 0
D14
Text HLabel 10150 4600 0    50   BiDi ~ 0
D10
Text HLabel 10150 4500 0    50   BiDi ~ 0
D9
Text HLabel 6700 4700 2    50   Output ~ 0
CC-BUZZER
Text HLabel 10150 6150 0    50   BiDi ~ 0
D5
Wire Wire Line
	6400 4600 6550 4600
Wire Wire Line
	6400 4500 6550 4500
Wire Wire Line
	6400 4400 6550 4400
Wire Wire Line
	6400 4300 6550 4300
Text HLabel 10150 6050 0    50   BiDi ~ 0
D4
Wire Wire Line
	6400 4800 6550 4800
$Comp
L Ventilator-rescue:+3.3V_Ctrl-RespiraWorks #PWR?
U 1 1 601C83BE
P 9500 750
F 0 "#PWR?" H 9500 750 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 9485 923 50  0000 C CNN
F 2 "" H 9500 750 50  0001 C CNN
F 3 "" H 9500 750 50  0001 C CNN
	1    9500 750 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 601C8478
P 9500 900
F 0 "C?" H 9615 926 50  0000 L CNN
F 1 "1uF, 10V" H 9550 800 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 9538 750 50  0001 C CNN
F 3 "~" H 9500 900 50  0001 C CNN
	1    9500 900 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 601C84D8
P 9500 1050
F 0 "#PWR?" H 9500 800 50  0001 C CNN
F 1 "GND" H 9505 877 50  0000 C CNN
F 2 "" H 9500 1050 50  0001 C CNN
F 3 "" H 9500 1050 50  0001 C CNN
	1    9500 1050
	1    0    0    -1  
$EndComp
Text Notes 9350 550  2    50   ~ 0
VBAT pin
Connection ~ 5700 700 
Wire Wire Line
	5700 700  5700 1400
Connection ~ 5600 700 
Wire Wire Line
	5500 5000 5500 5350
$Comp
L power:GNDA #PWR?
U 1 1 602C1A6B
P 5900 5300
F 0 "#PWR?" H 5900 5050 50  0001 C CNN
F 1 "GNDA" H 5905 5127 50  0000 C CNN
F 2 "" H 5900 5300 50  0001 C CNN
F 3 "" H 5900 5300 50  0001 C CNN
	1    5900 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 5000 5900 5300
$Comp
L RespiraWorks:503182-1852 J?
U 1 1 6016937E
P 2500 6650
AR Path="/5FCD4D4C/6016937E" Ref="J?"  Part="1" 
AR Path="/5FCD4D18/6016937E" Ref="J?"  Part="1" 
F 0 "J?" H 2300 7000 50  0000 L CNN
F 1 "503182-1852" H 2850 6300 50  0000 L CNN
F 2 "RespiraWorks:5031821852" H 3300 6250 50  0001 C CNN
F 3 "https://www.molex.com/pdm_docs/sd/5031821852_sd.pdf" H 3900 6150 50  0001 C CNN
	1    2500 6650
	1    0    0    -1  
$EndComp
Text Notes 2950 5950 2    50   ~ 0
MicroSD Card Connector
$Comp
L Ventilator-rescue:+3.3V_Ctrl-RespiraWorks #PWR?
U 1 1 60169386
P 2700 6250
AR Path="/5FCD4D4C/60169386" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D18/60169386" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2700 6250 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 2685 6423 50  0000 C CNN
F 2 "" H 2700 6250 50  0001 C CNN
F 3 "" H 2700 6250 50  0001 C CNN
	1    2700 6250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 6016938C
P 3150 6350
AR Path="/5FCD4D4C/6016938C" Ref="C?"  Part="1" 
AR Path="/5FCD4D18/6016938C" Ref="C?"  Part="1" 
F 0 "C?" H 3200 6450 50  0000 L CNN
F 1 "1uF/10V" H 3200 6250 39  0000 L CNN
F 2 "" H 3188 6200 50  0001 C CNN
F 3 "~" H 3150 6350 50  0001 C CNN
	1    3150 6350
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 60169393
P 3550 6350
AR Path="/5FCD4D4C/60169393" Ref="C?"  Part="1" 
AR Path="/5FCD4D18/60169393" Ref="C?"  Part="1" 
F 0 "C?" H 3600 6450 50  0000 L CNN
F 1 "0.1uF/16V" H 3600 6250 39  0000 L CNN
F 2 "" H 3588 6200 50  0001 C CNN
F 3 "~" H 3550 6350 50  0001 C CNN
	1    3550 6350
	1    0    0    -1  
$EndComp
$Comp
L Ventilator-rescue:+3.3V_Ctrl-RespiraWorks #PWR?
U 1 1 6016939A
P 3350 6150
AR Path="/5FCD4D4C/6016939A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D18/6016939A" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3350 6150 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 3335 6323 50  0000 C CNN
F 2 "" H 3350 6150 50  0001 C CNN
F 3 "" H 3350 6150 50  0001 C CNN
	1    3350 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 6200 3150 6150
Wire Wire Line
	3150 6150 3350 6150
Wire Wire Line
	3350 6150 3550 6150
Wire Wire Line
	3550 6150 3550 6200
Connection ~ 3350 6150
$Comp
L power:GND #PWR?
U 1 1 601693A5
P 3350 6550
AR Path="/5FCD4D4C/601693A5" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D18/601693A5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3350 6300 50  0001 C CNN
F 1 "GND" H 3355 6377 50  0000 C CNN
F 2 "" H 3350 6550 50  0001 C CNN
F 3 "" H 3350 6550 50  0001 C CNN
	1    3350 6550
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 6500 3350 6500
Wire Wire Line
	3350 6500 3350 6550
Wire Wire Line
	3550 6500 3350 6500
Connection ~ 3350 6500
$Comp
L power:GND #PWR?
U 1 1 601693AF
P 2650 7350
AR Path="/5FCD4D4C/601693AF" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D18/601693AF" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2650 7100 50  0001 C CNN
F 1 "GND" H 2655 7177 50  0000 C CNN
F 2 "" H 2650 7350 50  0001 C CNN
F 3 "" H 2650 7350 50  0001 C CNN
	1    2650 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 7350 2700 7350
Wire Wire Line
	2700 7350 2700 7300
Wire Wire Line
	2600 7300 2600 7350
Wire Wire Line
	2600 7350 2650 7350
Connection ~ 2650 7350
Text Notes 1750 6200 2    50   ~ 0
Double Check sw/dt footprint
NoConn ~ 2050 6500
NoConn ~ 2050 6600
NoConn ~ 2050 7000
Text Label 1750 7100 2    50   ~ 0
USD-Detect
Wire Wire Line
	1050 7100 2050 7100
Wire Wire Line
	1050 7100 1050 6950
$Comp
L Device:R R?
U 1 1 601693C1
P 1050 6800
AR Path="/5FCD4D4C/601693C1" Ref="R?"  Part="1" 
AR Path="/5FCD4D18/601693C1" Ref="R?"  Part="1" 
F 0 "R?" H 900 6850 50  0000 L CNN
F 1 "10K" H 850 6750 50  0000 L CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 980 6800 50  0001 C CNN
F 3 "~" H 1050 6800 50  0001 C CNN
	1    1050 6800
	1    0    0    -1  
$EndComp
$Comp
L Ventilator-rescue:+3.3V_Ctrl-RespiraWorks #PWR?
U 1 1 601693C8
P 1050 6650
AR Path="/5FCD4D4C/601693C8" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D18/601693C8" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1050 6650 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 1035 6823 50  0000 C CNN
F 2 "" H 1050 6650 50  0001 C CNN
F 3 "" H 1050 6650 50  0001 C CNN
	1    1050 6650
	1    0    0    -1  
$EndComp
Wire Notes Line
	500  5850 4050 5850
Wire Notes Line
	4050 5850 4050 7800
NoConn ~ 6400 1600
Wire Wire Line
	5300 1350 5300 1400
Wire Wire Line
	5300 1400 5500 1400
Wire Wire Line
	5300 700  5300 750 
Wire Wire Line
	5600 700  5300 700 
$Comp
L Device:Jumper JP?
U 1 1 601E0703
P 5300 1050
F 0 "JP?" H 5300 1314 50  0000 C CNN
F 1 "Jumper" H 5300 1223 50  0000 C CNN
F 2 "" H 5300 1050 50  0001 C CNN
F 3 "~" H 5300 1050 50  0001 C CNN
	1    5300 1050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5300 1400 4850 1400
Connection ~ 5300 1400
$Comp
L power:+BATT #PWR?
U 1 1 601BF7A9
P 4850 900
F 0 "#PWR?" H 4850 750 50  0001 C CNN
F 1 "+BATT" H 4865 1073 50  0000 C CNN
F 2 "" H 4850 900 50  0001 C CNN
F 3 "" H 4850 900 50  0001 C CNN
	1    4850 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 601CB6EF
P 4600 1100
F 0 "C?" H 4715 1126 50  0000 L CNN
F 1 "1uF, 10V" H 4650 1000 25  0000 L CNN
F 2 "RespiraWorks_std:C_0603_1608Metric" H 4638 950 50  0001 C CNN
F 3 "~" H 4600 1100 50  0001 C CNN
	1    4600 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 900  4850 950 
Wire Wire Line
	4600 950  4850 950 
Connection ~ 4850 950 
Wire Wire Line
	4850 950  4850 1400
$Comp
L power:GND #PWR?
U 1 1 601E377B
P 4600 1250
F 0 "#PWR?" H 4600 1000 50  0001 C CNN
F 1 "GND" H 4605 1077 50  0000 C CNN
F 2 "" H 4600 1250 50  0001 C CNN
F 3 "" H 4600 1250 50  0001 C CNN
	1    4600 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 700  6000 1400
Wire Wire Line
	5600 700  5600 1400
Text Notes 7650 1500 2    39   ~ 0
Had trouble copying symbol to move VDDA to outside
$Comp
L RespiraWorks:+3.3V_Ctrl_A #PWR?
U 1 1 6020D1C2
P 6200 700
F 0 "#PWR?" H 6200 700 50  0001 C CNN
F 1 "+3.3V_Ctrl_A" H 6185 873 50  0000 C CNN
F 2 "" H 6200 700 50  0001 C CNN
F 3 "" H 6200 700 50  0001 C CNN
	1    6200 700 
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 1400 5900 850 
Wire Wire Line
	5900 850  6200 850 
Wire Wire Line
	6200 850  6200 700 
Text Notes 10100 1450 2    39   ~ 0
Match name of power port for VDDA with the one from edwin's branch!
Wire Wire Line
	5600 5350 5500 5350
Connection ~ 5600 5350
Text HLabel 10150 5950 0    50   BiDi ~ 0
D3
Text HLabel 10150 4700 0    50   BiDi ~ 0
D11
Text HLabel 10150 4900 0    50   BiDi ~ 0
D13
Text HLabel 10150 5650 0    50   BiDi ~ 0
D0
Text HLabel 10150 5750 0    50   BiDi ~ 0
D1
Text Label 4350 4000 0    50   ~ 0
USD-Detect
Text Notes 9900 1700 2    39   ~ 0
0.1in headers?
Wire Notes Line
	9350 1550 9350 6550
NoConn ~ 10650 3600
Wire Wire Line
	10650 3700 10150 3700
Wire Wire Line
	10650 3800 10150 3800
Wire Wire Line
	10650 3900 10150 3900
Wire Wire Line
	10650 4000 10150 4000
Wire Wire Line
	10650 4100 10150 4100
Text Label 10150 3700 0    39   ~ 0
INH-FLOW-OUT
Text Label 6400 1700 0    24   ~ 0
INH-FLOW-OUT
Text Label 6450 2400 0    39   ~ 0
EXH-FLOW-OUT
Text Label 10150 3800 0    39   ~ 0
EXH-FLOW-OUT
Text Label 6450 3900 0    39   ~ 0
OXY-FLOW-OUT
Text Label 10150 3900 0    39   ~ 0
OXY-FLOW-OUT
Text Label 5050 3400 2    39   ~ 0
PPO
Text Label 10150 4000 0    39   ~ 0
PPO
Text Label 5050 3300 2    39   ~ 0
OXY
Text Label 10150 4100 0    39   ~ 0
OXY
Text HLabel 10150 6350 0    50   BiDi ~ 0
D7
Wire Wire Line
	10650 4400 10150 4400
Wire Wire Line
	10650 4500 10150 4500
Wire Wire Line
	10650 4600 10150 4600
Wire Wire Line
	10650 4700 10150 4700
Wire Wire Line
	10650 4800 10150 4800
Wire Wire Line
	10650 4900 10150 4900
Wire Wire Line
	10650 5200 10150 5200
Wire Wire Line
	10650 5300 10150 5300
Text Label 10200 4400 0    39   ~ 0
D8
Text Label 10200 4500 0    39   ~ 0
D9
Text Label 10200 4600 0    39   ~ 0
D10
Text Label 10200 5200 0    39   ~ 0
D14
Text Label 10200 5300 0    39   ~ 0
D15
$Comp
L RespiraWorks:+3.3V_Ctrl_A #PWR?
U 1 1 60390EE2
P 9800 4450
F 0 "#PWR?" H 9800 4450 50  0001 C CNN
F 1 "+3.3V_Ctrl_A" H 9785 4623 50  0000 C CNN
F 2 "" H 9800 4450 50  0001 C CNN
F 3 "" H 9800 4450 50  0001 C CNN
	1    9800 4450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60390FBB
P 9900 5350
F 0 "#PWR?" H 9900 5100 50  0001 C CNN
F 1 "GND" H 9905 5177 50  0000 C CNN
F 2 "" H 9900 5350 50  0001 C CNN
F 3 "" H 9900 5350 50  0001 C CNN
	1    9900 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 5000 9900 5000
Wire Wire Line
	9900 5000 9900 5350
Wire Wire Line
	10650 5100 9800 5100
Wire Wire Line
	9800 5100 9800 4450
Wire Notes Line
	9350 1550 11250 1550
Text Label 6550 4300 2    39   ~ 0
D8
Text Label 6550 4600 2    39   ~ 0
D9
Text Label 6550 4500 2    39   ~ 0
D10
Text Label 6550 4400 2    39   ~ 0
D2
Text Label 6550 4800 2    39   ~ 0
D4
Text Label 6450 4200 0    39   ~ 0
D14
Text Label 6450 4100 0    39   ~ 0
D15
Text Label 6450 3500 0    39   ~ 0
D3
Text Label 6450 3400 0    39   ~ 0
D7
Text Label 6450 2600 0    39   ~ 0
D0
Text Label 6450 2500 0    39   ~ 0
D1
Wire Wire Line
	10650 5650 10150 5650
Wire Wire Line
	10650 5750 10150 5750
Wire Wire Line
	10650 5850 10150 5850
Wire Wire Line
	10650 5950 10150 5950
Wire Wire Line
	10650 6050 10150 6050
Wire Wire Line
	10650 6150 10150 6150
Wire Wire Line
	10650 6250 10150 6250
Wire Wire Line
	10650 6350 10150 6350
Text Label 10200 5650 0    39   ~ 0
D0
Text Label 10200 5750 0    39   ~ 0
D1
Text Label 10200 5850 0    39   ~ 0
D2
Text Label 10200 5950 0    39   ~ 0
D3
Text Label 10200 6050 0    39   ~ 0
D4
Text Label 10200 6350 0    39   ~ 0
D7
Text Label 6550 4700 2    39   ~ 0
D5
Text Label 10200 6150 0    39   ~ 0
D5
Text HLabel 10150 6250 0    50   BiDi ~ 0
D6
Text Label 6450 1800 0    39   ~ 0
D6
Text Label 10200 6250 0    39   ~ 0
D6
Wire Wire Line
	1550 6900 2050 6900
Text Label 1550 6900 0    39   ~ 0
USD-SCLK
Wire Wire Line
	1550 6800 2050 6800
Text Label 1550 6800 0    39   ~ 0
USD-MOSI
Wire Wire Line
	1550 6700 2050 6700
Text Label 1550 6700 0    39   ~ 0
USD-CS
Wire Wire Line
	1550 6400 2050 6400
Text Label 1550 6400 0    39   ~ 0
USD-MISO
Wire Wire Line
	6400 2000 6800 2000
Text Label 6800 2000 2    39   ~ 0
USD-CS
Wire Wire Line
	6400 2100 6800 2100
Text Label 6800 2100 2    39   ~ 0
USD-SCLK
Text Label 6800 2200 2    39   ~ 0
USD-MISO
Text Label 6800 2300 2    39   ~ 0
USD-MOSI
Wire Wire Line
	6400 2300 6800 2300
Text HLabel 10150 4800 0    50   BiDi ~ 0
D12
Wire Wire Line
	6400 2200 6800 2200
Wire Wire Line
	6400 1800 6700 1800
Text Label 10200 4900 0    39   ~ 0
USD-SCLK
Text Label 10200 4800 0    39   ~ 0
USD-MISO
Text Label 10200 4700 0    39   ~ 0
USD-MOSI
Wire Wire Line
	6400 2500 6700 2500
Wire Wire Line
	6400 3400 6700 3400
Wire Wire Line
	6400 3500 6700 3500
Wire Wire Line
	6400 4100 6700 4100
Wire Wire Line
	6400 4200 6700 4200
Wire Wire Line
	6400 4700 6700 4700
$Comp
L power:GND #PWR?
U 1 1 60648211
P 4150 5450
F 0 "#PWR?" H 4150 5200 50  0001 C CNN
F 1 "GND" H 4155 5277 50  0000 C CNN
F 2 "" H 4150 5450 50  0001 C CNN
F 3 "" H 4150 5450 50  0001 C CNN
	1    4150 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60648554
P 4250 5250
F 0 "R?" H 4150 5250 50  0000 C CNN
F 1 "0" H 4150 5350 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 4180 5250 50  0001 C CNN
F 3 "~" H 4250 5250 50  0001 C CNN
	1    4250 5250
	-1   0    0    1   
$EndComp
Wire Wire Line
	3950 4600 4250 4600
Wire Wire Line
	3950 5050 4100 5050
$Comp
L Device:R R?
U 1 1 60653848
P 4100 5250
F 0 "R?" H 4200 5250 50  0000 C CNN
F 1 "0" H 4200 5350 50  0000 C CNN
F 2 "RespiraWorks_std:R_0603_1608Metric" V 4030 5250 50  0001 C CNN
F 3 "~" H 4100 5250 50  0001 C CNN
	1    4100 5250
	-1   0    0    1   
$EndComp
Wire Wire Line
	4250 4600 4250 5100
Connection ~ 4250 4600
Wire Wire Line
	4250 4600 4300 4600
Wire Wire Line
	4100 5050 4100 5100
Connection ~ 4100 5050
Wire Wire Line
	4100 5050 4300 5050
Wire Wire Line
	4100 5400 4100 5450
Wire Wire Line
	4100 5450 4150 5450
Wire Wire Line
	4250 5450 4250 5400
Wire Wire Line
	4150 5450 4250 5450
Connection ~ 4150 5450
Wire Wire Line
	4800 4450 4950 4450
Wire Wire Line
	4950 4450 4950 4500
Wire Wire Line
	4950 4500 5100 4500
Wire Wire Line
	4350 4000 5100 4000
NoConn ~ 5100 4400
$Comp
L power:GNDA #PWR?
U 1 1 606ED0BF
P 8550 1150
F 0 "#PWR?" H 8550 900 50  0001 C CNN
F 1 "GNDA" H 8555 977 50  0000 C CNN
F 2 "" H 8550 1150 50  0001 C CNN
F 3 "" H 8550 1150 50  0001 C CNN
	1    8550 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 1600 4100 1600
Connection ~ 4100 1600
Wire Wire Line
	5700 5000 5700 5350
Wire Wire Line
	5600 5000 5600 5350
Wire Wire Line
	6400 2600 6700 2600
Wire Wire Line
	8350 750  8700 750 
$Comp
L Connector_Generic:Conn_01x06 J?
U 1 1 600E75AE
P 10850 3800
F 0 "J?" H 10930 3792 50  0000 L CNN
F 1 "Arduino Connector" H 10500 3400 50  0000 L CNN
F 2 "" H 10850 3800 50  0001 C CNN
F 3 "~" H 10850 3800 50  0001 C CNN
	1    10850 3800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J?
U 1 1 600E7637
P 10850 5950
F 0 "J?" H 10930 5942 50  0000 L CNN
F 1 "Arduino Connector" H 10450 5450 50  0000 L CNN
F 2 "" H 10850 5950 50  0001 C CNN
F 3 "~" H 10850 5950 50  0001 C CNN
	1    10850 5950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x10 J?
U 1 1 600E76C6
P 10850 4800
F 0 "J?" H 10930 4792 50  0000 L CNN
F 1 "Arduino Connector" H 10500 4200 50  0000 L CNN
F 2 "" H 10850 4800 50  0001 C CNN
F 3 "~" H 10850 4800 50  0001 C CNN
	1    10850 4800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J?
U 1 1 600F2AA2
P 10850 2950
F 0 "J?" H 10930 2942 50  0000 L CNN
F 1 "Arduino Connector" H 10450 2450 50  0000 L CNN
F 2 "" H 10850 2950 50  0001 C CNN
F 3 "~" H 10850 2950 50  0001 C CNN
	1    10850 2950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 600F2B33
P 10850 1950
F 0 "J?" H 10930 1992 50  0000 L CNN
F 1 "Optical end stop 1" H 9600 1950 50  0000 L CNN
F 2 "" H 10850 1950 50  0001 C CNN
F 3 "~" H 10850 1950 50  0001 C CNN
	1    10850 1950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 600F2BC2
P 10850 2350
F 0 "J?" H 10930 2392 50  0000 L CNN
F 1 "Optical end stop 2" H 9600 2400 50  0000 L CNN
F 2 "" H 10850 2350 50  0001 C CNN
F 3 "~" H 10850 2350 50  0001 C CNN
	1    10850 2350
	1    0    0    -1  
$EndComp
NoConn ~ 10650 2650
Text Label 3850 1600 0    50   ~ 0
NRST
Text Label 10350 2850 0    50   ~ 0
NRST
Wire Wire Line
	10650 2850 10350 2850
Wire Wire Line
	10650 3150 9750 3150
Wire Wire Line
	9750 3150 9750 3250
Wire Wire Line
	10650 3250 9750 3250
Connection ~ 9750 3250
Wire Wire Line
	9750 3250 9750 3550
$Comp
L power:GND #PWR?
U 1 1 601571EF
P 9750 3550
F 0 "#PWR?" H 9750 3300 50  0001 C CNN
F 1 "GND" H 9755 3377 50  0000 C CNN
F 2 "" H 9750 3550 50  0001 C CNN
F 3 "" H 9750 3550 50  0001 C CNN
	1    9750 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 3350 9850 3350
Wire Wire Line
	9850 3350 9850 3050
$Comp
L power:+5V #PWR?
U 1 1 60165EBB
P 9850 2650
F 0 "#PWR?" H 9850 2500 50  0001 C CNN
F 1 "+5V" H 9865 2823 50  0000 C CNN
F 2 "" H 9850 2650 50  0001 C CNN
F 3 "" H 9850 2650 50  0001 C CNN
	1    9850 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 3050 9850 3050
Connection ~ 9850 3050
Wire Wire Line
	9850 3050 9850 2650
Wire Wire Line
	10650 2950 10200 2950
Wire Wire Line
	10200 2950 10200 2750
$Comp
L Ventilator-rescue:+3.3V_Ctrl-RespiraWorks #PWR?
U 1 1 6017D8F4
P 10200 2650
F 0 "#PWR?" H 10200 2650 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 10185 2823 50  0000 C CNN
F 2 "" H 10200 2650 50  0001 C CNN
F 3 "" H 10200 2650 50  0001 C CNN
	1    10200 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 2750 10200 2750
Connection ~ 10200 2750
Wire Wire Line
	10200 2750 10200 2650
$Comp
L power:+5V #PWR?
U 1 1 6020E488
P 10350 1800
F 0 "#PWR?" H 10350 1650 50  0001 C CNN
F 1 "+5V" H 10365 1973 50  0000 C CNN
F 2 "" H 10350 1800 50  0001 C CNN
F 3 "" H 10350 1800 50  0001 C CNN
	1    10350 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 2250 10350 2250
Wire Wire Line
	10350 2250 10350 1850
Wire Wire Line
	10650 1850 10350 1850
Connection ~ 10350 1850
Wire Wire Line
	10350 1850 10350 1800
$Comp
L power:GND #PWR?
U 1 1 60226FFE
P 10500 2450
F 0 "#PWR?" H 10500 2200 50  0001 C CNN
F 1 "GND" H 10505 2277 50  0000 C CNN
F 2 "" H 10500 2450 50  0001 C CNN
F 3 "" H 10500 2450 50  0001 C CNN
	1    10500 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10650 2050 10500 2050
Wire Wire Line
	10500 2050 10500 2450
Wire Wire Line
	10650 2450 10500 2450
Connection ~ 10500 2450
Text Label 10550 1950 0    39   ~ 0
D6
Text Label 10550 2350 0    39   ~ 0
D7
Wire Wire Line
	10650 1950 10550 1950
Wire Wire Line
	10650 2350 10550 2350
$EndSCHEMATC
