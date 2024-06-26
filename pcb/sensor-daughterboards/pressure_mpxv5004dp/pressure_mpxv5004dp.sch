EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Pressure Sensor Daughterboard: MPXV5004DP"
Date ""
Rev "1.0"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RespiraWorks:MPXV5004DP U1
U 1 1 5FFA0C0F
P 3550 3600
F 0 "U1" H 3222 3621 50  0000 R CNN
F 1 "MPXV5004DP" H 3222 3530 50  0000 R CNN
F 2 "RespiraWorks:NXP_MPXV5004DP" H 3550 3750 50  0001 C CNN
F 3 "" H 3550 3650 50  0001 C CNN
F 4 "NXP" H 3550 3600 50  0001 C CNN "Manufacturer"
F 5 "MPXV5004DP" H 3550 3600 50  0001 C CNN "Manufacturer PN"
F 6 "SMT, hand-solder" H 3550 3600 50  0001 C CNN "Type"
	1    3550 3600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FFA6CAD
P 3750 2650
AR Path="/5FCD4B8E/5FCD4BC5/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/5FFA6CAD" Ref="C?"  Part="1" 
AR Path="/5FFA6CAD" Ref="C1"  Part="1" 
F 0 "C1" H 3950 2650 50  0000 L CNN
F 1 "1uF" H 3850 2750 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 3788 2500 50  0001 C CNN
F 3 "~" H 3750 2650 50  0001 C CNN
F 4 "50V" H 3950 2850 50  0000 C CNN "Voltage"
F 5 "X5R" H 3750 2650 50  0001 C CNN "Dielectric"
F 6 "Samsung" H 3750 2650 50  0001 C CNN "Manufacturer"
F 7 "CL10A105KB8NNNC" H 3750 2650 50  0001 C CNN "Manufacturer PN"
F 8 "SMT" H 3750 2650 50  0001 C CNN "Type"
	1    3750 2650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFA6CB3
P 3750 2800
AR Path="/5FCD4B8E/5FCD4BC5/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFA6CB3" Ref="#PWR?"  Part="1" 
AR Path="/5FFA6CB3" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 3750 2550 50  0001 C CNN
F 1 "GND" H 3755 2627 50  0000 C CNN
F 2 "" H 3750 2800 50  0001 C CNN
F 3 "" H 3750 2800 50  0001 C CNN
	1    3750 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 3100 3350 2500
Wire Wire Line
	3350 2500 3750 2500
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
P 3350 4150
AR Path="/5FCD4B8E/5FCD4BC5/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFA361A" Ref="#PWR?"  Part="1" 
AR Path="/5FFA361A" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 3350 3900 50  0001 C CNN
F 1 "GND" H 3355 3977 50  0000 C CNN
F 2 "" H 3350 4150 50  0001 C CNN
F 3 "" H 3350 4150 50  0001 C CNN
	1    3350 4150
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
Wire Wire Line
	5000 3600 4550 3600
Wire Wire Line
	6750 4600 7000 4600
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
$Comp
L Device:C C?
U 1 1 5FFBE46B
P 4550 3750
AR Path="/5FCD4B8E/5FCD4BC5/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/5FFBE46B" Ref="C?"  Part="1" 
AR Path="/5FFBE46B" Ref="C3"  Part="1" 
F 0 "C3" H 4700 3750 50  0000 L CNN
F 1 "470pF" H 4650 3850 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 4588 3600 50  0001 C CNN
F 3 "~" H 4550 3750 50  0001 C CNN
F 4 "NP0" H 4550 3750 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 4550 3750 50  0001 C CNN "Manufacturer"
F 6 "C0603C471J5GACTU" H 4550 3750 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 4550 3750 50  0001 C CNN "Voltage"
F 8 "SMT" H 4550 3750 50  0001 C CNN "Type"
	1    4550 3750
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFBE471
P 4550 3900
AR Path="/5FCD4B8E/5FCD4BC5/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/5FFBE471" Ref="#PWR?"  Part="1" 
AR Path="/5FFBE471" Ref="#PWR0106"  Part="1" 
F 0 "#PWR0106" H 4550 3650 50  0001 C CNN
F 1 "GND" H 4555 3727 50  0000 C CNN
F 2 "" H 4550 3900 50  0001 C CNN
F 3 "" H 4550 3900 50  0001 C CNN
	1    4550 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 5FFBE477
