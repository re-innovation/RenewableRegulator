EESchema Schematic File Version 2
LIBS:RenewableRegulatorPCB_2017-rescue
LIBS:power
LIBS:device
LIBS:matts_components
LIBS:conn
LIBS:RenewableRegulatorPCB_2017
LIBS:RenewableRegulatorPCB_2017-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
Title "Renewable Regulator"
Date "2017-04-04"
Rev "1"
Comp "RE-Innovation"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	5300 5200 5550 5200
$Comp
L CP1-RESCUE-RenewableRegulatorPCB_2017 C7
U 1 1 50D3631F
P 4400 5450
F 0 "C7" H 4450 5550 50  0000 L CNN
F 1 "1uf" H 4450 5350 50  0000 L CNN
F 2 "RenewableRegulatorPCB_2017_Library_Yoan:Capacitor_C7_1V7_TH" H 4400 5450 60  0001 C CNN
F 3 "" H 4400 5450 210 0001 C CNN
F 4 "~" H 4400 5450 60  0000 C CNN "Notes"
F 5 ">25V Electrolytic/Ceramic" H 4400 5450 60  0001 C CNN "Description"
F 6 "Murata" H 4400 5450 60  0001 C CNN "Manufacturer"
F 7 "RCER71H105K2M1H03A" H 4400 5450 200 0001 C CNN "Manufacturer Part No"
F 8 "RS" H 4400 5450 60  0001 C CNN "Supplier 1"
F 9 "841-0755" H 4400 5450 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.28" H 4400 5450 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 4400 5450 60  0001 C CNN "Supplier 2"
F 12 "~" H 4400 5450 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 4400 5450 60  0001 C CNN "Supplier 2 Cost"
	1    4400 5450
	1    0    0    -1  
$EndComp
Text Notes 5200 1400 0    60   ~ 0
12V and 5V Power Supply
Wire Wire Line
	5550 5700 5550 5950
Connection ~ 5550 5200
Wire Wire Line
	5550 4800 5550 5300
Wire Wire Line
	4900 5500 4900 5950
Wire Wire Line
	4400 5650 4400 5950
Text Notes 5200 1600 0    60   ~ 0
Need up to 60V DC Input
Text Notes 5200 1800 0    60   ~ 0
Low Quiescent current
Text Notes 5200 1950 0    60   ~ 0
Supply up to 500mA for 12V
Text HLabel 2650 2900 0    60   Input ~ 0
+PWR
Text HLabel 2600 4400 0    60   Input ~ 0
GND
Text HLabel 6500 4800 2    60   Output ~ 0
+5V
Wire Wire Line
	2600 5950 5750 5950
Connection ~ 4900 5950
Connection ~ 4400 5950
Wire Wire Line
	5550 4800 6500 4800
Text HLabel 6900 3200 2    60   Output ~ 0
+10V
$Comp
L LMR16006 U2
U 1 1 58E39056
P 4250 3400
F 0 "U2" H 4250 3750 60  0000 C CNN
F 1 "LMR16006" H 4250 3000 60  0000 C CNN
F 2 "REInnovationFootprint:SM_SOT23-6" H 4800 3500 60  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lmr16006.pdf" H 4800 3500 60  0001 C CNN
F 4 "~" H 4250 3400 60  0000 C CNN "Notes"
F 5 "60V 0.6A Buck Reg" H 4250 3400 60  0001 C CNN "Description"
F 6 "TI" H 4250 3400 60  0001 C CNN "Manufacturer"
F 7 "LMR16006YDDCT" H 4250 3400 60  0001 C CNN "Manufacturer Part No"
F 8 "RS" H 4250 3400 60  0001 C CNN "Supplier 1"
F 9 "900-9790" H 4250 3400 60  0001 C CNN "Supplier 1 Part No"
F 10 "2.495" H 4250 3400 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 4250 3400 60  0001 C CNN "Supplier 2"
F 12 "~" H 4250 3400 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 4250 3400 60  0001 C CNN "Supplier 2 Cost"
	1    4250 3400
	1    0    0    -1  
