EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Indicator Board"
Date ""
Rev "A"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:R R?
U 1 1 62B2EB09
P 6600 3950
AR Path="/5FCD50A5/62B2EB09" Ref="R?"  Part="1" 
AR Path="/6052CC9F/62B2EB09" Ref="R?"  Part="1" 
AR Path="/62B2EB09" Ref="R2"  Part="1" 
F 0 "R2" H 6450 4000 50  0000 C CNN
F 1 "1k" H 6450 3900 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 6530 3950 50  0001 C CNN
F 3 "~" H 6600 3950 50  0001 C CNN
F 4 "Yageo" H 6600 3950 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 6600 3950 50  0001 C CNN "Manufacturer PN"
F 6 " " H 6600 3950 50  0001 C CNN "DNP"
	1    6600 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_ALT D?
U 1 1 62B2EB12
P 6600 4250
AR Path="/5FCD50A5/62B2EB12" Ref="D?"  Part="1" 
AR Path="/6052CC9F/62B2EB12" Ref="D?"  Part="1" 
AR Path="/62B2EB12" Ref="D2"  Part="1" 
F 0 "D2" V 6650 4400 50  0000 C CNN
F 1 "Green" V 6550 4450 50  0000 C CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 6600 4250 50  0001 C CNN
F 3 "~" H 6600 4250 50  0001 C CNN
F 4 "Lite-On" H 6600 4250 50  0001 C CNN "Manufacturer"
F 5 "LTST-C171KGKT" H 6600 4250 50  0001 C CNN "Manufacturer PN"
F 6 " " H 6600 4250 50  0001 C CNN "DNP"
	1    6600 4250
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62B2EB18
P 6600 4400
AR Path="/5FCD50A5/62B2EB18" Ref="#PWR?"  Part="1" 
AR Path="/6052CC9F/62B2EB18" Ref="#PWR?"  Part="1" 
AR Path="/62B2EB18" Ref="#PWR0101"  Part="1" 
F 0 "#PWR0101" H 6600 4150 50  0001 C CNN
F 1 "GND" H 6605 4227 50  0000 C CNN
F 2 "" H 6600 4400 50  0001 C CNN
F 3 "" H 6600 4400 50  0001 C CNN
	1    6600 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_ALT D?
U 1 1 62B50127
P 7200 4250
AR Path="/5FCD50A5/62B50127" Ref="D?"  Part="1" 
AR Path="/6052CC9F/62B50127" Ref="D?"  Part="1" 
AR Path="/62B50127" Ref="D3"  Part="1" 
F 0 "D3" V 7250 4450 50  0000 R CNN
F 1 "Yellow" V 7150 4550 50  0000 R CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 7200 4250 50  0001 C CNN
F 3 "~" H 7200 4250 50  0001 C CNN
F 4 "Lite-On" H 7200 4250 50  0001 C CNN "Manufacturer"
F 5 "LTST-C170KSKT" H 7200 4250 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7200 4250 50  0001 C CNN "DNP"
	1    7200 4250
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED_ALT D?
U 1 1 62B70B3F
P 7800 4250
AR Path="/5FCD4D85/62B70B3F" Ref="D?"  Part="1" 
AR Path="/62B70B3F" Ref="D4"  Part="1" 
F 0 "D4" V 7850 4450 50  0000 R CNN
F 1 "Red" V 7750 4500 50  0000 R CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 7800 4250 50  0001 C CNN
F 3 "~" H 7800 4250 50  0001 C CNN
F 4 "Lite-On" H 7800 4250 50  0001 C CNN "Manufacturer"
F 5 "LTST-C170KRKT" H 7800 4250 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7800 4250 50  0001 C CNN "DNP"
	1    7800 4250
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62B71235
P 7200 4400
AR Path="/5FCD50A5/62B71235" Ref="#PWR?"  Part="1" 
AR Path="/6052CC9F/62B71235" Ref="#PWR?"  Part="1" 
AR Path="/62B71235" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 7200 4150 50  0001 C CNN
F 1 "GND" H 7205 4227 50  0000 C CNN
F 2 "" H 7200 4400 50  0001 C CNN
F 3 "" H 7200 4400 50  0001 C CNN
	1    7200 4400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62B7143E
