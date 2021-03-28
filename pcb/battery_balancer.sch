EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 32 38
Title "Battery Balancer"
Date ""
Rev "A"
Comp "RespiraWorks"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L RespiraWorks:MAX17320G20+ U24
U 1 1 603D10E7
P 5600 3800
F 0 "U24" H 5575 2077 50  0000 C CNN
F 1 "MAX17320G20+" H 5575 1986 50  0000 C CNN
F 2 "RespiraWorks_Std:WQFN-24-1EP_4x4mm_P0.5mm_EP2.6x2.6mm" H 5500 1950 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX17320.pdf" H 8300 4850 50  0001 C CNN
F 4 "Maxim IC" H 5600 3800 50  0000 C CNN "Manufacturer"
F 5 "MAX17320G22+" H 5600 3700 50  0000 C CNN "Manufacturer PN"
F 6 "Alt. MAX17320G20+" H 5600 3800 50  0001 C CNN "Sourcing Notes"
	1    5600 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 603D71EF
P 6750 5200
AR Path="/601A64F8/603D71EF" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603D71EF" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603D71EF" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603D71EF" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603D71EF" Ref="R194"  Part="1" 
F 0 "R194" H 6900 5250 50  0000 C CNN
F 1 "0.005" H 6950 5150 50  0000 C CNN
F 2 "RespiraWorks_Std:R_2512_6332Metric" V 6680 5200 50  0001 C CNN
F 3 "~" H 6750 5200 50  0001 C CNN
F 4 "Rohm" H 6750 5200 50  0001 C CNN "Manufacturer"
F 5 "PMR100HZPFU5L00" H 6750 5200 50  0001 C CNN "Manufacturer PN"
	1    6750 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 5350 6750 5350
$Comp
L Device:R R?
U 1 1 603D7932
P 7150 4750
AR Path="/601A64F8/603D7932" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603D7932" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603D7932" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603D7932" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603D7932" Ref="R199"  Part="1" 
F 0 "R199" V 7350 4750 50  0000 C CNN
F 1 "49.9" V 7250 4750 50  0000 C CNN
F 2 "RespiraWorks_Std:R_1206_3216Metric" V 7080 4750 50  0001 C CNN
F 3 "~" H 7150 4750 50  0001 C CNN
	1    7150 4750
	0    -1   -1   0   
$EndComp
$Comp
L Device:C C?
U 1 1 603D9107
P 6600 4600
AR Path="/601A64F8/603D9107" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/603D9107" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/603D9107" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/603D9107" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/603D9107" Ref="C137"  Part="1" 
F 0 "C137" H 6750 4650 50  0000 L CNN
F 1 "10nF" H 6750 4550 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6638 4450 50  0001 C CNN
F 3 "~" H 6600 4600 50  0001 C CNN
F 4 "NP0" H 6600 4600 50  0001 C CNN "Dielectric"
F 5 "Murata" H 6600 4600 50  0001 C CNN "Manufacturer"
F 6 "GRM1885C1H103JA01D" H 6600 4600 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 6600 4600 50  0001 C CNN "Voltage"
	1    6600 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 603D9CD3
P 6500 4300
AR Path="/601A64F8/603D9CD3" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/603D9CD3" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/603D9CD3" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/603D9CD3" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/603D9CD3" Ref="C134"  Part="1" 
F 0 "C134" H 6650 4350 50  0000 L CNN
F 1 "10nF" H 6650 4250 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6538 4150 50  0001 C CNN
F 3 "~" H 6500 4300 50  0001 C CNN
F 4 "NP0" H 6500 4300 50  0001 C CNN "Dielectric"
F 5 "Murata" H 6500 4300 50  0001 C CNN "Manufacturer"
F 6 "GRM1885C1H103JA01D" H 6500 4300 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 6500 4300 50  0001 C CNN "Voltage"
	1    6500 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 603D9EC2
P 6600 4000
AR Path="/601A64F8/603D9EC2" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/603D9EC2" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/603D9EC2" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/603D9EC2" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/603D9EC2" Ref="C136"  Part="1" 
F 0 "C136" H 6750 4050 50  0000 L CNN
F 1 "10nF" H 6750 3950 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6638 3850 50  0001 C CNN
F 3 "~" H 6600 4000 50  0001 C CNN
F 4 "NP0" H 6600 4000 50  0001 C CNN "Dielectric"
F 5 "Murata" H 6600 4000 50  0001 C CNN "Manufacturer"
F 6 "GRM1885C1H103JA01D" H 6600 4000 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 6600 4000 50  0001 C CNN "Voltage"
	1    6600 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 603DA817