$EndComp
$Comp
L D_Schottky D9
U 1 1 58E391EF
P 5400 3500
F 0 "D9" H 5400 3600 50  0000 C CNN
F 1 "1N5819" H 5400 3400 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Diode_1" H 5400 3500 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds23001.pdf" H 5400 3500 50  0001 C CNN
F 4 "~" H 5400 3500 60  0000 C CNN "Notes"
F 5 "1A 0.850V Schottky 1A" H 5400 3500 60  0001 C CNN "Description"
F 6 "Diodes Inc" H 5400 3500 60  0001 C CNN "Manufacturer"
F 7 "1N5819" H 5400 3500 60  0001 C CNN "Manufacturer Part No"
F 8 "Aliexpress" H 5400 3500 60  0001 C CNN "Supplier 1"
F 9 "~" H 5400 3500 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 5400 3500 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 5400 3500 60  0001 C CNN "Supplier 2"
F 12 "~" H 5400 3500 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 5400 3500 60  0001 C CNN "Supplier 2 Cost"
	1    5400 3500
	0    1    1    0   
$EndComp
$Comp
L CP C6
U 1 1 58E39234
P 2850 3200
F 0 "C6" H 2875 3300 50  0000 L CNN
F 1 "2.2uf" H 2875 3100 50  0000 L CNN
F 2 "RenewableRegulatorPCB_2017_Library_Yoan:Capacitor_C6_1V7_TH" H 2888 3050 50  0001 C CNN
F 3 "http://docs-europe.electrocomponents.com/webdocs/156e/0900766b8156ecdd.pdf" H 2850 3200 210 0001 C CNN
F 4 "~" H 2850 3200 60  0000 C CNN "Notes"
F 5 "60V+ Ceramic Low ESR" H 2850 3200 60  0001 C CNN "Description"
F 6 "Murata" H 2850 3200 60  0001 C CNN "Manufacturer"
F 7 "RDEC72A225K3M1H03A" H 2850 3200 200 0001 C CNN "Manufacturer Part No"
F 8 "RS" H 2850 3200 60  0001 C CNN "Supplier 1"
F 9 "133-5714" H 2850 3200 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.79" H 2850 3200 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 2850 3200 60  0001 C CNN "Supplier 2"
F 12 "~" H 2850 3200 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 2850 3200 60  0001 C CNN "Supplier 2 Cost"
	1    2850 3200
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 58E392A9
P 5050 2800
F 0 "C8" H 5075 2900 50  0000 L CNN
F 1 "100nf" H 5075 2700 50  0000 L CNN
F 2 "matts_components:C1_wide_lg_pad" H 5088 2650 50  0001 C CNN
F 3 "" H 5050 2800 50  0000 C CNN
	1    5050 2800
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L1
U 1 1 58E39458
P 5850 3200
F 0 "L1" V 5800 3200 50  0000 C CNN
F 1 "22uH" V 5950 3200 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Inductor" H 5850 3200 50  0001 C CNN
F 3 "" H 5850 3200 50  0000 C CNN
F 4 "~" H 5850 3200 60  0000 C CNN "Notes"
F 5 ">1.6A Rated 2.3A" H 5850 3200 60  0001 C CNN "Description"
F 6 "Wurth" H 5850 3200 60  0001 C CNN "Manufacturer"
F 7 "744772220" H 5850 3200 60  0001 C CNN "Manufacturer Part No"
F 8 "RS" H 5850 3200 60  0001 C CNN "Supplier 1"
F 9 "489-0043" H 5850 3200 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.669" H 5850 3200 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 5850 3200 60  0001 C CNN "Supplier 2"
F 12 "~" H 5850 3200 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 5850 3200 60  0001 C CNN "Supplier 2 Cost"
	1    5850 3200
	0    -1   -1   0   
$EndComp
$Comp
L R R10
U 1 1 58E394A9
P 6300 3500
F 0 "R10" V 6380 3500 50  0000 C CNN
F 1 "100k" V 6300 3500 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Resistor_1" V 6230 3500 50  0001 C CNN
F 3 "" H 6300 3500 50  0000 C CNN
F 4 "~" H 6300 3500 60  0000 C CNN "Notes"
F 5 "~" H 6300 3500 60  0001 C CNN "Description"
F 6 "~" H 6300 3500 60  0001 C CNN "Manufacturer"
F 7 "~" H 6300 3500 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 6300 3500 60  0001 C CNN "Supplier 1"
F 9 "~" H 6300 3500 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 6300 3500 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 6300 3500 60  0001 C CNN "Supplier 2"
F 12 "~" H 6300 3500 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 6300 3500 60  0001 C CNN "Supplier 2 Cost"
	1    6300 3500
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 58E394EA
P 6300 3900
F 0 "R11" V 6380 3900 50  0000 C CNN
F 1 "8.2k" V 6300 3900 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Resistor_1" V 6230 3900 50  0001 C CNN
F 3 "" H 6300 3900 50  0000 C CNN
	1    6300 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3200 5550 3200
