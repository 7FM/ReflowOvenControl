EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ReflowOvenPCB"
Date "2020-05-08"
Rev "v0.1"
Comp "Author: 7FM"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 4750 2725 0    60   ~ 0
Arduino Nano
$Comp
L Connector_Generic:Conn_01x15 P1
U 1 1 56D73FAC
P 5800 3950
F 0 "P1" H 5800 4750 50  0000 C CNN
F 1 "Digital" V 5900 3950 50  0000 C CNN
F 2 "Socket_Arduino_Nano:Socket_Strip_Arduino_1x15" H 5800 3950 50  0001 C CNN
F 3 "" H 5800 3950 50  0000 C CNN
	1    5800 3950
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x15 P2
U 1 1 56D740C7
P 6200 3950
F 0 "P2" H 6200 4750 50  0000 C CNN
F 1 "Analog" V 6300 3950 50  0000 C CNN
F 2 "Socket_Arduino_Nano:Socket_Strip_Arduino_1x15" H 6200 3950 50  0001 C CNN
F 3 "" H 6200 3950 50  0000 C CNN
	1    6200 3950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5600 3550 5500 3550
Wire Wire Line
	5600 3250 4950 3250
Wire Wire Line
	4950 3350 5600 3350
Wire Wire Line
	5600 3450 4950 3450
Wire Wire Line
	6500 4750 6500 3350
Wire Wire Line
	6500 3350 6400 3350
Wire Wire Line
	6600 3550 6400 3550
Wire Wire Line
	6700 3100 6700 4550
Wire Wire Line
	6700 4550 6400 4550
Wire Wire Line
	6800 4450 6400 4450
Wire Notes Line
	7450 5000 4700 5000
$Comp
L CustomSymbols:KY-040 S1
U 1 1 5EB5AA5F
P 2700 2800
F 0 "S1" H 2700 3200 50  0000 C CNN
F 1 "KY-040" H 2700 3100 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 2800 2650 50  0001 C CNN
F 3 "" H 2800 2650 50  0001 C CNN
	1    2700 2800
	1    0    0    -1  
$EndComp
Wire Notes Line
	3100 2200 2150 2200
Wire Notes Line
	2150 2200 2150 3100
Wire Notes Line
	2150 3100 3100 3100
Text Notes 2200 2300 0    50   ~ 0
Rotary Encoder Switch
Wire Notes Line
	2900 4350 2150 4350
Wire Notes Line
	2900 5400 2900 4350
Text Notes 2200 4450 0    50   ~ 0
TFT-Display
$Comp
L CustomSymbols:TFT DS1
U 1 1 5EB79F92
P 2650 4900
F 0 "DS1" H 2423 4951 50  0000 R CNN
F 1 "TFT" H 2423 4860 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Vertical" H 2450 5350 50  0001 C CNN
F 3 "" H 2450 5350 50  0001 C CNN
	1    2650 4900
	1    0    0    -1  
$EndComp
Wire Notes Line
	4700 2600 7450 2600
Wire Notes Line
	4700 2600 4700 5000
Wire Notes Line
	7450 2600 7450 5000
Wire Notes Line
	4700 2750 5400 2750
Wire Notes Line
	5400 2750 5400 2600
Wire Notes Line
	2900 5400 2150 5400
Wire Notes Line
	2150 4350 2150 5400
$Comp
L Device:Buzzer LS1
U 1 1 5EC3C357
P 4150 6000
F 0 "LS1" V 4108 6152 50  0000 L CNN
F 1 "Buzzer" V 4350 5900 50  0000 L CNN
F 2 "Buzzer_Beeper:Buzzer_12x9.5RM7.6" V 4125 6100 50  0001 C CNN
F 3 "~" V 4125 6100 50  0001 C CNN
	1    4150 6000
	-1   0    0    -1  
$EndComp
NoConn ~ 6700 3100
$Comp
L Device:R R2
U 1 1 5EC59048
P 8050 2750
F 0 "R2" V 7843 2750 50  0000 C CNN
F 1 "220" V 7934 2750 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7980 2750 50  0001 C CNN
F 3 "~" H 8050 2750 50  0001 C CNN
	1    8050 2750
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 4450 3300 5200
Wire Wire Line
	3300 4450 5600 4450
