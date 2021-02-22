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
Text Notes 2900 5900 0    60   ~ 0
  NOTE!!! Wire colouring and pin numbers\nmay change! Always check with a meter first.
Text GLabel 4900 4850 2    57   Output ~ 0
V.Battery
Text GLabel 4900 4950 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 4900 5250 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 4900 5450 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 4900 5050 2    57   Input ~ 0
TSDZ2_Rx
NoConn ~ 4600 5350
Text GLabel 4900 6550 2    57   Output ~ 0
V.Battery
Text GLabel 4900 6750 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 4900 6350 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 4900 6650 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 4900 6450 2    57   Input ~ 0
TSDZ2_Rx
Text GLabel 5425 1100 0    57   Input ~ 0
V.Battery
Text GLabel 5450 2875 0    57   Output ~ 0
TSDZ2_Rx
Text GLabel 5450 2975 0    57   Input ~ 0
TSDZ2_Tx
Text GLabel 4900 6250 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 4900 4750 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 5525 1300 0    57   Input ~ 0
TSDZ2_GND
Wire Wire Line
	4600 6250 4900 6250
Wire Wire Line
	4600 4750 4900 4750
Wire Wire Line
	4900 4850 4600 4850
Wire Wire Line
	4900 4950 4600 4950
Wire Wire Line
	4600 5050 4900 5050
Wire Wire Line
	4600 5250 4900 5250
Wire Wire Line
	4600 5450 4900 5450
Wire Wire Line
	4900 6350 4600 6350
Wire Wire Line
	4600 6450 4900 6450
Wire Wire Line
	4900 6550 4600 6550
Wire Wire Line
	4600 6650 4900 6650
Wire Wire Line
	4900 6750 4600 6750
Wire Wire Line
	5525 1300 5725 1300
Text Notes 2880 4460 0    60   ~ 0
If you have the TSDZ2 with the throttle option you will\nhave the 8-Way cable. If you don't have a throttle, you\n      are more likely to have the 6-Way cable
Wire Wire Line
	8975 2475 8775 2475
Wire Wire Line
	7375 2875 5450 2875
Wire Wire Line
	5450 2975 7375 2975
Wire Wire Line
	7050 3075 7375 3075
Text GLabel 5475 1625 0    57   Output ~ 0
TSDZ2_Vin
$Comp
L Wireless_Ebike:power_GND #PWR?
U 1 1 6013E89A
P 6875 1350
F 0 "#PWR?" H 6875 1100 50  0001 C CNN
F 1 "power_GND" H 6880 1177 50  0000 C CNN
F 2 "" H 6875 1350 50  0001 C CNN
F 3 "" H 6875 1350 50  0001 C CNN
	1    6875 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8975 1100 8975 2475
Wire Wire Line
	6725 1100 8975 1100
NoConn ~ 7375 2475
NoConn ~ 7375 2575
NoConn ~ 7375 2675
NoConn ~ 7375 2775
NoConn ~ 7375 3275
NoConn ~ 8775 2875
NoConn ~ 8775 2775
NoConn ~ 8775 2975
NoConn ~ 8775 3075
NoConn ~ 8775 3175
NoConn ~ 8775 3275
NoConn ~ 8775 3375
$Comp
L Wired_remote:8-Way_Higo-Dongle K?
U 1 1 600F98A9
P 4000 5250
F 0 "K?" H 3758 6037 60  0001 C CNN
F 1 "8 Way Higo" H 3900 5920 60  0000 C CNN
F 2 "" H 4000 5250 60  0000 C CNN
F 3 "" H 4000 5250 60  0000 C CNN
	1    4000 5250
	1    0    0    -1  
$EndComp
$Comp
L Wired_remote:6-Way_Higo-Dongle K?
U 1 1 600FCB69
P 4050 6650
F 0 "K?" H 3833 7337 60  0001 C CNN
F 1 "6 Way Higo" H 3833 7231 60  0000 C CNN
F 2 "" H 4050 6650 60  0000 C CNN
F 3 "" H 4050 6650 60  0000 C CNN
	1    4050 6650
	1    0    0    -1  
$EndComp
NoConn ~ 7375 3375
$Comp
L Wireless_Ebike:Generic_DC-DC-Wireless_eBike U?
U 1 1 60103A2E
P 6225 1200
F 0 "U?" H 6225 1587 60  0001 C CNN
F 1 "Generic DC-DC (min input allowed 60V, output 5V)" H 6175 1500 60  0001 C CNN
F 2 "" H 6175 1200 60  0000 C CNN
F 3 "" H 6175 1200 60  0000 C CNN
	1    6225 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6875 1300 6875 1350
Wire Wire Line
	6725 1300 6875 1300
