EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 21 38
Title "Oxygen and Temperature Sensors"
Date ""
Rev "A"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1100 1000 1450 350 
U 600F5EF3
F0 "NTC Temp. Sensor (Stepper 1)" 50
F1 "sensors_temp_ntc.sch" 50
F2 "Out" O R 2550 1250 50 
F3 "Vcc" I R 2550 1100 50 
$EndSheet
$Sheet
S 1100 1700 1450 350 
U 600F7154
F0 "NTC Temp. Sensor (Stepper 2)" 50
F1 "sensors_temp_ntc.sch" 50
F2 "Out" O R 2550 1950 50 
F3 "Vcc" I R 2550 1800 50 
$EndSheet
$Sheet
S 1100 2400 1450 350 
U 600F719E
F0 "NTC Temp. Sensor (Blower)" 50
F1 "sensors_temp_ntc.sch" 50
F2 "Out" O R 2550 2650 50 
F3 "Vcc" I R 2550 2500 50 
$EndSheet
$Sheet
S 1100 3100 1450 350 
U 600F71F1
F0 "NTC Temp. Sensor (Battery)" 50
F1 "sensors_temp_ntc.sch" 50
F2 "Out" O R 2550 3350 50 
F3 "Vcc" I R 2550 3200 50 
$EndSheet
$Sheet
S 1100 3800 1450 350 
U 600F723B
F0 "NTC Temp. Sensor (Spare)" 50
F1 "sensors_temp_ntc.sch" 50
F2 "Out" O R 2550 4050 50 
F3 "Vcc" I R 2550 3900 50 
$EndSheet
Wire Wire Line
	2550 1100 3000 1100
Wire Wire Line
	2550 1250 3450 1250
$Comp
L Connector:TestPoint TP?
U 1 1 6011BCE0
P 3450 1250
AR Path="/5FCD4E9D/6011BCE0" Ref="TP?"  Part="1" 
AR Path="/5FCD4EEA/6011BCE0" Ref="TP92"  Part="1" 
F 0 "TP92" H 3508 1368 50  0000 L CNN
F 1 "Stepper1-Temp" H 3300 1450 50  0000 L CNN
F 2 "RespiraWorks_Std:TestPoint_Pad_2.0x2.0mm" H 3650 1250 50  0001 C CNN
F 3 "~" H 3650 1250 50  0001 C CNN
	1    3450 1250
	1    0    0    -1  
$EndComp
Text HLabel 3750 1250 2    50   Output ~ 0
Stepper1-Temp
Wire Wire Line
	3750 1250 3450 1250
Connection ~ 3450 1250
Wire Wire Line
	2550 1800 3000 1800
Wire Wire Line
	2550 1950 3450 1950
$Comp
L Connector:TestPoint TP?
U 1 1 6011C56A
P 3450 1950
AR Path="/5FCD4E9D/6011C56A" Ref="TP?"  Part="1" 
AR Path="/5FCD4EEA/6011C56A" Ref="TP93"  Part="1" 
F 0 "TP93" H 3508 2068 50  0000 L CNN
F 1 "Stepper2-Temp" H 3300 2150 50  0000 L CNN
F 2 "RespiraWorks_Std:TestPoint_Pad_2.0x2.0mm" H 3650 1950 50  0001 C CNN
F 3 "~" H 3650 1950 50  0001 C CNN
	1    3450 1950
	1    0    0    -1  
$EndComp
Text HLabel 3750 1950 2    50   Output ~ 0
Stepper2-Temp
Wire Wire Line
	3750 1950 3450 1950
Connection ~ 3450 1950
Wire Wire Line
	2550 2500 3000 2500
Wire Wire Line
	2550 2650 3450 2650
$Comp
L Connector:TestPoint TP?
U 1 1 6011C9CC
P 3450 2650
AR Path="/5FCD4E9D/6011C9CC" Ref="TP?"  Part="1" 
AR Path="/5FCD4EEA/6011C9CC" Ref="TP94"  Part="1" 
F 0 "TP94" H 3508 2768 50  0000 L CNN
F 1 "Blower-Temp" H 3300 2850 50  0000 L CNN
F 2 "RespiraWorks_Std:TestPoint_Pad_2.0x2.0mm" H 3650 2650 50  0001 C CNN
F 3 "~" H 3650 2650 50  0001 C CNN
	1    3450 2650
	1    0    0    -1  
