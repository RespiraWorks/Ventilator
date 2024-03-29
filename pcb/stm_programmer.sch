EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 37 38
Title "STM Programmer for Cycle Controller"
Date ""
Rev "A"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RespiraWorks:STM32F103CxTx U30
U 1 1 5FEEC109
P 7750 3700
F 0 "U30" H 7500 5450 50  0000 C CNN
F 1 "STM32F103CBT6" H 7600 5350 50  0000 C CNN
F 2 "RespiraWorks_Std:LQFP-48_7x7mm_P0.5mm" H 7150 2000 50  0001 R CNN
F 3 "" H 7700 3350 50  0001 C CNN
F 4 "STMicroelectronics" H 7750 3700 50  0001 C CNN "Manufacturer"
F 5 "STM32F103CBT6" H 7750 3700 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7750 3700 50  0001 C CNN "DNP"
	1    7750 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEF0F3B
P 8250 1600
AR Path="/5FCD4B8E/5FCD4BC5/5FEF0F3B" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF0F3B" Ref="C?"  Part="1" 
AR Path="/5FCD4D85/5FEF0F3B" Ref="C182"  Part="1" 
F 0 "C182" V 8000 1600 50  0000 L CNN
F 1 "100nF" V 8100 1550 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 8288 1450 50  0001 C CNN
F 3 "~" H 8250 1600 50  0001 C CNN
F 4 "X7R" H 8250 1600 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 8250 1600 50  0001 C CNN "Manufacturer"
F 6 "C0603C104K5RACTU" H 8250 1600 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 8250 1600 50  0001 C CNN "Voltage"
F 8 " " H 8250 1600 50  0001 C CNN "DNP"
	1    8250 1600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FEF0F41
P 8250 1750
AR Path="/5FCD4B8E/5FCD4BC5/5FEF0F41" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF0F41" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FEF0F41" Ref="#PWR0434"  Part="1" 
F 0 "#PWR0434" H 8250 1500 50  0001 C CNN
F 1 "GND" H 8255 1577 50  0000 C CNN
F 2 "" H 8250 1750 50  0001 C CNN
F 3 "" H 8250 1750 50  0001 C CNN
	1    8250 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEF14C7
P 8650 1600
AR Path="/5FCD4B8E/5FCD4BC5/5FEF14C7" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF14C7" Ref="C?"  Part="1" 
AR Path="/5FCD4D85/5FEF14C7" Ref="C183"  Part="1" 
F 0 "C183" V 8400 1600 50  0000 L CNN
F 1 "100nF" V 8500 1550 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 8688 1450 50  0001 C CNN
F 3 "~" H 8650 1600 50  0001 C CNN
F 4 "X7R" H 8650 1600 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 8650 1600 50  0001 C CNN "Manufacturer"
F 6 "C0603C104K5RACTU" H 8650 1600 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 8650 1600 50  0001 C CNN "Voltage"
F 8 " " H 8650 1600 50  0001 C CNN "DNP"
	1    8650 1600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FEF14CD
P 8650 1750
AR Path="/5FCD4B8E/5FCD4BC5/5FEF14CD" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF14CD" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FEF14CD" Ref="#PWR0435"  Part="1" 
F 0 "#PWR0435" H 8650 1500 50  0001 C CNN
F 1 "GND" H 8655 1577 50  0000 C CNN
F 2 "" H 8650 1750 50  0001 C CNN
F 3 "" H 8650 1750 50  0001 C CNN
	1    8650 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEF1925
P 9050 1600
AR Path="/5FCD4B8E/5FCD4BC5/5FEF1925" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF1925" Ref="C?"  Part="1" 
AR Path="/5FCD4D85/5FEF1925" Ref="C184"  Part="1" 
F 0 "C184" V 8800 1600 50  0000 L CNN
F 1 "100nF" V 8900 1550 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 9088 1450 50  0001 C CNN
F 3 "~" H 9050 1600 50  0001 C CNN
F 4 "X7R" H 9050 1600 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 9050 1600 50  0001 C CNN "Manufacturer"
F 6 "C0603C104K5RACTU" H 9050 1600 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 9050 1600 50  0001 C CNN "Voltage"
F 8 " " H 9050 1600 50  0001 C CNN "DNP"
	1    9050 1600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FEF192B
P 9050 1750
AR Path="/5FCD4B8E/5FCD4BC5/5FEF192B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF192B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FEF192B" Ref="#PWR0438"  Part="1" 
F 0 "#PWR0438" H 9050 1500 50  0001 C CNN
F 1 "GND" H 9055 1577 50  0000 C CNN
F 2 "" H 9050 1750 50  0001 C CNN
F 3 "" H 9050 1750 50  0001 C CNN
	1    9050 1750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FEF1DD5
P 9450 1600
AR Path="/5FCD4B8E/5FCD4BC5/5FEF1DD5" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF1DD5" Ref="C?"  Part="1" 
AR Path="/5FCD4D85/5FEF1DD5" Ref="C185"  Part="1" 
F 0 "C185" V 9200 1600 50  0000 L CNN
F 1 "100nF" V 9300 1550 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 9488 1450 50  0001 C CNN
F 3 "~" H 9450 1600 50  0001 C CNN
F 4 "X7R" H 9450 1600 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 9450 1600 50  0001 C CNN "Manufacturer"
F 6 "C0603C104K5RACTU" H 9450 1600 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 9450 1600 50  0001 C CNN "Voltage"
F 8 " " H 9450 1600 50  0001 C CNN "DNP"
	1    9450 1600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FEF1DDB
P 9450 1750
AR Path="/5FCD4B8E/5FCD4BC5/5FEF1DDB" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF1DDB" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FEF1DDB" Ref="#PWR0442"  Part="1" 
F 0 "#PWR0442" H 9450 1500 50  0001 C CNN
F 1 "GND" H 9455 1577 50  0000 C CNN
F 2 "" H 9450 1750 50  0001 C CNN
F 3 "" H 9450 1750 50  0001 C CNN
	1    9450 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 2250 7950 2150
Wire Wire Line
	7950 1450 8250 1450
