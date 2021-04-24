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
Text Notes 2025 3925 0    60   ~ 0
  NOTE!!! Wire colouring and pin numbers\nmay change! Always check with a meter first.
Text GLabel 4025 2875 2    57   Output ~ 0
V.Battery
Text GLabel 4025 2975 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 4025 3275 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 4025 3475 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 4025 3075 2    57   Input ~ 0
TSDZ2_Rx
NoConn ~ 3725 3375
Text GLabel 4025 4575 2    57   Output ~ 0
V.Battery
Text GLabel 4025 4775 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 4025 4375 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 4025 4675 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 4025 4475 2    57   Input ~ 0
TSDZ2_Rx
Text GLabel 5900 2550 0    57   Input ~ 0
V.Battery
Text GLabel 5925 4325 0    57   Output ~ 0
TSDZ2_Rx
Text GLabel 5925 4425 0    57   Input ~ 0
TSDZ2_Tx
Text GLabel 4025 4275 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 4025 2775 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 6000 2750 0    57   Input ~ 0
TSDZ2_GND
Wire Wire Line
	3725 4275 4025 4275
Wire Wire Line
	3725 2775 4025 2775
Wire Wire Line
	4025 2875 3725 2875
Wire Wire Line
	4025 2975 3725 2975
Wire Wire Line
	3725 3075 4025 3075
Wire Wire Line
	3725 3275 4025 3275
Wire Wire Line
	3725 3475 4025 3475
Wire Wire Line
	4025 4375 3725 4375
Wire Wire Line
	3725 4475 4025 4475
Wire Wire Line
	4025 4575 3725 4575
Wire Wire Line
	3725 4675 4025 4675
Wire Wire Line
	4025 4775 3725 4775
Wire Wire Line
	6000 2750 6200 2750
Text Notes 2005 2485 0    60   ~ 0
If you have the TSDZ2 with the throttle option you will\nhave the 8-Way cable. If you don't have a throttle, you\n      are more likely to have the 6-Way cable.
Wire Wire Line
	9450 3925 9250 3925
Wire Wire Line
	7850 4325 5925 4325
Wire Wire Line
	5925 4425 7850 4425
Wire Wire Line
	7525 4525 7850 4525
Text GLabel 5950 3075 0    57   Output ~ 0
TSDZ2_Vin
$Comp
L power:GND #PWR?
U 1 1 6013E89A
P 7350 2800
F 0 "#PWR?" H 7350 2550 50  0001 C CNN
F 1 "GND" H 7355 2627 50  0000 C CNN
F 2 "" H 7350 2800 50  0001 C CNN
F 3 "" H 7350 2800 50  0001 C CNN
	1    7350 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 2550 9450 3925
Wire Wire Line
	7200 2550 9450 2550
Text GLabel 6325 4625 0    57   Output ~ 0
TSDZ2_Brake_Input
Wire Wire Line
	7850 4625 6325 4625
NoConn ~ 7850 3925
NoConn ~ 7850 4025
NoConn ~ 7850 4125
NoConn ~ 7850 4225
NoConn ~ 7850 4725
NoConn ~ 9250 4325
NoConn ~ 9250 4225
NoConn ~ 9250 4425
NoConn ~ 9250 4525
NoConn ~ 9250 4625
NoConn ~ 9250 4725
NoConn ~ 9250 4825
$Comp
L Wireless_eBike:8-Way_Higo-Dongle K?
U 1 1 600F98A9
P 3125 3275
F 0 "K?" H 2883 4062 60  0001 C CNN
F 1 "8 Way Higo" H 3025 3945 60  0000 C CNN
F 2 "" H 3125 3275 60  0000 C CNN
F 3 "" H 3125 3275 60  0000 C CNN
	1    3125 3275
	1    0    0    -1  
$EndComp
$Comp
L Wireless_eBike:6-Way_Higo-Dongle K?
U 1 1 600FCB69
P 3175 4675
F 0 "K?" H 2958 5362 60  0001 C CNN
F 1 "6 Way Higo" H 2958 5256 60  0000 C CNN
F 2 "" H 3175 4675 60  0000 C CNN
F 3 "" H 3175 4675 60  0000 C CNN
	1    3175 4675
	1    0    0    -1  