P 7800 4400
AR Path="/5FCD50A5/62B7143E" Ref="#PWR?"  Part="1" 
AR Path="/6052CC9F/62B7143E" Ref="#PWR?"  Part="1" 
AR Path="/62B7143E" Ref="#PWR0103"  Part="1" 
F 0 "#PWR0103" H 7800 4150 50  0001 C CNN
F 1 "GND" H 7805 4227 50  0000 C CNN
F 2 "" H 7800 4400 50  0001 C CNN
F 3 "" H 7800 4400 50  0001 C CNN
	1    7800 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62B71731
P 7200 3950
AR Path="/5FCD50A5/62B71731" Ref="R?"  Part="1" 
AR Path="/6052CC9F/62B71731" Ref="R?"  Part="1" 
AR Path="/62B71731" Ref="R3"  Part="1" 
F 0 "R3" H 7050 4000 50  0000 C CNN
F 1 "1k" H 7050 3900 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 7130 3950 50  0001 C CNN
F 3 "~" H 7200 3950 50  0001 C CNN
F 4 "Yageo" H 7200 3950 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 7200 3950 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7200 3950 50  0001 C CNN "DNP"
	1    7200 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62B71C4A
P 7800 3950
AR Path="/5FCD50A5/62B71C4A" Ref="R?"  Part="1" 
AR Path="/6052CC9F/62B71C4A" Ref="R?"  Part="1" 
AR Path="/62B71C4A" Ref="R4"  Part="1" 
F 0 "R4" H 7650 4000 50  0000 C CNN
F 1 "1k" H 7650 3900 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 7730 3950 50  0001 C CNN
F 3 "~" H 7800 3950 50  0001 C CNN
F 4 "Yageo" H 7800 3950 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 7800 3950 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7800 3950 50  0001 C CNN "DNP"
	1    7800 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62B759DB
P 6600 5200
AR Path="/5FCD50A5/62B759DB" Ref="R?"  Part="1" 
AR Path="/6052CC9F/62B759DB" Ref="R?"  Part="1" 
AR Path="/62B759DB" Ref="R5"  Part="1" 
F 0 "R5" H 6450 5250 50  0000 C CNN
F 1 "1k" H 6450 5150 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 6530 5200 50  0001 C CNN
F 3 "~" H 6600 5200 50  0001 C CNN
F 4 "Yageo" H 6600 5200 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 6600 5200 50  0001 C CNN "Manufacturer PN"
F 6 " " H 6600 5200 50  0001 C CNN "DNP"
	1    6600 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_ALT D?
U 1 1 62B759E4
P 6600 5500
AR Path="/5FCD50A5/62B759E4" Ref="D?"  Part="1" 
AR Path="/6052CC9F/62B759E4" Ref="D?"  Part="1" 
AR Path="/62B759E4" Ref="D5"  Part="1" 
F 0 "D5" V 6650 5650 50  0000 C CNN
F 1 "Green" V 6550 5700 50  0000 C CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 6600 5500 50  0001 C CNN
F 3 "~" H 6600 5500 50  0001 C CNN
F 4 "Lite-On" H 6600 5500 50  0001 C CNN "Manufacturer"
F 5 "LTST-C171KGKT" H 6600 5500 50  0001 C CNN "Manufacturer PN"
F 6 " " H 6600 5500 50  0001 C CNN "DNP"
	1    6600 5500
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62B759EA
P 6600 5650
AR Path="/5FCD50A5/62B759EA" Ref="#PWR?"  Part="1" 
AR Path="/6052CC9F/62B759EA" Ref="#PWR?"  Part="1" 
AR Path="/62B759EA" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 6600 5400 50  0001 C CNN
F 1 "GND" H 6605 5477 50  0000 C CNN
F 2 "" H 6600 5650 50  0001 C CNN
F 3 "" H 6600 5650 50  0001 C CNN
	1    6600 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_ALT D?
