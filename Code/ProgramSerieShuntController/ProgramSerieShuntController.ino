#include <U8glib.h>

#include <EEPROMex.h>
#include <EEPROMVar.h>
#include <Encoder.h>

#include <LCD5110_Graph.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

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

Encoder rotEncoder(2, 3);                                         // Rotary encoder object declaration

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);    // OLED display declaration

LCD5110 myGLCD(12,11,10,8,9);                                     // LCD display declaration

volatile int f_wdt=1;                                             // Sleep arduino

//long posit = 0;                                                   // This value will be the previous value of the position of the rotary encoder
//long newPos;

const int analogInPinV = A0;                 // Analog input pin that the potentiometer (Voltmeter) is attached to / Default = A0
const int analogInPinA = A1;                 // Analog input pin that the potentiometer (Ammeter) is attached to / Default = A1
const int buttonC = 4;                       // Digital input pin that the button of the rotary encoder is attached to / Default = 4

const int  overchargeIndicatorLed = 5;       // LedOutput  Default = 2

const char serialDisplayVoltage[] PROGMEM  = {"Actual Voltage : "};
bool testFirstSleep = HIGH;
bool testCase5 = LOW;

/*float VinMax;                                // Maximum Input Voltage Default = 4
unsigned int R1;                             // Bottom resistance Kohm Default = 10
unsigned int R2 ;                            // top resistance Kohm     Default = 100
float maxControllerVoltage ;                 // the arduino Uno boardcard cant excess 5 volt   default = 5v
float hysteresis ;                           // Value of the hysteresis
float setpointHigh = (VinMax + hysteresis);  // Set setpointHigh Caution the battery may go beyond the overcharge
float setpointLow = (VinMax - hysteresis) ;  // Set setpointLow
unsigned int times;                          // Defines the time between two points when drawing the curve
char selection = 0;                          // Choose true for PWM, false for no PWM*/