Connection ~ 8250 1450
Wire Wire Line
	8250 1450 8650 1450
Connection ~ 8650 1450
Wire Wire Line
	8650 1450 9050 1450
Connection ~ 9050 1450
Wire Wire Line
	9050 1450 9450 1450
Wire Wire Line
	7850 2250 7850 2150
Wire Wire Line
	7850 2150 7950 2150
Connection ~ 7950 2150
Wire Wire Line
	7950 2150 7950 1450
Wire Wire Line
	7750 2250 7750 2150
Wire Wire Line
	7750 2150 7850 2150
Connection ~ 7850 2150
Wire Wire Line
	7650 2250 7650 2150
Wire Wire Line
	7650 2150 7750 2150
Connection ~ 7750 2150
Wire Wire Line
	7550 2250 7550 2150
Wire Wire Line
	7550 2150 7650 2150
Connection ~ 7650 2150
$Comp
L Device:C C?
U 1 1 5FEF4F07
P 6550 2000
AR Path="/5FCD4B8E/5FCD4BC5/5FEF4F07" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF4F07" Ref="C?"  Part="1" 
AR Path="/5FCD4D85/5FEF4F07" Ref="C181"  Part="1" 
F 0 "C181" H 6750 1950 50  0000 L CNN
F 1 "100nF" H 6700 2050 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6588 1850 50  0001 C CNN
F 3 "~" H 6550 2000 50  0001 C CNN
F 4 "X7R" H 6550 2000 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 6550 2000 50  0001 C CNN "Manufacturer"
F 6 "C0603C104K5RACTU" H 6550 2000 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 6550 2000 50  0001 C CNN "Voltage"
F 8 " " H 6550 2000 50  0001 C CNN "DNP"
	1    6550 2000
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FEF4F0D
P 6550 2150
AR Path="/5FCD4B8E/5FCD4BC5/5FEF4F0D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEF4F0D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FEF4F0D" Ref="#PWR0431"  Part="1" 
F 0 "#PWR0431" H 6550 1900 50  0001 C CNN
F 1 "GND" H 6555 1977 50  0000 C CNN
F 2 "" H 6550 2150 50  0001 C CNN
F 3 "" H 6550 2150 50  0001 C CNN
	1    6550 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5FEFCD09
P 6550 1700
AR Path="/5FCD4B8E/5FCD4BC5/5FEFCD09" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FEFCD09" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FEFCD09" Ref="R258"  Part="1" 
F 0 "R258" H 6300 1750 50  0000 L CNN
F 1 "100k" H 6100 1650 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 6480 1700 50  0001 C CNN
F 3 "~" H 6550 1700 50  0001 C CNN
F 4 "Yageo" H 6550 1700 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-07100KL" H 6550 1700 50  0001 C CNN "Manufacturer PN"
F 6 " " H 6550 1700 50  0001 C CNN "DNP"
	1    6550 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 1850 7000 1850
Connection ~ 6550 1850
Wire Wire Line
	7000 2450 7000 1850
$Comp
L power:GND #PWR?
U 1 1 5FF08D73
P 7600 5250
AR Path="/5FCD4B8E/5FCD4BC5/5FF08D73" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF08D73" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FF08D73" Ref="#PWR0432"  Part="1" 
F 0 "#PWR0432" H 7600 5000 50  0001 C CNN
F 1 "GND" H 7605 5077 50  0000 C CNN
F 2 "" H 7600 5250 50  0001 C CNN
F 3 "" H 7600 5250 50  0001 C CNN
	1    7600 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 5250 7700 5250
Connection ~ 7600 5250
Connection ~ 7700 5250
Wire Wire Line
	7700 5250 7800 5250
Connection ~ 7800 5250
Wire Wire Line
	7800 5250 7900 5250
$Comp
L Device:R R?
U 1 1 5FF0A33E
P 9150 3800
AR Path="/5FCD4B8E/5FCD4BC5/5FF0A33E" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF0A33E" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF0A33E" Ref="R260"  Part="1" 
F 0 "R260" H 8850 3900 50  0000 L CNN
F 1 "100" H 8900 3800 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 9080 3800 50  0001 C CNN
F 3 "~" H 9150 3800 50  0001 C CNN
F 4 "Yageo" H 9150 3800 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-07100RL" H 9150 3800 50  0001 C CNN "Manufacturer PN"
F 6 " " H 9150 3800 50  0001 C CNN "DNP"
	1    9150 3800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8450 3750 9050 3750
$Comp
L Device:R R?
U 1 1 5FF10D79
P 6400 2650
AR Path="/5FCD4B8E/5FCD4BC5/5FF10D79" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF10D79" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF10D79" Ref="R257"  Part="1" 
F 0 "R257" V 6200 2600 50  0000 L CNN
F 1 "100k" V 6300 2500 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 6330 2650 50  0001 C CNN
F 3 "~" H 6400 2650 50  0001 C CNN
F 4 "Yageo" H 6400 2650 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-07100KL" H 6400 2650 50  0001 C CNN "Manufacturer PN"
F 6 " " H 6400 2650 50  0001 C CNN "DNP"
	1    6400 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	7000 2650 6550 2650
$Comp
L power:GND #PWR?
U 1 1 5FF14111
P 6050 2650
AR Path="/5FCD4B8E/5FCD4BC5/5FF14111" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF14111" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FF14111" Ref="#PWR0429"  Part="1" 
F 0 "#PWR0429" H 6050 2400 50  0001 C CNN
F 1 "GND" H 6055 2477 50  0000 C CNN
F 2 "" H 6050 2650 50  0001 C CNN
F 3 "" H 6050 2650 50  0001 C CNN
	1    6050 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	6250 2650 6050 2650