P 7150 4450
AR Path="/601A64F8/603DA817" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603DA817" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603DA817" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603DA817" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603DA817" Ref="R198"  Part="1" 
F 0 "R198" V 7350 4450 50  0000 C CNN
F 1 "49.9" V 7250 4450 50  0000 C CNN
F 2 "RespiraWorks_Std:R_1206_3216Metric" V 7080 4450 50  0001 C CNN
F 3 "~" H 7150 4450 50  0001 C CNN
	1    7150 4450
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 603DAA1D
P 7150 3850
AR Path="/601A64F8/603DAA1D" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603DAA1D" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603DAA1D" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603DAA1D" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603DAA1D" Ref="R196"  Part="1" 
F 0 "R196" V 7350 3850 50  0000 C CNN
F 1 "49.9" V 7250 3850 50  0000 C CNN
F 2 "RespiraWorks_Std:R_1206_3216Metric" V 7080 3850 50  0001 C CNN
F 3 "~" H 7150 3850 50  0001 C CNN
	1    7150 3850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6300 4150 6500 4150
Connection ~ 6500 4150
Wire Wire Line
	6500 4150 6600 4150
Wire Wire Line
	6300 4450 6500 4450
Connection ~ 6500 4450
Wire Wire Line
	6500 4450 6600 4450
Connection ~ 6600 3850
Wire Wire Line
	6600 3850 7000 3850
Connection ~ 6600 4150
Connection ~ 6600 4450
Wire Wire Line
	6600 4450 7000 4450
Wire Wire Line
	6600 4150 7000 4150
$Comp
L Device:R R?
U 1 1 603DA8CE
P 7150 4150
AR Path="/601A64F8/603DA8CE" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603DA8CE" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603DA8CE" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603DA8CE" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603DA8CE" Ref="R197"  Part="1" 
F 0 "R197" V 7350 4150 50  0000 C CNN
F 1 "49.9" V 7250 4150 50  0000 C CNN
F 2 "RespiraWorks_Std:R_1206_3216Metric" V 7080 4150 50  0001 C CNN
F 3 "~" H 7150 4150 50  0001 C CNN
	1    7150 4150
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 603DAD46
P 9950 3400
AR Path="/601A64F8/603DAD46" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603DAD46" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603DAD46" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603DAD46" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603DAD46" Ref="R201"  Part="1" 
F 0 "R201" H 10100 3450 50  0000 C CNN
F 1 "10" H 10100 3350 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 9880 3400 50  0001 C CNN
F 3 "~" H 9950 3400 50  0001 C CNN
	1    9950 3400
	1    0    0    -1  
$EndComp
Connection ~ 6600 4750
Wire Wire Line
	7300 4750 8400 4750
Wire Wire Line
	7300 4450 8300 4450
Wire Wire Line
	8300 4450 8300 4250
Wire Wire Line
	7300 3850 8300 3850
Wire Wire Line
	6600 4750 6500 4750
Wire Wire Line
	6750 6100 1600 6100
Connection ~ 6750 5350
Wire Wire Line
	6600 4750 7000 4750
Connection ~ 6750 5050
$Comp
L Device:C C?
U 1 1 603E4953
P 6250 1850
AR Path="/601A64F8/603E4953" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/603E4953" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/603E4953" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/603E4953" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/603E4953" Ref="C132"  Part="1" 
F 0 "C132" V 6200 2100 50  0000 L CNN
F 1 "100nF" V 6300 2100 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6288 1700 50  0001 C CNN
F 3 "~" H 6250 1850 50  0001 C CNN
F 4 "X7R" H 6250 1850 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 6250 1850 50  0001 C CNN "Manufacturer"
F 6 "C0603C104K5RACTU" H 6250 1850 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 6250 1850 50  0001 C CNN "Voltage"
	1    6250 1850
	0    1    1    0   