Wire Wire Line
	5400 3350 5400 3200
Connection ~ 5400 3200
Wire Wire Line
	2600 4400 6700 4400
Wire Wire Line
	6700 4400 6700 3700
Wire Wire Line
	6300 4400 6300 4050
Connection ~ 6300 4400
Wire Wire Line
	6300 3750 6300 3650
Wire Wire Line
	5400 3650 5400 4400
Connection ~ 5400 4400
Wire Wire Line
	6700 3400 6700 3200
Wire Wire Line
	6150 3200 6900 3200
Connection ~ 6700 3200
Wire Wire Line
	6300 3350 6300 3200
Connection ~ 6300 3200
Wire Wire Line
	5050 2950 5050 3200
Connection ~ 5050 3200
Wire Wire Line
	5050 2650 3600 2650
Wire Wire Line
	3600 2650 3600 3200
Wire Wire Line
	3600 3200 3700 3200
Wire Wire Line
	3700 3350 3600 3350
Wire Wire Line
	3600 3350 3600 4400
Connection ~ 3600 4400
Wire Wire Line
	3700 3500 3700 4200
Wire Wire Line
	3700 4200 5950 4200
Wire Wire Line
	5950 4200 5950 3700
Wire Wire Line
	5950 3700 6300 3700
Connection ~ 6300 3700
Text HLabel 4900 3350 2    60   Input ~ 0
+PWR
Wire Wire Line
	4800 3350 4900 3350
Wire Wire Line
	2850 3050 2850 2900
Wire Wire Line
	2850 3350 2850 4400
Connection ~ 2850 4400
$Comp
L MCP1702-50 U3
U 1 1 58E3A6F5
P 4900 5250
F 0 "U3" H 5050 5054 60  0000 C CNN
F 1 "MCP1702-50" H 4900 5450 60  0000 C CNN
F 2 "matts_components:TO92_MCP9701_matt" H 4900 5250 60  0001 C CNN
F 3 "http://docs-europe.electrocomponents.com/webdocs/0a77/0900766b80a77df4.pdf" H 4900 5250 60  0001 C CNN
F 4 "~" H 4900 5250 60  0000 C CNN "Notes"
F 5 "LDO 5V supply. 250mA" H 4900 5250 60  0001 C CNN "Description"
F 6 "MicroChip" H 4900 5250 60  0001 C CNN "Manufacturer"
F 7 "MCP1702-5002E/TO" H 4900 5250 60  0001 C CNN "Manufacturer Part No"
F 8 "RS" H 4900 5250 60  0001 C CNN "Supplier 1"
F 9 "403-856" H 4900 5250 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.358" H 4900 5250 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 4900 5250 60  0001 C CNN "Supplier 2"
F 12 "~" H 4900 5250 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 4900 5250 60  0001 C CNN "Supplier 2 Cost"
	1    4900 5250
	1    0    0    -1  
$EndComp
Text HLabel 2600 5950 0    60   Input ~ 0
GND
Text HLabel 3750 5200 0    60   Input ~ 0
+10V
Wire Wire Line
	3750 5200 4500 5200
Wire Wire Line
	4400 5250 4400 5200
Connection ~ 4400 5200
Text Notes 6750 1750 0    60   ~ 0
VOUT = 0.765 V (1+(R1/R2))\n10/0.765 -1 = R1/R2\n12.07 = R1/R2
$Comp
L R R9
U 1 1 58E3B2B4
P 4950 3650
F 0 "R9" V 5030 3650 50  0000 C CNN
F 1 "100k" V 4950 3650 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Resistor_1" V 4880 3650 50  0001 C CNN
F 3 "" H 4950 3650 50  0000 C CNN
F 4 "~" H 4950 3650 60  0000 C CNN "Notes"
F 5 "~" H 4950 3650 60  0001 C CNN "Description"
F 6 "~" H 4950 3650 60  0001 C CNN "Manufacturer"
F 7 "~" H 4950 3650 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 4950 3650 60  0001 C CNN "Supplier 1"
F 9 "~" H 4950 3650 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 4950 3650 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 4950 3650 60  0001 C CNN "Supplier 2"
F 12 "~" H 4950 3650 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 4950 3650 60  0001 C CNN "Supplier 2 Cost"
	1    4950 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3500 4800 3850
