EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
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
Text Notes 4250 7050 0    60   ~ 0
  NOTE!!! Wire colouring and pin numbers\nmay change! Always check with a meter first.
Text GLabel 6250 6000 2    57   Output ~ 0
V.Battery
Text GLabel 6250 6100 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 6250 6400 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 6250 6600 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 6250 6200 2    57   Input ~ 0
TSDZ2_Rx
NoConn ~ 5950 6500
Text GLabel 6250 7700 2    57   Output ~ 0
V.Battery
Text GLabel 6250 7900 2    57   Input ~ 0
TSDZ2_Vin
Text GLabel 6250 7500 2    57   Input ~ 0
TSDZ2_Brake_Input
Text GLabel 6250 7800 2    57   Output ~ 0
TSDZ2_Tx
Text GLabel 6250 7600 2    57   Input ~ 0
TSDZ2_Rx
Text GLabel 7575 2250 0    57   Input ~ 0
V.Battery
Text GLabel 7600 4025 0    57   Output ~ 0
TSDZ2_Rx
Text GLabel 7600 4125 0    57   Input ~ 0
TSDZ2_Tx
Text GLabel 6250 7400 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 6250 5900 2    57   Output ~ 0
TSDZ2_GND
Text GLabel 7675 2450 0    57   Input ~ 0
TSDZ2_GND
Wire Wire Line
	5950 7400 6250 7400
Wire Wire Line
	5950 5900 6250 5900
Wire Wire Line
	6250 6000 5950 6000
Wire Wire Line
	6250 6100 5950 6100
Wire Wire Line
	5950 6200 6250 6200
Wire Wire Line
	5950 6400 6250 6400
Wire Wire Line
	5950 6600 6250 6600
Wire Wire Line
	6250 7500 5950 7500
Wire Wire Line
	5950 7600 6250 7600
Wire Wire Line
	6250 7700 5950 7700
Wire Wire Line
	5950 7800 6250 7800
Wire Wire Line
	6250 7900 5950 7900
Wire Wire Line
	7675 2450 7875 2450
Text Notes 4230 5610 0    60   ~ 0
If you have the TSDZ2 with the throttle option you will\nhave the 8-Way cable. If you don't have a throttle, you\n      are more likely to have the 6-Way cable
Wire Wire Line
	11125 3625 10925 3625
Wire Wire Line
	9525 4025 7600 4025
Wire Wire Line
	7600 4125 9525 4125
Wire Wire Line
	9200 4225 9525 4225
Text GLabel 7625 2775 0    57   Output ~ 0
TSDZ2_Vin
$Comp
L Wired_remote-rescue:power_GND-Wireless_Ebike #PWR?
U 1 1 6013E89A
P 9025 2500
F 0 "#PWR?" H 9025 2250 50  0001 C CNN
F 1 "power_GND" H 9030 2327 50  0000 C CNN
F 2 "" H 9025 2500 50  0001 C CNN
F 3 "" H 9025 2500 50  0001 C CNN
	1    9025 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	11125 2250 11125 3625
Wire Wire Line
	8875 2250 11125 2250
NoConn ~ 9525 3625
NoConn ~ 9525 3725
NoConn ~ 9525 3825
NoConn ~ 9525 3925
NoConn ~ 9525 4425
NoConn ~ 10925 4025
NoConn ~ 10925 3925
NoConn ~ 10925 4125
NoConn ~ 10925 4225
NoConn ~ 10925 4325
NoConn ~ 10925 4425
NoConn ~ 10925 4525
$Comp
L Wired_remote:8-Way_Higo-Dongle K?
U 1 1 600F98A9
P 5350 6400
F 0 "K?" H 5108 7187 60  0001 C CNN
F 1 "8 Way Higo" H 5250 7070 60  0000 C CNN
F 2 "" H 5350 6400 60  0000 C CNN
F 3 "" H 5350 6400 60  0000 C CNN
	1    5350 6400
	1    0    0    -1  
$EndComp
$Comp
L Wired_remote:6-Way_Higo-Dongle K?
U 1 1 600FCB69
P 5400 7800
F 0 "K?" H 5183 8487 60  0001 C CNN
F 1 "6 Way Higo" H 5183 8381 60  0000 C CNN
F 2 "" H 5400 7800 60  0000 C CNN
F 3 "" H 5400 7800 60  0000 C CNN
	1    5400 7800
	1    0    0    -1  
$EndComp
NoConn ~ 9525 4525
$Comp
L Wired_remote-rescue:Generic_DC-DC-Wireless_eBike-Wireless_Ebike U?
U 1 1 60103A2E
P 8375 2350
F 0 "U?" H 8375 2737 60  0001 C CNN
F 1 "Generic DC-DC (min input allowed 60V, output 5V)" H 8325 2650 60  0001 C CNN
F 2 "" H 8325 2350 60  0000 C CNN
F 3 "" H 8325 2350 60  0000 C CNN
	1    8375 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9025 2450 9025 2500