$EndComp
$Comp
L RespiraWorks_Std:Q_NMOS_SGD Q?
U 1 1 603E494C
P 5300 1650
AR Path="/601A64F8/603E494C" Ref="Q?"  Part="1" 
AR Path="/601A64F8/60567144/603E494C" Ref="Q?"  Part="1" 
AR Path="/601A64F8/60567757/603E494C" Ref="Q?"  Part="1" 
AR Path="/6023BF6A/60567757/603E494C" Ref="Q?"  Part="1" 
AR Path="/6041384A/60567757/603E494C" Ref="Q21"  Part="1" 
F 0 "Q21" V 5850 1650 50  0000 C CNN
F 1 "BSZ019N03LSATMA1" V 5750 1650 50  0000 C CNN
F 2 "RespiraWorks:VSON-8-SGD_3.3x3.3mm_P0.65mm_NexFET" H 5500 1575 50  0001 L CIN
F 3 "https://www.infineon.com/dgdl/BSZ019N03LS_Rev+2.1.pdf?folderId=db3a304326dfb1300126fb3bec803f1a&fileId=db3a304326dfb1300126fb7ddf173f4e" V 5300 1650 50  0001 L CNN
F 4 "Infineon" H 5300 1650 50  0001 C CNN "Manufacturer"
F 5 "BSZ019N03LSATMA1" H 5300 1650 50  0001 C CNN "Manufacturer PN"
	1    5300 1650
	0    1    -1   0   
$EndComp
$Comp
L Device:C C?
U 1 1 603EA3B2
P 9950 3700
AR Path="/601A64F8/603EA3B2" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/603EA3B2" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/603EA3B2" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/603EA3B2" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/603EA3B2" Ref="C138"  Part="1" 
F 0 "C138" H 10100 3750 50  0000 L CNN
F 1 "100nF" H 10100 3650 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 9988 3550 50  0001 C CNN
F 3 "~" H 9950 3700 50  0001 C CNN
F 4 "X7R" H 9950 3700 50  0001 C CNN "Dielectric"
F 5 "Kemet" H 9950 3700 50  0001 C CNN "Manufacturer"
F 6 "C0603C104K5RACTU" H 9950 3700 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 9950 3700 50  0001 C CNN "Voltage"
	1    9950 3700
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 603EACF1
P 6500 4900
AR Path="/601A64F8/603EACF1" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/603EACF1" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/603EACF1" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/603EACF1" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/603EACF1" Ref="C135"  Part="1" 
F 0 "C135" H 6650 4950 50  0000 L CNN
F 1 "10nF" H 6650 4850 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 6538 4750 50  0001 C CNN
F 3 "~" H 6500 4900 50  0001 C CNN
F 4 "NP0" H 6500 4900 50  0001 C CNN "Dielectric"
F 5 "Murata" H 6500 4900 50  0001 C CNN "Manufacturer"
F 6 "GRM1885C1H103JA01D" H 6500 4900 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 6500 4900 50  0001 C CNN "Voltage"
	1    6500 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 4750 8400 4350
Wire Wire Line
	8400 4350 8500 4350
Wire Wire Line
	8300 4250 8500 4250
Wire Wire Line
	7300 4150 8500 4150
Wire Wire Line
	6300 3850 6600 3850
Connection ~ 9950 3550
Wire Wire Line
	6300 3550 6500 3550
$Comp
L Device:C C?
U 1 1 603F4DD5
P 6500 3400
AR Path="/601A64F8/603F4DD5" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/603F4DD5" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/603F4DD5" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/603F4DD5" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/603F4DD5" Ref="C133"  Part="1" 
F 0 "C133" H 6650 3450 50  0000 L CNN
F 1 "470nF" H 6650 3350 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 6538 3250 50  0001 C CNN
F 3 "~" H 6500 3400 50  0001 C CNN
F 4 "X7R" H 6500 3400 50  0001 C CNN "Dielectric"
F 5 "50V" H 6500 3400 50  0001 C CNN "Voltage"
F 6 "Kemet" H 6500 3400 50  0001 C CNN "Manufacturer"
F 7 "C0805C474K5RACTU" H 6500 3400 50  0001 C CNN "Manufacturer PN"
	1    6500 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 3250 6500 3250