Wire Wire Line
	4300 6100 4250 6100
$Comp
L Device:R R1
U 1 1 5EC7163C
P 4550 5900
F 0 "R1" H 4620 5946 50  0000 L CNN
F 1 "220" H 4620 5855 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4480 5900 50  0001 C CNN
F 3 "~" H 4550 5900 50  0001 C CNN
	1    4550 5900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3200 4200 3200 4900
Wire Wire Line
	3200 4900 2950 4900
Wire Wire Line
	6950 5300 3450 5300
Wire Wire Line
	3450 5300 3450 4800
Wire Wire Line
	3150 2800 4450 2800
Wire Wire Line
	3150 2700 4400 2700
Wire Wire Line
	3150 2600 4350 2600
Wire Wire Line
	4350 5000 4350 4250
Wire Wire Line
	4350 4250 5600 4250
Wire Wire Line
	4400 5100 4400 4350
Wire Wire Line
	4600 4050 5600 4050
$Comp
L CustomSymbols:MAX6675 U3
U 1 1 5EC2828A
P 9300 4350
F 0 "U3" H 9300 4650 50  0000 C CNN
F 1 "MAX6675" H 9300 4750 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 9500 4300 50  0001 C CNN
F 3 "" H 9500 4300 50  0001 C CNN
	1    9300 4350
	-1   0    0    1   
$EndComp
Wire Notes Line
	10000 3750 10000 4850
Wire Notes Line
	8650 3750 10000 3750
Wire Notes Line
	8650 4850 8650 3750
Wire Notes Line
	10000 4850 8650 4850
Wire Wire Line
	8600 2950 8600 3000
$Comp
L CustomSymbols:SSR U2
U 1 1 5EC42E89
P 8800 1950
F 0 "U2" V 8846 1770 50  0000 R CNN
F 1 "SSR" V 8755 1770 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 8600 1750 50  0001 L CIN
F 3 "" H 8800 1950 50  0001 L CNN
	1    8800 1950
	0    -1   -1   0   
$EndComp
$Comp
L Motor:Motor_Servo M1
U 1 1 5ED3A51B
P 4600 1600
F 0 "M1" V 4640 1431 50  0000 R CNN
F 1 "Motor_Servo" V 4549 1431 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4600 1410 50  0001 C CNN
F 3 "http://forums.parallax.com/uploads/attachments/46831/74481.png" H 4600 1410 50  0001 C CNN
	1    4600 1600
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR012
U 1 1 5ED3DC02
P 4600 1950
F 0 "#PWR012" H 4600 1800 50  0001 C CNN
F 1 "VCC" H 4617 2123 50  0000 C CNN
F 2 "" H 4600 1950 50  0001 C CNN
F 3 "" H 4600 1950 50  0001 C CNN
	1    4600 1950
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5ED3E7ED
P 4700 1950
F 0 "#PWR013" H 4700 1700 50  0001 C CNN
F 1 "GND" H 4705 1777 50  0000 C CNN
F 2 "" H 4700 1950 50  0001 C CNN
F 3 "" H 4700 1950 50  0001 C CNN
	1    4700 1950
	1    0    0    -1  
$EndComp
$Comp
L CustomSymbols:MicroSDReader U1
U 1 1 5ED469BA
P 2650 3700
F 0 "U1" H 2223 3746 50  0000 R CNN
F 1 "MicroSDReader" H 2223 3655 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 2700 3850 50  0001 C CNN
F 3 "" H 2700 3850 50  0001 C CNN
	1    2650 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 5200 3550 5200
Wire Wire Line
	3550 5200 3550 3550
Wire Wire Line
	3550 3550 3200 3550
Wire Wire Line
	8300 2750 8200 2750
Wire Wire Line
	8700 2250 8600 2250
Wire Wire Line
	8600 2250 8600 2550
Wire Notes Line
	8350 1450 9550 1450
Wire Notes Line
	8350 2200 9550 2200
Wire Notes Line
	8350 1600 9000 1600
Text Notes 8400 1550 0    50   ~ 0
Oven Control
Wire Wire Line
	9000 3000 9000 2900
Wire Wire Line
	8900 2250 9000 2250
