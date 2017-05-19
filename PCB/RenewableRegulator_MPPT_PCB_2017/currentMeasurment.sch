EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:conn
LIBS:matts_components
LIBS:RenewableRegulator_MPPT_PCB_2017-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
Title "Renewable Regulator"
Date "2017-04-04"
Rev "1"
Comp "RE-Innovation"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L C-RESCUE-RenewableRegulatorPCB_2017 C15
U 1 1 590785B7
P 6500 3950
AR Path="/5907092C/590785B7" Ref="C15"  Part="1" 
AR Path="/59096D23/590785B7" Ref="C17"  Part="1" 
F 0 "C15" H 6550 4050 50  0000 L CNN
F 1 "1nf" H 6250 3800 50  0000 L CNN
F 2 "REInnovationFootprint:cond_1" H 6500 3950 60  0001 C CNN
F 3 "" H 6500 3950 60  0001 C CNN
	1    6500 3950
	1    0    0    -1  
$EndComp
Text HLabel 6500 4400 3    60   Input ~ 0
GND
$Comp
L C-RESCUE-RenewableRegulatorPCB_2017 C16
U 1 1 5907AE5F
P 6850 3000
AR Path="/5907092C/5907AE5F" Ref="C16"  Part="1" 
AR Path="/59096D23/5907AE5F" Ref="C18"  Part="1" 
F 0 "C16" H 6900 3100 50  0000 L CNN
F 1 "0.1µF" H 6600 2850 50  0000 L CNN
F 2 "REInnovationFootprint:cond_1" H 6850 3000 60  0001 C CNN
F 3 "" H 6850 3000 60  0001 C CNN
	1    6850 3000
	1    0    0    -1  
$EndComp
Text HLabel 6850 2600 1    60   Input ~ 0
+5V
Text HLabel 6850 3400 3    60   Input ~ 0
GND
Text HLabel 6250 3200 2    60   Output ~ 0
VmOUT
Text HLabel 4150 2900 0    60   BiDi ~ 0
IN
Text HLabel 4150 4050 0    60   BiDi ~ 0
OUT
$Comp
L ACS712 U5
U 1 1 590725D2
P 5400 3400
AR Path="/5907092C/590725D2" Ref="U5"  Part="1" 
AR Path="/59096D23/590725D2" Ref="U1"  Part="1" 
F 0 "U5" H 5400 4100 98  0000 C CNN
F 1 "ACS712" H 5400 2550 98  0000 C CNN
F 2 "acs712:acs712" H 5400 3400 60  0001 C CNN
F 3 "" H 5400 3400 60  0001 C CNN
	1    5400 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3750 6500 3750
Wire Wire Line
	6100 4150 6500 4150
Wire Wire Line
	6500 4150 6500 4400
Wire Wire Line
	6100 2800 6850 2800
Wire Wire Line
	6850 2800 6850 2600
Wire Wire Line
	6850 3200 6850 3400
Wire Wire Line
	6100 3200 6250 3200
Wire Wire Line
	4600 2800 4500 2800
Wire Wire Line
	4500 2800 4500 3000
Wire Wire Line
	4500 3000 4600 3000
Wire Wire Line
	4150 2900 4500 2900
Connection ~ 4500 2900
Wire Wire Line
	4500 4150 4600 4150
Wire Wire Line
	4600 3950 4500 3950
Wire Wire Line
	4150 4050 4500 4050
Connection ~ 4500 4050
Wire Wire Line
	4500 3950 4500 4150
$EndSCHEMATC