$Comp
L Device:R R?
U 1 1 603F615C
P 7050 3000
AR Path="/601A64F8/603F615C" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603F615C" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603F615C" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603F615C" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603F615C" Ref="R195"  Part="1" 
F 0 "R195" H 7200 3050 50  0000 C CNN
F 1 "1k" H 7200 2950 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 6980 3000 50  0001 C CNN
F 3 "~" H 7050 3000 50  0001 C CNN
	1    7050 3000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 603F83DA
P 3000 2100
AR Path="/601A64F8/603F83DA" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603F83DA" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603F83DA" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603F83DA" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603F83DA" Ref="R189"  Part="1" 
F 0 "R189" H 3150 2150 50  0000 C CNN
F 1 "1k" H 3100 2050 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 2930 2100 50  0001 C CNN
F 3 "~" H 3000 2100 50  0001 C CNN
	1    3000 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 2850 7050 2850
$Comp
L Device:R R?
U 1 1 603FE142
P 3400 2100
AR Path="/601A64F8/603FE142" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/603FE142" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/603FE142" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/603FE142" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/603FE142" Ref="R190"  Part="1" 
F 0 "R190" H 3250 2050 50  0000 C CNN
F 1 "2k" H 3300 2150 50  0000 C CNN
F 2 "RespiraWorks_Std:R_1206_3216Metric" V 3330 2100 50  0001 C CNN
F 3 "~" H 3400 2100 50  0001 C CNN
	1    3400 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	6000 1550 6400 1550
Wire Wire Line
	6400 1550 6400 1850
Connection ~ 6400 1550
Wire Wire Line
	5800 2150 5800 2200
Wire Wire Line
	5300 2150 5300 2200
Wire Wire Line
	5500 1550 5600 1550
$Comp
L Device:C C?
U 1 1 60411B41
P 3250 5100
AR Path="/601A64F8/60411B41" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/60411B41" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/60411B41" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/60411B41" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/60411B41" Ref="C129"  Part="1" 
F 0 "C129" H 3400 5150 50  0000 L CNN
F 1 "470nF" H 3400 5050 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 3288 4950 50  0001 C CNN
F 3 "~" H 3250 5100 50  0001 C CNN
F 4 "X7R" H 3250 5100 50  0001 C CNN "Dielectric"
F 5 "50V" H 3250 5100 50  0001 C CNN "Voltage"
F 6 "Kemet" H 3250 5100 50  0001 C CNN "Manufacturer"
F 7 "C0805C474K5RACTU" H 3250 5100 50  0001 C CNN "Manufacturer PN"
	1    3250 5100
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 604128F9
P 4000 3900
AR Path="/601A64F8/604128F9" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/604128F9" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/604128F9" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/604128F9" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/604128F9" Ref="C131"  Part="1" 
F 0 "C131" H 4150 3950 50  0000 L CNN
F 1 "470nF" H 4150 3850 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 4038 3750 50  0001 C CNN
F 3 "~" H 4000 3900 50  0001 C CNN
F 4 "X7R" H 4000 3900 50  0001 C CNN "Dielectric"
F 5 "50V" H 4000 3900 50  0001 C CNN "Voltage"
F 6 "Kemet" H 4000 3900 50  0001 C CNN "Manufacturer"
F 7 "C0805C474K5RACTU" H 4000 3900 50  0001 C CNN "Manufacturer PN"
	1    4000 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 60413618
P 3800 5100
AR Path="/601A64F8/60413618" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/60413618" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/60413618" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/60413618" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/60413618" Ref="C130"  Part="1" 
F 0 "C130" H 3950 5150 50  0000 L CNN
F 1 "470nF" H 3950 5050 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0805_2012Metric" H 3838 4950 50  0001 C CNN
F 3 "~" H 3800 5100 50  0001 C CNN
F 4 "X7R" H 3800 5100 50  0001 C CNN "Dielectric"
F 5 "50V" H 3800 5100 50  0001 C CNN "Voltage"
F 6 "Kemet" H 3800 5100 50  0001 C CNN "Manufacturer"
F 7 "C0805C474K5RACTU" H 3800 5100 50  0001 C CNN "Manufacturer PN"
	1    3800 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 1550 9950 3250