Wire Wire Line
	4800 3850 4950 3850
Wire Wire Line
	4950 3850 4950 3800
Wire Wire Line
	4950 3500 4950 3450
Wire Wire Line
	4950 3450 4850 3450
Wire Wire Line
	4850 3450 4850 3350
Connection ~ 4850 3350
$Comp
L CP C10
U 1 1 58E3B616
P 6700 3550
F 0 "C10" H 6725 3650 50  0000 L CNN
F 1 "10uf" H 6725 3450 50  0000 L CNN
F 2 "RenewableRegulatorPCB_2017_Library_Yoan:Capacitor_C10_1V7_TH" H 6738 3400 50  0001 C CNN
F 3 "" H 6700 3550 200 0000 C CNN
F 4 "~" H 6700 3550 60  0000 C CNN "Notes"
F 5 "Low ESR. 25V" H 6700 3550 60  0001 C CNN "Description"
F 6 "Panasonic" H 6700 3550 60  0001 C CNN "Manufacturer"
F 7 "EEUFR1H100" H 6700 3550 200 0001 C CNN "Manufacturer Part No"
F 8 "RS" H 6700 3550 60  0001 C CNN "Supplier 1"
F 9 "923-2745" H 6700 3550 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.047" H 6700 3550 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 6700 3550 60  0001 C CNN "Supplier 2"
F 12 "~" H 6700 3550 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 6700 3550 60  0001 C CNN "Supplier 2 Cost"
	1    6700 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 2900 2650 2900
$Comp
L CONN_01X01 P13
U 1 1 58E51FB7
P 6750 2950
F 0 "P13" H 6750 3050 50  0000 C CNN
F 1 "TP_10V" V 6850 2950 50  0000 C CNN
F 2 "matts_components:1pin_0_8mm" H 6750 2950 50  0001 C CNN
F 3 "" H 6750 2950 50  0000 C CNN
	1    6750 2950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 P12
U 1 1 58E5205A
P 6300 5050
F 0 "P12" H 6300 5150 50  0000 C CNN
F 1 "TP_5V" V 6400 5050 50  0000 C CNN
F 2 "matts_components:1pin_0_8mm" H 6300 5050 50  0001 C CNN
F 3 "" H 6300 5050 50  0000 C CNN
	1    6300 5050
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 P11
U 1 1 58E520E9
P 5950 5950
F 0 "P11" H 5950 6050 50  0000 C CNN
F 1 "TP_GND" V 6050 5950 50  0000 C CNN
F 2 "matts_components:1pin_0_8mm" H 5950 5950 50  0001 C CNN
F 3 "" H 5950 5950 50  0000 C CNN
	1    5950 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 2950 6450 2950
Wire Wire Line
	6450 2950 6450 3200
Connection ~ 6450 3200
Wire Wire Line
	6100 5050 5950 5050
Wire Wire Line
	5950 5050 5950 4800
Connection ~ 5950 4800
Connection ~ 5550 5950
$Comp
L CP1-RESCUE-RenewableRegulatorPCB_2017 C9
U 1 1 58E5783F
P 5550 5500
F 0 "C9" H 5600 5600 50  0000 L CNN
F 1 "1uf" H 5600 5400 50  0000 L CNN
F 2 "RenewableRegulatorPCB_2017_Library_Yoan:Capacitor_C9_1V7_TH" H 5550 5500 60  0001 C CNN
F 3 "" H 5550 5500 210 0001 C CNN
F 4 "~" H 5550 5500 60  0000 C CNN "Notes"
F 5 ">25V Electrolytic/Ceramic" H 5550 5500 60  0001 C CNN "Description"
F 6 "Murata" H 5550 5500 60  0001 C CNN "Manufacturer"
F 7 "RCER71H105K2M1H03A" H 5550 5500 200 0001 C CNN "Manufacturer Part No"
F 8 "RS" H 5550 5500 60  0001 C CNN "Supplier 1"
F 9 "841-0755" H 5550 5500 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.28" H 5550 5500 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 5550 5500 60  0001 C CNN "Supplier 2"
F 12 "~" H 5550 5500 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 5550 5500 60  0001 C CNN "Supplier 2 Cost"
	1    5550 5500
	1    0    0    -1  
$EndComp
$EndSCHEMATC
