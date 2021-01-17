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
Text Notes 850  3650 0    60   ~ 0
  NOTE!!! Wire colouring and pin numbers\nmay change! Always check with a meter first.
Text GLabel 2850 2600 2    57   Output ~ 0
V.Battery
Text GLabel 2850 2700 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 2850 3000 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 2850 3200 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 2850 2800 2    57   Input ~ 0
TSDZ2_Rx
NoConn ~ 2550 3100
Text GLabel 2850 4300 2    57   Output ~ 0
V.Battery
Text GLabel 2850 4500 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 2850 4100 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 2850 4400 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 2850 4200 2    57   Input ~ 0
TSDZ2_Rx
Text GLabel 4700 2400 0    57   Input ~ 0
V.Battery
Text GLabel 4700 4450 0    57   Output ~ 0
TSDZ2_Rx
Text GLabel 4700 4550 0    57   Input ~ 0
TSDZ2_Tx
Text Notes 4200 2000 0    79   ~ 0
BEFORE WIRING THE DC-DC output signal,\nconfigure it to be 5V or it will burn the NRF52840 board!!!
Text GLabel 2850 4000 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 2850 2500 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 4800 2600 0    57   Input ~ 0
TSDZ2_GND
Wire Wire Line
	2550 4000 2850 4000
Wire Wire Line
	2550 2500 2850 2500
Wire Wire Line
	2850 2600 2550 2600
Wire Wire Line
	2850 2700 2550 2700
Wire Wire Line
	2550 2800 2850 2800
Wire Wire Line
	2550 3000 2850 3000
Wire Wire Line
	2550 3200 2850 3200
Wire Wire Line
	2850 4100 2550 4100
Wire Wire Line
	2550 4200 2850 4200
Wire Wire Line
	2850 4300 2550 4300
Wire Wire Line
	2550 4400 2850 4400
Wire Wire Line
	2850 4500 2550 4500
Wire Wire Line
	4800 2600 5000 2600
Text Notes 830  2210 0    60   ~ 0
If you have the TSDZ2 with the throttle option you will\nhave the 8-Way cable. If you don't have a throttle, you\n      are more likely to have the 6-Way cable.
$Comp
L Wireless_eBike:Nordic_nRF52840_USB_Dongle U?
U 1 1 6003704A
P 7350 4400
F 0 "U?" H 7350 4400 60  0001 C CNN
F 1 "Nordic nRF52840 USB Dongle" H 7400 3700 60  0000 C CNN
F 2 "" H 7200 4250 60  0000 C CNN
F 3 "" H 7200 4250 60  0000 C CNN
	1    7350 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 4050 8050 4050
Wire Wire Line
	4700 2400 4900 2400
Wire Wire Line
	6650 4450 4700 4450
Wire Wire Line
	4700 4550 6650 4550
Wire Wire Line
	6500 4650 6650 4650
$Comp
L power:GND #PWR?
U 1 1 600E12D9
P 8250 4350
F 0 "#PWR?" H 8250 4100 50  0001 C CNN
F 1 "GND" H 8255 4177 50  0000 C CNN
F 2 "" H 8250 4350 50  0001 C CNN
F 3 "" H 8250 4350 50  0001 C CNN
	1    8250 4350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 600E3836
P 6500 5050
F 0 "#PWR?" H 6500 4800 50  0001 C CNN
F 1 "GND" H 6505 4877 50  0000 C CNN
F 2 "" H 6500 5050 50  0001 C CNN
F 3 "" H 6500 5050 50  0001 C CNN
	1    6500 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4950 6500 5050
Wire Wire Line
	6650 4950 6500 4950
Text GLabel 4750 3400 0    57   Output ~ 0
TSDZ2_Vin
Wire Wire Line
	5000 3400 4750 3400
$Comp
L power:GND #PWR?
U 1 1 6013E89A
P 6150 2650
F 0 "#PWR?" H 6150 2400 50  0001 C CNN
F 1 "GND" H 6155 2477 50  0000 C CNN
F 2 "" H 6150 2650 50  0001 C CNN
F 3 "" H 6150 2650 50  0001 C CNN
	1    6150 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 2400 8250 4050
Wire Wire Line
	6000 2400 8250 2400
Text GLabel 5100 4750 0    57   Output ~ 0
TSDZ2_Brake_Input
Wire Wire Line
	6650 4750 5100 4750
Text Label 9400 4000 0    60   ~ 0
VBUS
Text Label 9400 4100 0    60   ~ 0
SWDCLK
Text Label 9400 4200 0    60   ~ 0
SWDIO
Wire Wire Line
	5000 3200 4900 3200
Wire Wire Line
	4900 3200 4900 2400
Connection ~ 4900 2400
Wire Wire Line
	4900 2400 5000 2400
Text Label 9400 3900 0    60   ~ 0
GND
Wire Wire Line
	9400 3900 9750 3900
Wire Wire Line
	9750 4000 9400 4000
Wire Wire Line
	9750 4100 9400 4100
