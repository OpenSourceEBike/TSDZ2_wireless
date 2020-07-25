EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L BTS4140N U?
U 1 1 5EEB0A4F
P 9250 2050
F 0 "U?" V 9250 1750 60  0000 C CNN
F 1 "BTS4140N" V 9400 1800 60  0000 C CNN
F 2 "" H 8650 2100 60  0000 C CNN
F 3 "" H 8650 2100 60  0000 C CNN
	1    9250 2050
	0    -1   -1   0   
$EndComp
$Comp
L nRF52840 U?
U 1 1 5EEB15B7
P 7750 3400
F 0 "U?" H 7750 3450 60  0000 C CNN
F 1 "nRF52840" H 7750 2850 60  0000 C CNN
F 2 "" H 7750 3400 60  0000 C CNN
F 3 "" H 7750 3400 60  0000 C CNN
	1    7750 3400
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P?
U 1 1 5EEB15DE
P 10300 3150
F 0 "P?" H 10300 3400 50  0000 C CNN
F 1 "Flashing" V 10400 3150 50  0000 C CNN
F 2 "" H 10300 3150 50  0000 C CNN
F 3 "" H 10300 3150 50  0000 C CNN
	1    10300 3150
	1    0    0    -1  
$EndComp
$Comp
L Generic_DC-DC U?
U 1 1 5EEB1756
P 5600 2400
F 0 "U?" H 5600 2400 60  0000 C CNN
F 1 "Generic_DC-DC" H 5600 2650 60  0000 C CNN
F 2 "" H 5550 2400 60  0000 C CNN
F 3 "" H 5550 2400 60  0000 C CNN
	1    5600 2400
	1    0    0    -1  
$EndComp
NoConn ~ 7200 2900
NoConn ~ 8300 2900
$Comp
L 6-Way_Higo K?
U 1 1 5EEB225F
P 2350 3500
F 0 "K?" H 2350 3250 60  0000 C CNN
F 1 "6-Way_Higo" H 2300 4050 60  0000 C CNN
F 2 "" H 2350 3500 60  0000 C CNN
F 3 "" H 2350 3500 60  0000 C CNN
	1    2350 3500
	1    0    0    -1  
$EndComp
$Comp
L 8-Way_Higo K?
U 1 1 5EEB22D5
P 2300 2100
F 0 "K?" H 2150 1750 60  0000 C CNN
F 1 "8-Way_Higo" H 2150 2750 60  0000 C CNN
F 2 "" H 2300 2100 60  0000 C CNN
F 3 "" H 2300 2100 60  0000 C CNN
	1    2300 2100
	1    0    0    -1  
$EndComp
Text Notes 1350 2750 0    60   ~ 0
  NOTE!!! Wire colouring and pin numbers\nmay change! Always check with a meter first.
Text GLabel 3200 1700 2    57   Output ~ 0
V.Battery
Text GLabel 3200 1800 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 3200 2100 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 3200 2300 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 3200 1900 2    57   Input ~ 0
TSDZ2_Rx
NoConn ~ 2900 2200
Text GLabel 3200 3400 2    57   Output ~ 0
V.Battery
Text GLabel 3200 3600 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 3200 3200 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 3200 3500 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 3200 3300 2    57   Input ~ 0
TSDZ2_Rx
Text GLabel 4900 2300 0    57   Input ~ 0
V.Battery
Text Notes 8750 2850 0    60   ~ 0
Floating = OFF\n    GND = On
Text GLabel 9900 2500 2    57   Output ~ 0
TSDZ2_Vin
Text GLabel 4900 3400 0    57   Output ~ 0
TSDZ2_Rx
Text GLabel 4900 3500 0    57   Input ~ 0
TSDZ2_Tx
Text Notes 5100 2050 0    60   ~ 0
Check voltage of DC-DC\nBEFORE WIRING IT UP!!!
Text GLabel 3200 3100 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 3200 1600 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 4900 2500 0    57   Input ~ 0
TSDZ2_GND
Text GLabel 3350 4550 2    57   Input ~ 0
TSDZ2_GND
Text GLabel 3350 5150 2    57   Output ~ 0
TSDZ2_Brake_Input
Text Notes 5600 1600 0    60   ~ 0
V.Battery MUST!!!!!! be less than 60V otherwise smoke may escape.....
Text Notes 1550 5900 0    60   ~ 0
NOTE - The brake input to the TSDZ2 is pulled up to 5V\nby a resistor inside the motor controller. Pull the input\nto GND to activate. This is normally done with a switch\n(2-Pin Connector). If you have the 8-Way Higo cable\nyou can use a via a powered brake sensor with 3-wires.
$Comp
L CONN_01X03 P?
U 1 1 5EEB45E0
P 2000 6200
F 0 "P?" H 2000 6400 50  0000 C CNN
F 1 "Left_Brake" V 2100 6200 50  0000 C CNN
F 2 "" H 2000 6200 50  0000 C CNN
F 3 "" H 2000 6200 50  0000 C CNN
	1    2000 6200
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X03 P?
U 1 1 5EEB4667
P 2000 6800
F 0 "P?" H 2000 7000 50  0000 C CNN
F 1 "Right_Brake" V 2100 6800 50  0000 C CNN
F 2 "" H 2000 6800 50  0000 C CNN
F 3 "" H 2000 6800 50  0000 C CNN
	1    2000 6800
	-1   0    0    -1  