//float valuesArray[8]; // 0: Vinmax; 1: R1; 2: R2; 3: maxControllerVoltage; 4: hysteresis; 5: setPointHigh; 6: setPointLow; 7: times
//int limitOverchargeInAnalog;                 // Limit of the overcharge in analog value
//float limitOverchargeInVolt;                 // Limit of the overcharge in voltage
//float limitSetpointHighInVolt;               // Limit of the setpointhigh in voltage
//float limitSetpointLowInVolt;                // Limit of the setpointLow in voltage
//bool verification;                           // False = Overcharged / True = not overcharged
//float Vin;                                   // Actual Value of the voltage Input
//int intensityInPercent;
//char state = 0;
/* Value read from the potentiometers */
//int sensorValue = 0;       
//int ampvalue = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////SETUP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
   
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);
                                           
  pinMode(2, INPUT_PULLUP);                 // Set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(3, INPUT_PULLUP);                 // Set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  //attachInterrupt(0,2,RISING);            // Set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  //attachInterrupt(1,3,RISING);            // Set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  pinMode(overchargeIndicatorLed, OUTPUT);  // Set the LED indicator in OUTPUT mode
  pinMode(13, OUTPUT);                      // Set the pin 13 as an output
  digitalWrite(13,HIGH);                    // Set the pin 13 to HIGH (that turns on the display's blue light so we can see something)
  pinMode(buttonC, INPUT_PULLUP);           // Initialize the encoder button
  
  //initEEPROMValue(VinMax, maxControllerVoltage, R1, R2, hysteresis, times);                        // initialization of VinMax, maxControllerVoltage, R1, R2, hysteresis and times with the last values stored in the EEPROM
  initOLED();                               // Setup the WDT
  
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
  static float curveTab[120] ;
  
  if (first_time == 1)
  {
    initEEPROMValue(VinMax, maxControllerVoltage, R1, R2, hysteresis, times);                        // initialization of VinMax, maxControllerVoltage, R1, R2, hysteresis and times with the last values stored in the EEPROM
    updateValue(setpointHigh, setpointLow, VinMax, hysteresis);
    for (i = 0; i<120; i++)
    {
      curveTab[i]= 0;
    }
    first_time = 0;  
  }
  
  if(f_wdt == 1)
  {
//    ampvalue = analogRead(analogInPinA);
//    Serial.println("current:");
//    Serial.println(ampvalue);
    selection = configureSelection(sensorValue, setpointHigh, setpointLow, maxControllerVoltage, newPos, posit, limitSetpointHighInAnalog, limitSetpointLowInAnalog);           // Changes the value of "selection" depending on the position of the encoder
    seriesRegulator(sensorValue, maxControllerVoltage, selection, setpointHigh, setpointLow, Vin, limitSetpointHighInAnalog, limitSetpointLowInAnalog);              // Write 0 in the parenthesis for no PWM or 1 for the PWM //default selection
    displayOLED(selection, sensorValue, setpointHigh, setpointLow, Vin, VinMax, maxControllerVoltage, times, limitSetpointHighInAnalog, limitSetpointLowInAnalog, cpt, curveTab, curveCpt);         // Default selection
    displaySerialInformation(Vin);     // Read the global declaration in order to display information on the serial monitor
    
    enableMenu(testButtonC(), VinMax, maxControllerVoltage, R1, R2, hysteresis, times, setpointHigh, setpointLow, newPos, posit);      // Displays the menu if you press the rotary encoder // You can change Vlim, Vmax, R1, R2, hysteresis, times and decide if you the arduino to sleep depending on the position of the encoder
    f_wdt = 0;

    enterSleepArduino();            // Re-enter sleep mode
  }
  else
  {
    /* Do nothing. */
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////FUNCTION/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void displaySerialInformation(float Vin)                                                              // Read the global declaration in order to display information on the serial monitor // 0 for all value
{        
  int k;  
  char myChar;                                                     
  int len = strlen_P(serialDisplayVoltage);
  for (k = 0; k < len; k++)
  {
    myChar =  pgm_read_byte_near(serialDisplayVoltage + k);
    Serial.print(myChar);
  }
  Serial.println(Vin);
}

float getVin(int sensorValue, float maxControllerVoltage)                                                                // Get Vin value and store it in the global declaration Vin
{
  float Vin;
  Vin = ((sensorValue) * maxControllerVoltage / 1024);                                      
  return (Vin);
}

bool verificationOvercharge(int sensorValue, int VinMax, float maxControllerVoltage)                                     // Check for overcharge
{
  int limitOverchargeInAnalog;
  float limitOverchargeInVolt;
  limitOverchargeInVolt = VinMax;                                                            // * (((R1 + R2)*1000) / (R1*1000)); // PotentialDiviser R1 = bottom // R2 = top // VinMax= Maximum Input Voltage
  limitOverchargeInAnalog = 1024 * ( limitOverchargeInVolt / maxControllerVoltage) ; 
  
  if (sensorValue >= limitOverchargeInAnalog)
  {
    return (true);
  }
  else
  {
    return (false);
  }
  
}

bool verificationSetpointHigh(int sensorValue, float setpointHigh, float maxControllerVoltage, int & limitSetpointHighInAnalog)                           // Check if sensorValue is higher than setpointHigh
{
  float limitSetpointHighInVolt;
  limitSetpointHighInVolt = setpointHigh ;
  limitSetpointHighInAnalog = 1024 * ( limitSetpointHighInVolt / maxControllerVoltage) ;
  
  if (sensorValue >= limitSetpointHighInAnalog)
  {
    return (true);
  }
  else
  {
    return (false);
  }
}

bool verificationSetpointLow(int sensorValue, float setpointLow, float maxControllerVoltage, int & limitSetpointLowInAnalog)                             // Check if sensorValue is lower than setpointLow
{
  float limitSetpointLowInVolt;
  limitSetpointLowInVolt = setpointLow; 
  limitSetpointLowInAnalog = 1024 * ( limitSetpointLowInVolt / maxControllerVoltage) ;
  
  if (sensorValue <= limitSetpointLowInAnalog)
  {
    return (true);
  }
  else 
  {
    return (false);
  }
}

void seriesRegulator(int & sensorValue, float maxControllerVoltage, char selection, float setpointHigh, float setpointLow, float & Vin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                            // The function that does the regulation
{
  char i;
  for (i=0; i<11; i++)
  {
    delay(50);
    sensorValue = analogRead(analogInPinV);
    Vin = getVin(sensorValue, maxControllerVoltage);   
    controlMosfet(selection, maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog);
    //CheckButtonA();
  } 
}

void PWMwrite(int intensity, int outputPin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)       // Generate the PWM square wave with the right duty cycle
{  
  int mappedIntensity;
  mappedIntensity = map(intensity,limitSetpointLowInAnalog,limitSetpointHighInAnalog, 0, 255);
  analogWrite(outputPin, mappedIntensity);
}

void controlShuntPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                        
{
  if(verificationSetpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                                      // Turn on the led // Overcharged
  }
  else if (verificationSetpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1) //Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, LOW);                                       // Turn off the led // Not overcharged
  }
  else
  {
    PWMwrite(sensorValue, overchargeIndicatorLed, limitSetpointHighInAnalog, limitSetpointLowInAnalog);                                   // Hysteresis
  }
}

void controlShuntNoPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)
{
  if(verificationSetpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                                      // Turn on the led // Overcharged
  }
  else if (verificationSetpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1) //Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, LOW);                                       // Turn off the led // Not overcharged
  }
  else
  {
                                                                                     // Hysteresis
  }
}

