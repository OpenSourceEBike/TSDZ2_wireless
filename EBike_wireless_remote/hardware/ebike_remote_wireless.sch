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
NoConn ~ 3800 8100
$Comp
L Switch:SW_Push button_1
U 1 1 5F4A3714
P 4100 2650
F 0 "button_1" H 4100 2850 50  0000 C CNN
F 1 "UP" H 4100 2950 50  0000 C CNB
F 2 "" H 4100 2850 50  0001 C CNN
F 3 "~" H 4100 2850 50  0001 C CNN
	1    4100 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 2650 3900 2650
Connection ~ 3750 3650
Wire Wire Line
	3750 3650 3900 3650
$Comp
L Switch:SW_Push button_3
U 1 1 5F4A4B3B
P 4100 3650
F 0 "button_3" H 4100 3850 50  0000 C CNN
F 1 "ENTER" H 4100 3950 50  0000 C CNB
F 2 "" H 4100 3850 50  0001 C CNN
F 3 "~" H 4100 3850 50  0001 C CNN
	1    4100 3650
	1    0    0    -1  
$EndComp
Connection ~ 3750 3150
Wire Wire Line
	3750 3650 3750 3150
Wire Wire Line
	3750 3150 3750 2650
Wire Wire Line
	3750 3150 3900 3150
$Comp
L Switch:SW_Push button_2
U 1 1 5F4A3F33
P 4100 3150
F 0 "button_2" H 4100 3350 50  0000 C CNN
F 1 "DOWN" H 4100 3450 50  0000 C CNB
F 2 "" H 4100 3350 50  0001 C CNN
F 3 "~" H 4100 3350 50  0001 C CNN
	1    4100 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 3650 4450 3650
Wire Wire Line
	4450 3650 4450 3450
Wire Wire Line
	4450 3350 4450 3150
Wire Wire Line
	4450 3150 4300 3150
Wire Wire Line
	4300 2650 4550 2650
Wire Wire Line
	4550 2650 4550 3250
Wire Notes Line
	3600 2200 4800 2200
Wire Notes Line
	4800 2200 4800 4450
Wire Notes Line
	4800 4450 3600 4450
Wire Notes Line
	3600 2200 3600 4450
Text Notes 3600 2150 0    79   ~ 0
VLCD5 remote keypad
Wire Wire Line
	4300 4150 4550 4150
Wire Wire Line
	3750 4150 3750 3650
Wire Wire Line
	4550 4150 4550 3550
Wire Wire Line
	3900 4150 3750 4150
$Comp
L Switch:SW_Push button_4
U 1 1 5F4A4B45
P 4100 4150
F 0 "button_4" H 4100 4350 50  0000 C CNN
F 1 "POWER" H 4100 4450 50  0000 C CNB
F 2 "" H 4100 4350 50  0001 C CNN
F 3 "~" H 4100 4350 50  0001 C CNN
	1    4100 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 4150 3750 4300
Wire Wire Line
	3750 4300 4650 4300
Wire Wire Line
	4650 4300 4650 4150
Connection ~ 3750 4150
$Comp
L Connector_Generic:Conn_01x02 J?
U 1 1 600CF908
P 4000 5350
F 0 "J?" H 3918 5025 50  0001 C CNN
F 1 "Brake" V 4125 5300 50  0000 C CNB
F 2 "" H 4000 5350 50  0001 C CNN
F 3 "~" H 4000 5350 50  0001 C CNN
	1    4000 5350
	-1   0    0    -1  
$EndComp
Wire Notes Line
	3600 5050 3600 5800
Wire Notes Line
	4400 5050 4400 5800
Text Notes 3600 5000 0    79   ~ 0
Brake sensor\nconnector
Wire Notes Line
	4400 5800 3600 5800
Wire Notes Line
	4400 5050 3600 5050
Wire Wire Line
	4200 5450 5250 5450
Wire Wire Line
	5150 5350 4200 5350
Wire Wire Line
	5150 3650 5150 5350
Wire Wire Line
	5400 3650 5150 3650
Wire Wire Line
	5250 4150 5400 4150
Connection ~ 5250 4150
Wire Wire Line
	5250 5450 5250 4150
NoConn ~ 6800 3250
Wire Wire Line
	7600 3600 7600 3550
Wire Wire Line
	7400 3600 7600 3600
Wire Wire Line
	7400 3450 7400 3600
Wire Wire Line
	6800 3350 7400 3350
Wire Wire Line
	7600 3200 7600 3250
Wire Wire Line
	7400 3200 7600 3200
Wire Wire Line
	7400 3350 7400 3200
Wire Wire Line
	6800 3450 7400 3450
NoConn ~ 6800 3550
NoConn ~ 6800 3650
NoConn ~ 6800 3750
NoConn ~ 6800 3850
NoConn ~ 6800 3950
NoConn ~ 6800 4050
NoConn ~ 5400 4050
NoConn ~ 5400 3950
NoConn ~ 5400 3850
NoConn ~ 5400 3750
Wire Notes Line
	7150 3700 7900 3700
Wire Notes Line
	7900 3050 7900 3700
Wire Wire Line
	4650 4150 5250 4150
$Comp
L Device:Battery_Cell coincell
U 1 1 600B4826
P 7600 3450
F 0 "coincell" V 7450 3500 50  0001 C CNN
F 1 "CR2032 coin cell" H 7530 3780 50  0000 C CNN
F 2 "" V 7600 3510 50  0001 C CNN
F 3 "~" V 7600 3510 50  0001 C CNN
	1    7600 3450
	1    0    0    -1  
$EndComp
Text Notes 7150 3000 0    79   ~ 0
should work for\nat least 2 years
Wire Notes Line
	7150 3050 7900 3050
Wire Notes Line
	7150 3700 7150 3050
$Comp
L Wireless_eBike:Nordic_nRF52840_USB_Dongle U?
U 1 1 600AD575
P 6100 3600
F 0 "U?" H 6100 4637 60  0001 C CNN
F 1 "Nordic nRF52840 USB Dongle" H 6100 4550 60  0000 C CNN
F 2 "" H 5950 3450 60  0000 C CNN
F 3 "" H 5950 3450 60  0000 C CNN
	1    6100 3600
	1    0    0    -1  
$EndComp
Text Notes 5600 2550 0    79   ~ 0
wireless board
Wire Wire Line
	4550 3250 5400 3250
Wire Wire Line
	5400 3350 4450 3350
Wire Wire Line
	4450 3450 5400 3450
Wire Wire Line
	4550 3550 5400 3550
NoConn ~ 5550 3100
NoConn ~ 6650 3100
NoConn ~ 6800 4150
$EndSCHEMATC