$EndComp
Text Notes 1050 4350 0    60   ~ 0
NOTE!!!! Pinout will depend on powered brake-sensor model - check first!
Text GLabel 3200 2000 2    57   Output ~ 0
TSDZ2_5V_Output
Text GLabel 3300 6500 2    57   Input ~ 0
TSDZ2_5V_Output
Text GLabel 3300 6100 2    57   Input ~ 0
TSDZ2_GND
Text GLabel 3300 6900 2    57   Output ~ 0
TSDZ2_Brake_Input
Wire Wire Line
	9250 2400 9250 2500
Wire Wire Line
	9250 2500 8850 2500
Wire Wire Line
	8850 2500 8850 1650
Wire Wire Line
	9250 1650 9250 1750
Wire Wire Line
	9150 3400 9150 2400
Wire Wire Line
	9350 2400 9350 2500
Wire Wire Line
	9350 2500 9900 2500
Wire Wire Line
	5000 1650 8850 1650
Wire Wire Line
	6100 2300 6250 2300
Wire Wire Line
	2900 3100 3200 3100
Wire Wire Line
	2900 1600 3200 1600
Wire Wire Line
	3200 1700 2900 1700
Wire Wire Line
	3200 1800 2900 1800
Wire Wire Line
	2900 1900 3200 1900
Wire Wire Line
	2900 2100 3200 2100
Wire Wire Line
	2900 2300 3200 2300
Wire Wire Line
	3200 3200 2900 3200
Wire Wire Line
	2900 3300 3200 3300
Wire Wire Line
	3200 3400 2900 3400
Wire Wire Line
	2900 3500 3200 3500
Wire Wire Line
	3200 3600 2900 3600
Wire Wire Line
	4900 2500 5100 2500
Wire Wire Line
	4900 2300 5000 2300
Connection ~ 8850 1650
Connection ~ 5000 2300
Wire Wire Line
	2200 5050 2650 5050
Wire Wire Line
	2650 5050 2650 4550
Wire Wire Line
	2200 4550 2650 4550
Wire Wire Line
	2200 4650 2550 4650
Wire Wire Line
	2550 4650 2550 5150
Wire Wire Line
	2200 5150 2550 5150
Connection ~ 2650 4550
Connection ~ 2550 5150
Wire Wire Line
	3200 2000 2900 2000
Wire Wire Line
	2200 6300 2500 6300
Wire Wire Line
	2500 6300 2500 6900
Wire Wire Line
	2200 6900 2500 6900
Wire Wire Line
	2200 6200 2600 6200
Wire Wire Line
	2600 6200 2600 6500
Wire Wire Line
	2600 6800 2200 6800
Wire Wire Line
	2200 6100 2700 6100
Wire Wire Line
	2700 6100 2700 6700
Wire Wire Line
	2700 6700 2200 6700
Connection ~ 2700 6100
Connection ~ 2600 6500
Connection ~ 2500 6900
Text Notes 1300 1350 0    60   ~ 0
If you have the TSDZ2 with the throttle option you will\nhave the 8-Way cable. If you don't have a throttle, you\n      are more likely to have the 6-Way cable.
Text Label 9600 3000 0    60   ~ 0
GND
Text Label 9600 3100 0    60   ~ 0
RST
Text Label 9600 3200 0    60   ~ 0
SWDCLK
Text Label 9600 3300 0    60   ~ 0
SWDIO
Text Notes 9300 3650 0    60   ~ 0
Connect to STLinkV2 for flashing\n nRF52840 must be powered\n   seperately for this to work.
Wire Wire Line
	3350 5150 3200 5150
Wire Wire Line
	3300 6900 3150 6900