void controlMosfet(char selection, float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                                            // Selection of the control mode
{
  if (selection == 0)
  {
    controlSerieNoPWM(maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog); 
  }
  else if (selection == 1)
  {
    controlSeriePWM(maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog); 
  }
  else if (selection == 2)
  {
    controlShuntNoPWM(maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog); 
  }
  else if (selection == 3)
  {
    controlShuntPWM(maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog); 
  }
}

char getIntensityInPercent(char selection, int sensorValue, float setpointHigh, float setpointLow, float maxControllerVoltage, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                                    // Turn sensorValue into a percentage depending on the value of "selection"
{  
  int intensityInPercent;
  if (selection == 2)
  {
    if (verificationSetpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog) == 1)
    {
      intensityInPercent = 100; 
    }
    else if (verificationSetpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog) == 1)
    {
      intensityInPercent = 0;
    }
  }
  else if (selection == 3)
  {
    intensityInPercent = map(sensorValue,limitSetpointLowInAnalog,limitSetpointHighInAnalog, 0, 100);
    if(intensityInPercent >= 100)
    {
      intensityInPercent = 100;
    }
    else if ( intensityInPercent <= 0)
    {
      intensityInPercent = 0;
    }
  }
  else if (selection == 0)
  {
    if (verificationSetpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)
    {
      intensityInPercent = 0;
    }
    else if (verificationSetpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1)
    {
      intensityInPercent = 100;
    }
  }
  else if (selection == 1)
  {
    intensityInPercent = map(sensorValue,limitSetpointHighInAnalog,limitSetpointLowInAnalog, 0, 100);
    if(intensityInPercent >= 100)
    {
      intensityInPercent = 100;
    }
    else if (intensityInPercent <= 0)
    {
      intensityInPercent = 0;
    }
  }
  return (intensityInPercent);
}


void initOLED()                         // Initialize the OLED
{  
  u8g.begin();
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  u8g.firstPage(); 
  u8g.setFont(u8g_font_unifont);
do{
  u8g.drawStr(8, 10 ,"SerieRegulator");
  u8g.drawStr(12 , 30,"RE-INNOVATION");
  u8g.drawStr(20 , 50,"(C)2016 by");
  u8g.drawStr(8 , 64,"Matthew Little");
} while(u8g.nextPage());
  delay(500);
  
}

