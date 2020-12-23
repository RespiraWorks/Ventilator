EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 15
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RespiraWorks:BQ25713 U?
U 1 1 5FE10AFA
P 4400 3900
F 0 "U?" H 4425 5565 50  0000 C CNN
F 1 "BQ25713" H 4425 5474 50  0000 C CNN
F 2 "Package_DFN_QFN:Texas_S-PWQFN-N32_EP2.8x2.8mm_ThermalVias" H 4450 1600 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/bq25713.pdf" H 7100 4950 50  0001 C CNN
	1    4400 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 4900 3650 4900
Wire Wire Line
	3650 5100 2950 5100
Text HLabel 2950 4900 0    50   Input ~ 0
SCL
Text HLabel 2950 5100 0    50   BiDi ~ 0
SDA
Text Notes 6450 1650 0    50   ~ 0
Design notes:\n- 8.2A max current, 19V input\n- Inductor selection: Isat 9.1A, 3.3uH\n- Wurth 7443330330 or Bourns SRP1038A-3R3M (OPL alt: MWSA1004-3R3MT)\n- Voltage will range from 10.8V to 12.6V for 3s Li-Ion battery\n- 11.5 to 13.0V for SLA (lead acid)\n- N-CH FET: CSD17304Q3 (OPL) or BSZ050N03LSGATMA1 (OPL)\n- P-CH FET: CSD25402Q3A (OPL) or CSD25404Q3T (OPL)\n
$Comp
L RespiraWorks_Std:SRP1038A-3R3M L?
U 1 1 5FE317DE
P 4400 1900
F 0 "L?" H 4400 2090 50  0000 C CNN
F 1 "SRP1038A-3R3M" H 4400 1999 50  0000 C CNN
F 2 "" V 4400 1900 50  0001 C CNN
F 3 "https://www.bourns.com/docs/Product-Datasheets/SRP1038A.pdf" V 4400 1900 50  0001 C CNN
	1    4400 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1900 3350 2700
Wire Wire Line
	3350 2700 3650 2700
Wire Wire Line
	5200 2700 5500 2700
Wire Wire Line
	5500 2700 5500 1900
Wire Wire Line
	5500 1900 5250 1900
$Comp
L Device:C C?
U 1 1 5FE331F7
P 5250 2050
F 0 "C?" H 5365 2096 50  0000 L CNN
F 1 "C" H 5365 2005 50  0000 L CNN
F 2 "" H 5288 1900 50  0001 C CNN
F 3 "~" H 5250 2050 50  0001 C CNN
	1    5250 2050
	1    0    0    -1  
$EndComp
Connection ~ 5250 1900
Wire Wire Line
	5250 1900 4550 1900
Wire Wire Line
	5250 2200 5250 2500
Wire Wire Line
	3600 2200 3600 2500
Wire Wire Line
	3600 1900 3350 1900
Connection ~ 3600 1900
Wire Wire Line
	4250 1900 3600 1900
$Comp
L Device:C C?
U 1 1 5FE32A3B
P 3600 2050
F 0 "C?" H 3715 2096 50  0000 L CNN
F 1 "C" H 3715 2005 50  0000 L CNN
F 2 "" H 3638 1900 50  0001 C CNN
F 3 "~" H 3600 2050 50  0001 C CNN
	1    3600 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 2500 5250 2500
Wire Wire Line
	3650 2500 3600 2500
$EndSCHEMATC