$Comp
L D_Schottky D?
U 1 1 5EEBA014
P 3050 5150
F 0 "D?" H 3050 5250 50  0000 C CNN
F 1 "D_Schottky" H 3050 5050 50  0000 C CNN
F 2 "" H 3050 5150 50  0000 C CNN
F 3 "" H 3050 5150 50  0000 C CNN
	1    3050 5150
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D?
U 1 1 5EEBA05E
P 3000 6900
F 0 "D?" H 3000 7000 50  0000 C CNN
F 1 "D_Schottky" H 3000 6800 50  0000 C CNN
F 2 "" H 3000 6900 50  0000 C CNN
F 3 "" H 3000 6900 50  0000 C CNN
	1    3000 6900
	1    0    0    -1  
$EndComp
Text Label 2250 4550 0    60   ~ 0
Blue
Text Label 2250 4650 0    60   ~ 0
Red
Text Label 2250 5050 0    60   ~ 0
Blue
Text Label 2250 5150 0    60   ~ 0
Red
Text Label 2200 6100 0    60   ~ 0
Black
Text Label 2200 6200 0    60   ~ 0
Red
Text Label 2200 6300 0    60   ~ 0
Yellow
Text Label 2200 6700 0    60   ~ 0
Black
Text Label 2200 6800 0    60   ~ 0
Red
Text Label 2200 6900 0    60   ~ 0
Yellow
$Comp
L FUSE F?
U 1 1 5EEBC0DD
P 3000 6500
F 0 "F?" H 3100 6550 50  0000 C CNN
F 1 "PTC_FUSE" H 3000 6400 50  0000 C CNN
F 2 "" H 3000 6500 50  0000 C CNN
F 3 "" H 3000 6500 50  0000 C CNN
	1    3000 6500
	1    0    0    -1  
$EndComp
$Comp
L R R?
U 1 1 5EEBCDCD
P 5900 3500
F 0 "R?" V 5980 3500 50  0000 C CNN
F 1 "2K" V 5900 3500 50  0000 C CNN
F 2 "" V 5830 3500 50  0000 C CNN
F 3 "" H 5900 3500 50  0000 C CNN
	1    5900 3500
	0    1    1    0   
$EndComp
Wire Wire Line
	5750 3500 4900 3500
Text Notes 5200 4100 0    60   ~ 0
Current-limiting 2K resistor to allow\n5V TSDZ2 UART Tx to work with the\n3.3V nRF52840 UART Rx. Going in\nthe other direction is okay because\n3.3V is considered HIGH in 5V logic.
Wire Wire Line
	3300 6500 3250 6500
Wire Wire Line
	2750 6500 2600 6500
Text Notes 1600 7500 0    60   ~ 0
Diode to protect from high voltage into brake input, use\na Schottky type to reduce the dropped voltage when it's\nconducting. The Fuse is a PPTC self-resetting type to\nprotect the TSDZ2 5V - 50mA should be about right.
$Comp
L FUSE F?
U 1 1 5EEBDA76
P 6500 2300
F 0 "F?" H 6600 2350 50  0000 C CNN
F 1 "PTC_FUSE" H 6400 2200 50  0000 C CNN
F 2 "" H 6500 2300 50  0000 C CNN
F 3 "" H 6500 2300 50  0000 C CNN
	1    6500 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2300 6750 2300
Wire Notes Line
	2800 5300 2800 5000
Wire Notes Line
	2800 5000 3300 5000
Wire Notes Line
	3300 5000 3300 5300
Wire Notes Line
	3300 5300 2800 5300
Wire Notes Line
	2750 7050 2750 6350
Wire Notes Line
	2750 6350 3250 6350
Wire Notes Line
	3250 6350 3250 7050
Wire Notes Line
	3250 7050 2750 7050
Text Notes 2850 4950 0    60   ~ 0
Optional
Text Notes 2800 6750 0    60   ~ 0
Optional
Wire Notes Line
	6200 2150 6200 2450
Wire Notes Line
	6200 2150 6800 2150
Wire Notes Line
	6800 2150 6800 2450
Wire Notes Line
	6800 2450 6200 2450
Text Notes 6600 2100 0    60   ~ 0
Optional fuse, value around 250mA,\ndepends on hardware and firmware.
$Comp
L R R?
U 1 1 5EF0E0CE
P 5900 3400
F 0 "R?" V 5980 3400 50  0000 C CNN
F 1 "100R" V 5900 3400 50  0000 C CNN
F 2 "" V 5830 3400 50  0000 C CNN
F 3 "" H 5900 3400 50  0000 C CNN
	1    5900 3400
	0    1    -1   0   