$Comp
L Device:R R3
U 1 1 5EC4A045
P 9000 2750
F 0 "R3" H 9070 2796 50  0000 L CNN
F 1 "220" H 9070 2705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 8930 2750 50  0001 C CNN
F 3 "~" H 9000 2750 50  0001 C CNN
	1    9000 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 2250 9000 2600
Wire Notes Line
	4100 1100 4100 2150
Wire Notes Line
	4100 2150 5300 2150
Wire Notes Line
	5300 2150 5300 1100
Wire Notes Line
	5300 1100 4100 1100
Text Notes 4150 1200 0    50   ~ 0
Oven door servo
Wire Notes Line
	4100 1250 4800 1250
Wire Notes Line
	4800 1250 4800 1100
Wire Notes Line
	9000 1450 9000 1600
Wire Notes Line
	8350 1450 8350 2200
Wire Notes Line
	9550 1450 9550 2200
Wire Wire Line
	4350 2600 4350 3850
Wire Wire Line
	4400 2700 4400 3750
Wire Wire Line
	4450 2800 4450 3650
Wire Notes Line
	3100 2350 2150 2350
Wire Notes Line
	2700 4350 2700 4500
Wire Notes Line
	2700 4500 2150 4500
Wire Notes Line
	3200 4150 1650 4150
Wire Notes Line
	1650 4150 1650 3150
Wire Notes Line
	1650 3150 3200 3150
Wire Notes Line
	3200 3150 3200 4150
Wire Notes Line
	2600 3300 2600 3150
Text Notes 1700 3250 0    50   ~ 0
Micro SD Card Reader
Wire Notes Line
	2600 3300 1650 3300
Wire Notes Line
	3100 2200 3100 3100
Wire Notes Line
	8650 3900 9600 3900
Wire Notes Line
	9600 3900 9600 3750
Text Notes 8700 3850 0    50   ~ 0
Thermocouple Reader
$Comp
L Device:Q_PNP_CBE Q1
U 1 1 5EC506C1
P 8500 2750
F 0 "Q1" H 8400 2900 50  0000 L CNN
F 1 "Q_PNP_CBE" V 8700 2550 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline_Wide" H 8700 2850 50  0001 C CNN
F 3 "~" H 8500 2750 50  0001 C CNN
	1    8500 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3550 5500 4750
Wire Wire Line
	6950 4650 6400 4650
Wire Wire Line
	6600 2500 6600 3550
Connection ~ 6950 5200
Wire Wire Line
	6950 5200 6950 5300
Wire Wire Line
	5600 4550 4200 4550
Wire Wire Line
	5600 4650 4550 4650
Wire Wire Line
	4000 3750 4000 4650
Wire Wire Line
	4200 3650 4200 4200
Connection ~ 4200 4200
Wire Wire Line
	4200 4200 4200 4550
Wire Wire Line
	3200 4200 4200 4200
Wire Wire Line
	3200 3650 4200 3650
NoConn ~ 6400 3450
Wire Wire Line
	6500 3250 6500 3100
Wire Wire Line
	6400 3250 6500 3250
NoConn ~ 4950 3450
NoConn ~ 6800 4450
NoConn ~ 6500 3100
$Comp
L power:VCC #PWR019
U 1 1 5EC5C9AD
P 8600 3000
F 0 "#PWR019" H 8600 2850 50  0001 C CNN
F 1 "VCC" H 8617 3173 50  0000 C CNN
F 2 "" H 8600 3000 50  0001 C CNN
F 3 "" H 8600 3000 50  0001 C CNN
	1    8600 3000
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5EC5AA97
P 9000 3000
F 0 "#PWR021" H 9000 2750 50  0001 C CNN
F 1 "GND" H 9005 2827 50  0000 C CNN
F 2 "" H 9000 3000 50  0001 C CNN
F 3 "" H 9000 3000 50  0001 C CNN
	1    9000 3000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5EC03A06
