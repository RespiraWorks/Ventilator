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
S 5150 3200 1200 700 
U 60567144
F0 "Sheet60567143" 50
F1 "battery_charger.sch" 50
F2 "SCL" I L 5150 3700 50 
F3 "SDA" B L 5150 3800 50 
F4 "VLOGIC" I L 5150 3600 50 
F5 "VIN_ADP" I L 5150 3300 50 
F6 "PACK_P" B R 6350 3400 50 
F7 "VLINK" O R 6350 3300 50 
F8 "PSYS" O R 6350 3500 50 
F9 "IBAT" O R 6350 3600 50 
F10 "IADPT" O R 6350 3700 50 
F11 "ACOK" O R 6350 3800 50 
$EndSheet
$Sheet
S 7550 4550 900  550 
U 60567757
F0 "Sheet60567756" 50
F1 "battery_balancer.sch" 50
F2 "BAT_N" I R 8450 4950 50 
F3 "SYS_N" O L 7550 5050 50 
F4 "BAT_P" I R 8450 4650 50 
F5 "SYS_P" O L 7550 4650 50 
F6 "SCL" I L 7550 4750 50 
F7 "SDA" B L 7550 4850 50 
F8 "ALRT" O L 7550 4950 50 
$EndSheet
$Sheet
S 8400 3200 700  450 
U 60567D6E
F0 "Sheet60567D6D" 50
F1 "power_vsys.sch" 50
F2 "VIN" I L 8400 3300 50 
F3 "12VOUT" O R 9100 3300 50 
F4 "VLOGIC" I L 8400 3450 50 
F5 "PG" O R 9100 3450 50 
$EndSheet
Text Label 8950 4950 0    50   ~ 0
BAT_N
Text Label 8950 4650 0    50   ~ 0
BAT_P
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 605C357B
P 9900 4950
AR Path="/601A64F8/605C357B" Ref="J?"  Part="1" 
AR Path="/601A64F8/60567144/605C357B" Ref="J?"  Part="1" 
AR Path="/6023BF6A/605C357B" Ref="J?"  Part="1" 
F 0 "J?" H 10000 5050 50  0000 R CNN
F 1 "BAT-PWR MegaFit 2x1" H 10000 5150 50  0000 R CNN
F 2 "RespiraWorks_Std:Molex_Mega-Fit_76829-0102_2x01_P5.70mm_Vertical" H 9900 4950 50  0001 C CNN
F 3 "~" H 9900 4950 50  0001 C CNN
F 4 "Molex" H 9900 4950 50  0001 C CNN "Manufacturer"
F 5 "768290102" H 9800 5250 50  0000 C CNN "Manufacturer P/N"
	1    9900 4950
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605C3581
P 1750 3400
AR Path="/601A64F8/605C3581" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/605C3581" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/605C3581" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 1750 3150 50  0001 C CNN
F 1 "GND" H 1755 3227 50  0000 C CNN
F 2 "" H 1750 3400 50  0001 C CNN
F 3 "" H 1750 3400 50  0001 C CNN
	1    1750 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Zener D?
U 1 1 605C3588
P 3100 3450
AR Path="/601A64F8/605C3588" Ref="D?"  Part="1" 
AR Path="/601A64F8/60567144/605C3588" Ref="D?"  Part="1" 
AR Path="/6023BF6A/605C3588" Ref="D?"  Part="1" 
F 0 "D?" V 3050 3550 50  0000 L CNN
F 1 "20V" V 3150 3550 50  0000 L CNN
F 2 "RespiraWorks_Std:D_SMC" H 3100 3450 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds40740.pdf" H 3100 3450 50  0001 C CNN
F 4 "Diodes Inc." V 3100 3450 50  0001 C CNN "Manufacturer"
F 5 "SMBJ20AQ-13-F" V 3250 3800 50  0000 C CNN "Manufacturer PN"
	1    3100 3450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 605C358F
P 3100 3600
AR Path="/601A64F8/605C358F" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/605C358F" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/605C358F" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3100 3350 50  0001 C CNN
F 1 "GND" H 3105 3427 50  0000 C CNN
F 2 "" H 3100 3600 50  0001 C CNN
F 3 "" H 3100 3600 50  0001 C CNN
	1    3100 3600
	1    0    0    -1  
$EndComp
Text Label 4800 3300 0    50   ~ 0
VIN_ADP
Wire Wire Line
	1750 3200 2350 3200