$EndComp
Text HLabel 3750 2650 2    50   Output ~ 0
Blower-Temp
Wire Wire Line
	3750 2650 3450 2650
Connection ~ 3450 2650
Wire Wire Line
	2550 3200 3000 3200
Wire Wire Line
	2550 3350 3450 3350
$Comp
L Connector:TestPoint TP?
U 1 1 6011CE5B
P 3450 3350
AR Path="/5FCD4E9D/6011CE5B" Ref="TP?"  Part="1" 
AR Path="/5FCD4EEA/6011CE5B" Ref="TP95"  Part="1" 
F 0 "TP95" H 3508 3468 50  0000 L CNN
F 1 "Batt-Temp" H 3300 3550 50  0000 L CNN
F 2 "RespiraWorks_Std:TestPoint_Pad_2.0x2.0mm" H 3650 3350 50  0001 C CNN
F 3 "~" H 3650 3350 50  0001 C CNN
	1    3450 3350
	1    0    0    -1  
$EndComp
Text HLabel 3750 3350 2    50   Output ~ 0
Batt-Temp
Wire Wire Line
	3750 3350 3450 3350
Connection ~ 3450 3350
Wire Wire Line
	2550 3900 3000 3900
Wire Wire Line
	2550 4050 3450 4050
$Comp
L Connector:TestPoint TP?
U 1 1 6011D2A0
P 3450 4050
AR Path="/5FCD4E9D/6011D2A0" Ref="TP?"  Part="1" 
AR Path="/5FCD4EEA/6011D2A0" Ref="TP96"  Part="1" 
F 0 "TP96" H 3508 4168 50  0000 L CNN
F 1 "Spare-Temp" H 3300 4250 50  0000 L CNN
F 2 "RespiraWorks_Std:TestPoint_Pad_2.0x2.0mm" H 3650 4050 50  0001 C CNN
F 3 "~" H 3650 4050 50  0001 C CNN
	1    3450 4050
	1    0    0    -1  
$EndComp
Text HLabel 3750 4050 2    50   Output ~ 0
Spare-Temp
Wire Wire Line
	3750 4050 3450 4050
Connection ~ 3450 4050
Text HLabel 7700 5950 2    50   Output ~ 0
Oxygen-FiO2
$Comp
L RespiraWorks:INA333AIDGKR U?
U 1 1 60132AA0
P 5400 5950
AR Path="/5FCD4EEA/600F5EF3/60132AA0" Ref="U?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132AA0" Ref="U?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132AA0" Ref="U?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132AA0" Ref="U?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132AA0" Ref="U?"  Part="1" 
AR Path="/5FCD4EEA/60132AA0" Ref="U10"  Part="1" 
F 0 "U10" H 5600 6200 50  0000 L CNN
F 1 "INA333AIDGKR" H 5550 6100 50  0000 L CNN
F 2 "RespiraWorks_Std:VSSOP-8_3.0x3.0mm_P0.65mm" H 5400 5950 50  0001 C CNN
F 3 "" H 5400 5950 50  0001 C CNN
F 4 "Texas Instruments" H 5400 5950 50  0001 C CNN "Manufacturer"
F 5 "INA333AIDGKR" H 5400 5950 50  0001 C CNN "Manufacturer PN"
	1    5400 5950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60132AA8