P 6150 3750
AR Path="/5FCD4B8E/5FCD4BC5/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/5FFBE477" Ref="C?"  Part="1" 
AR Path="/5FFBE477" Ref="C4"  Part="1" 
F 0 "C4" H 6300 3750 50  0000 L CNN
F 1 "1uF" H 6250 3850 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6188 3600 50  0001 C CNN
F 3 "~" H 6150 3750 50  0001 C CNN
F 4 "X5R" H 6150 3750 50  0001 C CNN "Dielectric"
F 5 "Samsung" H 6150 3750 50  0001 C CNN "Manufacturer"
F 6 "CL10A105KB8NNNC" H 6150 3750 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 6350 3950 50  0000 C CNN "Voltage"
F 8 "SMT" H 6150 3750 50  0001 C CNN "Type"
	1    6150 3750
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFBE47D
P 6150 3900
AR Path="/5FCD4B8E/5FCD4BC5/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/5FFBE47D" Ref="#PWR?"  Part="1" 
AR Path="/5FFBE47D" Ref="#PWR0107"  Part="1" 
F 0 "#PWR0107" H 6150 3650 50  0001 C CNN
F 1 "GND" H 6155 3727 50  0000 C CNN
F 2 "" H 6150 3900 50  0001 C CNN
F 3 "" H 6150 3900 50  0001 C CNN
	1    6150 3900
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5FFBE485
P 5150 3600
AR Path="/5FFBE485" Ref="R1"  Part="1" 
AR Path="/5E8AFE9D/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5E8C8865/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/5FFBE485" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/5FFBE485" Ref="R?"  Part="1" 
F 0 "R1" V 4950 3600 50  0000 C CNN
F 1 "2.4K" V 5050 3600 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 5080 3600 50  0001 C CNN
F 3 "~" H 5150 3600 50  0001 C CNN
F 4 "Yageo" H 5150 3600 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-072K4L" H 5150 3600 50  0001 C CNN "Manufacturer PN"
F 6 "SMT" H 5150 3600 50  0001 C CNN "Type"
	1    5150 3600
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 5FFBE48D
P 5550 3750
AR Path="/5FFBE48D" Ref="R2"  Part="1" 
AR Path="/5E8AFE9D/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5E8C8865/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/5FFBE48D" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/5FFBE48D" Ref="R?"  Part="1" 
F 0 "R2" H 5400 3750 50  0000 C CNN
F 1 "4.7K" H 5400 3850 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 5480 3750 50  0001 C CNN
F 3 "~" H 5550 3750 50  0001 C CNN
F 4 "Yageo" H 5550 3750 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-074K7L" H 5550 3750 50  0001 C CNN "Manufacturer PN"
F 6 "SMT" H 5550 3750 50  0001 C CNN "Type"
	1    5550 3750
	-1   0    0    1   
$EndComp
Connection ~ 4550 3600
Wire Wire Line
	5300 3600 5550 3600
Connection ~ 5550 3600
Wire Wire Line
	5550 3600 6150 3600
$Comp
L power:GND #PWR?
U 1 1 5FFBE498
P 5550 3900
AR Path="/5FCD4B8E/5FCD4BC5/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/5FFBE498" Ref="#PWR?"  Part="1" 
AR Path="/5FFBE498" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 5550 3650 50  0001 C CNN
F 1 "GND" H 5555 3727 50  0000 C CNN
F 2 "" H 5550 3900 50  0001 C CNN
F 3 "" H 5550 3900 50  0001 C CNN
	1    5550 3900
	-1   0    0    -1  
$EndComp
Connection ~ 6150 3600
Text Notes 5200 3300 0    50   ~ 0
x0.662\nLPF fc = 100 Hz
Wire Wire Line
	3950 3600 4550 3600
Wire Wire Line
	6900 4000 6900 3700
Wire Wire Line
	6900 3700 7000 3700
Wire Wire Line
	6150 3600 7000 3600
NoConn ~ 7000 3800
NoConn ~ 7000 3900
$Comp
L RespiraWorks:+5V_Sns #PWR0109
U 1 1 5FFD6716
P 3350 2500
F 0 "#PWR0109" H 3350 2500 50  0001 C CNN
F 1 "+5V_Sns" H 3335 2673 50  0000 C CNN
F 2 "" H 3350 2500 50  0001 C CNN
F 3 "" H 3350 2500 50  0001 C CNN
	1    3350 2500
	1    0    0    -1  
$EndComp
Connection ~ 3350 2500
$Comp
L RespiraWorks:+5V_Sns #PWR0110
U 1 1 5FFD6C8C
P 6750 4600
F 0 "#PWR0110" H 6750 4600 50  0001 C CNN
F 1 "+5V_Sns" H 6735 4773 50  0000 C CNN
F 2 "" H 6750 4600 50  0001 C CNN
F 3 "" H 6750 4600 50  0001 C CNN
	1    6750 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4900 6750 4900
Connection ~ 6750 4900
Wire Wire Line
	6750 4900 6750 5000
NoConn ~ 7000 4800
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
$EndSCHEMATC