P 3200 3000
F 0 "#PWR06" H 3200 2750 50  0001 C CNN
F 1 "GND" V 3205 2872 50  0000 R CNN
F 2 "" H 3200 3000 50  0001 C CNN
F 3 "" H 3200 3000 50  0001 C CNN
	1    3200 3000
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR05
U 1 1 5EC01345
P 3200 2900
F 0 "#PWR05" H 3200 2750 50  0001 C CNN
F 1 "VCC" V 3217 3028 50  0000 L CNN
F 2 "" H 3200 2900 50  0001 C CNN
F 3 "" H 3200 2900 50  0001 C CNN
	1    3200 2900
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR015
U 1 1 56D746ED
P 6500 4750
F 0 "#PWR015" H 6500 4500 50  0001 C CNN
F 1 "GND" H 6500 4600 50  0000 C CNN
F 2 "" H 6500 4750 50  0000 C CNN
F 3 "" H 6500 4750 50  0000 C CNN
	1    6500 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 56D7422C
P 5500 4750
F 0 "#PWR014" H 5500 4500 50  0001 C CNN
F 1 "GND" H 5500 4600 50  0000 C CNN
F 2 "" H 5500 4750 50  0000 C CNN
F 3 "" H 5500 4750 50  0000 C CNN
	1    5500 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5EC1E99A
P 3000 4700
F 0 "#PWR04" H 3000 4450 50  0001 C CNN
F 1 "GND" V 3005 4572 50  0000 R CNN
F 2 "" H 3000 4700 50  0001 C CNN
F 3 "" H 3000 4700 50  0001 C CNN
	1    3000 4700
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR03
U 1 1 5EC000E7
P 3000 4600
F 0 "#PWR03" H 3000 4450 50  0001 C CNN
F 1 "VCC" V 3000 4800 50  0000 C CNN
F 2 "" H 3000 4600 50  0001 C CNN
F 3 "" H 3000 4600 50  0001 C CNN
	1    3000 4600
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5EC6E105
P 4300 6100
F 0 "#PWR011" H 4300 5850 50  0001 C CNN
F 1 "GND" H 4305 5927 50  0000 C CNN
F 2 "" H 4300 6100 50  0001 C CNN
F 3 "" H 4300 6100 50  0001 C CNN
	1    4300 6100
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR016
U 1 1 5EBFB2BD
P 6600 2500
F 0 "#PWR016" H 6600 2350 50  0001 C CNN
F 1 "VCC" H 6617 2673 50  0000 C CNN
F 2 "" H 6600 2500 50  0001 C CNN
F 3 "" H 6600 2500 50  0001 C CNN
	1    6600 2500
	1    0    0    -1  
$EndComp
Text Notes 6400 3450 0    60   ~ 0
Reset
Text Notes 4950 3450 0    60   ~ 0
Reset
Text Notes 4950 3350 0    60   ~ 0
0(Rx)
Text Notes 4950 3250 0    60   ~ 0
1(Tx)
Text Notes 6800 4450 0    60   ~ 0
AREF
Text Notes 6800 4350 0    60   ~ 0
A0
Text Notes 4950 4550 0    60   ~ 0
11(**_MOSI)
Text Notes 4950 4650 0    60   ~ 0
12(MISO)
Text Notes 4950 4450 0    60   ~ 0
10(**_SS)
Text Notes 4950 4350 0    60   ~ 0
9(**)
Text Notes 4950 4250 0    60   ~ 0
8
Text Notes 4950 4150 0    60   ~ 0
7
Text Notes 4950 4050 0    60   ~ 0
6(**)
Text Notes 4950 3950 0    60   ~ 0
5(**)
Text Notes 4950 3850 0    60   ~ 0
4
Text Notes 4950 3750 0    60   ~ 0
3(**)
Text Notes 4950 3650 0    60   ~ 0
2
Text Notes 6800 3850 0    60   ~ 0
A5
Text Notes 6800 3950 0    60   ~ 0
A4
Text Notes 6800 4050 0    60   ~ 0
A3
Text Notes 6800 4150 0    60   ~ 0
A2
Text Notes 6800 4250 0    60   ~ 0
A1
Text Notes 6550 4650 0    60   ~ 0
13(SCK)
Text Notes 6500 3100 1    60   ~ 0
Vin
Text Notes 6800 3650 0    60   ~ 0
A7
$Comp
L power:GND #PWR08
U 1 1 5ED4E017
P 3250 3950
F 0 "#PWR08" H 3250 3700 50  0001 C CNN
F 1 "GND" V 3255 3822 50  0000 R CNN
F 2 "" H 3250 3950 50  0001 C CNN
F 3 "" H 3250 3950 50  0001 C CNN
	1    3250 3950
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR07
U 1 1 5ED5DA2B
P 3250 3850
F 0 "#PWR07" H 3250 3700 50  0001 C CNN
F 1 "VCC" V 3250 4050 50  0000 C CNN
F 2 "" H 3250 3850 50  0001 C CNN
F 3 "" H 3250 3850 50  0001 C CNN
	1    3250 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	3250 3850 3200 3850
