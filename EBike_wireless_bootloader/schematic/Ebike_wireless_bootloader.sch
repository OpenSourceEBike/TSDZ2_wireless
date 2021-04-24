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
NoConn ~ 4600 3550
NoConn ~ 4600 3650
NoConn ~ 4600 3750
NoConn ~ 4600 3850
NoConn ~ 4600 4350
NoConn ~ 6000 3950
NoConn ~ 6000 3850
NoConn ~ 6000 4050
NoConn ~ 6000 4150
NoConn ~ 6000 4250
NoConn ~ 6000 4350
NoConn ~ 6000 4450
Text Label 6950 3550 2    60   ~ 0
SWDCLK
Text Label 6950 3750 2    60   ~ 0
GND
Text Label 6950 3450 2    60   ~ 0
SWDIO
NoConn ~ 6000 3650
$Comp
L Wireless_eBike:Nordic_nRF52840_USB_Dongle U?
U 1 1 6003704A
P 5300 3900
F 0 "U?" H 5300 3900 60  0001 C CNN
F 1 "Nordic nRF52840 USB Dongle" H 5350 3200 60  0000 C CNN
F 2 "" H 5150 3750 60  0000 C CNN
F 3 "" H 5150 3750 60  0000 C CNN
	1    5300 3900
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J?
U 1 1 600C848A
P 7150 3650
F 0 "J?" H 7230 3642 50  0001 L CNN
F 1 "Bootloader flash" V 7250 3300 50  0000 L CNN
F 2 "" H 7150 3650 50  0001 C CNN
F 3 "~" H 7150 3650 50  0001 C CNN
	1    7150 3650
	1    0    0    1   
$EndComp
Text Notes 6600 3450 0    60   ~ 0
Connector to flash the bootloader.\nYou will need it only once, so you\ncan remove the wires after flashing.\n\n\n
Wire Wire Line
	4750 3400 4650 3400
Wire Wire Line
	4650 3400 4650 2950
Wire Wire Line
	4650 2950 6300 2950
Wire Wire Line
	6000 3550 6100 3550
Wire Wire Line
	6000 3750 6950 3750
Wire Wire Line
	6100 3550 6100 3650
Wire Wire Line
	6200 3400 6200 3550
Wire Wire Line
	6200 3550 6950 3550
Wire Wire Line
	5850 3400 6200 3400
Wire Wire Line
	6300 2950 6300 3450
Wire Wire Line
	6300 3450 6950 3450
Text Label 6950 3650 2    60   ~ 0
VBUS(3.3V)
Wire Wire Line
	6100 3650 6950 3650
NoConn ~ 4600 3950
NoConn ~ 4600 4050
NoConn ~ 4600 4150
NoConn ~ 4600 4450
NoConn ~ 4600 4250
$EndSCHEMATC