U 1 1 62B759F3
P 7200 5500
AR Path="/5FCD50A5/62B759F3" Ref="D?"  Part="1" 
AR Path="/6052CC9F/62B759F3" Ref="D?"  Part="1" 
AR Path="/62B759F3" Ref="D6"  Part="1" 
F 0 "D6" V 7250 5700 50  0000 R CNN
F 1 "Yellow" V 7150 5800 50  0000 R CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 7200 5500 50  0001 C CNN
F 3 "~" H 7200 5500 50  0001 C CNN
F 4 "Lite-On" H 7200 5500 50  0001 C CNN "Manufacturer"
F 5 "LTST-C170KSKT" H 7200 5500 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7200 5500 50  0001 C CNN "DNP"
	1    7200 5500
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED_ALT D?
U 1 1 62B759FC
P 7800 5500
AR Path="/5FCD4D85/62B759FC" Ref="D?"  Part="1" 
AR Path="/62B759FC" Ref="D7"  Part="1" 
F 0 "D7" V 7850 5700 50  0000 R CNN
F 1 "Red" V 7750 5750 50  0000 R CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 7800 5500 50  0001 C CNN
F 3 "~" H 7800 5500 50  0001 C CNN
F 4 "Lite-On" H 7800 5500 50  0001 C CNN "Manufacturer"
F 5 "LTST-C170KRKT" H 7800 5500 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7800 5500 50  0001 C CNN "DNP"
	1    7800 5500
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62B75A02
P 7200 5650
AR Path="/5FCD50A5/62B75A02" Ref="#PWR?"  Part="1" 
AR Path="/6052CC9F/62B75A02" Ref="#PWR?"  Part="1" 
AR Path="/62B75A02" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 7200 5400 50  0001 C CNN
F 1 "GND" H 7205 5477 50  0000 C CNN
F 2 "" H 7200 5650 50  0001 C CNN
F 3 "" H 7200 5650 50  0001 C CNN
	1    7200 5650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62B75A08
P 7800 5650
AR Path="/5FCD50A5/62B75A08" Ref="#PWR?"  Part="1" 
AR Path="/6052CC9F/62B75A08" Ref="#PWR?"  Part="1" 
AR Path="/62B75A08" Ref="#PWR0106"  Part="1" 
F 0 "#PWR0106" H 7800 5400 50  0001 C CNN
F 1 "GND" H 7805 5477 50  0000 C CNN
F 2 "" H 7800 5650 50  0001 C CNN
F 3 "" H 7800 5650 50  0001 C CNN
	1    7800 5650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62B75A11
P 7200 5200
AR Path="/5FCD50A5/62B75A11" Ref="R?"  Part="1" 
AR Path="/6052CC9F/62B75A11" Ref="R?"  Part="1" 
AR Path="/62B75A11" Ref="R6"  Part="1" 
F 0 "R6" H 7050 5250 50  0000 C CNN
F 1 "1k" H 7050 5150 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 7130 5200 50  0001 C CNN
F 3 "~" H 7200 5200 50  0001 C CNN
F 4 "Yageo" H 7200 5200 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 7200 5200 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7200 5200 50  0001 C CNN "DNP"
	1    7200 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62B75A1A
P 7800 5200
AR Path="/5FCD50A5/62B75A1A" Ref="R?"  Part="1" 
AR Path="/6052CC9F/62B75A1A" Ref="R?"  Part="1" 
AR Path="/62B75A1A" Ref="R7"  Part="1" 
F 0 "R7" H 7650 5250 50  0000 C CNN
F 1 "1k" H 7650 5150 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 7730 5200 50  0001 C CNN
F 3 "~" H 7800 5200 50  0001 C CNN
F 4 "Yageo" H 7800 5200 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 7800 5200 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7800 5200 50  0001 C CNN "DNP"
	1    7800 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62B75E81
