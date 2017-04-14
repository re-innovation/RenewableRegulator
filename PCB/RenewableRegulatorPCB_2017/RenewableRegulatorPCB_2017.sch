EESchema Schematic File Version 2
LIBS:RenewableRegulatorPCB_2017-rescue
LIBS:power
LIBS:device
LIBS:conn
LIBS:RenewableRegulatorPCB_2017
LIBS:matts_components
LIBS:RenewableRegulatorPCB_2017-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title "Renewable Regulator"
Date "2017-04-04"
Rev "1"
Comp "RE-Innovation"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Connection ~ 1200 4700
Wire Wire Line
	950  4700 1500 4700
Wire Wire Line
	1500 4700 1500 4500
Wire Wire Line
	1200 3900 1800 3900
Connection ~ 1100 4700
Wire Wire Line
	1100 4800 1100 4700
Wire Wire Line
	950  4150 950  4000
Wire Wire Line
	950  4700 950  4550
Wire Wire Line
	1200 4700 1200 4550
Connection ~ 1200 4000
Wire Wire Line
	950  4000 1500 4000
Wire Wire Line
	1500 4000 1500 4100
Wire Wire Line
	1200 3000 1200 3250
NoConn ~ 6300 7050
NoConn ~ 6300 6850
NoConn ~ 6300 6700
Text Notes 6400 7300 0    60   ~ 0
PCB
$Comp
L CONN_1 LOGO2
U 1 1 516547D4
P 6450 7050
F 0 "LOGO2" H 6530 7050 40  0000 L CNN
F 1 "LOGO" H 6450 7105 30  0001 C CNN
F 2 "CuriousElectric3:TCEC_Words_13mm" H 6450 7050 60  0001 C CNN
F 3 "" H 6450 7050 60  0001 C CNN
F 4 "~" H 6450 7050 60  0000 C CNN "Notes"
F 5 "~" H 6450 7050 60  0001 C CNN "Description"
F 6 "~" H 6450 7050 60  0001 C CNN "Manufacturer"
F 7 "~" H 6450 7050 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 6450 7050 60  0001 C CNN "Supplier 1"
F 9 "~" H 6450 7050 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 6450 7050 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 6450 7050 60  0001 C CNN "Supplier 2"
F 12 "~" H 6450 7050 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 6450 7050 60  0001 C CNN "Supplier 2 Cost"
	1    6450 7050
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 LOGO1
U 1 1 516547D2
P 6450 6850
F 0 "LOGO1" H 6530 6850 40  0000 L CNN
F 1 "LOGO" H 6450 6905 30  0001 C CNN
F 2 "CuriousElectric3:CEC_Globe_10mm_FCU" H 6450 6850 60  0001 C CNN
F 3 "" H 6450 6850 60  0001 C CNN
F 4 "~" H 6450 6850 60  0000 C CNN "Notes"
F 5 "~" H 6450 6850 60  0001 C CNN "Description"
F 6 "~" H 6450 6850 60  0001 C CNN "Manufacturer"
F 7 "~" H 6450 6850 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 6450 6850 60  0001 C CNN "Supplier 1"
F 9 "~" H 6450 6850 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 6450 6850 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 6450 6850 60  0001 C CNN "Supplier 2"
F 12 "~" H 6450 6850 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 6450 6850 60  0001 C CNN "Supplier 2 Cost"
	1    6450 6850
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 PCB1
U 1 1 516547CF
P 6450 6700
F 0 "PCB1" H 6530 6700 40  0000 L CNN
F 1 "PCB" H 6450 6755 30  0001 C CNN
F 2 "REInnovationFootprint:PCB_75x100mm" H 6450 6700 60  0001 C CNN
F 3 "" H 6450 6700 60  0001 C CNN
F 4 "~" H 6450 6700 60  0000 C CNN "Notes"
F 5 "~" H 6450 6700 60  0001 C CNN "Description"
F 6 "~" H 6450 6700 60  0001 C CNN "Manufacturer"
F 7 "~" H 6450 6700 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 6450 6700 60  0001 C CNN "Supplier 1"
F 9 "~" H 6450 6700 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 6450 6700 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 6450 6700 60  0001 C CNN "Supplier 2"
F 12 "~" H 6450 6700 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 6450 6700 60  0001 C CNN "Supplier 2 Cost"
	1    6450 6700
	1    0    0    -1  
$EndComp
Text GLabel 8200 3000 0    60   Input ~ 0
SD_CS
Text GLabel 8200 3300 0    60   Input ~ 0
MISO
Text GLabel 8200 3100 0    60   Input ~ 0
MOSI
Text GLabel 8200 3200 0    60   Input ~ 0
SCK
Text Notes 1450 5150 0    60   ~ 0
Voltage Measurement
$Comp
L ZENER-RESCUE-RenewableRegulatorPCB_2017 D1
U 1 1 50D5DC34
P 1500 4300
F 0 "D1" H 1500 4400 50  0000 C CNN
F 1 "3v3" H 1500 4200 40  0000 C CNN
F 2 "REInnovationFootprint:TH_Diode_1" H 1500 4300 60  0001 C CNN
F 3 "" H 1500 4300 60  0001 C CNN
F 4 "~" H 1500 4300 60  0000 C CNN "Notes"
F 5 "~" H 1500 4300 60  0001 C CNN "Description"
F 6 "~" H 1500 4300 60  0001 C CNN "Manufacturer"
F 7 "~" H 1500 4300 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 1500 4300 60  0001 C CNN "Supplier 1"
F 9 "~" H 1500 4300 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 1500 4300 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 1500 4300 60  0001 C CNN "Supplier 2"
F 12 "~" H 1500 4300 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 1500 4300 60  0001 C CNN "Supplier 2 Cost"
	1    1500 4300
	0    -1   -1   0   
