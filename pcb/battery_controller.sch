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
Text Notes 6000 1100 0    50   ~ 0
Design notes:\n- 8.2A max current, 19V input\n- Inductor selection: Isat 9.1A, 3.3uH\n- Wurth 7443330330 or Bourns SRP1038A-3R3M\n- Voltage will range from 10.8V to 12.6V for 3s Li-Ion battery\n- 11.5 to 13.0V for SLA (lead acid)\n- \n
$EndSCHEMATC
