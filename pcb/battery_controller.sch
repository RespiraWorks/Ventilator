EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 31 34
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 3950 3200 1200 700 
U 60567144
F0 "Sheet60567143" 50
F1 "battery_charger.sch" 50
F2 "SCL" I L 3950 3700 50 
F3 "SDA" B L 3950 3800 50 
F4 "VLOGIC" I L 3950 3600 50 
F5 "PSYS" I R 5150 3500 50 
F6 "IBAT" I R 5150 3600 50 
F7 "IADPT" I R 5150 3700 50 
F8 "ACOK" I R 5150 3800 50 
F9 "VIN_ADP" I L 3950 3300 50 
F10 "VLINK" I R 5150 3300 50 
F11 "PACK_P" B R 5150 3400 50 
$EndSheet
$Sheet
S 6350 4550 900  550 
U 60567757
F0 "Sheet60567756" 50
F1 "battery_balancer.sch" 50
F2 "BAT_N" I R 7250 4950 50 
F3 "SYS_N" O L 6350 5050 50 
F4 "BAT_P" I R 7250 4650 50 
F5 "SYS_P" O L 6350 4650 50 
F6 "SCL" I L 6350 4750 50 
F7 "SDA" B L 6350 4850 50 
F8 "ALRT" O L 6350 4950 50 
$EndSheet
$Sheet
S 7200 3200 700  450 
U 60567D6E
F0 "Sheet60567D6D" 50
F1 "power_vsys.sch" 50
F2 "VIN" I L 7200 3300 50 
F3 "12VOUT" O R 7900 3300 50 
F4 "VLOGIC" I L 7200 3450 50 
F5 "PG" O R 7900 3450 50 
$EndSheet
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 605A607B
P 2000 3300
F 0 "J?" H 2250 2750 50  0000 R CNN
F 1 "MegaFit 2x2" H 2250 2850 50  0000 R CNN
F 2 "RespiraWorks_Std:Molex_Mega-Fit_76829-0104_2x02_P5.70mm_Vertical" H 2000 3300 50  0001 C CNN
F 3 "~" H 2000 3300 50  0001 C CNN
F 4 "Molex" H 2000 3300 50  0001 C CNN "Manufacturer"
F 5 "768290104" H 2250 2950 50  0000 R CNN "Manufacturer P/N"
	1    2000 3300
	1    0    0    1   
$EndComp
Text Label 7750 4950 0    50   ~ 0
BAT_N
Text Label 7750 4650 0    50   ~ 0
BAT_P
$Comp
L Device:C C?
U 1 1 605A6090
P 7550 4800
F 0 "C?" H 7350 4850 50  0000 L CNN
F 1 "10uF" H 7350 4700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7588 4650 50  0001 C CNN
F 3 "~" H 7550 4800 50  0001 C CNN
	1    7550 4800
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J?
U 1 1 605C357B
P 8700 4950
AR Path="/601A64F8/605C357B" Ref="J?"  Part="1" 
AR Path="/601A64F8/60567144/605C357B" Ref="J?"  Part="1" 
F 0 "J?" H 8800 5050 50  0000 R CNN
F 1 "MegaFit 2x1" H 8800 5150 50  0000 R CNN
F 2 "RespiraWorks_Std:Molex_Mega-Fit_76829-0102_2x01_P5.70mm_Vertical" H 8700 4950 50  0001 C CNN
F 3 "~" H 8700 4950 50  0001 C CNN
F 4 "Molex" H 8700 4950 50  0001 C CNN "Manufacturer"
F 5 "768290102" H 8600 5250 50  0000 C CNN "Manufacturer P/N"
	1    8700 4950
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605C3581
P 2200 3400
AR Path="/601A64F8/605C3581" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/605C3581" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2200 3150 50  0001 C CNN
F 1 "GND" H 2205 3227 50  0000 C CNN
F 2 "" H 2200 3400 50  0001 C CNN
F 3 "" H 2200 3400 50  0001 C CNN
	1    2200 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Zener D?
U 1 1 605C3588
P 2400 3450
AR Path="/601A64F8/605C3588" Ref="D?"  Part="1" 
AR Path="/601A64F8/60567144/605C3588" Ref="D?"  Part="1" 
F 0 "D?" V 2350 3550 50  0000 L CNN
F 1 "20V" V 2450 3550 50  0000 L CNN
F 2 "Diode_SMD:D_SMC" H 2400 3450 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds40740.pdf" H 2400 3450 50  0001 C CNN
F 4 "Diodes Inc." V 2400 3450 50  0001 C CNN "Manufacturer"
F 5 "SMBJ20AQ-13-F" V 2550 3800 50  0000 C CNN "Manufacturer PN"
	1    2400 3450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605C358F
P 2400 3600
AR Path="/601A64F8/605C358F" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/605C358F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2400 3350 50  0001 C CNN
F 1 "GND" H 2405 3427 50  0000 C CNN
F 2 "" H 2400 3600 50  0001 C CNN
F 3 "" H 2400 3600 50  0001 C CNN
	1    2400 3600
	1    0    0    -1  
$EndComp
Connection ~ 2400 3300
Text Label 3600 3300 0    50   ~ 0
VIN_ADP
Wire Wire Line
	2200 3200 2800 3200