$Comp
L Device:R R?
U 1 1 6041FF36
P 5800 2000
AR Path="/601A64F8/6041FF36" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/6041FF36" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/6041FF36" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/6041FF36" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/6041FF36" Ref="R193"  Part="1" 
F 0 "R193" H 5950 2050 50  0000 C CNN
F 1 "100" H 5950 1950 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 5730 2000 50  0001 C CNN
F 3 "~" H 5800 2000 50  0001 C CNN
	1    5800 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 60420C98
P 5300 2000
AR Path="/601A64F8/60420C98" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/60420C98" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/60420C98" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/60420C98" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/60420C98" Ref="R192"  Part="1" 
F 0 "R192" H 5450 2050 50  0000 C CNN
F 1 "100" H 5450 1950 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 5230 2000 50  0001 C CNN
F 3 "~" H 5300 2000 50  0001 C CNN
	1    5300 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 1950 3400 1550
Wire Wire Line
	3400 2250 3400 2500
Wire Wire Line
	3000 2250 3000 2300
Wire Wire Line
	3000 1550 3000 1950
Wire Wire Line
	4850 4950 3800 4950
Wire Wire Line
	4850 4750 3250 4750
Wire Wire Line
	3250 4750 3250 4950
Wire Wire Line
	4850 5150 4650 5150
Wire Wire Line
	4650 5150 4650 5350
Wire Wire Line
	4850 5350 4650 5350
Connection ~ 4650 5350
Wire Wire Line
	4650 5350 4650 5550
Wire Wire Line
	3800 5250 3800 5550
Wire Wire Line
	3250 5250 3250 5550
Wire Wire Line
	4850 3750 4000 3750
Text Label 4050 5550 0    50   ~ 0
Bat-N
Connection ~ 3250 5550
Wire Wire Line
	3250 5550 3800 5550
Connection ~ 3800 5550
Wire Wire Line
	3800 5550 4650 5550
$Comp
L Device:Thermistor_NTC TH1
U 1 1 60449BEC
P 2850 4850
F 0 "TH1" H 2947 4896 50  0000 L CNN
F 1 "10k" H 2947 4805 50  0000 L CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" H 2850 4900 50  0001 C CNN
F 3 "~" H 2850 4900 50  0001 C CNN
	1    2850 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 5000 2850 5550
Wire Wire Line
	2850 5550 3250 5550
Wire Wire Line
	2850 4700 2850 4550
Wire Wire Line
	2850 4550 4850 4550
NoConn ~ 4850 3950
NoConn ~ 4850 4150
NoConn ~ 4850 4350
Wire Wire Line
	9950 3850 9950 5050
Wire Wire Line
	7050 3150 7050 3350
Wire Wire Line
	7050 3350 7400 3350
$Comp
L Device:C C?
U 1 1 60421F86
P 2500 2450
AR Path="/601A64F8/60421F86" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567144/60421F86" Ref="C?"  Part="1" 
AR Path="/601A64F8/60567757/60421F86" Ref="C?"  Part="1" 
AR Path="/6023BF6A/60567757/60421F86" Ref="C?"  Part="1" 
AR Path="/6041384A/60567757/60421F86" Ref="C128"  Part="1" 
F 0 "C128" H 2650 2500 50  0000 L CNN
F 1 "10nF" H 2650 2400 50  0000 L CNN
F 2 "RespiraWorks_Std:C_0603_1608Metric" H 2538 2300 50  0001 C CNN
F 3 "~" H 2500 2450 50  0001 C CNN
F 4 "NP0" H 2500 2450 50  0001 C CNN "Dielectric"
F 5 "Murata" H 2500 2450 50  0001 C CNN "Manufacturer"
F 6 "GRM1885C1H103JA01D" H 2500 2450 50  0001 C CNN "Manufacturer PN"
F 7 "50V" H 2500 2450 50  0001 C CNN "Voltage"
	1    2500 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 2300 3000 2300
Connection ~ 3000 2300
Wire Wire Line
	3000 2300 3000 2700
Wire Wire Line
	4850 3150 4300 3150
Wire Wire Line
	4850 3350 4300 3350
Wire Wire Line
	4850 3550 4300 3550
