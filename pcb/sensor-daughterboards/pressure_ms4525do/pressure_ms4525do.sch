EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Pressure Sensor Daughterboard: MS4525DO"
Date ""
Rev "1.0"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:C C?
U 1 1 5FFA6CAD
P 5650 3150
AR Path="/5FCD4B8E/5FCD4BC5/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FFA6CAD" Ref="C1"  Part="1" 
F 0 "C1" H 5800 3150 50  0000 L CNN
F 1 "1uF" H 5750 3250 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 5688 3000 50  0001 C CNN
F 3 "~" H 5650 3150 50  0001 C CNN
F 4 "50V" H 5850 3350 50  0000 C CNN "Voltage"
F 5 "X7R" H 5650 3150 50  0001 C CNN "Dielectric"
F 6 "Kemet" H 5650 3150 50  0001 C CNN "Manufacturer"
F 7 "C0805C105K5RACTU" H 5650 3150 50  0001 C CNN "Manufacturer PN"
F 8 "SMT" H 5650 3150 50  0001 C CNN "Type"
	1    5650 3150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFA6CB3
P 5650 3300
AR Path="/5FCD4B8E/5FCD4BC5/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FFA6CB3" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 5650 3050 50  0001 C CNN
F 1 "GND" H 5655 3127 50  0000 C CNN
F 2 "" H 5650 3300 50  0001 C CNN
F 3 "" H 5650 3300 50  0001 C CNN
	1    5650 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3600 5250 3000
Wire Wire Line
	5250 3000 5650 3000
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 5FFA737A
P 7200 3700
F 0 "J1" H 7280 3742 50  0000 L CNN
F 1 "Main board connections (bottom entry)" H 7280 3651 50  0000 L CNN
F 2 "RespiraWorks_Std:PinSocket_1x04_P2.54mm_Vertical_SMD_TE_1241152-4" H 7200 3700 50  0001 C CNN
F 3 "~" H 7200 3700 50  0001 C CNN
F 4 "TE Connectivity" H 7200 3700 50  0001 C CNN "Manufacturer"
F 5 "1241152-4" H 7200 3700 50  0001 C CNN "Manufacturer PN"
F 6 "SMT" H 7200 3700 50  0001 C CNN "Type"
	1    7200 3700
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J2
U 1 1 5FFA26A3
P 7200 4700
F 0 "J2" H 7280 4742 50  0000 L CNN
F 1 "Main board connections (bottom entry)" H 7280 4651 50  0000 L CNN
F 2 "RespiraWorks_Std:PinSocket_1x04_P2.54mm_Vertical_SMD_TE_1241152-4" H 7200 4700 50  0001 C CNN
F 3 "~" H 7200 4700 50  0001 C CNN
F 4 "TE Connectivity" H 7200 4700 50  0001 C CNN "Manufacturer"
F 5 "1241152-4" H 7200 4700 50  0001 C CNN "Manufacturer PN"
F 6 "SMT" H 7200 4700 50  0001 C CNN "Type"
	1    7200 4700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFA361A
P 5250 4650
AR Path="/5FCD4B8E/5FCD4BC5/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FFA361A" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 5250 4400 50  0001 C CNN
F 1 "GND" H 5255 4477 50  0000 C CNN
F 2 "" H 5250 4650 50  0001 C CNN
F 3 "" H 5250 4650 50  0001 C CNN
	1    5250 4650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFA3C31
P 6900 4000
AR Path="/5FCD4B8E/5FCD4BC5/5FFA3C31" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFA3C31" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFA3C31" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFA3C31" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFA3C31" Ref="#PWR?"  Part="1" 
AR Path="/5FFA3C31" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 6900 3750 50  0001 C CNN
F 1 "GND" H 6905 3827 50  0000 C CNN
F 2 "" H 6900 4000 50  0001 C CNN
F 3 "" H 6900 4000 50  0001 C CNN
	1    6900 4000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFA7110
P 6750 5000
AR Path="/5FCD4B8E/5FCD4BC5/5FFA7110" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFA7110" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFA7110" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFA7110" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFA7110" Ref="#PWR?"  Part="1" 
AR Path="/5FFA7110" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 6750 4750 50  0001 C CNN
F 1 "GND" H 6755 4827 50  0000 C CNN
F 2 "" H 6750 5000 50  0001 C CNN
F 3 "" H 6750 5000 50  0001 C CNN
	1    6750 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 4700 6750 4900
Wire Wire Line
	6750 4700 7000 4700
Wire Wire Line
	6900 4000 6900 3700
Wire Wire Line
	6900 3700 7000 3700
Wire Wire Line
	7000 4900 6750 4900