P 7100 2800
AR Path="/5FCD50A5/62B75E81" Ref="R?"  Part="1" 
AR Path="/6052CC9F/62B75E81" Ref="R?"  Part="1" 
AR Path="/62B75E81" Ref="R1"  Part="1" 
F 0 "R1" H 6950 2850 50  0000 C CNN
F 1 "1k" H 6950 2750 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 7030 2800 50  0001 C CNN
F 3 "~" H 7100 2800 50  0001 C CNN
F 4 "Yageo" H 7100 2800 50  0001 C CNN "Manufacturer"
F 5 "RC0603FR-071KL" H 7100 2800 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7100 2800 50  0001 C CNN "DNP"
	1    7100 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:LED_ALT D?
U 1 1 62B75E8A
P 7100 3100
AR Path="/5FCD50A5/62B75E8A" Ref="D?"  Part="1" 
AR Path="/6052CC9F/62B75E8A" Ref="D?"  Part="1" 
AR Path="/62B75E8A" Ref="D1"  Part="1" 
F 0 "D1" V 7150 3250 50  0000 C CNN
F 1 "Green" V 7050 3300 50  0000 C CNN
F 2 "RespiraWorks_Std:LED_0805_2012Metric" H 7100 3100 50  0001 C CNN
F 3 "~" H 7100 3100 50  0001 C CNN
F 4 "Lite-On" H 7100 3100 50  0001 C CNN "Manufacturer"
F 5 "LTST-C171KGKT" H 7100 3100 50  0001 C CNN "Manufacturer PN"
F 6 " " H 7100 3100 50  0001 C CNN "DNP"
	1    7100 3100
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62B75E90
P 7100 3250
AR Path="/5FCD50A5/62B75E90" Ref="#PWR?"  Part="1" 
AR Path="/6052CC9F/62B75E90" Ref="#PWR?"  Part="1" 
AR Path="/62B75E90" Ref="#PWR0107"  Part="1" 
F 0 "#PWR0107" H 7100 3000 50  0001 C CNN
F 1 "GND" H 7105 3077 50  0000 C CNN
F 2 "" H 7100 3250 50  0001 C CNN
F 3 "" H 7100 3250 50  0001 C CNN
	1    7100 3250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 62B9196F
P 5450 2500
AR Path="/5FCD4E33/62B9196F" Ref="#PWR?"  Part="1" 
AR Path="/62B9196F" Ref="#PWR0108"  Part="1" 
F 0 "#PWR0108" H 5450 2350 50  0001 C CNN
F 1 "+5V" H 5465 2673 50  0000 C CNN
F 2 "" H 5450 2500 50  0001 C CNN
F 3 "" H 5450 2500 50  0001 C CNN
	1    5450 2500
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0109
U 1 1 62B91975
P 5300 3350
F 0 "#PWR0109" H 5300 3100 50  0001 C CNN
F 1 "GND" H 5305 3177 50  0000 C CNN
F 2 "" H 5300 3350 50  0001 C CNN
F 3 "" H 5300 3350 50  0001 C CNN
	1    5300 3350
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J?
U 1 1 62B9197E
P 3850 2750
AR Path="/5FCD4E33/62B9197E" Ref="J?"  Part="1" 
AR Path="/62B9197E" Ref="J1"  Part="1" 
F 0 "J1" H 3929 2742 50  0000 L CNN
F 1 "Cable to from main board" H 3929 2651 50  0000 L CNN
F 2 "RespiraWorks_Std:Molex_Micro-Fit_3.0_43650-0815_1x08_P3.00mm_Vertical" H 3850 2750 50  0001 C CNN
F 3 "~" H 3850 2750 50  0001 C CNN
F 4 "Molex" H 3850 2750 50  0001 C CNN "Manufacturer"
F 5 "43650-0815" H 3850 2750 50  0001 C CNN "Manufacturer PN"
F 6 " " H 3850 2750 50  0001 C CNN "DNP"
	1    3850 2750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4050 2450 5300 2450