Wire Wire Line
	2350 3100 1750 3100
Text Label 2350 3100 2    50   ~ 0
POWER_SW_P
Text Label 2350 3200 2    50   ~ 0
POWER_SW_N
Wire Wire Line
	7800 3300 8350 3300
Text Label 7800 3300 0    50   ~ 0
POWER_SW_N
Wire Wire Line
	6950 3300 6350 3300
Text Label 6950 3300 2    50   ~ 0
POWER_SW_P
$Comp
L RespiraWorks:0154010.DR F?
U 1 1 603A1854
P 2700 3300
F 0 "F?" V 2503 3300 50  0000 C CNN
F 1 "Fuse 10A" V 2594 3300 50  0000 C CNN
F 2 "RespiraWorks:Fuseholder_Littelfuse_OMNI-BLOK-154" V 2630 3300 50  0001 C CNN
F 3 "https://www.littelfuse.com/~/media/electronics/datasheets/fuses/littelfuse_fuse_154_154t_154l_154tl_datasheet.pdf.pdf" H 2700 3300 50  0001 C CNN
F 4 "Littelfuse" V 2700 3300 50  0001 C CNN "Manufacturer"
F 5 "0154010.DR" V 2800 3300 50  0000 C CNN "Manufacturer PN"
	1    2700 3300
	0    1    1    0   
$EndComp
Wire Wire Line
	9550 4850 9700 4850
Wire Wire Line
	5150 3600 4650 3600
Text HLabel 4650 3600 0    50   Input ~ 0
VLOGIC
Text Label 4700 3600 0    50   ~ 0
VLOGIC
Wire Wire Line
	8400 3450 7900 3450
Text Label 7900 3450 0    50   ~ 0
VLOGIC
Wire Wire Line
	9100 3450 9350 3450
Wire Wire Line
	9100 3300 9350 3300
Text HLabel 9350 3300 2    50   Output ~ 0
12VOUT
Text HLabel 9350 3450 2    50   Output ~ 0
PG_12V
Wire Wire Line
	6350 3500 6600 3500
Text HLabel 6600 3500 2    50   Output ~ 0
PMON_PSYS
Wire Wire Line
	6350 3600 6600 3600
Text HLabel 6600 3600 2    50   Output ~ 0
PMON_IBAT
Wire Wire Line
	6350 3700 6600 3700
Text HLabel 6600 3700 2    50   Output ~ 0
PMON_IADP
Wire Wire Line
	6350 3800 6600 3800
Text HLabel 6600 3800 2    50   Output ~ 0
PMON_ACOK
Wire Wire Line
	5150 3700 4650 3700
Text HLabel 4650 3700 0    50   Input ~ 0
SCL
Wire Wire Line
	5150 3800 4650 3800
Text HLabel 4650 3800 0    50   BiDi ~ 0
SDA
$Comp
L power:GND #PWR?
U 1 1 6047EA81
P 7350 5200
AR Path="/601A64F8/6047EA81" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/6047EA81" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/6047EA81" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 7350 4950 50  0001 C CNN
F 1 "GND" H 7355 5027 50  0000 C CNN
F 2 "" H 7350 5200 50  0001 C CNN
F 3 "" H 7350 5200 50  0001 C CNN
	1    7350 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 3400 7350 3400
Wire Wire Line
	7350 4650 7550 4650
Wire Wire Line
	7350 5050 7350 5200
Wire Wire Line
	7350 3400 7350 4650
Text Label 4750 3700 0    50   ~ 0
SCL
Text Label 4750 3800 0    50   ~ 0
SDA
Wire Wire Line
	7550 4750 7050 4750
Wire Wire Line
	7550 4850 7050 4850
Text Label 7050 4750 0    50   ~ 0
SCL
Text Label 7050 4850 0    50   ~ 0
SDA
Wire Wire Line
	7550 5050 7350 5050
Wire Wire Line
	7050 4950 7550 4950
Text HLabel 7050 4950 0    50   Output ~ 0
BAT_ALRT
Wire Wire Line
	9150 4650 9150 4850
Wire Wire Line
	9150 4850 9250 4850