Connection ~ 6750 4900
Wire Wire Line
	6750 4900 6750 5000
$Comp
L RespiraWorks_Std:MountingHole_Pad H1
U 1 1 605B378F
P 8300 1650
F 0 "H1" H 8400 1699 50  0000 L CNN
F 1 "M2.5" H 8400 1608 50  0000 L CNN
F 2 "RespiraWorks_Std:MountingHole_2.7mm_M2.5_Pad_Via" H 8300 1650 50  0001 C CNN
F 3 "~" H 8300 1650 50  0001 C CNN
	1    8300 1650
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks_Std:MountingHole_Pad H3
U 1 1 605B4C2D
P 8750 1650
F 0 "H3" H 8850 1699 50  0000 L CNN
F 1 "M2.5" H 8850 1608 50  0000 L CNN
F 2 "RespiraWorks_Std:MountingHole_2.7mm_M2.5_Pad_Via" H 8750 1650 50  0001 C CNN
F 3 "~" H 8750 1650 50  0001 C CNN
	1    8750 1650
	1    0    0    -1  
$EndComp
NoConn ~ 8300 1750
NoConn ~ 8750 1750
$Comp
L RespiraWorks_Std:MountingHole_Pad H2
U 1 1 605B55AD
P 8300 2000
F 0 "H2" H 8400 2049 50  0000 L CNN
F 1 "M2.5" H 8400 1958 50  0000 L CNN
F 2 "RespiraWorks_Std:MountingHole_2.7mm_M2.5_Pad_Via" H 8300 2000 50  0001 C CNN
F 3 "~" H 8300 2000 50  0001 C CNN
	1    8300 2000
	1    0    0    -1  
$EndComp
NoConn ~ 8300 2100
NoConn ~ 8750 2100
$Comp
L RespiraWorks_Std:MountingHole_Pad H4
U 1 1 605B582D
P 8750 2000
F 0 "H4" H 8850 2049 50  0000 L CNN
F 1 "M2.5" H 8850 1958 50  0000 L CNN
F 2 "RespiraWorks_Std:MountingHole_2.7mm_M2.5_Pad_Via" H 8750 2000 50  0001 C CNN
F 3 "~" H 8750 2000 50  0001 C CNN
	1    8750 2000
	1    0    0    -1  
$EndComp
Text Notes 8650 7050 0    79   Italic 0
Unless noted otherwise:\n- Resistors are 1% tolerance\n- Capacitors < 47 nF are 5% tolerance\n- Capacitors ≥ 47 nF are 10% tolerance
$Comp
L RespiraWorks:MS4525DO-DSxxxxxxDP U1
U 1 1 6065327C
P 5450 4100
F 0 "U1" H 5122 4166 50  0000 R CNN
F 1 "MS4525DO" H 5122 4075 50  0000 R CNN
F 2 "RespiraWorks:TE_MS4525DO-DSxxxxxxDP" H 5200 4250 50  0001 C CNN
F 3 "" H 5450 4150 50  0001 C CNN
F 4 "TE Connectivity" H 5122 3984 50  0001 R CNN "Manufacturer"
F 5 "4525DO-DS5AI001DP" H 4700 4000 50  0000 C CNN "Manufacturer PN"
F 6 "Through-hole" H 5450 4100 50  0001 C CNN "Type"
	1    5450 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3800 5850 3800
Wire Wire Line
	5850 3900 7000 3900
Text Label 6100 3800 0    50   ~ 0
SDA
Text Label 6100 3900 0    50   ~ 0
SCL
NoConn ~ 5850 4100
NoConn ~ 7000 3600
NoConn ~ 7000 4600
$Comp
L RespiraWorks:+3.3V_Sns #PWR?
U 1 1 60661DEB
P 6550 4800
F 0 "#PWR?" H 6550 4800 50  0001 C CNN
F 1 "+3.3V_Sns" H 6535 4973 50  0000 C CNN
F 2 "" H 6550 4800 50  0001 C CNN
F 3 "" H 6550 4800 50  0001 C CNN
	1    6550 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 4800 7000 4800
$Comp
L RespiraWorks:+3.3V_Sns #PWR?
U 1 1 606623FA
P 5250 3000
F 0 "#PWR?" H 5250 3000 50  0001 C CNN
F 1 "+3.3V_Sns" H 5235 3173 50  0000 C CNN
F 2 "" H 5250 3000 50  0001 C CNN
F 3 "" H 5250 3000 50  0001 C CNN
	1    5250 3000
	1    0    0    -1  
$EndComp
Connection ~ 5250 3000
$EndSCHEMATC