$Comp
L Wireless_Ebike:BTS4140N-Wireless_eBike U?
U 1 1 6007C216
P 5925 1925
F 0 "U?" H 5900 2312 60  0001 C CNN
F 1 "BTS4140N" V 5895 1925 60  0000 C CNN
F 2 "" H 5325 1975 60  0001 C CNN
F 3 "" H 5325 1975 60  0000 C CNN
	1    5925 1925
	-1   0    0    1   
$EndComp
Wire Wire Line
	5425 1100 5625 1100
Wire Wire Line
	5625 1925 5625 1100
Connection ~ 5625 1100
Wire Wire Line
	5625 1100 5725 1100
Wire Wire Line
	6275 1825 6375 1825
Wire Wire Line
	6375 1825 6375 1625
Wire Wire Line
	6375 1625 5425 1625
NoConn ~ 6275 1925
NoConn ~ 7525 2325
NoConn ~ 8625 2325
NoConn ~ 8775 2575
$Comp
L Wireless_Ebike:power_GND #PWR?
U 1 1 600CCF08
P 8975 2725
F 0 "#PWR?" H 8975 2475 50  0001 C CNN
F 1 "power_GND" H 8980 2552 50  0000 C CNN
F 2 "" H 8975 2725 50  0001 C CNN
F 3 "" H 8975 2725 50  0001 C CNN
	1    8975 2725
	1    0    0    -1  
$EndComp
Wire Wire Line
	8975 2675 8975 2725
$Comp
L Wired_remote:Nordic_nRF52840_USB_Dongle U?
U 1 1 6003704A
P 8075 2825
F 0 "U?" H 8075 2825 60  0001 C CNN
F 1 "Nordic nRF52840 USB Dongle" H 8175 3750 60  0000 C CNN
F 2 "" H 7925 2675 60  0000 C CNN
F 3 "" H 7925 2675 60  0000 C CNN
	1    8075 2825
	1    0    0    -1  
$EndComp
Text Notes 7725 1800 0    60   ~ 0
wireless board
Wire Wire Line
	6275 2025 6350 2025
$Comp
L Wireless_Ebike:BSP296 U?
U 1 1 6006EF16
P 6650 2025
F 0 "U?" H 6625 1638 60  0001 C CNN
F 1 "BSP296" V 6620 2025 60  0000 C CNN
F 2 "" H 6050 2075 60  0001 C CNN
F 3 "" H 6050 2075 60  0001 C CNN
	1    6650 2025
	-1   0    0    1   
$EndComp
Wire Wire Line
	7000 1925 7250 1925
Wire Wire Line
	7250 1925 7250 1975
$Comp
L Wireless_Ebike:power_GND #PWR?
U 1 1 6005D0CE
P 7250 1975
F 0 "#PWR?" H 7250 1725 50  0001 C CNN
F 1 "power_GND" H 7255 1802 50  0000 C CNN
F 2 "" H 7250 1975 50  0001 C CNN
F 3 "" H 7250 1975 50  0001 C CNN
	1    7250 1975
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 2125 7050 2125
NoConn ~ 7000 2025
Wire Wire Line
	7050 2125 7050 3075
Text Notes 5780 960  0    60   ~ 0
Generic DC-DC\nmin input allowed 60V, output 5V
Wire Wire Line
	8775 2675 8975 2675
NoConn ~ 3800 8100
$Comp
L Wired_remote:Switch_SW_Push button_1
U 1 1 5F4A3714
P 2601 1874
F 0 "button_1" H 2601 2074 50  0000 C CNN
F 1 "UP" H 2601 2174 50  0000 C CNB
F 2 "" H 2601 2074 50  0001 C CNN
F 3 "~" H 2601 2074 50  0001 C CNN
	1    2601 1874
	1    0    0    -1  
$EndComp
Wire Wire Line
	2251 1874 2401 1874
Connection ~ 2251 2874
Wire Wire Line
	2251 2874 2401 2874
$Comp
L Wired_remote:Switch_SW_Push button_3
U 1 1 5F4A4B3B
P 2601 2874
F 0 "button_3" H 2601 3074 50  0000 C CNN
F 1 "ENTER" H 2601 3174 50  0000 C CNB
F 2 "" H 2601 3074 50  0001 C CNN
F 3 "~" H 2601 3074 50  0001 C CNN
	1    2601 2874
	1    0    0    -1  
$EndComp
Connection ~ 2251 2374
Wire Wire Line
	2251 2874 2251 2374
Wire Wire Line
	2251 2374 2251 1874
Wire Wire Line
	2251 2374 2401 2374