Wire Wire Line
	3250 3950 3200 3950
Wire Wire Line
	3000 4600 2950 4600
Wire Wire Line
	3000 4700 2950 4700
Wire Wire Line
	4600 1950 4600 1900
Wire Wire Line
	4700 1900 4700 1950
Wire Wire Line
	3200 2900 3150 2900
Wire Wire Line
	3200 3000 3150 3000
Text Notes 6700 3100 1    50   ~ 0
+3.3V
$Comp
L power:VCC #PWR017
U 1 1 5EC718CF
P 8550 4450
F 0 "#PWR017" H 8550 4300 50  0001 C CNN
F 1 "VCC" V 8567 4623 50  0000 C CNN
F 2 "" H 8550 4450 50  0001 C CNN
F 3 "" H 8550 4450 50  0001 C CNN
	1    8550 4450
	0    -1   1    0   
$EndComp
Wire Wire Line
	8550 4450 8600 4450
Wire Wire Line
	8600 4550 8550 4550
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 5EC83616
P 7200 3850
F 0 "J4" H 7280 3842 50  0000 L CNN
F 1 "Free Pins" H 7280 3751 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7200 3850 50  0001 C CNN
F 3 "~" H 7200 3850 50  0001 C CNN
	1    7200 3850
	1    0    0    -1  
$EndComp
$Comp
L Jumper:Jumper_3_Open JP2
U 1 1 5EC8F130
P 7700 4400
F 0 "JP2" V 7746 4487 50  0000 L CNN
F 1 "Jumper_3_Open" V 7655 4487 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 7700 4400 50  0001 C CNN
F 3 "~" H 7700 4400 50  0001 C CNN
	1    7700 4400
	0    -1   -1   0   
$EndComp
$Comp
L Jumper:Jumper_3_Open JP1
U 1 1 5EC8FD57
P 7300 5200
F 0 "JP1" V 7346 5287 50  0000 L CNN
F 1 "Jumper_3_Open" V 7150 4550 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 7300 5200 50  0001 C CNN
F 3 "~" H 7300 5200 50  0001 C CNN
	1    7300 5200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7700 4650 6950 4650
Wire Wire Line
	6950 4650 6950 5200
Connection ~ 6950 4650
Wire Wire Line
	7850 4350 8600 4350
Wire Wire Line
	4550 4650 4550 5450
Wire Wire Line
	4550 5450 7300 5450
Connection ~ 4550 4650
Wire Wire Line
	4550 4650 4000 4650
Wire Wire Line
	8150 4150 8600 4150
Wire Wire Line
	7850 4400 7850 4350
Wire Wire Line
	7450 5200 8150 5200
Wire Wire Line
	8150 5200 8150 4150
Wire Wire Line
	7800 4250 8600 4250
Wire Wire Line
	7800 4250 7800 4350
Wire Wire Line
	6400 4350 7800 4350
$Comp
L Connector_Generic:Conn_01x04 J5
U 1 1 5ED0921C
P 9250 5600
F 0 "J5" H 9200 5950 50  0000 L CNN
F 1 "GND Outs" H 9000 5850 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 9250 5600 50  0001 C CNN
F 3 "~" H 9250 5600 50  0001 C CNN
	1    9250 5600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 5ED126F0
P 8900 5650
F 0 "#PWR020" H 8900 5400 50  0001 C CNN
F 1 "GND" H 8900 5500 50  0000 C CNN
F 2 "" H 8900 5650 50  0000 C CNN
F 3 "" H 8900 5650 50  0000 C CNN
	1    8900 5650
	0    1    1    0   
$EndComp
Wire Wire Line
	8900 5650 9000 5650