Text HLabel 4300 3150 0    50   Input ~ 0
SCL
Text HLabel 4300 3350 0    50   BiDi ~ 0
SDA
Text HLabel 4300 3550 0    50   Output ~ 0
Alrt
Text HLabel 1600 1550 0    50   Output ~ 0
Sys-P
Text HLabel 1600 6100 0    50   Output ~ 0
Sys-N
Wire Wire Line
	8300 3850 8300 4050
Wire Wire Line
	8300 4050 8500 4050
Text HLabel 10050 5050 2    50   Input ~ 0
Bat-N
Text HLabel 10050 1550 2    50   Input ~ 0
Bat-P
Wire Wire Line
	6100 1850 5800 1850
Connection ~ 5800 1850
Text Notes 7400 3850 0    50   ~ 0
IC max balancing current: 100mA\nSet balancing current: 71mA\n= 4.2V cell voltage / (49.9Ω + 9Ω internal)
Connection ~ 6500 4750
Connection ~ 6500 5050
Wire Wire Line
	6500 5050 6750 5050
Connection ~ 9950 1550
Wire Wire Line
	9950 1550 10050 1550
Connection ~ 9950 5050
Wire Wire Line
	9950 5050 10050 5050
Wire Wire Line
	6300 5050 6500 5050
Wire Wire Line
	6400 1550 9950 1550
Wire Wire Line
	6300 4750 6500 4750
Wire Wire Line
	6500 3550 9950 3550
Connection ~ 6500 3550
$Comp
L Connector_Generic:Conn_01x05 J46
U 1 1 603D30AF
P 8700 4250
F 0 "J46" H 8900 4100 50  0000 R CNN
F 1 "BAT-BALANCE JST XH 5-pin" H 9900 4200 50  0000 R CNN
F 2 "RespiraWorks_Std:JST_XH_B5B-XH-A_1x05_P2.50mm_Vertical" H 8700 4250 50  0001 C CNN
F 3 "https://www.jst-mfg.com/product/pdf/eng/eXH.pdf" H 8700 4250 50  0001 C CNN
F 4 "JST" H 8700 4250 50  0001 C CNN "Manufacturer"
F 5 "B5B-XH-A(LF)(SN)" H 9550 4300 50  0000 R CNN "Manufacturer PN"
	1    8700 4250
	1    0    0    1   
$EndComp
Text Notes 8800 4450 0    50   ~ 0
Battery Balancing Connector
NoConn ~ 8500 4450
$Comp
L Device:R R?
U 1 1 6029693C
P 3550 3900
AR Path="/601A64F8/6029693C" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567144/6029693C" Ref="R?"  Part="1" 
AR Path="/601A64F8/60567757/6029693C" Ref="R?"  Part="1" 
AR Path="/6023BF6A/60567757/6029693C" Ref="R?"  Part="1" 
AR Path="/6041384A/60567757/6029693C" Ref="R191"  Part="1" 
F 0 "R191" H 3700 3950 50  0000 C CNN
F 1 "10k" H 3700 3850 50  0000 C CNN
F 2 "RespiraWorks_Std:R_0603_1608Metric" V 3480 3900 50  0001 C CNN
F 3 "~" H 3550 3900 50  0001 C CNN
	1    3550 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 3750 3550 3750
Connection ~ 4000 3750
Text Label 7400 3350 2    50   ~ 0
Bat-N
Wire Wire Line
	4000 4050 4000 4150
Wire Wire Line
	4000 4150 3550 4150
Wire Wire Line
	3550 4050 3550 4150
Connection ~ 3550 4150
Wire Wire Line
	3550 4150 2850 4150
Text Label 2850 4150 0    50   ~ 0
Bat-N
Wire Wire Line
	2500 2600 2500 2700
Wire Wire Line
	2500 2700 2100 2700
Text Label 2100 2700 0    50   ~ 0
Bat-N
Text Notes 3700 2150 0    50   ~ 0
Max. recovery charge current =\nVin/(2K+10) = 8.5 mA @ 17 Vin
Connection ~ 3000 1550
Wire Wire Line
	3000 1550 1600 1550
Connection ~ 3400 1550
Wire Wire Line
	3400 1550 3000 1550
Wire Wire Line
	3400 1550 5100 1550
Wire Wire Line
	3400 2500 4850 2500
Wire Wire Line
	3000 2700 4850 2700