$Comp
L Connector:USB_B_Micro J?
U 1 1 5FF1A7AE
P 1150 2500
AR Path="/5FF1A7AE" Ref="J?"  Part="1" 
AR Path="/5FCD4D85/5FF1A7AE" Ref="J50"  Part="1" 
F 0 "J50" H 1050 2950 50  0000 C CNN
F 1 "USB B micro" H 1150 2850 50  0000 C CNN
F 2 "RespiraWorks_Std:USB_Micro-B_Molex-105017-0001" H 1300 2450 50  0001 C CNN
F 3 "~" H 1300 2450 50  0001 C CNN
F 4 "Wuerth Elektronik" H 1150 2500 50  0001 C CNN "Manufacturer"
F 5 "65100516121" H 1150 2500 50  0001 C CNN "Manufacturer PN"
F 6 " " H 1150 2500 50  0001 C CNN "DNP"
	1    1150 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FF23C6F
P 1150 2900
AR Path="/5FCD4B8E/5FCD4BC5/5FF23C6F" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF23C6F" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FF23C6F" Ref="#PWR0415"  Part="1" 
F 0 "#PWR0415" H 1150 2650 50  0001 C CNN
F 1 "GND" H 1155 2727 50  0000 C CNN
F 2 "" H 1150 2900 50  0001 C CNN
F 3 "" H 1150 2900 50  0001 C CNN
	1    1150 2900
	1    0    0    -1  
$EndComp
Connection ~ 1150 2900
$Comp
L Device:R R?
U 1 1 5FF24FD8
P 1600 2850
AR Path="/5FCD4B8E/5FCD4BC5/5FF24FD8" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF24FD8" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF24FD8" Ref="R246"  Part="1" 
F 0 "R246" H 1350 2800 50  0000 L CNN
F 1 "100k" H 1350 2900 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 1530 2850 50  0001 C CNN
F 3 "~" H 1600 2850 50  0001 C CNN
F 4 "Yageo" H 1600 2850 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-07100KL" H 1600 2850 50  0001 C CNN "Manufacturer PN"
F 6 " " H 1600 2850 50  0001 C CNN "DNP"
	1    1600 2850
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FF25BD1
P 1600 3000
AR Path="/5FCD4B8E/5FCD4BC5/5FF25BD1" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF25BD1" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FF25BD1" Ref="#PWR0416"  Part="1" 
F 0 "#PWR0416" H 1600 2750 50  0001 C CNN
F 1 "GND" H 1605 2827 50  0000 C CNN
F 2 "" H 1600 3000 50  0001 C CNN
F 3 "" H 1600 3000 50  0001 C CNN
	1    1600 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5FF26F08
P 1950 2500
AR Path="/5FCD4B8E/5FCD4BC5/5FF26F08" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF26F08" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF26F08" Ref="R248"  Part="1" 
F 0 "R248" V 1750 2450 50  0000 L CNN
F 1 "0" V 1850 2450 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 1880 2500 50  0001 C CNN
F 3 "~" H 1950 2500 50  0001 C CNN
F 4 "Yageo" H 1950 2500 50  0001 C CNN "Manufacturer"
F 5 "RC0603JR-070RL" H 1950 2500 50  0001 C CNN "Manufacturer PN"
F 6 " " H 1950 2500 50  0001 C CNN "DNP"
	1    1950 2500
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 5FF27747
P 1950 2600
AR Path="/5FCD4B8E/5FCD4BC5/5FF27747" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF27747" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF27747" Ref="R249"  Part="1" 
F 0 "R249" V 1750 2400 50  0000 L CNN
F 1 "0" V 1850 2550 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 1880 2600 50  0001 C CNN
F 3 "~" H 1950 2600 50  0001 C CNN
F 4 "Yageo" H 1950 2600 50  0001 C CNN "Manufacturer"
F 5 "RC0603JR-070RL" H 1950 2600 50  0001 C CNN "Manufacturer PN"
F 6 " " H 1950 2600 50  0001 C CNN "DNP"
	1    1950 2600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1800 2500 1750 2500
Text Label 2550 2600 2    50   ~ 0
USB-D-
Text Label 2550 2500 2    50   ~ 0
USB-D+
Wire Wire Line
	2100 2500 2550 2500
Wire Wire Line
	2100 2600 2550 2600
Wire Wire Line
	7000 4400 6500 4400
Wire Wire Line
	7000 4300 6500 4300
Text Label 6500 4400 0    50   ~ 0
USB-D+
Text Label 6500 4300 0    50   ~ 0
USB-D-
$Comp
L Device:R R?
U 1 1 5FF302E3
P 1750 1900
AR Path="/5FCD4B8E/5FCD4BC5/5FF302E3" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF302E3" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF302E3" Ref="R247"  Part="1" 
F 0 "R247" H 1500 1850 50  0000 L CNN
F 1 "1.5k" H 1500 1950 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 1680 1900 50  0001 C CNN
F 3 "~" H 1750 1900 50  0001 C CNN
F 4 "Yageo" H 1750 1900 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071K5L" H 1750 1900 50  0001 C CNN "Manufacturer PN"
F 6 " " H 1750 1900 50  0001 C CNN "DNP"
	1    1750 1900
	-1   0    0    1   
$EndComp
Wire Wire Line
	1750 2050 1750 2500
Connection ~ 1750 2500
$Comp
L Device:Q_NPN_BEC Q27
U 1 1 5FF31A97
P 1850 1550
F 0 "Q27" H 1700 1700 50  0000 L CNN
F 1 "MMBT3904" H 1450 1800 50  0000 L CNN
F 2 "RespiraWorks_Std:SOT-23" H 2050 1650 50  0001 C CNN
F 3 "~" H 1850 1550 50  0001 C CNN
F 4 "Diodes Inc." H 1850 1550 50  0001 C CNN "Manufacturer"
F 5 "MMBT3904-7-F" H 1850 1550 50  0001 C CNN "Manufacturer PN"
F 6 " " H 1850 1550 50  0001 C CNN "DNP"
	1    1850 1550
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5FF3BDCF
P 2350 1400
AR Path="/5FCD4B8E/5FCD4BC5/5FF3BDCF" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF3BDCF" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF3BDCF" Ref="R250"  Part="1" 
F 0 "R250" H 2100 1350 50  0000 L CNN
F 1 "10k" H 2150 1450 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2280 1400 50  0001 C CNN
F 3 "~" H 2350 1400 50  0001 C CNN
F 4 "Yageo" H 2350 1400 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-0710KL" H 2350 1400 50  0001 C CNN "Manufacturer PN"
F 6 " " H 2350 1400 50  0001 C CNN "DNP"
	1    2350 1400
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5FF3C26A
P 2350 1700
AR Path="/5FCD4B8E/5FCD4BC5/5FF3C26A" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF3C26A" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF3C26A" Ref="R251"  Part="1" 
F 0 "R251" H 2100 1650 50  0000 L CNN
F 1 "39k" H 2150 1750 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2280 1700 50  0001 C CNN
F 3 "~" H 2350 1700 50  0001 C CNN
F 4 "Yageo" H 2350 1700 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-0739KL" H 2350 1700 50  0001 C CNN "Manufacturer PN"
F 6 " " H 2350 1700 50  0001 C CNN "DNP"
	1    2350 1700
	-1   0    0    1   