Wire Wire Line
	2800 3100 2200 3100
Text Label 2800 3100 2    50   ~ 0
POWER_SW_P
Text Label 2800 3200 2    50   ~ 0
POWER_SW_N
Wire Wire Line
	2200 3300 2400 3300
Wire Wire Line
	6600 3300 7200 3300
Text Label 6600 3300 0    50   ~ 0
POWER_SW_N
Wire Wire Line
	5750 3300 5150 3300
Text Label 5750 3300 2    50   ~ 0
POWER_SW_P
$Comp
L Device:Fuse F?
U 1 1 603A1854
P 3200 3300
F 0 "F?" V 3003 3300 50  0000 C CNN
F 1 "Fuse 10A" V 3094 3300 50  0000 C CNN
F 2 "" V 3130 3300 50  0001 C CNN
F 3 "https://www.littelfuse.com/~/media/electronics/datasheets/fuses/littelfuse_fuse_154_154t_154l_154tl_datasheet.pdf.pdf" H 3200 3300 50  0001 C CNN
F 4 "Littelfuse" V 3200 3300 50  0001 C CNN "Manufacturer"
F 5 "0154010.DR" V 3300 3300 50  0000 C CNN "Manufacturer PN"
	1    3200 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	2400 3300 3050 3300
Wire Wire Line
	3350 3300 3950 3300
Wire Wire Line
	8350 4850 8500 4850
Wire Wire Line
	3950 3600 3450 3600
Text HLabel 3450 3600 0    50   Input ~ 0
VLOGIC
Text Label 3500 3600 0    50   ~ 0
VLOGIC
Wire Wire Line
	7200 3450 6700 3450
Text Label 6700 3450 0    50   ~ 0
VLOGIC
Wire Wire Line
	7900 3450 8150 3450
Wire Wire Line
	7900 3300 8150 3300
Text HLabel 8150 3300 2    50   Input ~ 0
12VOUT
Text HLabel 8150 3450 2    50   Input ~ 0
PG_12V
Wire Wire Line
	5150 3500 5400 3500
Text HLabel 5400 3500 2    50   Input ~ 0
PMON_PSYS
Wire Wire Line
	5150 3600 5400 3600
Text HLabel 5400 3600 2    50   Input ~ 0
PMON_IBAT
Wire Wire Line
	5150 3700 5400 3700
Text HLabel 5400 3700 2    50   Input ~ 0
PMON_IADP
Wire Wire Line
	5150 3800 5400 3800
Text HLabel 5400 3800 2    50   Input ~ 0
PMON_ACOK
Wire Wire Line
	3950 3700 3450 3700
Text HLabel 3450 3700 0    50   Input ~ 0
SCL
Wire Wire Line
	3950 3800 3450 3800
Text HLabel 3450 3800 0    50   BiDi ~ 0
SDA
Wire Wire Line
	7550 4950 8500 4950
Wire Wire Line
	7550 4650 7250 4650
Connection ~ 7550 4650
Wire Wire Line
	7550 4950 7250 4950
Connection ~ 7550 4950
$Comp
L power:GND #PWR?
U 1 1 6047EA81
P 6150 5200
AR Path="/601A64F8/6047EA81" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/6047EA81" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 6150 4950 50  0001 C CNN
F 1 "GND" H 6155 5027 50  0000 C CNN
F 2 "" H 6150 5200 50  0001 C CNN
F 3 "" H 6150 5200 50  0001 C CNN
	1    6150 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3400 6150 3400
Wire Wire Line
	6150 4650 6350 4650
Wire Wire Line
	6150 5050 6150 5200
Wire Wire Line
	6150 3400 6150 4650
Text Label 3550 3700 0    50   ~ 0
SCL
Text Label 3550 3800 0    50   ~ 0
SDA
Wire Wire Line
	6350 4750 5850 4750
Wire Wire Line
	6350 4850 5850 4850
Text Label 5850 4750 0    50   ~ 0
SCL
Text Label 5850 4850 0    50   ~ 0
SDA
Wire Wire Line
	6350 5050 6150 5050
Wire Wire Line
	5850 4950 6350 4950
Text HLabel 5850 4950 0    50   Output ~ 0
BAT_ALRT
Wire Wire Line
	7550 4650 7950 4650
Wire Wire Line
	7950 4650 7950 4850
Wire Wire Line
	7950 4850 8050 4850
$Comp
L Device:Fuse F?
U 1 1 602914E1
P 8200 4850
F 0 "F?" V 7900 4850 50  0000 C CNN
F 1 "Fuse 10A" V 8000 4850 50  0000 C CNN
F 2 "" V 8130 4850 50  0001 C CNN
F 3 "https://www.littelfuse.com/~/media/electronics/datasheets/fuses/littelfuse_fuse_154_154t_154l_154tl_datasheet.pdf.pdf" H 8200 4850 50  0001 C CNN
F 4 "Littelfuse" V 8200 4850 50  0001 C CNN "Manufacturer"
F 5 "0154010.DR" V 8100 4850 50  0000 C CNN "Manufacturer PN"
	1    8200 4850
	0    1    1    0   
$EndComp
$EndSCHEMATC