Wire Wire Line
	8875 2450 9025 2450
$Comp
L Wired_remote-rescue:BTS4140N-Wireless_eBike-Wireless_Ebike U?
U 1 1 6007C216
P 8075 3075
F 0 "U?" H 8050 3462 60  0001 C CNN
F 1 "BTS4140N" V 8045 3075 60  0000 C CNN
F 2 "" H 7475 3125 60  0001 C CNN
F 3 "" H 7475 3125 60  0000 C CNN
	1    8075 3075
	-1   0    0    1   
$EndComp
Wire Wire Line
	7575 2250 7775 2250
Wire Wire Line
	7775 3075 7775 2250
Connection ~ 7775 2250
Wire Wire Line
	7775 2250 7875 2250
Wire Wire Line
	8425 2975 8525 2975
Wire Wire Line
	8525 2975 8525 2775
Wire Wire Line
	8525 2775 7575 2775
NoConn ~ 8425 3075
NoConn ~ 9675 3475
NoConn ~ 10775 3475
NoConn ~ 10925 3725
$Comp
L Wired_remote-rescue:power_GND-Wireless_Ebike #PWR?
U 1 1 600CCF08
P 11125 3875
F 0 "#PWR?" H 11125 3625 50  0001 C CNN
F 1 "power_GND" H 11130 3702 50  0000 C CNN
F 2 "" H 11125 3875 50  0001 C CNN
F 3 "" H 11125 3875 50  0001 C CNN
	1    11125 3875
	1    0    0    -1  
$EndComp
Wire Wire Line
	11125 3825 11125 3875
$Comp
L Wired_remote:Nordic_nRF52840_USB_Dongle U?
U 1 1 6003704A
P 10225 3975
F 0 "U?" H 10225 3975 60  0001 C CNN
F 1 "Nordic nRF52840 USB Dongle" H 10325 4900 60  0000 C CNN
F 2 "" H 10075 3825 60  0000 C CNN
F 3 "" H 10075 3825 60  0000 C CNN
	1    10225 3975
	1    0    0    -1  
$EndComp
Text Notes 9875 2950 0    60   ~ 0
wireless board
Wire Wire Line
	8425 3175 8500 3175
$Comp
L Wired_remote-rescue:BSP296-Wireless_Ebike U?
U 1 1 6006EF16
P 8800 3175
F 0 "U?" H 8775 2788 60  0001 C CNN
F 1 "BSP296" V 8770 3175 60  0000 C CNN
F 2 "" H 8200 3225 60  0001 C CNN
F 3 "" H 8200 3225 60  0001 C CNN
	1    8800 3175
	-1   0    0    1   
$EndComp
Wire Wire Line
	9150 3075 9400 3075
Wire Wire Line
	9400 3075 9400 3125
$Comp
L Wired_remote-rescue:power_GND-Wireless_Ebike #PWR?
U 1 1 6005D0CE
P 9400 3125
F 0 "#PWR?" H 9400 2875 50  0001 C CNN
F 1 "power_GND" H 9405 2952 50  0000 C CNN
F 2 "" H 9400 3125 50  0001 C CNN
F 3 "" H 9400 3125 50  0001 C CNN
	1    9400 3125
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 3275 9200 3275
NoConn ~ 9150 3175
Wire Wire Line
	9200 3275 9200 4225
Text Notes 7930 2110 0    60   ~ 0
Generic DC-DC\nmin input allowed 60V, output 5V
Wire Wire Line
	10925 3825 11125 3825
NoConn ~ 5150 9250
$Comp
L Wired_remote:Switch_SW_Push button_1
U 1 1 5F4A3714
P 4751 3024
F 0 "button_1" H 4751 3224 50  0000 C CNN
F 1 "UP" H 4751 3324 50  0000 C CNB
F 2 "" H 4751 3224 50  0001 C CNN
F 3 "~" H 4751 3224 50  0001 C CNN
	1    4751 3024
	1    0    0    -1  
$EndComp
Wire Wire Line
	4401 3024 4551 3024
Connection ~ 4401 4024
Wire Wire Line
	4401 4024 4551 4024
$Comp
L Wired_remote:Switch_SW_Push button_3
U 1 1 5F4A4B3B
P 4751 4024
F 0 "button_3" H 4751 4224 50  0000 C CNN
F 1 "ENTER" H 4751 4324 50  0000 C CNB
F 2 "" H 4751 4224 50  0001 C CNN
F 3 "~" H 4751 4224 50  0001 C CNN
	1    4751 4024
	1    0    0    -1  
$EndComp
Connection ~ 4401 3524
Wire Wire Line
	4401 4024 4401 3524
Wire Wire Line
	4401 3524 4401 3024