void displayOLED(char selection, int sensorValue, float setpointHigh, float setpointLow, float Vin, float VinMax, float maxControllerVoltage, unsigned int times, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog, unsigned char & cpt, float curveTab[], unsigned char & curveCpt)                   // Diplays the informations on the OLED
{ 
  char percent;
  float mappedval;
  
  u8g.firstPage();
  u8g.setFont(u8g_font_unifont);

  do{
  u8g.drawStr(0, 10 , "Voltage");
  if(selection == 0)
  {
    u8g.drawStr(70, 10, "SER: ");
  }
  else if (selection == 1)
  {
    u8g.drawStr(70, 10, "SER: ");
    u8g.drawStr(100, 10,"PWM");
  }
  else if (selection == 2)
  {
    u8g.drawStr(70, 10,"SHU: ");
  }
  else if (selection == 3)
  {
    u8g.drawStr(70, 10,"SHU: ");
    u8g.drawStr(100, 10,"PWM");
  }
  
  if(Vin >= 9.95)
  {
    u8g.setPrintPos(0, 30);
    u8g.print(Vin, 1);
  }
  else
  {
    u8g.setPrintPos(8, 30);
    u8g.print(Vin, 1);
  }
  
  u8g.setPrintPos(35, 30);
  u8g.print('v');

  percent = getIntensityInPercent(selection, sensorValue, setpointHigh, setpointLow, maxControllerVoltage, limitSetpointHighInAnalog, limitSetpointLowInAnalog);
  if ((float)percent == 100)
  {
    u8g.setPrintPos(92, 30);
    u8g.print((float)percent,0);
  }
  else if ((float)percent <= 9)
  {
    u8g.setPrintPos(108, 30);
    u8g.print((float)percent,0);
  }
  else
  {
    u8g.setPrintPos(100, 30);
    u8g.print((float)percent,0);
  }
  u8g.setPrintPos(120, 30);
  u8g.print('%');
  
  u8g.drawHLine(5, 60, 120);
  u8g.drawLine(5, 60, 5, 35);
  
  drawBatteryLimit(VinMax, maxControllerVoltage);
  curveDisplay(Vin, maxControllerVoltage, times, cpt, curveTab, curveCpt); 
  }while(u8g.nextPage());
  if (curveCpt < 120)
  {
    curveCpt++;
  }
  else
  {
    curveCpt = 0;
  }
  delay(500);
}

bool testButtonC()           // Return LOW if the encoder is pressed and HIGH if not
{ 
  bool newValue;
  
  newValue = digitalRead(buttonC);
  if(newValue != HIGH)
  {
    return (LOW);
  }
  else
  {
    return (HIGH);
  }
}

