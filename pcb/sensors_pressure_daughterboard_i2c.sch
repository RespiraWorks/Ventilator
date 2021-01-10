EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 20 30
Title "Pressure Sensor Daughterboard Connections, I2C"
Date ""
Rev "2.0"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 600D289A
P 5300 4150
AR Path="/5FCD4E9D/600630A6/600D289A" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/600D289A" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/600D289A" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/600D289A" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/600D289A" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/600D289A" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/600D289A" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/600CF516/600D289A" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/600D74A6/600D289A" Ref="J?"  Part="1" 
F 0 "J?" H 5380 4192 50  0000 L CNN
F 1 "0.1\" male header" H 5380 4101 50  0000 L CNN
F 2 "" H 5300 4150 50  0001 C CNN
F 3 "~" H 5300 4150 50  0001 C CNN
	1    5300 4150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J?
U 1 1 600D28A0
P 5300 3350
AR Path="/5FCD4E9D/600630A6/600D28A0" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/600D28A0" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/600D28A0" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/600D28A0" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/600D28A0" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/600D28A0" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/600D28A0" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/600CF516/600D28A0" Ref="J?"  Part="1" 
AR Path="/5FCD4E9D/600D74A6/600D28A0" Ref="J?"  Part="1" 
F 0 "J?" H 5218 3667 50  0000 C CNN
F 1 "0.1\" male header" H 5218 3576 50  0000 C CNN
F 2 "" H 5300 3350 50  0001 C CNN
F 3 "~" H 5300 3350 50  0001 C CNN
	1    5300 3350
	-1   0    0    -1  
$EndComp
Text Notes 4400 2950 0    50   ~ 0
TODO: create dummy component for mech placement\nw/mounting holes, etc.
$Comp
L power:GND #PWR?
U 1 1 600D28A7
P 5100 4300
AR Path="/5FCD4B8E/5FCD4BC5/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600CF516/600D28A7" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600D74A6/600D28A7" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5100 4050 50  0001 C CNN
F 1 "GND" H 5105 4127 50  0000 C CNN
F 2 "" H 5100 4300 50  0001 C CNN
F 3 "" H 5100 4300 50  0001 C CNN
	1    5100 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 4300 5100 4250
Connection ~ 5100 4250
Wire Wire Line
	5100 4250 5100 4150
$Comp
L power:GND #PWR?
U 1 1 600D28B0
P 5500 3550
AR Path="/5FCD4B8E/5FCD4BC5/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4B8E/5FE27F70/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4D85/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600630A6/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/60073FCF/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007AC3B/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007BD34/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007DA64/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/6007E2BB/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600CF516/600D28B0" Ref="#PWR?"  Part="1" 
AR Path="/5FCD4E9D/600D74A6/600D28B0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 5500 3300 50  0001 C CNN
F 1 "GND" H 5505 3377 50  0000 C CNN
F 2 "" H 5500 3550 50  0001 C CNN
F 3 "" H 5500 3550 50  0001 C CNN
	1    5500 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3550 5500 3450
Text HLabel 5950 3250 2    50   BiDi ~ 0
CC-SDA
Text HLabel 5950 3350 2    50   Input ~ 0
CC-SCL
Wire Wire Line
	5950 3350 5500 3350
Wire Wire Line
	5500 3250 5950 3250
Wire Wire Line
	4850 4050 5100 4050
Text HLabel 4850 4050 0    50   Input ~ 0
Vcc
$EndSCHEMATC