$EndComp
Text GLabel 1800 3900 2    60   Output ~ 0
VINm
$Comp
L C-RESCUE-RenewableRegulatorPCB_2017 C1
U 1 1 50D5D72D
P 950 4350
F 0 "C1" H 1000 4450 50  0000 L CNN
F 1 "100nf" H 700 4200 50  0000 L CNN
F 2 "matts_components:C1_wide_lg_pad" H 950 4350 60  0001 C CNN
F 3 "" H 950 4350 60  0001 C CNN
	1    950  4350
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-RenewableRegulatorPCB_2017 R2
U 1 1 50D36545
P 1200 4300
F 0 "R2" V 1280 4300 50  0000 C CNN
F 1 "Ra" V 1200 4300 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Resistor_1" H 1200 4300 60  0001 C CNN
F 3 "" H 1200 4300 60  0001 C CNN
	1    1200 4300
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-RenewableRegulatorPCB_2017 R1
U 1 1 50D36543
P 1200 3500
F 0 "R1" V 1280 3500 50  0000 C CNN
F 1 "Rb" V 1200 3500 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Resistor_1" H 1200 3500 60  0001 C CNN
F 3 "" H 1200 3500 60  0001 C CNN
	1    1200 3500
	1    0    0    -1  
$EndComp
$Comp
L CONN_2-RESCUE-RenewableRegulatorPCB_2017 P1
U 1 1 50D3652B
P 1400 1600
F 0 "P1" V 1350 1600 40  0000 C CNN
F 1 "INPUT" V 1450 1600 40  0000 C CNN
F 2 "RenewableRegulatorPCB_2017_Library_Yoan:Connecteurs_IO_SIL-2_screw_terminal_32A" H 1400 1600 60  0001 C CNN
F 3 "" H 1400 1600 60  0001 C CNN
	1    1400 1600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1200 3750 1200 4050
Connection ~ 1200 3900
Wire Wire Line
	1750 1500 1750 1150
Wire Wire Line
	1750 1700 1750 2100
Text GLabel 7150 1650 0    60   Input ~ 0
FET1
Text GLabel 3100 800  2    60   Output ~ 0
Vinput
$Comp
L Arduino_Nano_Header J2
U 1 1 58E28067
P 5450 4450
F 0 "J2" H 5450 5250 60  0000 C CNN
F 1 "Arduino_Nano" H 5450 3650 60  0000 C CNN
F 2 "REInnovationFootprint:NANO_DIP_30_600" H 5450 4450 60  0001 C CNN
F 3 "" H 5450 4450 60  0000 C CNN
F 4 "~" H 5450 4450 60  0000 C CNN "Notes"
F 5 "~" H 5450 4450 60  0001 C CNN "Description"
F 6 "~" H 5450 4450 60  0001 C CNN "Manufacturer"
F 7 "~" H 5450 4450 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 5450 4450 60  0001 C CNN "Supplier 1"
F 9 "~" H 5450 4450 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 5450 4450 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 5450 4450 60  0001 C CNN "Supplier 2"
F 12 "~" H 5450 4450 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 5450 4450 60  0001 C CNN "Supplier 2 Cost"
	1    5450 4450
	1    0    0    -1  
$EndComp
$Sheet
S 7350 1050 1250 1050
U 58E26C6A
F0 "FET Control" 60
F1 "FETControl.sch" 60
F2 "FET1" I L 7350 1650 60 
F3 "GND" I L 7350 1950 60 
F4 "+PWR" I L 7350 1150 60 
F5 "OUTPUT1+" O R 8600 1200 60 
F6 "+FET_Supply" I L 7350 1400 60 
$EndSheet
$Sheet
S 4850 1950 650  650 
U 58E26E4F
F0 "Power Supply" 60
F1 "powersupply.sch" 60
F2 "+PWR" I L 4850 2100 60 
F3 "GND" I L 4850 2450 60 
F4 "+5V" O R 5500 2300 60 
F5 "+10V" O R 5500 2100 60 
$EndSheet
$Comp
L FUSE F1
U 1 1 58E2A6D5
P 4450 2100
F 0 "F1" H 4550 2150 50  0000 C CNN
F 1 "500mA" H 4350 2050 50  0000 C CNN
F 2 "REInnovationFootprint:TH_MC36188_FUSE" H 4450 2100 50  0001 C CNN
F 3 "" H 4450 2100 50  0000 C CNN
F 4 "~" H 4450 2100 60  0000 C CNN "Notes"
F 5 "Resettable Fuse 60V" H 4450 2100 60  0001 C CNN "Description"
F 6 "TE" H 4450 2100 60  0001 C CNN "Manufacturer"
F 7 "RXEF050" H 4450 2100 60  0001 C CNN "Manufacturer Part No"
F 8 "RS" H 4450 2100 60  0001 C CNN "Supplier 1"
F 9 "517-6635" H 4450 2100 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.263" H 4450 2100 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 4450 2100 60  0001 C CNN "Supplier 2"
F 12 "~" H 4450 2100 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 4450 2100 60  0001 C CNN "Supplier 2 Cost"
	1    4450 2100
	-1   0    0    1   
$EndComp
Wire Wire Line
	1750 1150 7350 1150
Wire Wire Line
	7350 1950 7200 1950
Wire Wire Line
	7200 1950 7200 2000
Wire Wire Line
	4850 2450 4700 2450
Wire Wire Line
	4700 2450 4700 2550
Wire Wire Line
	3100 800  3000 800 
Text GLabel 1050 3000 0    60   Input ~ 0
Vinput
Wire Wire Line
	1050 3000 1200 3000
$Comp
L Encoder_1 ENC1
U 1 1 58E3D5E0
P 1250 6450
F 0 "ENC1" H 1750 6450 60  0000 C CNN
F 1 "Encoder" H 1350 6100 60  0000 C CNN
F 2 "REInnovationFootprint:TH_Encoder" H 1250 6450 60  0001 C CNN
F 3 "" H 1250 6450 60  0000 C CNN
F 4 "~" H 1250 6450 60  0000 C CNN "Notes"
F 5 "~" H 1250 6450 60  0001 C CNN "Description"
F 6 "~" H 1250 6450 60  0001 C CNN "Manufacturer"
F 7 "~" H 1250 6450 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 1250 6450 60  0001 C CNN "Supplier 1"
F 9 "~" H 1250 6450 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 1250 6450 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 1250 6450 60  0001 C CNN "Supplier 2"
F 12 "~" H 1250 6450 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 1250 6450 60  0001 C CNN "Supplier 2 Cost"
	1    1250 6450
	1    0    0    -1  
