/* 
	SerieShuntController.h - Library for the series/shunt controller, its display
	and its rotary encoder
	Created by Arnaud Moulas, Matthew Little and Yoan Gornes, date : 21/04/2017
	Open source project
*/

#ifndef SerieShuntController_h
#define SerieShuntController_h

#include "Arduino.h"

#include "U8glib.h"

#include "EEPROMex.h"
#include "EEPROMVar.h"
#include "Encoder.h"

#include "LCD5110_Graph.h"
#include "avr/sleep.h"
#include "avr/power.h"
#include "avr/wdt.h"
#include "avr/pgmspace.h"

//OLED_Display class

class OLED_Display
{
	public:
		OLED_Display(void); 
		void initOLED(void);    // Initialize the OLED
		void displayOLED(char selection, int sensorValue, float setpointHigh, float setpointLow, float Vin, float VinMax, float maxControllerVoltage, unsigned int times, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog, unsigned char & cpt, unsigned char curveTab[], unsigned char & curveCpt);    // Diplays the informations on the OLED
		void enableMenu(bool enable, float & VinMax, float & maxControllerVoltage, unsigned int & R1, unsigned int & R2, float & hysteresis, unsigned int & times, float setpointHigh, float setpointLow, long & newPos, long & posit);    // Displays the menu if you press the rotary encoder // You can change Vlim, Vmax, R1, R2, hysteresis, times and decide if you the arduino to sleep depending on the position of the encoder
		void curveDisplay(float Vin, float maxControllerVoltage, unsigned int times, unsigned char & cpt, unsigned char curveTab[], unsigned char curveCpt);
		void drawBatteryLimit(float VinMax, float maxControllerVoltage);

};

//Rotary Encoder class

class Encoder_control
{
	public:
		Encoder_control(void); 
		bool testButtonC(void);    // Return LOW if the encoder is pressed and HIGH if not
		char configureSelection(int sensorValue, float setpointHigh, float setpointLow, float maxControllerVoltage, long & newPos, long & posit, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);    // Changes the value of "selection" depending on the position of the encoder

};

//Values manager class

class Value
{
	public:
		Value(void); 
		void updateValue(float & setpointHigh, float & setpointLow, float VinMax, float hysteresis);    // Update setpointHigh and setpointLow depending on VinMax and hysteresis
		void initEEPROMValue (float & VinMax, float & maxControllerVoltage, unsigned int & R1, unsigned int & R2, float & hysteresis, unsigned int & times);    // initialization of VinMax, maxControllerVoltage, R1, R2, hysteresis and times with the last values stored in the EEPROM

};

//Verification class

class Verification
{
	public:
		Verification(void);
		bool overcharge(int sensorValue, int VinMax, float maxControllerVoltage);    // Check for overcharge
		bool setpointHigh(int sensorValue, float setpointHigh, float maxControllerVoltage, int & limitSetpointHighInAnalog);    // Check if sensorValue is higher than setpointHigh
		bool setpointLow(int sensorValue, float setpointLow, float maxControllerVoltage, int & limitSetpointLowInAnalog);    // Check if sensorValue is lower than setpointLow
	
};

//Get class

class Get
{
	public:
		Get(void);
		float vin(int sensorValue, float maxControllerVoltage);    // Get Vin value and store it in the global declaration Vin
		char intensityInPercent(char selection, int sensorValue, float setpointHigh, float setpointLow, float maxControllerVoltage, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);    // Turn sensorValue into a percentage depending on the value of "selection"
	
};

//PWMwrite class

class PWMwrite
{
	public:
		PWMwrite(void);
		void serie(int intensity, int outputPin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);    // Generate the PWM square wave with the right duty cycle
		void shunt(int intensity, int outputPin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);    // Generate the PWM square wave with the right duty cycle
	
};

//Controller class
class Control
{
	public:
		Control(void); 
		void seriesRegulator(int & sensorValue, float maxControllerVoltage, char selection, float setpointHigh, float setpointLow, float & Vin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);    // The function that does the regulation
		void shuntPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);
		void shuntNoPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);
		void mosfet(char selection, float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);    // Selection of the control mode
		void serieNoPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);
		void seriePWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog);
		void enterSleepArduino(void);    //Enters the arduino into sleep mode
	
};

#endif