P 4400 6000
AR Path="/60132AA8" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/60132AA8" Ref="R?"  Part="1" 
AR Path="/5E8C8865/60132AA8" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132AA8" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/60132AA8" Ref="R99"  Part="1" 
F 0 "R99" H 4250 6000 50  0000 C CNN
F 1 "2k" H 4250 6100 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 4330 6000 50  0001 C CNN
F 3 "~" H 4400 6000 50  0001 C CNN
F 4 "" H 4400 6000 50  0001 C CNN "Manufacturer"
	1    4400 6000
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60132AAE
P 5400 6250
AR Path="/5FCD4B8E/5FCD4BC5/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132AAE" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/60132AAE" Ref="#PWR0229"  Part="1" 
F 0 "#PWR0229" H 5400 6000 50  0001 C CNN
F 1 "GND" H 5405 6077 50  0000 C CNN
F 2 "" H 5400 6250 50  0001 C CNN
F 3 "" H 5400 6250 50  0001 C CNN
	1    5400 6250
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 60132AB6
P 5650 5250
AR Path="/60132AB6" Ref="C?"  Part="1" 
AR Path="/5E8AFE9D/60132AB6" Ref="C?"  Part="1" 
AR Path="/5E8FBD97/60132AB6" Ref="C?"  Part="1" 
AR Path="/5E8E0754/60132AB6" Ref="C?"  Part="1" 
AR Path="/5E8DFCAD/60132AB6" Ref="C?"  Part="1" 
AR Path="/5E8DEDC0/60132AB6" Ref="C?"  Part="1" 
AR Path="/5E8C8865/60132AB6" Ref="C?"  Part="1" 
AR Path="/5E8E1F08/60132AB6" Ref="C?"  Part="1" 
AR Path="/5FCD4DF5/60132AB6" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132AB6" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132AB6" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132AB6" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132AB6" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132AB6" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/60132AB6" Ref="C58"  Part="1" 
F 0 "C58" H 5750 5050 50  0000 C CNN
F 1 "100nF" H 6000 5150 50  0000 C CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 5688 5100 50  0001 C CNN
F 3 "~" H 5650 5250 50  0001 C CNN
F 4 "Kemet" H 5650 5250 50  0001 C CNN "Manufacturer"
F 5 "X7R" H 5650 5250 50  0001 C CNN "Dielectric"
F 6 "C0603C104K5RACTU" H 5650 5250 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 5650 5250 50  0001 C CNN "Voltage"
	1    5650 5250
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60132ABC
P 5650 5400
AR Path="/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5E8AFE9D/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5E8FBD97/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5E8E0754/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5E8DFCAD/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5E8DEDC0/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5E8C8865/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5E8E1F08/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4DF5/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132ABC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/60132ABC" Ref="#PWR0230"  Part="1" 
F 0 "#PWR0230" H 5650 5150 50  0001 C CNN
F 1 "GND" H 5655 5227 50  0000 C CNN
F 2 "" H 5650 5400 50  0001 C CNN
F 3 "" H 5650 5400 50  0001 C CNN
	1    5650 5400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5400 5100 5400 5650
Wire Wire Line
	5650 5100 5400 5100
Wire Wire Line
	5400 6250 5500 6250
Connection ~ 5400 6250
Wire Wire Line
	5100 6050 4850 6050
Wire Wire Line
	4850 6050 4850 6150
Wire Wire Line
	4850 6150 4400 6150
Wire Wire Line
	5100 5850 4400 5850
Text Notes 6850 5400 0    50   ~ 0
Gain = 51\nSensor output = approx. 0.0006V/%FiO2\n0 to 3.3V output = 0V (0% FiO2) to 3.06V (100% FiO2)\nMust be calibrated in software - this is just a general range - galvanic sensors drift!
$Comp
L Device:R R?
U 1 1 60132ACE
P 3000 6050
AR Path="/60132ACE" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/60132ACE" Ref="R?"  Part="1" 
AR Path="/5E8C8865/60132ACE" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132ACE" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/60132ACE" Ref="R95"  Part="1" 
F 0 "R95" H 2850 6050 50  0000 C CNN
F 1 "10k" H 2800 6150 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2930 6050 50  0001 C CNN
F 3 "~" H 3000 6050 50  0001 C CNN
F 4 "KOA Speer" H 3000 6050 50  0001 C CNN "Manufacturer"
	1    3000 6050
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 60132AD6
P 3000 6350
AR Path="/60132AD6" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/60132AD6" Ref="R?"  Part="1" 
AR Path="/5E8C8865/60132AD6" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132AD6" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/60132AD6" Ref="R96"  Part="1" 
F 0 "R96" H 2850 6350 50  0000 C CNN
F 1 "10k" H 2800 6450 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2930 6350 50  0001 C CNN
F 3 "~" H 3000 6350 50  0001 C CNN
F 4 "KOA Speer" H 3000 6350 50  0001 C CNN "Manufacturer"
	1    3000 6350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60132ADC