$EndComp
$Comp
L OLED_I2C DISP1
U 1 1 58E3E571
P 8350 6350
F 0 "DISP1" H 8750 7350 60  0000 C CNN
F 1 "OLED_I2C" H 8750 6600 60  0000 C CNN
F 2 "REInnovationFootprint:TH_OLED1306" H 9750 6700 60  0001 C CNN
F 3 "" H 9750 6700 60  0000 C CNN
F 4 "~" H 8350 6350 60  0000 C CNN "Notes"
F 5 "~" H 8350 6350 60  0001 C CNN "Description"
F 6 "~" H 8350 6350 60  0001 C CNN "Manufacturer"
F 7 "~" H 8350 6350 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 8350 6350 60  0001 C CNN "Supplier 1"
F 9 "~" H 8350 6350 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 8350 6350 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 8350 6350 60  0001 C CNN "Supplier 2"
F 12 "~" H 8350 6350 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 8350 6350 60  0001 C CNN "Supplier 2 Cost"
	1    8350 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2100 6300 2100
Wire Wire Line
	6300 2100 6300 1400
Wire Wire Line
	6300 1400 7350 1400
Text GLabel 5750 2300 2    60   Output ~ 0
+5V
Wire Wire Line
	5500 2300 5750 2300
Text GLabel 5600 6950 2    60   Input ~ 0
+5V
Wire Wire Line
	5700 6500 5700 6600
Text GLabel 7450 5850 0    60   Input ~ 0
+5V
Text GLabel 5900 4050 2    60   Input ~ 0
+5V
Text GLabel 5950 4450 2    60   Output ~ 0
SDA
Text GLabel 5950 4350 2    60   Output ~ 0
SCL
Wire Wire Line
	5800 4350 5950 4350
Wire Wire Line
	5800 4450 5950 4450
Text GLabel 7900 5750 0    60   Input ~ 0
SCL
Text GLabel 7900 5650 0    60   Input ~ 0
SDA
Wire Wire Line
	7450 5850 8000 5850
Wire Wire Line
	7850 5950 8000 5950
Wire Wire Line
	7900 5750 8000 5750
Wire Wire Line
	7900 5650 8000 5650
Wire Wire Line
	7400 6250 7400 6200
Wire Wire Line
	7400 6200 7850 6200
Wire Wire Line
	7850 6200 7850 5950
Text GLabel 1050 5950 1    60   Output ~ 0
EN_A
Text GLabel 1250 5950 1    60   Output ~ 0
EN_B
Text GLabel 1550 5950 1    60   Output ~ 0
EN_SW
Wire Wire Line
	1050 6650 1050 7000
Wire Wire Line
	1050 7000 1550 7000
Wire Wire Line
	1550 7000 1550 6650
Wire Wire Line
	1550 6250 1550 5950
Wire Wire Line
	1250 6250 1250 5950
Wire Wire Line
	1050 6250 1050 5950
Wire Wire Line
	1300 7100 1300 7000
Connection ~ 1300 7000
Text GLabel 4600 6750 0    60   Input ~ 0
LED_D
Wire Wire Line
	4600 6750 4850 6750
NoConn ~ 4850 6950
Text GLabel 4950 4150 0    60   Input ~ 0
EN_A
Text GLabel 4950 4250 0    60   Input ~ 0
EN_B
Text GLabel 4950 4350 0    60   Input ~ 0
EN_SW
Text GLabel 4550 4450 0    60   Input ~ 0
LED_D
Text GLabel 4950 4550 0    60   Input ~ 0
FET1
Wire Wire Line
	7150 1650 7350 1650
Text GLabel 4850 4950 0    60   Output ~ 0
SD_CS
Text GLabel 4850 5150 0    60   Output ~ 0
MISO
Text GLabel 4850 5050 0    60   Output ~ 0
MOSI
Text GLabel 5950 5150 2    60   Output ~ 0
SCK
Wire Wire Line
	4550 4450 5100 4450
Wire Wire Line
	4950 4550 5100 4550
Wire Wire Line
	4950 4350 5100 4350
Wire Wire Line
	4950 4250 5100 4250
Wire Wire Line
	4950 4150 5100 4150
$Comp
L CONN_01X04 P6
U 1 1 58E47BCB
P 10350 3100
F 0 "P6" H 10350 3350 50  0000 C CNN
F 1 "SERIAL" V 10450 3100 50  0000 C CNN
F 2 "matts_components:SIL-4_Grove_SIL" H 10350 3100 50  0001 C CNN
F 3 "" H 10350 3100 50  0000 C CNN
	1    10350 3100
	1    0    0    -1  
$EndComp
Text GLabel 4900 3750 0    60   Input ~ 0
Tx
Text GLabel 4900 3850 0    60   Input ~ 0
Rx
Text GLabel 9850 3150 0    60   Output ~ 0
Tx
Text GLabel 9850 3250 0    60   Output ~ 0
Rx
Wire Wire Line
	5800 4050 5900 4050
$Comp
L GND #PWR01
U 1 1 58E48AC8
P 6300 3900
F 0 "#PWR01" H 6300 3650 50  0001 C CNN
F 1 "GND" H 6300 3750 50  0000 C CNN
F 2 "" H 6300 3900 50  0000 C CNN
F 3 "" H 6300 3900 50  0000 C CNN
	1    6300 3900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 58E48B46
P 4400 4050
F 0 "#PWR02" H 4400 3800 50  0001 C CNN
F 1 "GND" H 4400 3900 50  0000 C CNN
F 2 "" H 4400 4050 50  0000 C CNN
F 3 "" H 4400 4050 50  0000 C CNN
	1    4400 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 58E49461
P 1300 7100
F 0 "#PWR03" H 1300 6850 50  0001 C CNN
F 1 "GND" H 1300 6950 50  0000 C CNN
F 2 "" H 1300 7100 50  0000 C CNN
F 3 "" H 1300 7100 50  0000 C CNN
	1    1300 7100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 58E494D8
P 1100 4800
F 0 "#PWR04" H 1100 4550 50  0001 C CNN
F 1 "GND" H 1100 4650 50  0000 C CNN
F 2 "" H 1100 4800 50  0000 C CNN
F 3 "" H 1100 4800 50  0000 C CNN
	1    1100 4800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 58E495BD
P 5700 6600
F 0 "#PWR05" H 5700 6350 50  0001 C CNN
F 1 "GND" H 5700 6450 50  0000 C CNN
F 2 "" H 5700 6600 50  0000 C CNN
F 3 "" H 5700 6600 50  0000 C CNN
	1    5700 6600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 58E49634
P 7400 6250
F 0 "#PWR06" H 7400 6000 50  0001 C CNN
F 1 "GND" H 7400 6100 50  0000 C CNN
F 2 "" H 7400 6250 50  0000 C CNN
F 3 "" H 7400 6250 50  0000 C CNN
	1    7400 6250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 58E496AB