$EndComp
Wire Wire Line
	2050 1550 2350 1550
Connection ~ 2350 1550
$Comp
L power:GND #PWR?
U 1 1 5FF3DB70
P 2350 1850
AR Path="/5FCD4B8E/5FCD4BC5/5FF3DB70" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF3DB70" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FF3DB70" Ref="#PWR0422"  Part="1" 
F 0 "#PWR0422" H 2350 1600 50  0001 C CNN
F 1 "GND" H 2355 1677 50  0000 C CNN
F 2 "" H 2350 1850 50  0001 C CNN
F 3 "" H 2350 1850 50  0001 C CNN
	1    2350 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5FF418B2
P 2950 1550
AR Path="/5FCD4B8E/5FCD4BC5/5FF418B2" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF418B2" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FF418B2" Ref="R252"  Part="1" 
F 0 "R252" V 2750 1450 50  0000 L CNN
F 1 "100" V 2850 1450 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2880 1550 50  0001 C CNN
F 3 "~" H 2950 1550 50  0001 C CNN
F 4 "Yageo" H 2950 1550 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-07100RL" H 2950 1550 50  0001 C CNN "Manufacturer PN"
F 6 " " H 2950 1550 50  0001 C CNN "DNP"
	1    2950 1550
	0    1    1    0   
$EndComp
Wire Wire Line
	2800 1550 2350 1550
Text Label 3650 1550 2    50   ~ 0
nRenum-USB
Wire Wire Line
	3100 1550 3650 1550
Wire Wire Line
	7000 4700 6500 4700
Text Label 6500 4700 0    50   ~ 0
nRenum-USB
$Comp
L Connector_Generic:Conn_01x06 J51
U 1 1 5FF4C966
P 1550 5350
F 0 "J51" H 1468 5767 50  0000 C CNN
F 1 "Backup debug in for cycle controller" H 1750 5700 50  0000 C CNN
F 2 "RespiraWorks_Std:PinHeader_1x06_P2.54mm_Vertical" H 1550 5350 50  0001 C CNN
F 3 "~" H 1550 5350 50  0001 C CNN
F 4 "Harwin" H 1550 5350 50  0001 C CNN "Manufacturer"
F 5 "M20-9980345" H 1550 5350 50  0001 C CNN "Manufacturer PN"
F 6 "DNP" H 1550 4900 50  0000 C CNN "DNP"
	1    1550 5350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1750 5150 2100 5150
Wire Wire Line
	2100 5150 2100 4900
$Comp
L Device:C C?
U 1 1 5FF56FF1
P 4650 3300
AR Path="/5FCD4B8E/5FCD4BC5/5FF56FF1" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF56FF1" Ref="C?"  Part="1" 
AR Path="/5FCD4D85/5FF56FF1" Ref="C179"  Part="1" 
F 0 "C179" H 4800 3250 50  0000 L CNN
F 1 "18pF" H 4800 3350 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 4688 3150 50  0001 C CNN
F 3 "~" H 4650 3300 50  0001 C CNN
F 4 "NP0" H 4650 3300 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 4650 3300 50  0001 C CNN "Manufacturer"
F 6 "C0603C180J5GACTU" H 4650 3300 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 4650 3300 50  0001 C CNN "Voltage"
F 8 " " H 4650 3300 50  0001 C CNN "DNP"
	1    4650 3300
	-1   0    0    1   
$EndComp
$Comp
L Device:C C?
U 1 1 5FF5ADBF
P 5150 3300
AR Path="/5FCD4B8E/5FCD4BC5/5FF5ADBF" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF5ADBF" Ref="C?"  Part="1" 
AR Path="/5FCD4D85/5FF5ADBF" Ref="C180"  Part="1" 
F 0 "C180" H 5300 3250 50  0000 L CNN
F 1 "18pF" H 5300 3350 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 5188 3150 50  0001 C CNN
F 3 "~" H 5150 3300 50  0001 C CNN
F 4 "NP0" H 5150 3300 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 5150 3300 50  0001 C CNN "Manufacturer"
F 6 "C0603C180J5GACTU" H 5150 3300 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 5150 3300 50  0001 C CNN "Voltage"
F 8 " " H 5150 3300 50  0001 C CNN "DNP"
	1    5150 3300
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FF62DD3
P 4650 3450
AR Path="/5FCD4B8E/5FCD4BC5/5FF62DD3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF62DD3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FF62DD3" Ref="#PWR0424"  Part="1" 
F 0 "#PWR0424" H 4650 3200 50  0001 C CNN
F 1 "GND" H 4655 3277 50  0000 C CNN
F 2 "" H 4650 3450 50  0001 C CNN
F 3 "" H 4650 3450 50  0001 C CNN
	1    4650 3450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FF6336B
P 5150 3450
AR Path="/5FCD4B8E/5FCD4BC5/5FF6336B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF6336B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FF6336B" Ref="#PWR0426"  Part="1" 
F 0 "#PWR0426" H 5150 3200 50  0001 C CNN
F 1 "GND" H 5155 3277 50  0000 C CNN
F 2 "" H 5150 3450 50  0001 C CNN
F 3 "" H 5150 3450 50  0001 C CNN
	1    5150 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:Crystal Y3