Wire Wire Line
	4401 3524 4551 3524
$Comp
L Wired_remote:Switch_SW_Push button_2
U 1 1 5F4A3F33
P 4751 3524
F 0 "button_2" H 4751 3724 50  0000 C CNN
F 1 "DOWN" H 4751 3824 50  0000 C CNB
F 2 "" H 4751 3724 50  0001 C CNN
F 3 "~" H 4751 3724 50  0001 C CNN
	1    4751 3524
	1    0    0    -1  
$EndComp
Wire Wire Line
	4951 4024 5101 4024
Wire Wire Line
	5101 4024 5101 3824
Wire Wire Line
	5101 3724 5101 3524
Wire Wire Line
	5101 3524 4951 3524
Wire Wire Line
	4951 3024 5201 3024
Wire Wire Line
	5201 3024 5201 3624
Wire Notes Line
	4251 2574 5451 2574
Wire Notes Line
	5451 2574 5451 4824
Wire Notes Line
	5451 4824 4251 4824
Wire Notes Line
	4251 2574 4251 4824
Text Notes 4251 2524 0    79   ~ 0
VLCD5 remote keypad
Wire Wire Line
	4951 4524 5201 4524
Wire Wire Line
	4401 4524 4401 4024
Wire Wire Line
	5201 4524 5201 3924
Wire Wire Line
	4551 4524 4401 4524
$Comp
L Wired_remote:Switch_SW_Push button_4
U 1 1 5F4A4B45
P 4751 4524
F 0 "button_4" H 4751 4724 50  0000 C CNN
F 1 "POWER" H 4751 4824 50  0000 C CNB
F 2 "" H 4751 4724 50  0001 C CNN
F 3 "~" H 4751 4724 50  0001 C CNN
	1    4751 4524
	1    0    0    -1  
$EndComp
Wire Wire Line
	4401 4524 4401 4674
Wire Wire Line
	4401 4674 5301 4674
Wire Wire Line
	5301 4674 5301 4524
Connection ~ 4401 4524
Wire Wire Line
	5200 3625 9525 3625
Wire Wire Line
	9525 3725 5100 3725
Wire Wire Line
	5100 3725 5100 3825
Wire Wire Line
	5100 3825 9525 3825
Wire Wire Line
	5200 3925 9525 3925
Wire Wire Line
	5300 4525 9525 4525
NoConn ~ 9525 4325
Text GLabel 6250 6300 2    50   Output ~ 0
TSDZ2_5V
Wire Wire Line
	5950 6300 6250 6300
Wire Notes Line
	10975 5350 10975 5900
Wire Notes Line
	10175 5350 10175 5900
Text Notes 10975 5300 2    79   ~ 0
TSDZ2 Brake sensor\nconnector
Wire Notes Line
	10175 5900 10975 5900
Wire Notes Line
	10175 5350 10975 5350
Wire Wire Line
	9275 5650 10375 5650
Wire Wire Line
	10375 5550 9375 5550
$Comp
L Wired_remote:Connector_Generic_Conn_01x02 J?
U 1 1 600CF908
P 10575 5650
F 0 "J?" H 10493 5325 50  0001 C CNN
F 1 "Brake" V 10700 5600 50  0000 C CNB
F 2 "" H 10575 5650 50  0001 C CNN
F 3 "~" H 10575 5650 50  0001 C CNN
	1    10575 5650
	1    0    0    1   
$EndComp
$Comp
L Wired_remote:Connector_Generic_Conn_01x03 Brake
U 1 1 60323BC4
P 10600 6875
F 0 "Brake" V 10719 6877 50  0000 C CNB
F 1 "Brake" V 10680 6826 50  0001 L CNN
F 2 "" H 10600 6875 50  0001 C CNN
F 3 "" H 10600 6875 50  0001 C CNN
	1    10600 6875
	1    0    0    -1  
$EndComp
Wire Notes Line
	10975 6600 10975 7150
Wire Notes Line
	10175 6600 10175 7150
Text Notes 10975 6550 2    79   ~ 0
Bafang Brake sensor\nconnector
Wire Notes Line
	10175 7150 10975 7150
Wire Notes Line
	10175 6600 10975 6600
Text GLabel 9375 5550 0    57   Output ~ 0
TSDZ2_Brake_Input
Text GLabel 9275 5650 0    57   Input ~ 0
TSDZ2_GND
Text GLabel 9425 6775 0    57   Output ~ 0
TSDZ2_Brake_Input
Text GLabel 9325 6975 0    57   Input ~ 0
TSDZ2_GND
Text GLabel 9325 6875 0    50   Input ~ 0
TSDZ2_5V
Wire Wire Line
	10400 6975 9325 6975
Wire Wire Line
	10400 6775 9425 6775
Wire Wire Line
	9325 6875 10400 6875
$EndSCHEMATC