P 9550 2800
F 0 "#PWR07" H 9550 2550 50  0001 C CNN
F 1 "GND" H 9550 2650 50  0000 C CNN
F 2 "" H 9550 2800 50  0000 C CNN
F 3 "" H 9550 2800 50  0000 C CNN
	1    9550 2800
	1    0    0    -1  
$EndComp
Text GLabel 9950 3050 0    60   Input ~ 0
+5V
$Comp
L GND #PWR08
U 1 1 58E4989E
P 4700 2550
F 0 "#PWR08" H 4700 2300 50  0001 C CNN
F 1 "GND" H 4700 2400 50  0000 C CNN
F 2 "" H 4700 2550 50  0000 C CNN
F 3 "" H 4700 2550 50  0000 C CNN
	1    4700 2550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 58E49915
P 1750 2100
F 0 "#PWR09" H 1750 1850 50  0001 C CNN
F 1 "GND" H 1750 1950 50  0000 C CNN
F 2 "" H 1750 2100 50  0000 C CNN
F 3 "" H 1750 2100 50  0000 C CNN
	1    1750 2100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 58E4998C
P 7200 2000
F 0 "#PWR010" H 7200 1750 50  0001 C CNN
F 1 "GND" H 7200 1850 50  0000 C CNN
F 2 "" H 7200 2000 50  0000 C CNN
F 3 "" H 7200 2000 50  0000 C CNN
	1    7200 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10150 3250 9850 3250
Wire Wire Line
	9850 3150 10150 3150
Wire Wire Line
	9950 3050 10150 3050
Wire Wire Line
	10150 2950 9750 2950
Wire Wire Line
	9750 2950 9750 2800
Wire Wire Line
	9750 2800 9550 2800
$Comp
L CONN_01X06 P3
U 1 1 58E4B8E8
P 8600 3150
F 0 "P3" H 8600 3500 50  0000 C CNN
F 1 "MicroSD" V 8700 3150 50  0000 C CNN
F 2 "matts_components:SIL-6_large_pads_marker" H 8600 3150 50  0001 C CNN
F 3 "http://www.gearbest.com/development-boards/pp_63013.html?currency=GBP&vip=760169&gclid=CPHFuoqAjdMCFUK7GwodO08Omw" H 8600 3150 50  0001 C CNN
F 4 "~" H 8600 3150 60  0000 C CNN "Notes"
F 5 "External Micro SD slot with level shifter" H 8600 3150 60  0001 C CNN "Description"
F 6 "~" H 8600 3150 60  0001 C CNN "Manufacturer"
F 7 "~" H 8600 3150 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 8600 3150 60  0001 C CNN "Supplier 1"
F 9 "~" H 8600 3150 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 8600 3150 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 8600 3150 60  0001 C CNN "Supplier 2"
F 12 "~" H 8600 3150 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 8600 3150 60  0001 C CNN "Supplier 2 Cost"
	1    8600 3150
	1    0    0    -1  
$EndComp
Text GLabel 6450 5050 2    60   Input ~ 0
+3V3
Wire Wire Line
	6450 5050 5800 5050
$Comp
L CONN_01X02 P2
U 1 1 58E4BF9F
P 6650 4850
F 0 "P2" H 6650 5000 50  0000 C CNN
F 1 "Vref?" V 6750 4850 50  0000 C CNN
F 2 "matts_components:SIL-2_solder_connect" H 6650 4850 50  0001 C CNN
F 3 "" H 6650 4850 50  0000 C CNN
	1    6650 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6450 4900 6300 4900
Wire Wire Line
	6300 4900 6300 5050
Connection ~ 6300 5050
Wire Wire Line
	5800 4950 6250 4950
Wire Wire Line
	6250 4950 6250 4800
Wire Wire Line
	6250 4800 6450 4800
Text GLabel 8200 2900 0    60   Input ~ 0
+3V3
$Comp
L GND #PWR011
U 1 1 58E4CD4B
P 8100 3450
F 0 "#PWR011" H 8100 3200 50  0001 C CNN
F 1 "GND" H 8100 3300 50  0000 C CNN
F 2 "" H 8100 3450 50  0000 C CNN
F 3 "" H 8100 3450 50  0000 C CNN
	1    8100 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8400 3400 8100 3400
Wire Wire Line
	8100 3400 8100 3450
Wire Wire Line
	8200 3300 8400 3300
Wire Wire Line
	8200 3200 8400 3200
Wire Wire Line
	8200 3100 8400 3100
Wire Wire Line
	8200 3000 8400 3000
Wire Wire Line
	8200 2900 8400 2900
Wire Wire Line
	4850 4950 5100 4950
Wire Wire Line
	4850 5050 5100 5050
Wire Wire Line
	4850 5150 5100 5150
Wire Wire Line
	5800 5150 5950 5150
Wire Wire Line
	5800 3850 6300 3850
Wire Wire Line
	6300 3850 6300 3900
Wire Wire Line
	5100 4050 4400 4050
Text Notes 5000 7350 0    60   ~ 0
RGB LED
Text Notes 1000 7450 0    60   ~ 0
ROTARY ENCODER
Text Notes 8500 6400 0    60   ~ 0
OLED DISPLAY
Text Notes 9800 3600 0    60   ~ 0
SERIAL OUTPUT
Text Notes 7900 3750 0    60   ~ 0
SD CARD MODULE
Wire Wire Line
	4900 3750 5100 3750
Wire Wire Line
	4900 3850 5100 3850
Text Notes 7650 2350 0    60   ~ 0
FET & CONTROL
Text Notes 4750 2900 0    60   ~ 0
POWER SUPPLY
Text Notes 5050 5550 0    60   ~ 0
MICRO CONTROLLER
Text Notes 700  850  0    60   ~ 0
Designed for max 30A current\nMust have 30A fuse in Battery Line
Text Notes 2750 2250 0    60   ~ 0
Reverse Polarity Protection\nWill blow control fuse if reversed.
$Comp
L CONN_2-RESCUE-RenewableRegulatorPCB_2017 P5
U 1 1 58E5328E
P 10250 1750
F 0 "P5" V 10200 1750 40  0000 C CNN
F 1 "OUTPUT" V 10300 1750 40  0000 C CNN
F 2 "RenewableRegulatorPCB_2017_Library_Yoan:Connecteurs_IO_SIL-2_screw_terminal_32A" H 10250 1750 60  0001 C CNN
F 3 "" H 10250 1750 60  0001 C CNN
	1    10250 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 2050 9650 1850