U 1 1 5FF63F36
P 4900 3150
F 0 "Y3" H 4900 3400 50  0000 C CNN
F 1 "8 Mhz" H 4900 3327 50  0000 C CNN
F 2 "RespiraWorks_Std:Crystal_SMD_HC49-SD" H 4900 3150 50  0001 C CNN
F 3 "~" H 4900 3150 50  0001 C CNN
F 4 "Abracon" H 4900 3150 50  0001 C CNN "Manufacturer"
F 5 "ABLS-8.000MHZ-B4-T" H 4900 3150 50  0001 C CNN "Manufacturer PN"
F 6 " " H 4900 3150 50  0001 C CNN "DNP"
	1    4900 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 3150 5150 3150
Wire Wire Line
	4750 3150 4650 3150
Wire Wire Line
	7000 2950 5150 2950
Wire Wire Line
	5150 2950 5150 3150
Connection ~ 5150 3150
Wire Wire Line
	7000 2850 4650 2850
Wire Wire Line
	4650 2850 4650 3150
Connection ~ 4650 3150
Wire Wire Line
	7000 3700 6250 3700
Text Label 6250 3700 0    50   ~ 0
CC-SWCLK-Out
Text Label 3800 5250 2    50   ~ 0
CC-SWCLK-Out
$Comp
L Device:Jumper_NC_Small JP14
U 1 1 5FF6C997
P 2800 5250
F 0 "JP14" H 2950 5300 50  0000 C CNN
F 1 "Jumper_NC_Small" H 2800 5371 50  0001 C CNN
F 2 "RespiraWorks_Std:SolderJumper-2_P1.3mm_Bridged_RoundedPad1.0x1.5mm" H 2800 5250 50  0001 C CNN
F 3 "~" H 2800 5250 50  0001 C CNN
F 4 " " H 2800 5250 50  0001 C CNN "DNP"
	1    2800 5250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2900 5250 3800 5250
Wire Wire Line
	1750 5250 2650 5250
$Comp
L Device:Jumper_NC_Small JP17
U 1 1 5FF73510
P 2800 5650
F 0 "JP17" H 2950 5700 50  0000 C CNN
F 1 "Jumper_NC_Small" H 2800 5771 50  0001 C CNN
F 2 "RespiraWorks_Std:SolderJumper-2_P1.3mm_Bridged_RoundedPad1.0x1.5mm" H 2800 5650 50  0001 C CNN
F 3 "~" H 2800 5650 50  0001 C CNN
F 4 " " H 2800 5650 50  0001 C CNN "DNP"
	1    2800 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Small JP15
U 1 1 5FF73711
P 2800 5450
F 0 "JP15" H 2950 5500 50  0000 C CNN
F 1 "Jumper_NC_Small" H 2800 5571 50  0001 C CNN
F 2 "RespiraWorks_Std:SolderJumper-2_P1.3mm_Bridged_RoundedPad1.0x1.5mm" H 2800 5450 50  0001 C CNN
F 3 "~" H 2800 5450 50  0001 C CNN
F 4 " " H 2800 5450 50  0001 C CNN "DNP"
	1    2800 5450
	1    0    0    -1  
$EndComp
$Comp
L Device:Jumper_NC_Small JP16
U 1 1 5FF73959
P 2800 5550
F 0 "JP16" H 2950 5600 50  0000 C CNN
F 1 "Jumper_NC_Small" H 2800 5671 50  0001 C CNN
F 2 "RespiraWorks_Std:SolderJumper-2_P1.3mm_Bridged_RoundedPad1.0x1.5mm" H 2800 5550 50  0001 C CNN
F 3 "~" H 2800 5550 50  0001 C CNN
F 4 " " H 2800 5550 50  0001 C CNN "DNP"
	1    2800 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FF74B1D
P 1850 5800
AR Path="/5FCD4B8E/5FCD4BC5/5FF74B1D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FF74B1D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FF74B1D" Ref="#PWR0418"  Part="1" 
F 0 "#PWR0418" H 1850 5550 50  0001 C CNN
F 1 "GND" H 1855 5627 50  0000 C CNN
F 2 "" H 1850 5800 50  0001 C CNN
F 3 "" H 1850 5800 50  0001 C CNN
	1    1850 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 5350 1850 5350
Wire Wire Line
	1850 5350 1850 5800
Wire Wire Line
	1750 5450 2500 5450
Wire Wire Line
	1750 5550 2550 5550
Wire Wire Line
	2700 5650 2600 5650
Wire Wire Line
	2900 5450 3800 5450
Wire Wire Line
	2900 5550 3800 5550
Wire Wire Line
	2900 5650 3800 5650
Text Label 3800 5450 2    50   ~ 0
CC-SWDIO-Out
Text Label 3800 5550 2    50   ~ 0
CC-nRST-Out
Text Label 3800 5650 2    50   ~ 0
CC-SWO-Out
Wire Wire Line
	9050 3750 9050 3650
Wire Wire Line
	9050 3650 9150 3650
Connection ~ 9150 3950
Wire Wire Line
	9150 3950 9850 3950
Wire Wire Line
	8450 3950 9150 3950
Text Label 9850 3950 2    50   ~ 0
CC-SWDIO-Out
Text Label 9000 2550 2    50   ~ 0
CC-nRST-Out
Wire Wire Line
	7000 4200 6500 4200
Text Label 6500 4200 0    50   ~ 0
CC-SWO-Out
Wire Wire Line
	7000 4500 6500 4500
Text Label 6500 4500 0    50   ~ 0
Prog-SWDIO
Wire Wire Line
	7000 4600 6500 4600