$Comp
L Wired_remote:Switch_SW_Push button_2
U 1 1 5F4A3F33
P 2601 2374
F 0 "button_2" H 2601 2574 50  0000 C CNN
F 1 "DOWN" H 2601 2674 50  0000 C CNB
F 2 "" H 2601 2574 50  0001 C CNN
F 3 "~" H 2601 2574 50  0001 C CNN
	1    2601 2374
	1    0    0    -1  
$EndComp
Wire Wire Line
	2801 2874 2951 2874
Wire Wire Line
	2951 2874 2951 2674
Wire Wire Line
	2951 2574 2951 2374
Wire Wire Line
	2951 2374 2801 2374
Wire Wire Line
	2801 1874 3051 1874
Wire Wire Line
	3051 1874 3051 2474
Wire Notes Line
	2101 1424 3301 1424
Wire Notes Line
	3301 1424 3301 3674
Wire Notes Line
	3301 3674 2101 3674
Wire Notes Line
	2101 1424 2101 3674
Text Notes 2101 1374 0    79   ~ 0
VLCD5 remote keypad
Wire Wire Line
	2801 3374 3051 3374
Wire Wire Line
	2251 3374 2251 2874
Wire Wire Line
	3051 3374 3051 2774
Wire Wire Line
	2401 3374 2251 3374
$Comp
L Wired_remote:Switch_SW_Push button_4
U 1 1 5F4A4B45
P 2601 3374
F 0 "button_4" H 2601 3574 50  0000 C CNN
F 1 "POWER" H 2601 3674 50  0000 C CNB
F 2 "" H 2601 3574 50  0001 C CNN
F 3 "~" H 2601 3574 50  0001 C CNN
	1    2601 3374
	1    0    0    -1  
$EndComp
Wire Wire Line
	2251 3374 2251 3524
Wire Wire Line
	2251 3524 3151 3524
Wire Wire Line
	3151 3524 3151 3374
Connection ~ 2251 3374
Wire Wire Line
	3050 2475 7375 2475
Wire Wire Line
	7375 2575 2950 2575
Wire Wire Line
	2950 2575 2950 2675
Wire Wire Line
	2950 2675 7375 2675
Wire Wire Line
	3050 2775 7375 2775
Wire Wire Line
	3150 3375 7375 3375
NoConn ~ 7375 3175
Text GLabel 4900 5150 2    50   Output ~ 0
TSDZ2_5V
Wire Wire Line
	4600 5150 4900 5150
Wire Notes Line
	9625 4200 9625 4750
Wire Notes Line
	8825 4200 8825 4750
Text Notes 9625 4150 2    79   ~ 0
TSDZ2 Brake sensor\nconnector
Wire Notes Line
	8825 4750 9625 4750
Wire Notes Line
	8825 4200 9625 4200
Wire Wire Line
	7925 4500 9025 4500
Wire Wire Line
	9025 4400 8025 4400
$Comp
L Wired_remote:Connector_Generic_Conn_01x02 J?
U 1 1 600CF908
P 9225 4500
F 0 "J?" H 9143 4175 50  0001 C CNN
F 1 "Brake" V 9350 4450 50  0000 C CNB
F 2 "" H 9225 4500 50  0001 C CNN
F 3 "~" H 9225 4500 50  0001 C CNN
	1    9225 4500
	1    0    0    1   
$EndComp
$Comp
L Wired_remote:Connector_Generic_Conn_01x03 Brake
U 1 1 60323BC4
P 9250 5725
F 0 "Brake" V 9369 5727 50  0000 C CNB
F 1 "Brake" V 9330 5676 50  0001 L CNN
F 2 "" H 9250 5725 50  0001 C CNN
F 3 "" H 9250 5725 50  0001 C CNN
	1    9250 5725
	1    0    0    -1  
$EndComp
Wire Notes Line
	9625 5450 9625 6000
Wire Notes Line
	8825 5450 8825 6000
Text Notes 9625 5400 2    79   ~ 0
Bafang Brake sensor\nconnector
Wire Notes Line
	8825 6000 9625 6000
Wire Notes Line
	8825 5450 9625 5450
Text GLabel 8025 4400 0    57   Output ~ 0
TSDZ2_Brake_Input
Text GLabel 7925 4500 0    57   Input ~ 0
TSDZ2_GND
Text GLabel 8075 5625 0    57   Output ~ 0
TSDZ2_Brake_Input
Text GLabel 7975 5825 0    57   Input ~ 0
TSDZ2_GND
Text GLabel 7975 5725 0    50   Input ~ 0
TSDZ2_5V
Wire Wire Line
	9050 5825 7975 5825
Wire Wire Line
	9050 5625 8075 5625
Wire Wire Line
	7975 5725 9050 5725
$EndSCHEMATC