$Comp
L GND #PWR012
U 1 1 58E533E0
P 9650 2050
F 0 "#PWR012" H 9650 1800 50  0001 C CNN
F 1 "GND" H 9650 1900 50  0000 C CNN
F 2 "" H 9650 2050 50  0000 C CNN
F 3 "" H 9650 2050 50  0000 C CNN
	1    9650 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 1850 9900 1850
Wire Wire Line
	8600 1200 9650 1200
Wire Wire Line
	9650 850  9650 1650
Wire Wire Line
	9650 1650 9900 1650
Text GLabel 5900 4850 2    60   Input ~ 0
VINm
Wire Wire Line
	5800 4850 5900 4850
$Comp
L VR VR1
U 1 1 58E58395
P 2400 1700
F 0 "VR1" V 2460 1654 50  0000 C TNN
F 1 "VR" V 2400 1700 50  0000 C CNN
F 2 "REInnovationFootprint:TH_MC36188_FUSE" H 2400 1700 50  0001 C CNN
F 3 "http://docs-europe.electrocomponents.com/webdocs/13c2/0900766b813c2445.pdf" H 2400 1700 50  0001 C CNN
F 4 "~" H 2400 1700 60  0000 C CNN "Notes"
F 5 "MO Varistor 50A. 135V, 82V" H 2400 1700 60  0001 C CNN "Description"
F 6 "EPCOS" H 2400 1700 60  0001 C CNN "Manufacturer"
F 7 "B72214S0500K101" H 2400 1700 60  0001 C CNN "Manufacturer Part No"
F 8 "RS" H 2400 1700 60  0001 C CNN "Supplier 1"
F 9 "289-7121" H 2400 1700 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.114" H 2400 1700 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 2400 1700 60  0001 C CNN "Supplier 2"
F 12 "~" H 2400 1700 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 2400 1700 60  0001 C CNN "Supplier 2 Cost"
	1    2400 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 1350 9250 1200
Connection ~ 9250 1200
Wire Wire Line
	9250 1850 9250 1950
Wire Wire Line
	8900 1950 9650 1950
Connection ~ 9650 1950
Wire Wire Line
	2400 1450 2400 1150
Connection ~ 2400 1150
Wire Wire Line
	2400 2000 1750 2000
Connection ~ 1750 2000
$Comp
L VR VR2
U 1 1 58E590B7
P 9250 1600
F 0 "VR2" V 9310 1554 50  0000 C TNN
F 1 "VR" V 9250 1600 50  0000 C CNN
F 2 "REInnovationFootprint:TH_MC36188_FUSE" H 9250 1600 50  0001 C CNN
F 3 "http://docs-europe.electrocomponents.com/webdocs/13c2/0900766b813c2445.pdf" H 9250 1600 50  0001 C CNN
F 4 "~" H 9250 1600 60  0000 C CNN "Notes"
F 5 "MO Varistor 50A. 135V, 82V" H 9250 1600 60  0001 C CNN "Description"
F 6 "EPCOS" H 9250 1600 60  0001 C CNN "Manufacturer"
F 7 "B72214S0500K101" H 9250 1600 60  0001 C CNN "Manufacturer Part No"
F 8 "RS" H 9250 1600 60  0001 C CNN "Supplier 1"
F 9 "289-7121" H 9250 1600 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.114" H 9250 1600 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 9250 1600 60  0001 C CNN "Supplier 2"
F 12 "~" H 9250 1600 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 9250 1600 60  0001 C CNN "Supplier 2 Cost"
	1    9250 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 2000 2400 1950
Wire Wire Line
	4100 1450 4100 1150
Connection ~ 4100 1150
Wire Wire Line
	4100 1750 4100 2100
Wire Wire Line
	4100 2100 4200 2100
Wire Wire Line
	4700 2100 4850 2100
Connection ~ 2450 4750
Wire Wire Line
	2200 4750 2750 4750
Wire Wire Line
	2750 4750 2750 4550
Wire Wire Line
	2450 3950 3050 3950
Connection ~ 2350 4750
Wire Wire Line
	2350 4850 2350 4750
Wire Wire Line
	2200 4200 2200 4050
Wire Wire Line
	2200 4750 2200 4600
Wire Wire Line
	2450 4750 2450 4600
Connection ~ 2450 4050
Wire Wire Line
	2200 4050 2750 4050
Wire Wire Line
	2750 4050 2750 4150
Wire Wire Line
	2450 3050 2450 3300
$Comp
L ZENER-RESCUE-RenewableRegulatorPCB_2017 D2
U 1 1 58E5B7CD
P 2750 4350
F 0 "D2" H 2750 4450 50  0000 C CNN
F 1 "3v3" H 2750 4250 40  0000 C CNN
F 2 "REInnovationFootprint:TH_Diode_1" H 2750 4350 60  0001 C CNN
F 3 "" H 2750 4350 60  0001 C CNN
	1    2750 4350
	0    -1   -1   0   
$EndComp
Text GLabel 3050 3950 2    60   Output ~ 0
VOUTm
$Comp
L C-RESCUE-RenewableRegulatorPCB_2017 C2
U 1 1 58E5B7D4
P 2200 4400
F 0 "C2" H 2250 4500 50  0000 L CNN
F 1 "100nf" H 1950 4250 50  0000 L CNN
F 2 "matts_components:C1_wide_lg_pad" H 2200 4400 60  0001 C CNN
F 3 "" H 2200 4400 60  0001 C CNN
	1    2200 4400
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-RenewableRegulatorPCB_2017 R4
U 1 1 58E5B7DA
P 2450 4350
F 0 "R4" V 2530 4350 50  0000 C CNN
F 1 "Ra" V 2450 4350 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Resistor_1" H 2450 4350 60  0001 C CNN
F 3 "" H 2450 4350 60  0001 C CNN
	1    2450 4350
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-RenewableRegulatorPCB_2017 R3
U 1 1 58E5B7E0
P 2450 3550
F 0 "R3" V 2530 3550 50  0000 C CNN
F 1 "Rb" V 2450 3550 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Resistor_1" H 2450 3550 60  0001 C CNN
F 3 "" H 2450 3550 60  0001 C CNN
	1    2450 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 3800 2450 4100