void enableMenu(bool enable, float & VinMax, float & maxControllerVoltage, unsigned int & R1, unsigned int & R2, float & hysteresis, unsigned int & times, float setpointHigh, float setpointLow, long & newPos, long & posit) // Displays the menu if you press the rotary encoder // You can change Vlim, Vmax, R1, R2, hysteresis, times and decide if you the arduino to sleep depending on the position of the encoder
{
  char y;
  char x;
  char choix = 0;
  char state = 0;

  if(testCase5 == HIGH)
  {
    testCase5 = LOW;
    
    Serial.println("I AM HERE");
     if (testFirstSleep== LOW)
  {
    myGLCD.disableSleep();
    digitalWrite(13, HIGH);
    testFirstSleep=HIGH;
    delay(1000);
  }
  }
  else
  {
  if(enable == LOW)
  {
    posit = 0;
    do{      
      newPos = rotEncoder.read() - posit;
      
      if((newPos >= -2) && (newPos < 2))
      {
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("VLIM");
        u8g.setPrintPos(10, 25);
        u8g.print(" VLIM =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)VinMax,1);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO SELECT");    
        }while (u8g.nextPage());
        state= 0x00;
        choix = 1 ;
        //posit = 0;        
      }
      if(((newPos >= 2) && (newPos < 6)) || ((newPos < -22) && (newPos >= -26)))
      {
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("VMAX");
        u8g.setPrintPos(10, 25);
        u8g.print(" VMAX =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)maxControllerVoltage,1);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO SELECT");    
        }while (u8g.nextPage());
        state= 0x01;
        choix = 1 ;
        //posit = 0;        
      }
      if(((newPos >= 6) && (newPos < 10)) || ((newPos < -18) && (newPos >= -22)))
      {
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("R1");
        u8g.setPrintPos(10, 25);
        u8g.print("   R1 =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)R1,0);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO SELECT");    
        }while (u8g.nextPage());
        state= 0x02;
        choix = 1 ;
        //posit = 0;
      }
      if(((newPos >= 10) && (newPos < 14)) || ((newPos < -14) && (newPos >= -18)))
      {
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("R2");
        u8g.setPrintPos(10, 25);
        u8g.print("   R2 =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)R2,0);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO SELECT");    
        }while (u8g.nextPage());
        state= 0x03;
        choix = 1 ;
        //posit = 0;
      }
      if(((newPos >= 14) && (newPos < 18)) || ((newPos < -10) && (newPos >= -14)))
      {
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("HYST");
        u8g.setPrintPos(10, 25);
        u8g.print(" HYST =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)hysteresis,1);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO SELECT");    
        }while (u8g.nextPage());
        state= 0x04;
        choix = 1 ;
        //posit = 0;        
      }
      
      if(((newPos >= 18) && (newPos < 22)) || ((newPos < -6) && (newPos >= -10)))
      {
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("CURVE");
        u8g.setPrintPos(10, 25);
        u8g.print("TIMES =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)times,0);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO SELECT");    
        }while (u8g.nextPage());
        state= 0x06;  
        choix = 1 ; 
        //posit = 0;
      }
      
      if(((newPos >= 22) && (newPos < 26)) || ((newPos < -2) && (newPos >= -6)))
      {
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(10, 25);
        u8g.print("GO TO SLEEP?");
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO SELECT");    
        }while (u8g.nextPage());
        state= 0x05; 
        choix = 1 ;
        //posit = newPos;
        //posit = 0;
      }
      
      if(( newPos >= 26) || (newPos < -26))
      {
        posit = rotEncoder.read();       
      }
      delay(200);
      
    }while (HIGH==digitalRead(buttonC));
    do 
    { 
      if(choix ==1){
      switch (state)
      {
        case 0x00:
        {
          VinMax = EEPROM.readFloat(0);
          u8g.firstPage();
          u8g.setFont(u8g_font_unifont);
          do{
          u8g.setPrintPos(10, 10);
          u8g.print("CHOOSE :");
          u8g.setPrintPos(80, 10);
          u8g.print("VLIM");
          u8g.setPrintPos(10, 25);
          u8g.print(" VLIM =");
          u8g.setPrintPos(80, 25);
          u8g.print((float)VinMax,1);
          u8g.setPrintPos(10, 40);
          u8g.print("PRESS BUTTON");
          u8g.setPrintPos(10, 55);
          u8g.print("TO CHANGE");
          }while(u8g.nextPage());
          do
          {
            newPos = rotEncoder.read();
            posit = newPos + 400;
            delay(200);
            newPos = rotEncoder.read() + 400;
            
            if(posit > newPos)
            {
              VinMax = VinMax +(posit - newPos) * 0.05;
              if(VinMax <= 0)
              {
                VinMax = 0;
              }
            }
            else if (posit < newPos)
            {
              
              VinMax = VinMax-(newPos - posit) * 0.05;
            }
            else
            {
              Serial.println("");
              Serial.println(posit);
              Serial.println(newPos);
              //Serial.println(encoderPos);  //nothing
            }
            u8g.firstPage();
            u8g.setFont(u8g_font_unifont);
            do{
            u8g.setPrintPos(10, 25);
            u8g.print(" VLIM =");
            u8g.setPrintPos(80, 25);
            u8g.print((float)VinMax,1);
            u8g.setPrintPos(10, 40);
            u8g.print("PRESS TO ACCEPT"); 
            
            }while(u8g.nextPage());
          
        }while(HIGH==digitalRead(buttonC));
        choix = 0;
        EEPROM.writeFloat(0, VinMax);
      }
      break;
      
      case 0x01:
      {
        maxControllerVoltage = EEPROM.readFloat(4);
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("VMAX");
        u8g.setPrintPos(10, 25);
        u8g.print(" VMAX =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)maxControllerVoltage,1);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO CHANGE");
         }while(u8g.nextPage());
        do{
          newPos = rotEncoder.read();
          posit = newPos + 400;
          delay(200);
          newPos = rotEncoder.read() + 400;
            
          if(posit > newPos)
          {
            maxControllerVoltage = maxControllerVoltage +(posit - newPos) * 0.05;
            if(maxControllerVoltage <= 0)
            {
              maxControllerVoltage = 0;
            }
          }
          else if (posit < newPos)
          {
            maxControllerVoltage = maxControllerVoltage-(newPos - posit) * 0.05;
          }
          else
          {
            //nothing
          }
          u8g.firstPage();
          u8g.setFont(u8g_font_unifont);
          do{
          u8g.setPrintPos(10, 25);
          u8g.print(" VMAX =");
          u8g.setPrintPos(80, 25);
          u8g.print((float)maxControllerVoltage,1);
          u8g.setPrintPos(10, 40);
          u8g.print("PRESS TO ACCEPT");
          }while(u8g.nextPage());
        }while(HIGH==digitalRead(buttonC));
        choix = 0;
        EEPROM.writeFloat(4, maxControllerVoltage);
      }
      break;
      
      case 0x02:
      {
        R1 = EEPROM.read(8);
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("R1");
        u8g.setPrintPos(10, 25);
        u8g.print("   R1 =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)R1,1);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO CHANGE");
        }while(u8g.nextPage());
        do{
          newPos = rotEncoder.read();
          posit = newPos + 400;
          delay(200);
          newPos = rotEncoder.read() + 400;
          
          if(posit > newPos)
          {
            R1 = R1 + (posit - newPos) * 0.25;
            if(R1 <= 0)
            {
              R1 = 0;
            }
          }
          else if (posit < newPos)
          {
            R1 = R1 - (newPos - posit) * 0.25;
          }
          else
          {
            //nothing
          }
          u8g.firstPage();
          u8g.setFont(u8g_font_unifont);
          do{
          u8g.setPrintPos(10, 25);
          u8g.print("   R1 =");
          u8g.setPrintPos(80, 25);
          u8g.print((float)R1,0);
          u8g.setPrintPos(10, 40);
          u8g.print("PRESS TO ACCEPT");
          }while(u8g.nextPage());
        }while(HIGH==digitalRead(buttonC));
        choix = 0;
        EEPROM.write(8,R1);
      }
      break;
      
      case 0x03:
      {
        R2 = EEPROM.read(12);
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("R2");
        u8g.setPrintPos(10, 25);
        u8g.print("   R2 =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)R2,1);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO CHANGE");
        }while(u8g.nextPage());
        do{
          
          newPos = rotEncoder.read();
          posit = newPos + 400;
          delay(200);
          newPos = rotEncoder.read() + 400;
          
          if(posit > newPos)
          {
            R2 = R2 + (posit - newPos) * 0.25;
            if(R2 <= 0)
            {
              R2 = 0;
            }
          }
          else if (posit < newPos)
          {
            R2 = R2 - (newPos - posit) * 0.25;
          }
          else
          {
            //nothing
          }
          u8g.firstPage();
          u8g.setFont(u8g_font_unifont);
          do{
          u8g.setPrintPos(10, 25);
          u8g.print("   R2 =");
          u8g.setPrintPos(80, 25);
          u8g.print((float)R2,0);
          u8g.setPrintPos(10, 40);
          u8g.print("PRESS TO ACCEPT");
         }while(u8g.nextPage());
          
        }while(HIGH==digitalRead(buttonC));
        choix = 0;
        EEPROM.write(12,R2);
      }
      break;
      
      case 0x04:
      {
        hysteresis=EEPROM.readFloat(16);
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("HYST");
        u8g.setPrintPos(10, 25);
        u8g.print(" HYST =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)hysteresis,1);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO CHANGE");
        }while(u8g.nextPage());
        do{
          
          newPos = rotEncoder.read();
          posit = newPos + 400;
          delay(200);
          newPos = rotEncoder.read() + 400;
          
          if(posit > newPos)
          {
            hysteresis = hysteresis + (posit - newPos) * 0.05;
            if(hysteresis <= 0)
            {
              hysteresis = 0;
            }
          }
          else if (posit < newPos)
          {
            hysteresis = hysteresis - (newPos - posit) * 0.05;
          }
          else
          {
            //nothing
          }
          u8g.firstPage();
          u8g.setFont(u8g_font_unifont);
          do{
          u8g.setPrintPos(10, 25);
          u8g.print(" HYST =");
          u8g.setPrintPos(80, 25);
          u8g.print((float)hysteresis,1);
          u8g.setPrintPos(10, 40);
          u8g.print("PRESS TO ACCEPT");
         }while(u8g.nextPage());
          
        }while(HIGH==digitalRead(buttonC));
        choix = 0;
        EEPROM.writeFloat(16, hysteresis);
      }
      break;
      
      case 0x05:
      { 
        u8g.sleepOff();
        digitalWrite(13,HIGH);
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 25);
        u8g.print("GOING TO");
        u8g.setPrintPos(10, 40);
        u8g.print("SLEEP");
        u8g.setPrintPos(10, 55);
        u8g.print("zzZZZZzzZZz");
        }while(u8g.nextPage());
        delay(1500);
        testFirstSleep = LOW;
        u8g.sleepOn();
        digitalWrite(13,LOW);
        testCase5 = HIGH;
        choix = 0;
      }
      break;
      
      case 0x06:
      {
        times=EEPROM.readInt(20);
        u8g.firstPage();
        u8g.setFont(u8g_font_unifont);
        do{
        u8g.setPrintPos(10, 10);
        u8g.print("CHOOSE :");
        u8g.setPrintPos(80, 10);
        u8g.print("SCALE");
        u8g.setPrintPos(10, 25);
        u8g.print("TIMES =");
        u8g.setPrintPos(80, 25);
        u8g.print((float)times,0);
        u8g.setPrintPos(10, 40);
        u8g.print("PRESS BUTTON");
        u8g.setPrintPos(10, 55);
        u8g.print("TO CHANGE");
        }while(u8g.nextPage());
        do{
          newPos = rotEncoder.read();
          posit = newPos + 400;
          delay(200);
          newPos = rotEncoder.read() + 400;
          
          if(posit > newPos)
          {
            times = times + (posit - newPos)*0.25 ;
            if(times <= 0)
            {
              times = 0;
            }
            
          }
          else if (posit < newPos)
          {
            times = times - (newPos - posit)*0.25;
          }
          else
          {
            //nothing
          }
          u8g.firstPage();
          u8g.setFont(u8g_font_unifont);
          do{
          u8g.setPrintPos(10, 25);
          u8g.print("TIMES =");
          u8g.setPrintPos(80, 25);
          u8g.print((float)times,1);
          u8g.setPrintPos(10, 40);
          u8g.print("PRESS TO ACCEPT");
         }while(u8g.nextPage());
          
        }while(digitalRead(buttonC)==HIGH);
        choix = 0;
        EEPROM.writeInt(20, times);
      }
      break;
      default: 
      //nothing
      break;
    }
    }
    delay(1000);
  }while(digitalRead(buttonC) == HIGH && testFirstSleep == HIGH);

  u8g.firstPage();
  u8g.setFont(u8g_font_unifont);
  do{
  u8g.setPrintPos(63, 25);
  u8g.print("OK");
  u8g.setPrintPos(10, 40);
  u8g.print("CHARGING VALUE");
  }while (u8g.nextPage());
  choix = 0;
  delay(1500);
}
updateValue(setpointHigh, setpointLow, VinMax, hysteresis);
}
}

