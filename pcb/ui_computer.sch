EESchema Schematic File Version 4
LIBS:Ventilator-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 14
Title "UI Computer"
Date ""
Rev "2.0"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RespiraWorks:ESQ-120-44-L-D J?
U 1 1 5FDAF122
P 5700 3350
F 0 "J?" H 5725 4625 50  0000 C CNN
F 1 "ESQ-120-44-L-D" H 5725 4534 50  0000 C CNN
F 2 "" H 5100 4450 50  0001 C CNN
F 3 "" H 5100 4450 50  0001 C CNN
	1    5700 3350
	1    0    0    -1  
$EndComp
$Comp
L RespiraWorks:+3V3_RPi #PWR?
U 1 1 5FDB01F4
P 4750 2250
F 0 "#PWR?" H 4750 2300 50  0001 C CNN
F 1 "+3V3_RPi" H 4735 2423 50  0000 C CNN
F 2 "" H 4750 2250 50  0001 C CNN
F 3 "" H 4750 2250 50  0001 C CNN
	1    4750 2250
	1    0    0    -1  
$EndComp
Text Notes 5200 1950 0    50   ~ 0
RPI has internal I2C pull-ups\n
NoConn ~ 5000 2400
NoConn ~ 5000 2500
Text HLabel 4550 2600 0    50   Output ~ 0
UI_LED_R
Wire Wire Line
	4550 2600 5000 2600
$Comp
L power:GND #PWR?
U 1 1 5FDB0314
P 4850 4300
F 0 "#PWR?" H 4850 4050 50  0001 C CNN
F 1 "GND" H 4855 4127 50  0000 C CNN
F 2 "" H 4850 4300 50  0001 C CNN
F 3 "" H 4850 4300 50  0001 C CNN
	1    4850 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 2700 4850 2700
Wire Wire Line
	4850 2700 4850 3500
Wire Wire Line
	5000 3500 4850 3500
Connection ~ 4850 3500
Wire Wire Line
	4850 3500 4850 4200
Wire Wire Line
	5000 4200 4850 4200
Connection ~ 4850 4200
Wire Wire Line
	4850 4200 4850 4300
Wire Wire Line
	5000 2300 4750 2300
Wire Wire Line
	4750 2300 4750 2250
Text HLabel 4550 2900 0    50   Output ~ 0
GUI_BUZZER
Wire Wire Line
	4550 2900 5000 2900
Text HLabel 4550 3700 0    50   Output ~ 0
UI_LED_Y
Wire Wire Line
	4550 3700 5000 3700
Text HLabel 4550 3800 0    50   Output ~ 0
UI_LED_G
Wire Wire Line
	4550 3800 5000 3800
Text Notes 5400 1850 0    50   ~ 0
RPI powered by 5V\n
Text HLabel 4550 3900 0    50   Output ~ 0
UART_CTS
Wire Wire Line
	5000 3900 4550 3900
$Comp
L Device:R R?
U 1 1 5FDB08DB
P 3950 4350
F 0 "R?" H 4020 4396 50  0000 L CNN
F 1 "R" H 4020 4305 50  0000 L CNN
F 2 "" V 3880 4350 50  0001 C CNN
F 3 "~" H 3950 4350 50  0001 C CNN
	1    3950 4350
	1    0    0    -1  
$EndComp
Text Notes 3350 4400 0    50   ~ 0
Change this R\n
$Comp
L RespiraWorks:+3V3_RPi #PWR?
U 1 1 5FDB0BF2
P 3950 4000
F 0 "#PWR?" H 3950 4050 50  0001 C CNN
F 1 "+3V3_RPi" H 3935 4173 50  0000 C CNN
F 2 "" H 3950 4000 50  0001 C CNN
F 3 "" H 3950 4000 50  0001 C CNN
	1    3950 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 4100 3950 4000
Wire Wire Line
	3950 4100 5000 4100
Wire Wire Line
	3950 4200 3950 4100
Connection ~ 3950 4100
$Comp
L power:GND #PWR?
U 1 1 5FDB0EEF
P 3950 4500
F 0 "#PWR?" H 3950 4250 50  0001 C CNN
F 1 "GND" H 3955 4327 50  0000 C CNN
F 2 "" H 3950 4500 50  0001 C CNN
F 3 "" H 3950 4500 50  0001 C CNN
	1    3950 4500
	1    0    0    -1  
$EndComp
Text Label 4200 4100 0    50   ~ 0
UI_ID1
NoConn ~ 5000 2800
$EndSCHEMATC