Connection ~ 2450 3950
Text GLabel 2300 3050 0    60   Input ~ 0
Voutput
Wire Wire Line
	2300 3050 2450 3050
$Comp
L GND #PWR013
U 1 1 58E5B7EA
P 2350 4850
F 0 "#PWR013" H 2350 4600 50  0001 C CNN
F 1 "GND" H 2350 4700 50  0000 C CNN
F 2 "" H 2350 4850 50  0000 C CNN
F 3 "" H 2350 4850 50  0000 C CNN
	1    2350 4850
	1    0    0    -1  
$EndComp
$Comp
L D D3
U 1 1 58E5BC33
P 3750 1600
F 0 "D3" H 3750 1700 50  0000 C CNN
F 1 "1N5819" H 3750 1500 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Diode_1" H 3750 1600 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds23001.pdf" H 3750 1600 50  0001 C CNN
F 4 "~" H 3750 1600 60  0000 C CNN "Notes"
F 5 "1A Schottky Diode" H 3750 1600 60  0001 C CNN "Description"
F 6 "Diodes Inc" H 3750 1600 60  0001 C CNN "Manufacturer"
F 7 "1N5819" H 3750 1600 60  0001 C CNN "Manufacturer Part No"
F 8 "AliExpress" H 3750 1600 60  0001 C CNN "Supplier 1"
F 9 "~" H 3750 1600 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 3750 1600 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 3750 1600 60  0001 C CNN "Supplier 2"
F 12 "~" H 3750 1600 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 3750 1600 60  0001 C CNN "Supplier 2 Cost"
	1    3750 1600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3750 1450 3750 850 
Wire Wire Line
	3750 850  9650 850 
Connection ~ 9650 1200
Wire Wire Line
	3000 800  3000 1150
Connection ~ 3000 1150
Wire Wire Line
	3750 1750 3750 1900
Wire Wire Line
	3750 1900 4100 1900
Connection ~ 4100 1900
Text GLabel 5900 4750 2    60   Input ~ 0
VOUTm
Wire Wire Line
	5900 4750 5800 4750
Text GLabel 9850 1050 2    60   Output ~ 0
Voutput
Wire Wire Line
	9850 1050 9650 1050
Connection ~ 9650 1050
Text GLabel 10550 5750 0    60   Input ~ 0
MISO
Text GLabel 10550 5550 0    60   Input ~ 0
MOSI
Text GLabel 10550 5450 0    60   Input ~ 0
SCK
Text GLabel 10550 6050 0    60   Input ~ 0
+3V3
$Comp
L GND #PWR014
U 1 1 58E5E4EA
P 10150 6050
F 0 "#PWR014" H 10150 5800 50  0001 C CNN
F 1 "GND" H 10150 5900 50  0000 C CNN
F 2 "" H 10150 6050 50  0000 C CNN
F 3 "" H 10150 6050 50  0000 C CNN
	1    10150 6050
	1    0    0    -1  
$EndComp
Text Notes 10100 6450 0    60   ~ 0
SPI 128x128 LCD
$Comp
L CONN_01X08 DISP2
U 1 1 58E5E80A
P 10900 5700
F 0 "DISP2" H 10900 6150 50  0000 C CNN
F 1 "SPI_LCD" V 11000 5700 50  0000 C CNN
F 2 "REInnovationFootprint:TH_LCD_SPI_128x128" H 10900 5700 50  0001 C CNN
F 3 "https://www.elecrow.com/wiki/index.php?title=1.44%27%27_128x_128_TFT_LCD_with_SPI_Interface" H 10900 5700 50  0001 C CNN
F 4 "~" H 10900 5700 60  0000 C CNN "Notes"
F 5 "An SPI Colour LCD with backlight" H 10900 5700 60  0001 C CNN "Description"
F 6 "Elecrow" H 10900 5700 60  0001 C CNN "Manufacturer"
F 7 "~" H 10900 5700 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 10900 5700 60  0001 C CNN "Supplier 1"
F 9 "~" H 10900 5700 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 10900 5700 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 10900 5700 60  0001 C CNN "Supplier 2"
F 12 "~" H 10900 5700 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 10900 5700 60  0001 C CNN "Supplier 2 Cost"
	1    10900 5700
	1    0    0    1   
$EndComp
Text GLabel 4850 4850 0    60   Output ~ 0
LCD_CS
Wire Wire Line
	4850 4850 5100 4850
Text GLabel 4850 4750 0    60   Output ~ 0
LCD_BL
Wire Wire Line
	5100 4750 4850 4750
Text GLabel 10550 5850 0    60   Input ~ 0
LCD_CS
Text GLabel 10550 5350 0    60   Input ~ 0
LCD_BL
Wire Wire Line
	10700 6050 10550 6050
Wire Wire Line
	10700 5950 10150 5950
Wire Wire Line
	10150 5950 10150 6050
Text GLabel 4850 4650 0    60   Output ~ 0
LCD_DC
Wire Wire Line
	5100 4650 4850 4650
Text GLabel 10550 5650 0    60   Input ~ 0
LCD_DC
Wire Wire Line
	10550 5350 10700 5350
Wire Wire Line
	10550 5450 10700 5450
Wire Wire Line
	10550 5550 10700 5550
Wire Wire Line
	10550 5650 10700 5650
Wire Wire Line
	10550 5750 10700 5750
Wire Wire Line
	10550 5850 10700 5850
$Comp
L CP C3
U 1 1 58E65BB3
P 8900 1550
F 0 "C3" H 8925 1650 50  0000 L CNN
F 1 "100uf" H 8925 1450 50  0000 L CNN
F 2 "RenewableRegulatorPCB_2017_Library_Yoan:Capacitor_C_1V7_TH" H 8938 1400 400 0001 C CNN
F 3 "" H 8900 1550 400 0001 C CNN
F 4 "~" H 8900 1550 60  0000 C CNN "Notes"
F 5 "100V Electrolytic" H 8900 1550 60  0001 C CNN "Description"
F 6 "Panasonic" H 8900 1550 60  0001 C CNN "Manufacturer"
F 7 "ECA-2AM101B" H 8900 1550 394 0001 C CNN "Manufacturer Part No"
F 8 "RS" H 8900 1550 60  0001 C CNN "Supplier 1"
F 9 "807-3431" H 8900 1550 60  0001 C CNN "Supplier 1 Part No"
F 10 "0.141" H 8900 1550 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 8900 1550 60  0001 C CNN "Supplier 2"
F 12 "~" H 8900 1550 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 8900 1550 60  0001 C CNN "Supplier 2 Cost"
	1    8900 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 1400 8900 1200