P 3000 6500
AR Path="/5FCD4B8E/5FCD4BC5/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132ADC" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/60132ADC" Ref="#PWR0228"  Part="1" 
F 0 "#PWR0228" H 3000 6250 50  0001 C CNN
F 1 "GND" H 3005 6327 50  0000 C CNN
F 2 "" H 3000 6500 50  0001 C CNN
F 3 "" H 3000 6500 50  0001 C CNN
	1    3000 6500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3000 5900 3000 5450
Wire Wire Line
	5100 6150 5050 6150
Wire Wire Line
	5050 6150 5050 6200
Wire Wire Line
	5050 6200 3850 6200
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 60132AF8
P 1500 5850
AR Path="/5FCD4EEA/600F5EF3/60132AF8" Ref="J?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132AF8" Ref="J?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132AF8" Ref="J?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132AF8" Ref="J?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132AF8" Ref="J?"  Part="1" 
AR Path="/5FCD4EEA/60132AF8" Ref="J30"  Part="1" 
F 0 "J30" H 1418 6167 50  0000 C CNN
F 1 "Oxygen sensor" H 1418 6076 50  0000 C CNN
F 2 "RespiraWorks_Std:Molex_Micro-Fit_3.0_43650-0315_1x03_P3.00mm_Vertical" H 1500 5850 50  0001 C CNN
F 3 "~" H 1500 5850 50  0001 C CNN
F 4 "Molex" H 1500 5850 50  0001 C CNN "Manufacturer"
F 5 "43650-0315" H 1500 5850 50  0001 C CNN "Manufacturer PN"
	1    1500 5850
	-1   0    0    -1  
$EndComp
Text Notes 750  5950 0    50   ~ 0
Teledyne R-24\nor equivalent
$Comp
L Device:R R?
U 1 1 60132B07
P 6300 5950
AR Path="/60132B07" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/60132B07" Ref="R?"  Part="1" 
AR Path="/5E8C8865/60132B07" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132B07" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/60132B07" Ref="R101"  Part="1" 
F 0 "R101" V 6500 5950 50  0000 C CNN
F 1 "1.5k" V 6400 5900 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 6230 5950 50  0001 C CNN
F 3 "~" H 6300 5950 50  0001 C CNN
F 4 "KOA Speer" H 6300 5950 50  0001 C CNN "Manufacturer"
	1    6300 5950
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6150 5950 5850 5950
$Comp
L Device:C C?
U 1 1 60132B0E
P 6750 6100
AR Path="/5FCD4B8E/5FCD4BC5/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132B0E" Ref="C?"  Part="1" 
AR Path="/5FCD4EEA/60132B0E" Ref="C59"  Part="1" 
F 0 "C59" H 7050 6100 50  0000 L CNN
F 1 "1uF" H 6850 6200 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6788 5950 50  0001 C CNN
F 3 "~" H 6750 6100 50  0001 C CNN
F 4 "Kemet" H 6750 6100 50  0001 C CNN "Manufacturer"
F 5 "X7R" H 6750 6100 50  0001 C CNN "Dielectric"
F 6 "C0805C105K5RACTU" H 6750 6100 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 6750 6100 50  0001 C CNN "Voltage"
	1    6750 6100
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 60132B14
P 6750 6250
AR Path="/5FCD4B8E/5FCD4BC5/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132B14" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/60132B14" Ref="#PWR0232"  Part="1" 
F 0 "#PWR0232" H 6750 6000 50  0001 C CNN
F 1 "GND" H 6755 6077 50  0000 C CNN
F 2 "" H 6750 6250 50  0001 C CNN
F 3 "" H 6750 6250 50  0001 C CNN
	1    6750 6250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6450 5950 6750 5950
Connection ~ 6750 5950
Wire Wire Line
	6750 5950 7450 5950
