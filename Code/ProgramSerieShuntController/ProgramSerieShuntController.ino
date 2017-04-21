/*#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif*/

/* Encoder Library, for measuring quadrature encoded signals
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 * Copyright (c) 2011,2013 PJRC.COM, LLC - Paul Stoffregen <paul@pjrc.com>
 *
 * Version 1.2 - fix -2 bug in C-only code
 * Version 1.1 - expand to support boards with up to 60 interrupts
 * Version 1.0 - initial release
 */

/*******Watchdog timer by http://donalmorrissey.blogspot.co.uk/2010/04/sleeping-arduino-part-5-wake-up-via.html *********/

/*******LCD5110_Library to control the lcd http://www.rinkydinkelectronics.com/library.php?id=48 *******/

/*******U8glib.h C++ Interface Universal 8bit Graphics Library Copyright (c) 2011, olikraus@gmail.com***/

/*******DECLARATIONS*******/
/*#define LED_PIN            7

Adafruit_NeoPixel LED_RGB = Adafruit_NeoPixel(1, LED_PIN, NEO_GRB + NEO_KHZ800);*/

#include <SerieShuntController.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

OLED_Display disp;
Encoder_control enco;
Value val;
Control contr;


volatile int f_wdt=1;                                             // Sleep arduino

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////SETUP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
   
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);
          
  disp.initOLED();                               // Setup the WDT
  
  MCUSR &= ~(1<<WDRF);                      // Clear the reset flag  
  WDTCSR |= (1<<WDCE) | (1<<WDE);           // In order to change WDE or the prescaler, we need to set WDCE (This will allow updates for 4 clock cycles).
  WDTCSR = 1<<WDP0 | 1<<WDP1;               // Set new watchdog timeout prescaler value (0.125 seconds)
  WDTCSR |= _BV(WDIE);                      // Enable the WD interrupt (note no reset)
  

  delay(100);                               //Allow for serial print to complete.
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////LOOP////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  // put your main code here, to run repeatedly:
  char i = 0; 
  static float setpointHigh;                          // Set setpointHigh Caution the battery may go beyond the overcharge
  static float setpointLow;                           // Set setpointLow
  static float VinMax;                                // Maximum Input Voltage Default = 4
  static unsigned int R1;                             // Bottom resistance Kohm Default = 10
  static unsigned int R2 ;                            // top resistance Kohm     Default = 100 
  static float maxControllerVoltage ;                 // the arduino Uno boardcard cant excess 5 volt   default = 5v
  static float hysteresis ;                           // Value of the hysteresis
  static unsigned int times;    
  static char first_time = 1;
  static char selection = 0;
  static float Vin;
  static int sensorValue = 0; 
  static long posit = 0;
  static long newPos = 0;
  static int limitSetpointHighInAnalog;               // Limit of the setpointhigh in analog value
  static int limitSetpointLowInAnalog;                // Limit of the setpointlow in analog value
  static unsigned char cpt =0;
  static unsigned char curveCpt = 0;
  static unsigned char curveTab[120] ;
  
  if (first_time == 1)
  {
    val.initEEPROMValue(VinMax, maxControllerVoltage, R1, R2, hysteresis, times);                        // initialization of VinMax, maxControllerVoltage, R1, R2, hysteresis and times with the last values stored in the EEPROM
    val.updateValue(setpointHigh, setpointLow, VinMax, hysteresis);
    for (i = 0; i<120; i++)
    {
      curveTab[i]= 0;
    }
    first_time = 0;  
  }
  
  if(f_wdt == 1)
  {
    selection = enco.configureSelection(sensorValue, setpointHigh, setpointLow, maxControllerVoltage, newPos, posit, limitSetpointHighInAnalog, limitSetpointLowInAnalog);           // Changes the value of "selection" depending on the position of the encoder
    contr.seriesRegulator(sensorValue, maxControllerVoltage, selection, setpointHigh, setpointLow, Vin, limitSetpointHighInAnalog, limitSetpointLowInAnalog);              // Write 0 in the parenthesis for no PWM or 1 for the PWM //default selection
    disp.displayOLED(selection, sensorValue, setpointHigh, setpointLow, Vin, VinMax, maxControllerVoltage, times, limitSetpointHighInAnalog, limitSetpointLowInAnalog, cpt, curveTab, curveCpt);         // Default selection
    disp.enableMenu(enco.testButtonC(), VinMax, maxControllerVoltage, R1, R2, hysteresis, times, setpointHigh, setpointLow, newPos, posit);      // Displays the menu if you press the rotary encoder // You can change Vlim, Vmax, R1, R2, hysteresis, times and decide if you the arduino to sleep depending on the position of the encoder
    f_wdt = 0;

    contr.enterSleepArduino();            // Re-enter sleep mode
  }
  else
  {
    /* Do nothing. */
  }
}

ISR(WDT_vect)                             //Watchdog Interrupt Service. This is executed when watchdog timed out
{
  if(f_wdt == 0)
  {
    f_wdt=1; 
  }
  else
  {
             //Serial.println("WDT Overrun!!!");
  }
}