Connection ~ 8900 1200
Wire Wire Line
	8900 1700 8900 1950
Connection ~ 9250 1950
$Comp
L D D4
U 1 1 58E67C68
P 4100 1600
F 0 "D4" H 4100 1700 50  0000 C CNN
F 1 "1N5819" H 4100 1500 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Diode_1" H 4100 1600 50  0001 C CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds23001.pdf" H 4100 1600 50  0001 C CNN
F 4 "~" H 4100 1600 60  0000 C CNN "Notes"
F 5 "1A Schottky Diode" H 4100 1600 60  0001 C CNN "Description"
F 6 "Diodes Inc" H 4100 1600 60  0001 C CNN "Manufacturer"
F 7 "1N5819" H 4100 1600 60  0001 C CNN "Manufacturer Part No"
F 8 "AliExpress" H 4100 1600 60  0001 C CNN "Supplier 1"
F 9 "~" H 4100 1600 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 4100 1600 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 4100 1600 60  0001 C CNN "Supplier 2"
F 12 "~" H 4100 1600 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 4100 1600 60  0001 C CNN "Supplier 2 Cost"
	1    4100 1600
	0    -1   -1   0   
$EndComp
$Comp
L JACK_TRS_6PINS J1
U 1 1 58E6BA28
P 3100 6650
F 0 "J1" H 3100 7050 50  0000 C CNN
F 1 "TEMP" H 3050 6350 50  0000 C CNN
F 2 "REInnovationFootprint:TH_3_5mm_Jack" H 3200 6500 50  0001 C CNN
F 3 "" H 3200 6500 50  0000 C CNN
	1    3100 6650
	0    -1   -1   0   
$EndComp
Text Notes 2600 7300 0    60   ~ 0
Temperature Sensor
Text Notes 2650 7700 0    60   ~ 0
DS18B20 1 Wire\n1: GND\n2: Data\n3: 5V
Text GLabel 5950 4550 2    60   Output ~ 0
TEMP
Text GLabel 3500 6150 2    60   Input ~ 0
TEMP
$Comp
L R R5
U 1 1 58E4A8DC
P 3300 5900
F 0 "R5" V 3380 5900 50  0000 C CNN
F 1 "4k7" V 3300 5900 50  0000 C CNN
F 2 "REInnovationFootprint:TH_Resistor_1" V 3230 5900 50  0001 C CNN
F 3 "" H 3300 5900 50  0000 C CNN
	1    3300 5900
	1    0    0    -1  
$EndComp
Text GLabel 3100 5650 1    60   Input ~ 0
+5V
$Comp
L GND #PWR015
U 1 1 58E4AD2E
P 2800 5900
F 0 "#PWR015" H 2800 5650 50  0001 C CNN
F 1 "GND" H 2800 5750 50  0000 C CNN
F 2 "" H 2800 5900 50  0000 C CNN
F 3 "" H 2800 5900 50  0000 C CNN
	1    2800 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 5900 2900 5900
Wire Wire Line
	2900 5900 2900 6250
Wire Wire Line
	3100 5650 3100 6250
Wire Wire Line
	3300 6250 3300 6050
Wire Wire Line
	3300 5750 3300 5700
Wire Wire Line
	3300 5700 3100 5700
Connection ~ 3100 5700
Wire Wire Line
	3300 6150 3500 6150
Connection ~ 3300 6150
$Comp
L CONN_01X04 P7
U 1 1 58E4B95C
P 10500 4550
F 0 "P7" H 10500 4800 50  0000 C CNN
F 1 "A2" V 10600 4550 50  0000 C CNN
F 2 "matts_components:SIL-4_Grove_SIL" H 10500 4550 50  0001 C CNN
F 3 "" H 10500 4550 50  0000 C CNN
	1    10500 4550
	1    0    0    -1  
$EndComp
Text GLabel 8150 4650 0    60   Output ~ 0
A6
Text GLabel 10000 4700 0    60   Output ~ 0
A2
$Comp
L GND #PWR016
U 1 1 58E4B964
P 9700 4250
F 0 "#PWR016" H 9700 4000 50  0001 C CNN
F 1 "GND" H 9700 4100 50  0000 C CNN
F 2 "" H 9700 4250 50  0000 C CNN
F 3 "" H 9700 4250 50  0000 C CNN
	1    9700 4250
	1    0    0    -1  
$EndComp
Text GLabel 10100 4500 0    60   Input ~ 0
+5V
Wire Wire Line
	10300 4700 10000 4700
Wire Wire Line
	10100 4500 10300 4500
Wire Wire Line
	10300 4400 9900 4400
Wire Wire Line
	9900 4400 9900 4250
Wire Wire Line
	9900 4250 9700 4250
Text Notes 9900 4950 0    60   ~ 0
A2 OUTPUT
NoConn ~ 10300 4600
$Comp
L CONN_01X04 P4
U 1 1 58E4C087
P 8650 4500
F 0 "P4" H 8650 4750 50  0000 C CNN
F 1 "A6_7" V 8750 4500 50  0000 C CNN
F 2 "matts_components:SIL-4_Grove_SIL" H 8650 4500 50  0001 C CNN
F 3 "" H 8650 4500 50  0000 C CNN
	1    8650 4500
	1    0    0    -1  
$EndComp
Text GLabel 8150 4550 0    60   Output ~ 0
A7
$Comp
L GND #PWR017
U 1 1 58E4C08E
P 7850 4200
F 0 "#PWR017" H 7850 3950 50  0001 C CNN
F 1 "GND" H 7850 4050 50  0000 C CNN
F 2 "" H 7850 4200 50  0000 C CNN
F 3 "" H 7850 4200 50  0000 C CNN
	1    7850 4200
	1    0    0    -1  
$EndComp
Text GLabel 8250 4450 0    60   Input ~ 0
+5V
Wire Wire Line
	8450 4650 8150 4650
Wire Wire Line
	8250 4450 8450 4450
Wire Wire Line
	8450 4350 8050 4350
Wire Wire Line
	8050 4350 8050 4200
Wire Wire Line
	8050 4200 7850 4200