Text Label 6500 4600 0    50   ~ 0
Prog-SWCLK
Text Label 6700 1850 0    50   ~ 0
Prog-nRST
$Comp
L Connector_Generic:Conn_01x06 J52
U 1 1 5FFB0E8E
P 1550 6750
F 0 "J52" H 1468 7167 50  0000 C CNN
F 1 "Debug in for ST-LINK" H 1468 7076 50  0000 C CNN
F 2 "RespiraWorks_Std:PinHeader_1x06_P2.54mm_Vertical" H 1550 6750 50  0001 C CNN
F 3 "~" H 1550 6750 50  0001 C CNN
F 4 "Harwin" H 1550 6750 50  0001 C CNN "Manufacturer"
F 5 "M20-9980345" H 1550 6750 50  0001 C CNN "Manufacturer PN"
F 6 " " H 1550 6750 50  0001 C CNN "DNP"
	1    1550 6750
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFB2D71
P 1850 7150
AR Path="/5FCD4B8E/5FCD4BC5/5FFB2D71" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFB2D71" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FFB2D71" Ref="#PWR0419"  Part="1" 
F 0 "#PWR0419" H 1850 6900 50  0001 C CNN
F 1 "GND" H 1855 6977 50  0000 C CNN
F 2 "" H 1850 7150 50  0001 C CNN
F 3 "" H 1850 7150 50  0001 C CNN
	1    1850 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 7150 1850 7050
Wire Wire Line
	1850 6750 1750 6750
Wire Wire Line
	1750 6550 2100 6550
Wire Wire Line
	2100 6550 2100 6400
Wire Wire Line
	1750 6650 2850 6650
Wire Wire Line
	1750 6950 2850 6950
Wire Wire Line
	1750 6850 2850 6850
Text Label 2850 6650 2    50   ~ 0
Prog-SWCLK
Text Label 2850 6850 2    50   ~ 0
Prog-SWDIO
Text Label 2850 6950 2    50   ~ 0
Prog-nRST
Wire Wire Line
	1750 7050 1850 7050
Connection ~ 1850 7050
Wire Wire Line
	1850 7050 1850 6750
Wire Wire Line
	7000 4100 6000 4100
$Comp
L Device:LED_ALT D42
U 1 1 5FFE2CDB
P 5450 5250
F 0 "D42" V 5489 5132 50  0000 R CNN
F 1 "Red" V 5398 5132 50  0000 R CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 5450 5250 50  0001 C CNN
F 3 "~" H 5450 5250 50  0001 C CNN
F 4 "Lite-On" H 5450 5250 50  0001 C CNN "Manufacturer"
F 5 "LTST-C170KRKT" H 5450 5250 50  0001 C CNN "Manufacturer PN"
F 6 " " H 5450 5250 50  0001 C CNN "DNP"
	1    5450 5250
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED_ALT D41
U 1 1 5FFE3E1D
P 5450 4700
F 0 "D41" V 5489 4582 50  0000 R CNN
F 1 "Green" V 5398 4582 50  0000 R CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 5450 4700 50  0001 C CNN
F 3 "~" H 5450 4700 50  0001 C CNN
F 4 "Lite-On" H 5450 4700 50  0001 C CNN "Manufacturer"
F 5 "LTST-C171KGKT" H 5450 4700 50  0001 C CNN "Manufacturer PN"
F 6 " " H 5450 4700 50  0001 C CNN "DNP"
	1    5450 4700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5450 4850 5450 5000
Wire Wire Line
	5450 5000 6000 5000
Wire Wire Line
	6000 4100 6000 5000
Connection ~ 5450 5000
Wire Wire Line
	5450 5000 5450 5100
$Comp
L Device:R R?
U 1 1 5FFEED71
P 5450 4400
AR Path="/5FCD4B8E/5FCD4BC5/5FFEED71" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFEED71" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FFEED71" Ref="R255"  Part="1" 
F 0 "R255" H 5200 4500 50  0000 L CNN
F 1 "1k" H 5300 4400 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 5380 4400 50  0001 C CNN
F 3 "~" H 5450 4400 50  0001 C CNN
F 4 "Yageo" H 5450 4400 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 5450 4400 50  0001 C CNN "Manufacturer PN"
F 6 " " H 5450 4400 50  0001 C CNN "DNP"
	1    5450 4400
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5FFEF31C
P 5450 5650
AR Path="/5FCD4B8E/5FCD4BC5/5FFEF31C" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFEF31C" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/5FFEF31C" Ref="R256"  Part="1" 
F 0 "R256" H 5200 5750 50  0000 L CNN
F 1 "1k" H 5300 5650 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 5380 5650 50  0001 C CNN
F 3 "~" H 5450 5650 50  0001 C CNN
F 4 "Yageo" H 5450 5650 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 5450 5650 50  0001 C CNN "Manufacturer PN"
F 6 " " H 5450 5650 50  0001 C CNN "DNP"
	1    5450 5650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5450 5500 5450 5400
$Comp
L power:GND #PWR?
U 1 1 5FFF2B82
P 5450 5800
AR Path="/5FCD4B8E/5FCD4BC5/5FFF2B82" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFF2B82" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/5FFF2B82" Ref="#PWR0428"  Part="1" 
F 0 "#PWR0428" H 5450 5550 50  0001 C CNN
F 1 "GND" H 5455 5627 50  0000 C CNN
F 2 "" H 5450 5800 50  0001 C CNN
F 3 "" H 5450 5800 50  0001 C CNN
	1    5450 5800
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks:+3.3V_Ctrl #PWR0417
U 1 1 6000178D
P 1750 1350
F 0 "#PWR0417" H 1750 1350 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 1735 1523 50  0000 C CNN
F 2 "" H 1750 1350 50  0001 C CNN
F 3 "" H 1750 1350 50  0001 C CNN
	1    1750 1350
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks:+3.3V_Ctrl #PWR0420
U 1 1 60001E93
P 2100 4900
F 0 "#PWR0420" H 2100 4900 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 2085 5073 50  0000 C CNN
F 2 "" H 2100 4900 50  0001 C CNN
F 3 "" H 2100 4900 50  0001 C CNN
	1    2100 4900
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks:+3.3V_Ctrl #PWR0421
U 1 1 6000A757
P 2100 6400
F 0 "#PWR0421" H 2100 6400 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 2085 6573 50  0000 C CNN
F 2 "" H 2100 6400 50  0001 C CNN
F 3 "" H 2100 6400 50  0001 C CNN
	1    2100 6400
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks:+3.3V_Ctrl #PWR0427
U 1 1 6000EE0D
P 5450 4250
F 0 "#PWR0427" H 5450 4250 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 5435 4423 50  0000 C CNN
F 2 "" H 5450 4250 50  0001 C CNN
F 3 "" H 5450 4250 50  0001 C CNN
	1    5450 4250
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks:+3.3V_Ctrl #PWR0430
U 1 1 6001037F
P 6550 1550
F 0 "#PWR0430" H 6550 1550 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 6535 1723 50  0000 C CNN
F 2 "" H 6550 1550 50  0001 C CNN
F 3 "" H 6550 1550 50  0001 C CNN
	1    6550 1550
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks:+3.3V_Ctrl #PWR0433
U 1 1 6001165E
P 7950 1450
F 0 "#PWR0433" H 7950 1450 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 7935 1623 50  0000 C CNN
F 2 "" H 7950 1450 50  0001 C CNN
F 3 "" H 7950 1450 50  0001 C CNN
	1    7950 1450
	1    0    0    -1  