$EndComp
Wire Wire Line
	5750 3400 4900 3400
Wire Wire Line
	5000 2300 5000 1650
Wire Wire Line
	6500 3100 6500 2500
Text Notes 5250 3300 0    60   ~ 0
A 100R resistor is used to help\nprotect the nRF52840 Tx output
Text Notes 4400 2850 0    60   ~ 0
Spec. of DC-DC converter must be\n  Input >60V and Output = 5V
Wire Wire Line
	6500 2500 6100 2500
Text Notes 1150 6600 0    60   ~ 12
3-WIRE BRAKE\n  SWITCHES
Text Notes 1150 4950 0    60   ~ 12
2-WIRE BRAKE\n  SWITCHES
$Comp
L CONN_01X03 P?
U 1 1 5EF1F31E
P 2000 4650
F 0 "P?" H 2000 4850 50  0000 C CNN
F 1 "Left_Brake" V 2100 4650 50  0000 C CNN
F 2 "" H 2000 4650 50  0000 C CNN
F 3 "" H 2000 4650 50  0000 C CNN
	1    2000 4650
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X03 P?
U 1 1 5EF1F377
P 2000 5150
F 0 "P?" H 2000 5350 50  0000 C CNN
F 1 "Right_Brake" V 2100 5150 50  0000 C CNN
F 2 "" H 2000 5150 50  0000 C CNN
F 3 "" H 2000 5150 50  0000 C CNN
	1    2000 5150
	-1   0    0    -1  
$EndComp
NoConn ~ 2200 4750
NoConn ~ 2200 5250
NoConn ~ 3800 8100
Wire Wire Line
	8850 1650 9250 1650
Wire Wire Line
	5000 2300 5100 2300
Wire Wire Line
	2650 4550 3350 4550
Wire Wire Line
	2550 5150 2900 5150
Wire Wire Line
	2700 6100 3300 6100
Wire Wire Line
	2600 6500 2600 6800
Wire Wire Line
	2500 6900 2850 6900
Text GLabel 7130 3400 0    50   Input ~ 0
TX
Text GLabel 7140 3500 0    50   Input ~ 0
RX
Text GLabel 7140 3000 0    50   Input ~ 0
5V
Text GLabel 7200 3100 0    50   Input ~ 0
GND
Wire Wire Line
	6500 3100 6750 3100
Wire Wire Line
	7000 2300 7000 2900
Text GLabel 8300 3400 2    50   Input ~ 0
MotorON
Text GLabel 9050 3400 0    50   Input ~ 0
MotorON
Wire Wire Line
	9050 3400 9150 3400
Text GLabel 6750 3100 2    50   Input ~ 0
GND
Wire Wire Line
	6700 2900 6700 3000
Wire Wire Line
	7000 2900 6700 2900
Text GLabel 6810 3000 2    50   Input ~ 0
5V
Text GLabel 6810 3500 2    50   Input ~ 0
RX
Text GLabel 6820 3400 2    50   Input ~ 0
TX
Wire Wire Line
	7140 3000 7200 3000
Wire Wire Line
	6700 3000 6810 3000
Wire Wire Line
	7130 3400 7200 3400
Wire Wire Line
	7200 3500 7140 3500
Wire Wire Line
	6050 3500 6810 3500
Wire Wire Line
	6820 3400 6050 3400
Text GLabel 8450 3000 2    50   Input ~ 0
GND
Text GLabel 8380 3300 2    50   Input ~ 0
SWDIO
Text GLabel 8310 3200 2    50   Input ~ 0
SWDCLK
Text GLabel 8470 3100 2    50   Input ~ 0
RST
Wire Wire Line
	8300 3200 8310 3200
Wire Wire Line
	8300 3300 8380 3300
Wire Wire Line
	8300 3100 8470 3100
Wire Wire Line
	8300 3000 8450 3000
Text GLabel 8970 3300 0    50   Input ~ 0
SWDIO
Text GLabel 9030 3200 0    50   Input ~ 0
SWDCLK
Text GLabel 8870 3100 0    50   Input ~ 0
RST
Text GLabel 8890 3000 0    50   Input ~ 0
GND
Wire Wire Line
	10100 3000 8890 3000
Wire Wire Line
	10100 3100 8870 3100
Wire Wire Line
	10100 3200 9030 3200
Wire Wire Line
	10100 3300 8970 3300
$EndSCHEMATC