Wire Wire Line
	9750 4200 9400 4200
NoConn ~ 6650 4050
NoConn ~ 6650 4150
NoConn ~ 6650 4250
NoConn ~ 6650 4350
NoConn ~ 6650 4850
NoConn ~ 8050 4450
NoConn ~ 8050 4350
NoConn ~ 8050 4550
NoConn ~ 8050 4650
NoConn ~ 8050 4750
NoConn ~ 8050 4850
NoConn ~ 8050 4950
NoConn ~ 5000 3300
Text Notes 9150 3800 0    60   ~ 0
Connector to flash the bootloader.\nYou will need it only once, so you\ncan remove the wires after flashing.\n\n\n
Text Notes 6030 2560 0    60   ~ 0
This wire MUST have about 5V (4.7V up to 5.3V)\n\n\n
Text Label 8450 3900 0    60   ~ 0
SWDCLK
Wire Wire Line
	7900 3900 8800 3900
Wire Wire Line
	8250 4050 8800 4050
Connection ~ 8250 4050
Text Label 8600 4250 0    60   ~ 0
GND
Wire Wire Line
	8050 4250 8250 4250
Wire Wire Line
	8250 4250 8250 4350
Wire Wire Line
	8250 4250 8800 4250
Connection ~ 8250 4250
Text Label 8550 4050 0    60   ~ 0
VBUS
Text Label 6350 3900 2    60   ~ 0
SWDIO
Wire Wire Line
	6800 3900 6100 3900
$Comp
L Wireless_eBike:8-Way_Higo-Dongle K?
U 1 1 600F98A9
P 1950 3000
F 0 "K?" H 1708 3787 60  0001 C CNN
F 1 "8 Way Higo" H 1850 3670 60  0000 C CNN
F 2 "" H 1950 3000 60  0000 C CNN
F 3 "" H 1950 3000 60  0000 C CNN
	1    1950 3000
	1    0    0    -1  
$EndComp
$Comp
L Wireless_eBike:6-Way_Higo-Dongle K?
U 1 1 600FCB69
P 2000 4400
F 0 "K?" H 1783 5087 60  0001 C CNN
F 1 "6 Way Higo" H 1783 4981 60  0000 C CNN
F 2 "" H 2000 4400 60  0000 C CNN
F 3 "" H 2000 4400 60  0000 C CNN
	1    2000 4400
	1    0    0    -1  
$EndComp
NoConn ~ 2550 2900
NoConn ~ 8050 4150
$Comp
L Wireless_eBike:Generic_DC-DC-Wireless_eBike U?
U 1 1 60103A2E
P 5500 2500
F 0 "U?" H 5500 2887 60  0001 C CNN
F 1 "Generic DC-DC (input 60V, output 5V)" H 5450 2800 60  0000 C CNN
F 2 "" H 5450 2500 60  0000 C CNN
F 3 "" H 5450 2500 60  0000 C CNN
	1    5500 2500
	1    0    0    -1  
$EndComp
$Comp
L Wireless_eBike:CONN_01X04-Wireless_eBike P?
U 1 1 60034D62
P 9950 4050
F 0 "P?" H 10028 4091 50  0001 L CNN
F 1 "Bootloader flash" V 10050 3750 50  0000 L CNN
F 2 "" H 9950 4050 50  0000 C CNN
F 3 "" H 9950 4050 50  0000 C CNN
	1    9950 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3400 6500 4650
NoConn ~ 6400 3300
Wire Wire Line
	5750 3300 5650 3300
Wire Wire Line
	6400 3400 6500 3400
Wire Wire Line
	6150 2600 6150 2650
$Comp
L Wireless_eBike:BTS4140N-Wireless_eBike U?
U 1 1 60104E35
P 5350 3300
F 0 "U?" H 5325 3687 60  0001 C CNN
F 1 "BTS4140N" H 5300 3550 60  0000 C CNN
F 2 "" H 4750 3350 60  0000 C CNN
F 3 "" H 4750 3350 60  0000 C CNN
	1    5350 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2600 6150 2600
$Comp
L power:GND #PWR?
U 1 1 6005D0CE
P 6700 3250
F 0 "#PWR?" H 6700 3000 50  0001 C CNN
F 1 "GND" H 6705 3077 50  0000 C CNN
F 2 "" H 6700 3250 50  0001 C CNN
F 3 "" H 6700 3250 50  0001 C CNN
	1    6700 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6700 3200 6700 3250
Wire Wire Line
	6400 3200 6700 3200
$Comp
L Wireless_eBike:BSP296 U?
U 1 1 6006EF16
P 6050 3300
F 0 "U?" H 6025 2913 60  0001 C CNN
F 1 "BSP296" H 6050 3050 60  0000 C CNN
F 2 "" H 5450 3350 60  0000 C CNN
F 3 "" H 5450 3350 60  0000 C CNN
	1    6050 3300
	-1   0    0    1   
$EndComp
$EndSCHEMATC