Wire Wire Line
	9000 5650 9000 5600
Wire Wire Line
	9000 5500 9050 5500
Wire Wire Line
	9050 5600 9000 5600
Connection ~ 9000 5600
Wire Wire Line
	9000 5600 9000 5500
Wire Wire Line
	9050 5700 9000 5700
Wire Wire Line
	9000 5700 9000 5650
Connection ~ 9000 5650
Wire Wire Line
	9050 5800 9000 5800
Wire Wire Line
	9000 5800 9000 5700
Connection ~ 9000 5700
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 5ED48EE4
P 7200 3650
F 0 "J3" H 7280 3642 50  0000 L CNN
F 1 "Analog Pins" H 7280 3551 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7200 3650 50  0001 C CNN
F 3 "~" H 7200 3650 50  0001 C CNN
	1    7200 3650
	1    0    0    -1  
$EndComp
Text Notes 6800 3750 0    60   ~ 0
A6
Wire Wire Line
	6400 3750 7000 3750
Wire Wire Line
	6400 3650 7000 3650
$Comp
L power:GND #PWR018
U 1 1 5EC723E6
P 8550 4550
F 0 "#PWR018" H 8550 4300 50  0001 C CNN
F 1 "GND" V 8555 4377 50  0000 C CNN
F 2 "" H 8550 4550 50  0001 C CNN
F 3 "" H 8550 4550 50  0001 C CNN
	1    8550 4550
	0    1    1    0   
$EndComp
Wire Wire Line
	4400 4350 5600 4350
Wire Wire Line
	4100 3450 4100 4150
Wire Wire Line
	4100 4150 5600 4150
Wire Wire Line
	2950 5200 3300 5200
Wire Wire Line
	2950 5100 4400 5100
Wire Wire Line
	2950 5000 4350 5000
Wire Wire Line
	2950 4800 3450 4800
Wire Wire Line
	3200 3750 4000 3750
Wire Wire Line
	3200 3450 4100 3450
Wire Wire Line
	7300 4150 7300 4950
Wire Wire Line
	6400 4150 7300 4150
Wire Wire Line
	7700 4150 7400 4150
Wire Wire Line
	7400 4150 7400 4250
Wire Wire Line
	6400 4250 7400 4250
$Comp
L Connector:Barrel_Jack_MountingPin J1
U 1 1 5EC292BF
P 2450 6200
F 0 "J1" H 2507 6517 50  0000 C CNN
F 1 "Barrel_Jack_MountingPin" H 2507 6426 50  0000 C CNN
F 2 "Connector_BarrelJack:BarrelJack_Horizontal" H 2500 6160 50  0001 C CNN
F 3 "~" H 2500 6160 50  0001 C CNN
	1    2450 6200
	1    0    0    -1  
$EndComp
NoConn ~ 2450 6500
$Comp
L power:VCC #PWR01
U 1 1 5EC30215
P 2800 6100
F 0 "#PWR01" H 2800 5950 50  0001 C CNN
F 1 "VCC" V 2800 6300 50  0000 C CNN
F 2 "" H 2800 6100 50  0001 C CNN
F 3 "" H 2800 6100 50  0001 C CNN
	1    2800 6100
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5EC30972
P 2800 6300
F 0 "#PWR02" H 2800 6050 50  0001 C CNN
F 1 "GND" H 2805 6127 50  0000 C CNN
F 2 "" H 2800 6300 50  0001 C CNN
F 3 "" H 2800 6300 50  0001 C CNN
	1    2800 6300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2800 6100 2750 6100
Wire Wire Line
	2800 6300 2750 6300
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5EC4B398
P 2800 6100
F 0 "#FLG01" H 2800 6175 50  0001 C CNN
F 1 "PWR_FLAG" H 2800 6273 50  0000 C CNN
F 2 "" H 2800 6100 50  0001 C CNN
F 3 "~" H 2800 6100 50  0001 C CNN
	1    2800 6100
	1    0    0    -1  
$EndComp
Connection ~ 2800 6100
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5EC4C207
P 2800 6300
F 0 "#FLG02" H 2800 6375 50  0001 C CNN
F 1 "PWR_FLAG" H 2800 6473 50  0000 C CNN
F 2 "" H 2800 6300 50  0001 C CNN
F 3 "~" H 2800 6300 50  0001 C CNN
	1    2800 6300
	-1   0    0    1   