Text Notes 8100 5000 0    60   ~ 0
A6_7 OUTPUT
Wire Wire Line
	8150 4550 8450 4550
Text GLabel 5950 4250 2    60   Output ~ 0
A6
Text GLabel 5950 4150 2    60   Output ~ 0
A7
Text GLabel 5950 4650 2    60   Output ~ 0
A2
Wire Wire Line
	5800 4150 5950 4150
Wire Wire Line
	5800 4250 5950 4250
Wire Wire Line
	5800 4650 5950 4650
Wire Wire Line
	5800 4550 5950 4550
Text Notes 2650 3700 0    60   ~ 0
For 72V input\n3.3V output\n3.3 = 72 *(Ra/(Ra+Rb))\nRa = 10k\nRb = 220k
Wire Bus Line
	450  2700 4000 2700
Wire Bus Line
	4000 2700 4000 7800
Wire Bus Line
	500  5300 4000 5300
Wire Bus Line
	2200 5300 2200 7800
Wire Bus Line
	7000 2500 7000 6500
Wire Bus Line
	7000 3150 4000 3150
Wire Bus Line
	7000 2500 11250 2500
Wire Bus Line
	7000 3900 11250 3900
Wire Bus Line
	11250 3900 11250 3950
Wire Bus Line
	7000 5150 11250 5150
Wire Bus Line
	9750 6550 9750 5150
Wire Bus Line
	9750 5150 9800 5150
Wire Bus Line
	9200 5150 9200 2500
Wire Bus Line
	4000 6200 7000 6200
Wire Bus Line
	6050 6200 6050 7800
Text Notes 5050 5700 0    60   ~ 0
Arduino Nano chosen
$Comp
L CONN_01X01 P9
U 1 1 58E529B4
P 3150 4550
F 0 "P9" H 3150 4650 50  0000 C CNN
F 1 "TP_Vout" V 3250 4550 50  0000 C CNN
F 2 "matts_components:1pin_0_8mm" H 3150 4550 50  0001 C CNN
F 3 "" H 3150 4550 50  0000 C CNN
F 4 "~" H 3150 4550 60  0000 C CNN "Notes"
F 5 "~" H 3150 4550 60  0001 C CNN "Description"
F 6 "~" H 3150 4550 60  0001 C CNN "Manufacturer"
F 7 "~" H 3150 4550 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 3150 4550 60  0001 C CNN "Supplier 1"
F 9 "~" H 3150 4550 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 3150 4550 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 3150 4550 60  0001 C CNN "Supplier 2"
F 12 "~" H 3150 4550 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 3150 4550 60  0001 C CNN "Supplier 2 Cost"
	1    3150 4550
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X01 P8
U 1 1 58E52AD3
P 1750 3600
F 0 "P8" H 1750 3700 50  0000 C CNN
F 1 "TP_Vin" V 1850 3600 50  0000 C CNN
F 2 "matts_components:1pin_0_8mm" H 1750 3600 50  0001 C CNN
F 3 "" H 1750 3600 50  0000 C CNN
	1    1750 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 4550 2950 3950
Connection ~ 2950 3950
Wire Wire Line
	1550 3600 1550 3900
Connection ~ 1550 3900
NoConn ~ 6300 6500
$Comp
L CONN_1 EN1
U 1 1 58E55463
P 6450 6500
F 0 "EN1" H 6530 6500 40  0000 L CNN
F 1 "PCB" H 6450 6555 30  0001 C CNN
F 2 "" H 6450 6500 60  0001 C CNN
F 3 "http://cpc.farnell.com/hammond/1590bb/box-diecast-119x94x30/dp/EN82042?ost=EN82042&selectedCategoryId=&categoryNameResp=All&searchView=table&iscrfnonsku=false" H 6450 6500 60  0001 C CNN
F 4 "~" H 6450 6500 60  0000 C CNN "Notes"
F 5 "Enclosure for unit" H 6450 6500 60  0001 C CNN "Description"
F 6 "~" H 6450 6500 60  0001 C CNN "Manufacturer"
F 7 "~" H 6450 6500 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 6450 6500 60  0001 C CNN "Supplier 1"
F 9 "~" H 6450 6500 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 6450 6500 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 6450 6500 60  0001 C CNN "Supplier 2"
F 12 "~" H 6450 6500 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 6450 6500 60  0001 C CNN "Supplier 2 Cost"
	1    6450 6500
	1    0    0    -1  
$EndComp
NoConn ~ 6300 6350
$Comp
L CONN_1 MH1
U 1 1 58EBB61E
P 6450 6350
F 0 "MH1" H 6530 6350 40  0000 L CNN
F 1 "Mounting holes" H 6450 6405 30  0001 C CNN
F 2 "" H 6450 6350 60  0001 C CNN
F 3 "" H 6450 6350 60  0001 C CNN
F 4 "~" H 6450 6350 60  0000 C CNN "Notes"
F 5 "4 x M3 mounting holes" H 6450 6350 60  0001 C CNN "Description"
F 6 "~" H 6450 6350 60  0001 C CNN "Manufacturer"
F 7 "~" H 6450 6350 60  0001 C CNN "Manufacturer Part No"
F 8 "~" H 6450 6350 60  0001 C CNN "Supplier 1"
F 9 "~" H 6450 6350 60  0001 C CNN "Supplier 1 Part No"
F 10 "~" H 6450 6350 60  0001 C CNN "Supplier 1 Cost"
F 11 "~" H 6450 6350 60  0001 C CNN "Supplier 2"
F 12 "~" H 6450 6350 60  0001 C CNN "Supplier 2 Part No"
F 13 "~" H 6450 6350 60  0001 C CNN "Supplier 2 Cost"
	1    6450 6350
	1    0    0    -1  
$EndComp
$Comp
L WS2811_EpiStar_8mm D5
U 1 1 58F0DF7B
P 5150 6850
F 0 "D5" H 5150 7075 50  0000 C CNN
F 1 "WS2811_EpiStar_8mm" H 5150 6600 50  0000 C CNN
F 2 "REInnovationFootprint:TH_WS2811_LED_8mm" H 5150 6850 50  0001 C CNN
F 3 "" H 5150 6850 50  0000 C CNN
	1    5150 6850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 6950 5600 6950
Wire Wire Line
	5450 6750 5550 6750
Wire Wire Line
	5550 6750 5550 6500
Wire Wire Line
	5550 6500 5700 6500
$EndSCHEMATC