Text Notes 6700 5850 0    50   ~ 0
LPF fc = 106 Hz
Text Notes 4100 5650 0    50   ~ 0
Rg = 100K/(Gain - 1)
Text Notes 4250 6050 0    50   ~ 0
Rg
$Comp
L Device:R R?
U 1 1 60132B22
P 5850 6200
AR Path="/60132B22" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/60132B22" Ref="R?"  Part="1" 
AR Path="/5E8C8865/60132B22" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132B22" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/60132B22" Ref="R100"  Part="1" 
F 0 "R100" H 5950 6150 50  0000 C CNN
F 1 "10k" H 6000 6250 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 5780 6200 50  0001 C CNN
F 3 "~" H 5850 6200 50  0001 C CNN
F 4 "KOA Speer" H 5850 6200 50  0001 C CNN "Manufacturer"
F 5 "DNP" H 6000 6050 50  0000 C CNN "DNP"
	1    5850 6200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 6050 5850 5950
Connection ~ 5850 5950
Wire Wire Line
	5850 5950 5800 5950
$Comp
L power:GND #PWR?
U 1 1 60132B2B
P 5850 6350
AR Path="/5FCD4B8E/5FCD4BC5/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FCD4BF6/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/606EF10F/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132B2B" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4EEA/60132B2B" Ref="#PWR0231"  Part="1" 
F 0 "#PWR0231" H 5850 6100 50  0001 C CNN
F 1 "GND" H 5855 6177 50  0000 C CNN
F 2 "" H 5850 6350 50  0001 C CNN
F 3 "" H 5850 6350 50  0001 C CNN
	1    5850 6350
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60132B33
P 3700 5750
AR Path="/60132B33" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/60132B33" Ref="R?"  Part="1" 
AR Path="/5E8C8865/60132B33" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60132B33" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/60132B33" Ref="R97"  Part="1" 
F 0 "R97" V 3900 5800 50  0000 C CNN
F 1 "10k" V 3800 5750 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 3630 5750 50  0001 C CNN
F 3 "~" H 3700 5750 50  0001 C CNN
F 4 "KOA Speer" H 3700 5750 50  0001 C CNN "Manufacturer"
	1    3700 5750
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3550 5750 2700 5750
Text Notes 3550 6750 0    50   ~ 0
Series input protection:\n200 pA max. input bias for INA333\n= 2 µV max. offset << 25 µV of\nINA333's own input offset
Text Notes 4900 7050 0    50   ~ 0
Optionally holds output low (10K or so)\nin case INA333 fails or its supply voltage\ndisappears
Wire Notes Line
	5800 6200 5650 6200
Wire Notes Line
	5650 6200 5650 6800
NoConn ~ 1700 5850
$Comp
L Device:R R?
U 1 1 601505E1
P 3700 6200
AR Path="/601505E1" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/601505E1" Ref="R?"  Part="1" 
AR Path="/5E8C8865/601505E1" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/601505E1" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/601505E1" Ref="R98"  Part="1" 
F 0 "R98" V 3900 6250 50  0000 C CNN
F 1 "10k" V 3800 6200 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 3630 6200 50  0001 C CNN
F 3 "~" H 3700 6200 50  0001 C CNN
F 4 "KOA Speer" H 3700 6200 50  0001 C CNN "Manufacturer"
	1    3700 6200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3550 6200 3000 6200
Connection ~ 3000 6200
Wire Wire Line
	3850 5750 5100 5750
$Comp
L Device:R R?
U 1 1 60153672
P 2700 5600
AR Path="/60153672" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/60153672" Ref="R?"  Part="1" 
AR Path="/5E8C8865/60153672" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/60153672" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/60153672" Ref="R93"  Part="1" 
F 0 "R93" H 2500 5550 50  0000 C CNN
F 1 "10k" H 2500 5650 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2630 5600 50  0001 C CNN
F 3 "~" H 2700 5600 50  0001 C CNN
F 4 "KOA Speer" H 2700 5600 50  0001 C CNN "Manufacturer"
	1    2700 5600
	1    0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 601544DC