char configureSelection(int sensorValue, float setpointHigh, float setpointLow, float maxControllerVoltage, long & newPos, long & posit, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                             // Changes the value of "selection" depending on the position of the encoder
{
  char selection;
  int intensityInPercent;
  
  newPos = rotEncoder.read() - posit;

  Serial.println(newPos);
  if(( newPos >= -2) & (newPos < 2))
  {
    selection = 1;
    return(selection);
  }
  else if(((newPos >= 2) && (newPos < 6)) || ((newPos < -10) && (newPos >= -14)))
  {
    selection = 0;
    intensityInPercent = getIntensityInPercent(selection,sensorValue,setpointHigh,setpointLow, maxControllerVoltage, limitSetpointHighInAnalog, limitSetpointLowInAnalog);
    if(intensityInPercent != 100)
    {
      digitalWrite(overchargeIndicatorLed, LOW);
      intensityInPercent =0;
    }
    return(selection);
  }
  else if(((newPos >= 6) && (newPos < 10)) || ((newPos < -6) && (newPos >= -10)))
  {
    selection = 2;
    return(selection);
  }
  else if(((newPos >= 10) && (newPos < 14)) || ((newPos < -2) && (newPos >= -6)))
  {
    selection = 3;
    return(selection);
  }
  else
  {
    posit = rotEncoder.read();
    Serial.print("oui");
  }
  
}

void enterSleepArduino(void)           //Enters the arduino into sleep mode
{
  set_sleep_mode(SLEEP_MODE_IDLE);  
  sleep_enable();
  
  sleep_mode();

  sleep_disable(); 

  power_all_enable();
}


void controlSerieNoPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)
{
  if(verificationSetpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, LOW);                  // Turn on the led // Overcharged
  }
  else if (verificationSetpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1) // Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                 // Turn off the led // Not overcharged
  }
  else
  {
                                                                // Hysteresis
  }
}

void controlSeriePWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)
{
  if(verificationSetpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, LOW);                  // TURN on the led // Overcharged
  }
  else if (verificationSetpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1) //Voltage lower than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                 // Light off the led // Not overcharged
  }
  else
  {
    PWMwriteShunt(sensorValue, overchargeIndicatorLed, limitSetpointHighInAnalog, limitSetpointLowInAnalog);         // Hysteresis
  }
}

void PWMwriteShunt(int intensity, int outputPin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog) //Generate the PWM square wave with the right duty cycle
{ 
  int mappedIntensity;
  mappedIntensity = map(intensity,limitSetpointHighInAnalog,limitSetpointLowInAnalog, 0, 255);
  analogWrite(outputPin, mappedIntensity);
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

void updateValue(float & setpointHigh, float & setpointLow, float VinMax, float hysteresis)                    // Update setpointHigh and setpointLow depending on VinMax and hysteresis
{
  setpointHigh = (VinMax + hysteresis);
  setpointLow = (VinMax - hysteresis) ;
}

void initEEPROMValue (float & VinMax, float & maxControllerVoltage, unsigned int & R1, unsigned int & R2, float & hysteresis, unsigned int & times)               // initialization of VinMax, maxControllerVoltage, R1, R2, hysteresis and times with the last values stored in the EEPROM
{
  VinMax = EEPROM.readFloat(0);
  maxControllerVoltage = EEPROM.readFloat(4);
  R1 = EEPROM.read(8);
  R2 = EEPROM.read(12);
  hysteresis = EEPROM.readFloat(16);
  times = EEPROM.readInt(20);
}

void drawBatteryLimit(float VinMax, float maxControllerVoltage)
{ 
  char i;
  float mappedVinMax;
  mappedVinMax=map(VinMax,0,maxControllerVoltage, 0, 25);
  u8g.drawHLine(5, 60 - mappedVinMax, 120);
  u8g.setColorIndex(0);
  for (i = 5 ; i < 124; i = i +2)
  {
    u8g.drawPixel(i , 60 - mappedVinMax );
  }
  u8g.setColorIndex(255);
}

void curveDisplay(float Vin, float maxControllerVoltage, unsigned int times, unsigned char & cpt, float curveTab[], unsigned char curveCpt)                   // Draw the curve
{
  char i;
  float mappedValueVin;
  if (cpt >= times)
  {
    cpt = 0;
    mappedValueVin=map(Vin,0,maxControllerVoltage, 0, 25);
    curveTab[curveCpt] = mappedValueVin;
  }
  else
  {
    cpt++;  
  }
  if (curveCpt < 120)
  {
    for (i = 0; i<curveCpt; i++)
    {
      u8g.setColorIndex(1);
      u8g.drawPixel(5 + i, 60 - curveTab[i]);  
      u8g.setColorIndex(255); 
    }   
  }
  else
  {
    for (i = 0; i<120; i++)
    {
      curveTab[i]= 0;
    }
  }
  
}