$EndComp
Connection ~ 2800 6300
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 5EB9C29C
P 4750 3250
F 0 "J2" H 4830 3242 50  0000 L CNN
F 1 "Serial Pins" H 4550 3400 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4750 3250 50  0001 C CNN
F 3 "~" H 4750 3250 50  0001 C CNN
	1    4750 3250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4500 1900 4500 3950
Wire Wire Line
	4500 3950 5600 3950
Wire Wire Line
	4350 3850 5600 3850
Wire Wire Line
	4400 3750 5600 3750
Wire Wire Line
	4450 3650 5600 3650
Wire Wire Line
	6400 3950 7000 3950
$Comp
L Device:CP C1
U 1 1 5EBC51CA
P 3400 6200
F 0 "C1" H 3518 6246 50  0000 L CNN
F 1 "CP" H 3518 6155 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 3438 6050 50  0001 C CNN
F 3 "~" H 3400 6200 50  0001 C CNN
	1    3400 6200
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR09
U 1 1 5EBC598E
P 3400 6000
F 0 "#PWR09" H 3400 5850 50  0001 C CNN
F 1 "VCC" V 3400 6200 50  0000 C CNN
F 2 "" H 3400 6000 50  0001 C CNN
F 3 "" H 3400 6000 50  0001 C CNN
	1    3400 6000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5EBC6BAD
P 3400 6400
F 0 "#PWR010" H 3400 6150 50  0001 C CNN
F 1 "GND" H 3405 6227 50  0000 C CNN
F 2 "" H 3400 6400 50  0001 C CNN
F 3 "" H 3400 6400 50  0001 C CNN
	1    3400 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 6400 3400 6350
Wire Wire Line
	3400 6050 3400 6000
Text Notes 3450 6350 0    50   ~ 0
470µF
$Comp
L Connector_Generic:Conn_01x02 J6
U 1 1 5EC0161D
P 9850 5400
F 0 "J6" H 9768 5075 50  0000 C CNN
F 1 "VCC Out1" H 9768 5166 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 9850 5400 50  0001 C CNN
F 3 "~" H 9850 5400 50  0001 C CNN
	1    9850 5400
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J7
U 1 1 5EC0264C
P 9850 6050
F 0 "J7" H 9768 5725 50  0000 C CNN
F 1 "VCC Out2" H 9768 5816 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x02_P2.54mm_Vertical" H 9850 6050 50  0001 C CNN
F 3 "~" H 9850 6050 50  0001 C CNN
	1    9850 6050
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR023
U 1 1 5EC03CCB
P 10100 6000
F 0 "#PWR023" H 10100 5850 50  0001 C CNN
F 1 "VCC" V 10100 6200 50  0000 C CNN
F 2 "" H 10100 6000 50  0001 C CNN
F 3 "" H 10100 6000 50  0001 C CNN
	1    10100 6000
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR022
U 1 1 5EC0436A
P 10100 5350
F 0 "#PWR022" H 10100 5200 50  0001 C CNN
F 1 "VCC" V 10100 5550 50  0000 C CNN
F 2 "" H 10100 5350 50  0001 C CNN
F 3 "" H 10100 5350 50  0001 C CNN
	1    10100 5350
	0    1    1    0   
$EndComp
Wire Wire Line
	10100 5350 10050 5350
Wire Wire Line
	10050 5350 10050 5300
Wire Wire Line
	10050 5350 10050 5400
Connection ~ 10050 5350
Wire Wire Line
	10100 6000 10050 6000
Wire Wire Line
	10050 6000 10050 5950
Wire Wire Line
	10050 6000 10050 6050
Connection ~ 10050 6000
Wire Wire Line
	4250 5900 4400 5900
Wire Wire Line
	4700 5900 7900 5900
Wire Wire Line
	7900 2750 4600 2750
Wire Wire Line
	4600 2750 4600 4050
Wire Wire Line
	6400 3850 7000 3850
Wire Wire Line
	7900 4050 7900 5900
Wire Wire Line
	6400 4050 7900 4050
$EndSCHEMATC