$EndComp
NoConn ~ 3725 3175
NoConn ~ 7850 4825
$Comp
L Wireless_eBike:Generic_DC-DC-Wireless_eBike U?
U 1 1 60103A2E
P 6700 2650
F 0 "U?" H 6700 3037 60  0001 C CNN
F 1 "Generic DC-DC (min input allowed 60V, output 5V)" H 6650 2950 60  0001 C CNN
F 2 "" H 6650 2650 60  0000 C CNN
F 3 "" H 6650 2650 60  0000 C CNN
	1    6700 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 2750 7350 2800
Wire Wire Line
	7200 2750 7350 2750
$Comp
L Wireless_eBike:BTS4140N-Wireless_eBike U?
U 1 1 6007C216
P 6400 3375
F 0 "U?" H 6375 3762 60  0001 C CNN
F 1 "BTS4140N" V 6370 3375 60  0000 C CNN
F 2 "" H 5800 3425 60  0001 C CNN
F 3 "" H 5800 3425 60  0000 C CNN
	1    6400 3375
	-1   0    0    1   
$EndComp
Wire Wire Line
	5900 2550 6100 2550
Wire Wire Line
	6100 3375 6100 2550
Connection ~ 6100 2550
Wire Wire Line
	6100 2550 6200 2550
Wire Wire Line
	6750 3275 6850 3275
Wire Wire Line
	6850 3275 6850 3075
Wire Wire Line
	6850 3075 5900 3075
NoConn ~ 6750 3375
NoConn ~ 8000 3775
NoConn ~ 9100 3775
NoConn ~ 9250 4025
$Comp
L power:GND #PWR?
U 1 1 600CCF08
P 9450 4175
F 0 "#PWR?" H 9450 3925 50  0001 C CNN
F 1 "GND" H 9455 4002 50  0000 C CNN
F 2 "" H 9450 4175 50  0001 C CNN
F 3 "" H 9450 4175 50  0001 C CNN
	1    9450 4175
	1    0    0    -1  
$EndComp
Wire Wire Line
	9450 4125 9450 4175
$Comp
L Wireless_eBike:Nordic_nRF52840_USB_Dongle U?
U 1 1 6003704A
P 8550 4275
F 0 "U?" H 8550 4275 60  0001 C CNN
F 1 "Nordic nRF52840 USB Dongle" H 8650 5200 60  0000 C CNN
F 2 "" H 8400 4125 60  0000 C CNN
F 3 "" H 8400 4125 60  0000 C CNN
	1    8550 4275
	1    0    0    -1  
$EndComp
Text Notes 8200 3250 0    60   ~ 0
wireless board
Wire Wire Line
	6750 3475 6825 3475
$Comp
L Wireless_eBike:BSP296 U?
U 1 1 6006EF16
P 7125 3475
F 0 "U?" H 7100 3088 60  0001 C CNN
F 1 "BSP296" V 7095 3475 60  0000 C CNN
F 2 "" H 6525 3525 60  0001 C CNN
F 3 "" H 6525 3525 60  0001 C CNN
	1    7125 3475
	-1   0    0    1   
$EndComp
Wire Wire Line
	7475 3375 7725 3375
Wire Wire Line
	7725 3375 7725 3425
$Comp
L power:GND #PWR?
U 1 1 6005D0CE
P 7725 3425
F 0 "#PWR?" H 7725 3175 50  0001 C CNN
F 1 "GND" H 7730 3252 50  0000 C CNN
F 2 "" H 7725 3425 50  0001 C CNN
F 3 "" H 7725 3425 50  0001 C CNN
	1    7725 3425
	1    0    0    -1  
$EndComp
Wire Wire Line
	7475 3575 7525 3575
NoConn ~ 7475 3475
Wire Wire Line
	7525 3575 7525 4525
Text Notes 6255 2410 0    60   ~ 0
Generic DC-DC\nmin input allowed 60V, output 5V
Wire Wire Line
	9250 4125 9450 4125
$EndSCHEMATC