Wire Wire Line
	5300 2450 5300 3350
Wire Wire Line
	5450 2500 5450 2550
Wire Wire Line
	4050 2550 5450 2550
Text Label 4100 2650 0    50   ~ 0
CC-LED-G
Text Label 4100 2750 0    50   ~ 0
CC-LED-R
Text Label 4100 2850 0    50   ~ 0
CC-LED-Y
Text Label 4100 2950 0    50   ~ 0
UI-LED-G
Text Label 4100 3050 0    50   ~ 0
UI-LED-R
Text Label 4100 3150 0    50   ~ 0
UI-LED-Y
Wire Wire Line
	7100 2550 7100 2650
Connection ~ 5450 2550
Wire Wire Line
	6100 2650 6100 3800
Wire Wire Line
	6100 3800 6600 3800
Wire Wire Line
	4050 2650 6100 2650
Wire Wire Line
	7200 3800 7200 3700
Wire Wire Line
	7200 3700 6200 3700
Wire Wire Line
	6200 3700 6200 2850
Wire Wire Line
	4050 2850 6200 2850
Wire Wire Line
	6300 2750 6300 3600
Wire Wire Line
	6300 3600 7800 3600
Wire Wire Line
	7800 3600 7800 3800
Wire Wire Line
	4050 2750 6300 2750
Wire Wire Line
	5450 2550 7100 2550
Wire Wire Line
	5550 2950 5550 5050
Wire Wire Line
	5550 5050 6600 5050
Wire Wire Line
	4050 2950 5550 2950
Wire Wire Line
	5650 3050 5650 4950
Wire Wire Line
	5650 4950 7800 4950
Wire Wire Line
	7800 4950 7800 5050
Wire Wire Line
	4050 3050 5650 3050
Wire Wire Line
	7200 5050 7200 4850
Wire Wire Line
	7200 4850 5750 4850
Wire Wire Line
	5750 4850 5750 3150
Wire Wire Line
	4050 3150 5750 3150
$Comp
L Mechanical:MountingHole H1
U 1 1 62BC081C
P 2950 5400
F 0 "H1" V 2904 5550 50  0000 L CNN
F 1 "M2.5" V 2995 5550 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm" H 2950 5400 50  0001 C CNN
F 3 "~" H 2950 5400 50  0001 C CNN
F 4 " " H 2950 5400 50  0001 C CNN "DNP"
	1    2950 5400
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 62BC2B8F
P 2950 5650
F 0 "H3" V 2904 5800 50  0000 L CNN
F 1 "M2.5" V 2995 5800 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm" H 2950 5650 50  0001 C CNN
F 3 "~" H 2950 5650 50  0001 C CNN
F 4 " " H 2950 5650 50  0001 C CNN "DNP"
	1    2950 5650
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 62BC2DBA
P 3450 5400
F 0 "H2" V 3404 5550 50  0000 L CNN
F 1 "M2.5" V 3495 5550 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm" H 3450 5400 50  0001 C CNN
F 3 "~" H 3450 5400 50  0001 C CNN
F 4 " " H 3450 5400 50  0001 C CNN "DNP"
	1    3450 5400
	0    1    1    0   
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 62BC30AB
P 3450 5650
F 0 "H4" V 3404 5800 50  0000 L CNN
F 1 "M2.5" V 3495 5800 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.7mm" H 3450 5650 50  0001 C CNN
F 3 "~" H 3450 5650 50  0001 C CNN
F 4 " " H 3450 5650 50  0001 C CNN "DNP"
	1    3450 5650
	0    1    1    0   
$EndComp
Text Notes 8200 4250 0    50   ~ 0
Cycle Controller LEDs
Text Notes 8250 5550 0    50   ~ 0
UI Computer LEDs
Text Notes 7550 3100 0    50   ~ 0
Power LED
$EndSCHEMATC