$EndComp
Connection ~ 7950 1450
$Comp
L RespiraWorks:+3.3V_Ctrl #PWR0440
U 1 1 60013B92
P 9350 2650
F 0 "#PWR0440" H 9350 2650 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 9335 2823 50  0000 C CNN
F 2 "" H 9350 2650 50  0001 C CNN
F 3 "" H 9350 2650 50  0001 C CNN
	1    9350 2650
	1    0    0    -1  
$EndComp
Text HLabel 2600 5100 0    50   Output ~ 0
CC-SWCLK
Text HLabel 2450 5700 0    50   BiDi ~ 0
CC-SWDIO
Text HLabel 2450 5800 0    50   Output ~ 0
CC-nRST
Text HLabel 2450 5900 0    50   BiDi ~ 0
CC-SWO
Wire Wire Line
	2450 5700 2500 5700
Wire Wire Line
	2500 5700 2500 5450
Connection ~ 2500 5450
Wire Wire Line
	2500 5450 2700 5450
Wire Wire Line
	2450 5800 2550 5800
Wire Wire Line
	2550 5800 2550 5550
Connection ~ 2550 5550
Wire Wire Line
	2550 5550 2700 5550
Wire Wire Line
	2450 5900 2600 5900
Wire Wire Line
	2600 5900 2600 5650
Connection ~ 2600 5650
Wire Wire Line
	2600 5650 1750 5650
Wire Wire Line
	2600 5100 2650 5100
Wire Wire Line
	2650 5100 2650 5250
Connection ~ 2650 5250
Wire Wire Line
	2650 5250 2700 5250
$Comp
L power:GND #PWR?
U 1 1 6005FA14
P 8800 2750
AR Path="/5FCD4B8E/5FCD4BC5/6005FA14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/6005FA14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/6005FA14" Ref="#PWR0436"  Part="1" 
F 0 "#PWR0436" H 8800 2500 50  0001 C CNN
F 1 "GND" H 8805 2577 50  0000 C CNN
F 2 "" H 8800 2750 50  0001 C CNN
F 3 "" H 8800 2750 50  0001 C CNN
	1    8800 2750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8450 2750 8800 2750
$Comp
L RespiraWorks:+3.3V_Ctrl #PWR0423
U 1 1 6006697C
P 4250 1850
F 0 "#PWR0423" H 4250 1850 50  0001 C CNN
F 1 "+3.3V_Ctrl" H 4235 2023 50  0000 C CNN
F 2 "" H 4250 1850 50  0001 C CNN
F 3 "" H 4250 1850 50  0001 C CNN
	1    4250 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60068022
P 4600 1850
AR Path="/5FCD4B8E/5FCD4BC5/60068022" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/60068022" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/60068022" Ref="R253"  Part="1" 
F 0 "R253" V 4400 1800 50  0000 L CNN
F 1 "4.7k" V 4500 1800 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 4530 1850 50  0001 C CNN
F 3 "~" H 4600 1850 50  0001 C CNN
F 4 "Yageo" H 4600 1850 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-074K7L" H 4600 1850 50  0001 C CNN "Manufacturer PN"
F 6 " " H 4600 1850 50  0001 C CNN "DNP"
	1    4600 1850
	0    1    1    0   
$EndComp
$Comp
L Device:R R?
U 1 1 600684BF
P 4850 2000
AR Path="/5FCD4B8E/5FCD4BC5/600684BF" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/600684BF" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/600684BF" Ref="R254"  Part="1" 
F 0 "R254" H 4550 2000 50  0000 L CNN
F 1 "4.7k" H 4600 2100 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 4780 2000 50  0001 C CNN
F 3 "~" H 4850 2000 50  0001 C CNN
F 4 "Yageo" H 4850 2000 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-074K7L" H 4850 2000 50  0001 C CNN "Manufacturer PN"
F 6 " " H 4850 2000 50  0001 C CNN "DNP"
	1    4850 2000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4250 1850 4450 1850
Wire Wire Line
	4750 1850 4850 1850
Connection ~ 4850 1850
Wire Wire Line
	4850 1850 5250 1850
$Comp
L power:GND #PWR?
U 1 1 600710EF
P 4850 2150
AR Path="/5FCD4B8E/5FCD4BC5/600710EF" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/600710EF" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/600710EF" Ref="#PWR0425"  Part="1" 
F 0 "#PWR0425" H 4850 1900 50  0001 C CNN
F 1 "GND" H 5000 2100 50  0000 C CNN
F 2 "" H 4850 2150 50  0001 C CNN
F 3 "" H 4850 2150 50  0001 C CNN
	1    4850 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 6007ED35
P 9150 4800
AR Path="/5FCD4B8E/5FCD4BC5/6007ED35" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/6007ED35" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/6007ED35" Ref="R261"  Part="1" 
F 0 "R261" H 8900 4750 50  0000 L CNN
F 1 "10k" H 8950 4850 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 9080 4800 50  0001 C CNN
F 3 "~" H 9150 4800 50  0001 C CNN
F 4 "Yageo" H 9150 4800 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-0710KL" H 9150 4800 50  0001 C CNN "Manufacturer PN"
F 6 " " H 9150 4800 50  0001 C CNN "DNP"
	1    9150 4800
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 60080C40
P 8800 5050
AR Path="/5FCD4B8E/5FCD4BC5/60080C40" Ref="R?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/60080C40" Ref="R?"  Part="1" 
AR Path="/5FCD4D85/60080C40" Ref="R259"  Part="1" 
F 0 "R259" H 8550 5000 50  0000 L CNN
F 1 "DNP" H 8600 5100 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 8730 5050 50  0001 C CNN
F 3 "~" H 8800 5050 50  0001 C CNN
F 4 "DNP" V 8800 5050 50  0000 C CNN "DNP"
	1    8800 5050
	-1   0    0    1   