Text Notes 6850 5900 0    50   ~ 0
5mΩ:\n5-155 mV short-circuit threshold = 1-31 A\n2.5-77.5 mV medium over-discharge threshold = 0.5-15.5 A\n0-51.2 mV slow over-discharge threshold = 0-10.24 A\n0-38.75 mV charge threshold = 0-7.75 A
Text Notes 4650 5850 0    50   ~ 0
I2C address: 0b01100110 and 0b0001110
$Comp
L RespiraWorks_Std:Q_NMOS_SGD Q?
U 1 1 603E4945
P 5800 1650
AR Path="/601A64F8/603E4945" Ref="Q?"  Part="1" 
AR Path="/601A64F8/60567144/603E4945" Ref="Q?"  Part="1" 
AR Path="/601A64F8/60567757/603E4945" Ref="Q?"  Part="1" 
AR Path="/6023BF6A/60567757/603E4945" Ref="Q?"  Part="1" 
AR Path="/6041384A/60567757/603E4945" Ref="Q22"  Part="1" 
F 0 "Q22" V 6200 1650 50  0000 C CNN
F 1 "BSZ019N03LSATMA1" V 6100 1550 50  0000 C CNN
F 2 "RespiraWorks:VSON-8-SGD_3.3x3.3mm_P0.65mm_NexFET" H 6000 1575 50  0001 L CIN
F 3 "https://www.infineon.com/dgdl/BSZ019N03LS_Rev+2.1.pdf?folderId=db3a304326dfb1300126fb3bec803f1a&fileId=db3a304326dfb1300126fb7ddf173f4e" V 5800 1650 50  0001 L CNN
F 4 "Infineon" H 5800 1650 50  0001 C CNN "Manufacturer"
F 5 "BSZ019N03LSATMA1" H 5800 1650 50  0001 C CNN "Manufacturer PN"
	1    5800 1650
	0    -1   -1   0   
$EndComp
$Comp
L RespiraWorks_Std:Heatsink_Pad HS?
U 1 1 6064C468
P 6550 900
AR Path="/6041384A/60567144/6064C468" Ref="HS?"  Part="1" 
AR Path="/6041384A/60567757/6064C468" Ref="HS5"  Part="1" 
F 0 "HS5" H 6500 1100 50  0000 L CNN
F 1 "Heatsink_Pad" H 6691 848 50  0001 L CNN
F 2 "RespiraWorks:Heatsink_Aavid_573100D00010G_HandSolder" H 6562 850 50  0001 C CNN
F 3 "~" H 6562 850 50  0001 C CNN
F 4 "Aavid Thermalloy" H 7000 1050 50  0000 C CNN "Manufacturer"
F 5 "573100D00010G" H 7000 950 50  0000 C CNN "Part Number"
	1    6550 900 
	1    0    0    -1  
$EndComp
Text Notes 6450 600  0    50   ~ 0
Optional heatsinks for Q21, Q22
Wire Wire Line
	6550 1000 6550 1050
Wire Wire Line
	6550 1050 6200 1050
Text Label 5500 1550 0    50   ~ 0
VMid
Text Label 6200 1050 0    50   ~ 0
VMid
$Comp
L RespiraWorks_Std:Heatsink_Pad HS?
U 1 1 606522B6
P 7550 900
AR Path="/6041384A/60567144/606522B6" Ref="HS?"  Part="1" 
AR Path="/6041384A/60567757/606522B6" Ref="HS6"  Part="1" 
F 0 "HS6" H 7500 1100 50  0000 L CNN
F 1 "Heatsink_Pad" H 7691 848 50  0001 L CNN
F 2 "RespiraWorks:Heatsink_Aavid_573100D00010G_HandSolder" H 7562 850 50  0001 C CNN
F 3 "~" H 7562 850 50  0001 C CNN
F 4 "Aavid Thermalloy" H 8000 1050 50  0000 C CNN "Manufacturer"
F 5 "573100D00010G" H 8000 950 50  0000 C CNN "Part Number"
	1    7550 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 1000 7550 1050
Wire Wire Line
	7550 1050 7200 1050
Text Label 7200 1050 0    50   ~ 0
VMid
Wire Wire Line
	6750 5050 9950 5050
Wire Wire Line
	6750 5350 6750 6100
$EndSCHEMATC