P 2200 5900
AR Path="/601544DC" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/601544DC" Ref="R?"  Part="1" 
AR Path="/5E8C8865/601544DC" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/601544DC" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/601544DC" Ref="R92"  Part="1" 
F 0 "R92" H 2000 5850 50  0000 C CNN
F 1 "10k" H 2000 5950 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2130 5900 50  0001 C CNN
F 3 "~" H 2200 5900 50  0001 C CNN
F 4 "KOA Speer" H 2200 5900 50  0001 C CNN "Manufacturer"
	1    2200 5900
	1    0    0    1   
$EndComp
Wire Wire Line
	2700 5750 2200 5750
Connection ~ 2700 5750
Wire Wire Line
	2700 5450 3000 5450
$Comp
L Device:R R?
U 1 1 6015D0A5
P 2700 5900
AR Path="/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5E8AFE9D/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5E8C8865/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5E8E1F08/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4DF5/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F5EF3/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F7154/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F719E/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F71F1/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/600F723B/6015D0A5" Ref="R?"  Part="1" 
AR Path="/5FCD4EEA/6015D0A5" Ref="R94"  Part="1" 
F 0 "R94" H 2500 5850 50  0000 C CNN
F 1 "10k" H 2500 5950 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2630 5900 50  0001 C CNN
F 3 "~" H 2700 5900 50  0001 C CNN
F 4 "KOA Speer" H 2700 5900 50  0001 C CNN "Manufacturer"
	1    2700 5900
	1    0    0    1   
$EndComp
Wire Notes Line
	3700 6400 3700 6250
Wire Notes Line
	4000 6400 4000 5900
Wire Notes Line
	3700 5900 3700 5800
Wire Notes Line
	3700 5900 4000 5900
Connection ~ 2200 5750
Wire Wire Line
	1700 5750 2200 5750
Wire Wire Line
	1700 5950 1700 6200
Wire Wire Line
	1700 6200 2200 6200
Wire Wire Line
	2700 6500 3000 6500
Connection ~ 3000 6500
Wire Wire Line
	2700 6050 2700 6500
Wire Wire Line
	2200 6050 2200 6200
Connection ~ 2200 6200
Wire Wire Line
	2200 6200 3000 6200
Text HLabel 5250 850  2    50   Input ~ 0
3V3
Wire Wire Line
	3000 1100 3000 850 
Wire Wire Line
	3000 850  5250 850 
Wire Wire Line
	2700 5450 2700 5300
Wire Wire Line
	2700 5300 2350 5300
Connection ~ 2700 5450
Text Label 2350 5300 0    50   ~ 0
3V3
Wire Wire Line
	5400 5100 5050 5100
Text Label 5050 5100 0    50   ~ 0
3V3
Connection ~ 5400 5100
Wire Wire Line
	3000 3900 3000 3200
Connection ~ 3000 1100
Connection ~ 3000 1800
Wire Wire Line
	3000 1800 3000 1100
Connection ~ 3000 2500
Wire Wire Line
	3000 2500 3000 1800
Connection ~ 3000 3200
Wire Wire Line
	3000 3200 3000 2500
$Comp
L Connector:TestPoint TP?
U 1 1 60F7C3BE
P 7450 5950
AR Path="/5FCD50A5/60F7C3BE" Ref="TP?"  Part="1" 
AR Path="/6052CC9F/60F7C3BE" Ref="TP?"  Part="1" 
AR Path="/60F7C3BE" Ref="TP?"  Part="1" 
AR Path="/5FCD4D18/60F7C3BE" Ref="TP?"  Part="1" 
AR Path="/5FCD4B8E/60F7C3BE" Ref="TP?"  Part="1" 
AR Path="/5FCD4EEA/60F7C3BE" Ref="TP97"  Part="1" 
F 0 "TP97" H 7450 6150 50  0000 C CNN
F 1 "TP" V 7500 6000 50  0001 C CNN
F 2 "RespiraWorks_Std:TestPoint_Pad_2.0x2.0mm" H 7650 5950 50  0001 C CNN
F 3 "~" H 7650 5950 50  0001 C CNN
	1    7450 5950
	1    0    0    -1  
$EndComp
Connection ~ 7450 5950
Wire Wire Line
	7450 5950 7700 5950
$EndSCHEMATC