$Comp
L RespiraWorks:0154010.DR F?
U 1 1 602914E1
P 9400 4850
F 0 "F?" V 9100 4850 50  0000 C CNN
F 1 "Fuse 10A" V 9200 4850 50  0000 C CNN
F 2 "RespiraWorks:Fuseholder_Littelfuse_OMNI-BLOK-154" V 9330 4850 50  0001 C CNN
F 3 "https://www.littelfuse.com/~/media/electronics/datasheets/fuses/littelfuse_fuse_154_154t_154l_154tl_datasheet.pdf.pdf" H 9400 4850 50  0001 C CNN
F 4 "Littelfuse" V 9400 4850 50  0001 C CNN "Manufacturer"
F 5 "0154010.DR" V 9300 4850 50  0000 C CNN "Manufacturer PN"
	1    9400 4850
	0    1    1    0   
$EndComp
Wire Wire Line
	8350 3300 8350 3000
Wire Wire Line
	8350 3000 9350 3000
Connection ~ 8350 3300
Wire Wire Line
	8350 3300 8400 3300
Text HLabel 9350 3000 2    50   Output ~ 0
SWITCHED_VLINK
Text Notes 8800 3000 0    50   ~ 0
12.0-16.8V
Wire Wire Line
	3100 3300 3900 3300
$Comp
L Device:CP C?
U 1 1 6026FB4D
P 3900 3450
AR Path="/601A64F8/6026FB4D" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/6026FB4D" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567144/6026FB4D" Ref="C?"  Part="1" 
AR Path="/6023BF6A/6026FB4D" Ref="C?"  Part="1" 
F 0 "C?" H 4050 3450 50  0000 L CNN
F 1 "33uF" H 4050 3350 50  0000 L CNN
F 2 "RespiraWorks_Std:CP_Elec_6.3x7.7" H 3938 3300 50  0001 C CNN
F 3 "https://industrial.panasonic.com/cdbs/www-data/pdf/RDE0000/ABA0000C1181.pdf" H 3900 3450 50  0001 C CNN
F 4 "50V" H 4050 3250 50  0000 L CNN "Voltage"
F 5 "Panasonic" H 3900 3450 50  0001 C CNN "Manufacturer"
F 6 "EEE-FK1H330XP" H 3900 3450 50  0001 C CNN "Manufacturer PN"
	1    3900 3450
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 602707F0
P 3900 3600
AR Path="/601A64F8/602707F0" Ref="#PWR?"  Part="1" 
AR Path="/601A64F8/60567144/602707F0" Ref="#PWR?"  Part="1" 
AR Path="/6023BF6A/602707F0" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3900 3350 50  0001 C CNN
F 1 "GND" H 3905 3427 50  0000 C CNN
F 2 "" H 3900 3600 50  0001 C CNN
F 3 "" H 3900 3600 50  0001 C CNN
	1    3900 3600
	1    0    0    -1  
$EndComp
Connection ~ 3900 3300
Wire Wire Line
	3900 3300 5150 3300
Wire Wire Line
	1750 3300 2550 3300
Wire Wire Line
	2850 3300 3100 3300
Connection ~ 3100 3300
$Comp
L RespiraWorks_Std:Conn_01x04 J?
U 1 1 605A607B
P 1550 3300
F 0 "J?" H 1500 2750 50  0000 R CNN
F 1 "POWER-ENTRY MegaFit 2x2" H 2150 2850 50  0000 R CNN
F 2 "RespiraWorks_Std:Molex_Mega-Fit_76829-0104_2x02_P5.70mm_Vertical" H 1550 3300 50  0001 C CNN
F 3 "~" H 1550 3300 50  0001 C CNN
F 4 "Molex" H 1550 3300 50  0001 C CNN "Manufacturer"
F 5 "768290104" H 1800 2950 50  0000 R CNN "Manufacturer P/N"
	1    1550 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	8450 4650 8650 4650
Wire Wire Line
	8450 4950 8650 4950
$Comp
L Device:C C?
U 1 1 603046C0
P 8650 4800
AR Path="/601A64F8/603046C0" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/603046C0" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567144/603046C0" Ref="C?"  Part="1" 
AR Path="/6023BF6A/603046C0" Ref="C?"  Part="1" 
F 0 "C?" H 8750 4900 50  0000 L CNN
F 1 "10uF" H 8750 4800 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 8688 4650 50  0001 C CNN
F 3 "~" H 8650 4800 50  0001 C CNN
F 4 "25V" H 8750 4700 50  0000 L CNN "Voltage"
	1    8650 4800
	1    0    0    -1  
$EndComp
Connection ~ 8650 4650
Wire Wire Line
	8650 4650 9150 4650
Connection ~ 8650 4950
Wire Wire Line
	8650 4950 9700 4950
$EndSCHEMATC