$EndComp
Wire Wire Line
	8450 4650 9150 4650
Wire Wire Line
	8450 4750 8800 4750
Wire Wire Line
	8800 4750 8800 4900
$Comp
L power:GND #PWR?
U 1 1 60088259
P 8800 5200
AR Path="/5FCD4B8E/5FCD4BC5/60088259" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/60088259" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/60088259" Ref="#PWR0437"  Part="1" 
F 0 "#PWR0437" H 8800 4950 50  0001 C CNN
F 1 "GND" H 8805 5027 50  0000 C CNN
F 2 "" H 8800 5200 50  0001 C CNN
F 3 "" H 8800 5200 50  0001 C CNN
	1    8800 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 600884F2
P 9150 4950
AR Path="/5FCD4B8E/5FCD4BC5/600884F2" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/600884F2" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/600884F2" Ref="#PWR0439"  Part="1" 
F 0 "#PWR0439" H 9150 4700 50  0001 C CNN
F 1 "GND" H 9155 4777 50  0000 C CNN
F 2 "" H 9150 4950 50  0001 C CNN
F 3 "" H 9150 4950 50  0001 C CNN
	1    9150 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 4050 8900 4050
Text HLabel 8900 4050 2    50   Output ~ 0
USB-Pwr-nEn
Text Label 5250 1850 2    50   ~ 0
VSense
Text Label 6250 3200 0    50   ~ 0
VSense
Wire Wire Line
	6250 3200 7000 3200
Wire Wire Line
	7000 3400 6700 3400
Wire Wire Line
	7000 3500 6700 3500
Text HLabel 6700 3400 0    50   Output ~ 0
UART-Tx
Text HLabel 6700 3500 0    50   Input ~ 0
UART-Rx
NoConn ~ 8450 2650
NoConn ~ 8450 2950
NoConn ~ 8450 3050
NoConn ~ 8450 3150
NoConn ~ 8450 3250
NoConn ~ 8450 3350
NoConn ~ 8450 3450
NoConn ~ 8450 3550
NoConn ~ 8450 3650
NoConn ~ 8450 4850
NoConn ~ 7000 4000
NoConn ~ 7000 3900
NoConn ~ 7000 3800
NoConn ~ 7000 3600
NoConn ~ 7000 3300
$Comp
L Device:Jumper_NC_Small JP19
U 1 1 600F8FCD
P 9350 2950
F 0 "JP19" V 9350 3100 50  0000 C CNN
F 1 "Jumper_NC_Small" H 9350 3071 50  0001 C CNN
F 2 "RespiraWorks_Std:SolderJumper-2_P1.3mm_Bridged_RoundedPad1.0x1.5mm" H 9350 2950 50  0001 C CNN
F 3 "~" H 9350 2950 50  0001 C CNN
F 4 " " H 9350 2950 50  0001 C CNN "DNP"
	1    9350 2950
	0    1    1    0   
$EndComp
$Comp
L Device:Jumper_NO_Small JP18
U 1 1 600FB375
P 9350 2750
F 0 "JP18" V 9350 2900 50  0000 C CNN
F 1 "Jumper_NO_Small" H 9350 2871 50  0001 C CNN
F 2 "RespiraWorks_Std:SolderJumper-2_P1.3mm_Open_RoundedPad1.0x1.5mm" H 9350 2750 50  0001 C CNN
F 3 "~" H 9350 2750 50  0001 C CNN
F 4 " " H 9350 2750 50  0001 C CNN "DNP"
	1    9350 2750
	0    1    1    0   
$EndComp
Connection ~ 9350 2850
$Comp
L power:GND #PWR?
U 1 1 600FBA53
P 9350 3050
AR Path="/5FCD4B8E/5FCD4BC5/600FBA53" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/600FBA53" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/600FBA53" Ref="#PWR0441"  Part="1" 
F 0 "#PWR0441" H 9350 2800 50  0001 C CNN
F 1 "GND" H 9355 2877 50  0000 C CNN
F 2 "" H 9350 3050 50  0001 C CNN
F 3 "" H 9350 3050 50  0001 C CNN
	1    9350 3050
	1    0    0    -1  
$EndComp
Text Notes 9650 2950 0    50   ~ 0
High: can draw 100 mA max. from USB\n  (will always self-power successfully)\nLow: can draw 300 mA max. from USB\n  (needs to negotiate power draw)
Wire Wire Line
	8450 2850 9350 2850
Text Notes 9200 3650 0    50   ~ 0
Unsure of purpose
Text Notes 9050 5350 0    50   ~ 0
Board identity
Text Notes 4300 1550 0    50   ~ 0
Ext. voltage sense; unsure why\nset @ 2.5V
Text Notes 2750 5050 0    50   ~ 0
Cut jumpers to use\nan external programmer\nto program the cycle\ncontroller instead of this\nbuilt-in programmer
Text Label 9050 3850 2    50   ~ 0
CC-SWCLK-Out
Wire Wire Line
	8450 3850 9050 3850
Wire Wire Line
	8450 2550 9000 2550
Text HLabel 2450 1000 2    50   Output ~ 0
5V-USB
Wire Wire Line
	1450 1000 2350 1000
Wire Wire Line
	1450 1000 1450 2300
Wire Wire Line
	2350 1250 2350 1000
Connection ~ 2350 1000
Wire Wire Line
	2350 1000 2450 1000
Wire Wire Line
	1450 2500 1750 2500
Wire Wire Line
	1450 2600 1800 2600
Wire Wire Line
	1450 2700 1600 2700
Wire Wire Line
	1150 2900 1050 2900
$EndSCHEMATC
