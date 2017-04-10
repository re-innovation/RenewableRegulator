#include <U8glib.h>

#include <EEPROMex.h>
#include <EEPROMVar.h>
#include <Encoder.h>

#include <LCD5110_Graph.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

/*******Interrupt-based Rotary Encoder Sketch*******
by Simon Merrett, based on insight from Oleg Mazurov, Nick Gammon, rt, Steve Spence
http://www.instructables.com/files/orig/FSD/13QB/IM3QGLHR/FSD13QBIM3QGLHR.ino */

/*******Watchdog timer by http://donalmorrissey.blogspot.co.uk/2010/04/sleeping-arduino-part-5-wake-up-via.html *********/

/*******LCD5110_Library to control the lcd http://www.rinkydinkelectronics.com/library.php?id=48 *******/

Encoder rotEncoder(2, 3);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

//static int pinA = 2;                  // Our first hardware interrupt pin is digital pin 2
//static int pinB = 3;                  // Our second hardware interrupt pin is digital pin 3
//volatile byte aFlag = 0;              // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
//volatile byte bFlag = 0;              // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
//volatile byte encoderPos = 0;         // This variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
//volatile byte oldEncPos = 0;          // Stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0;            // Somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
LCD5110 myGLCD(12,11,10,8,9);  //
extern unsigned char SmallFont[];
extern unsigned char TinyFont[];
extern unsigned char MediumNumbers[];
volatile int f_wdt=1;                 // Sleep arduino

long posit = 0;

// pins for the LEDs:

const int analogInPinV = A0;            // Analog input pin that the potentiometer is attached to / Default = A5
const int analogInPinA = A1; 
//const int buttonA = 2;                 // User Buttons from the rotary encoder
//const int buttonB = 3;
const int buttonC = 4;

char selection = 0;                    // Choose true for PWM, false for no PWM
const int  overchargeIndicatorLed = 5; // LedOutput  Default = 2


/* Global Declaration */

float VinMax;                                // Maximum Input Voltage Default = 4
unsigned int R1;                             // Bottom resistance Kohm Default = 10
unsigned int R2 ;                            // top resistance Kohm     Default = 100
float maxControllerVoltage ;                 // the arduino Uno boardcard cant excess 5 volt   default = 5v
float hysteresis ;                           // Value of the hysteresis
float setpointHigh = (VinMax + hysteresis);  // Set setpointHigh Caution the battery may go beyond the overcharge
float setpointLow = (VinMax - hysteresis) ;  // Set setpointLow
unsigned int times;
unsigned char cpt =0;
bool stateLCD =0;

int limitOverchargeInAnalog;                 // Limit of the overcharge in analog value
int limitSetpointHighInAnalog;               // Limit of the setpointhigh in analog value
int limitSetpointLowInAnalog;                // Limit of the setpointlow in analog value
float limitOverchargeInVolt;                 // Limit of the overcharge in voltage
float limitSetpointHighInVolt;               // Limit of the setpointhigh in voltage
float limitSetpointLowInVolt;                // Limit of the setpointLow in voltage
bool verification;                           // False = Overcharged / True = not overcharged
float Vin;                                   // Actual Value of the voltage Input
unsigned char intensity;                     // Used to set the intensity of the led with PWM control
char pourcentageIntensity;
int intensityInPercent;
bool oldValue = 0;
byte xCurve = 0;
int arrayVin[83];

const char serialDisplayVoltage[] PROGMEM  = {"Actual Voltage : "};
int k;
char myChar;
char state = 0;
bool testFirstSleep = HIGH;
bool testCase5 = LOW;

/* Value read from the pot */
int sensorValue = 0;       
int ampvalue = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////SETUP//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  // initialize serial:
  Serial.begin(9600);
                                           
  pinMode(2, INPUT_PULLUP);             // Set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(3, INPUT_PULLUP);             // Set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  //attachInterrupt(0,2,RISING);          // Set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  //attachInterrupt(1,3,RISING);          // Set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  pinMode(overchargeIndicatorLed, OUTPUT); // Set the LED indicator in OUTPUT mode
  pinMode(13, OUTPUT);                     // Set the pin 13 as an output
  digitalWrite(13,HIGH);                   // Set the pin 13 to HIGH (that turns on the display's blue light so we can see something)
  //pinMode(buttonA, INPUT_PULLUP);          // Initialize the user buttons
  //pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  
  initEEPROMValue();                       // initialization of VinMax, maxControllerVoltage, R1, R2, hysteresis and times with the last values stored in the EEPROM
  initLCD(500);                            // Setup the WDT
  
  MCUSR &= ~(1<<WDRF);                     // Clear the reset flag
  
  /* In order to change WDE or the prescaler, we need to
  * set WDCE (This will allow updates for 4 clock cycles).
  */
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  
  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP1; /* 0.125 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  
  //Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////LOOP////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  // put your main code here, to run repeatedly:
  if(f_wdt == 1)
  {
//    ampvalue = analogRead(analogInPinA);
//    Serial.println("current:");
//    Serial.println(ampvalue);
    configureSelection();         // Changes the value of "selection" depending on the position of the encoder
    seriesRegulator();            // Write 0 in the parenthesis for no PWM or 1 for the PWM //default selection
    displayLCD(selection);        // Default selection
    displaySerialInformation();   // Read the global declaration in order to display information on the serial monitor
    callCurveDisplay();           // Draw the curve and the battery limit
    
    Serial.println(xCurve);
    
    enableMenu(testButtonC());    // Displays the menu if you press the rotary encoder // You can change Vlim, Vmax, R1, R2, hysteresis, times and decide if you the arduino to sleep depending on the position of the encoder
    
    //if(oldEncPos != encoderPos)   // Updates the position of the encoder
    //{
    //  oldEncPos = encoderPos;  
    //}
    f_wdt = 0;
    
    /* Re-enter sleep mode. */
    enterSleepArduino();
  }
  else
  {
    /* Do nothing. */
  }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////FUNCTION/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//bool CheckButtonA(void)
//{
//  
//  if(digitalRead(buttonA) == LOW)
//  {
//    //something
//  }
//  else
//  {
//    //an other thing
//  }
//}

void displaySerialInformation()                      // Read the global declaration in order to display information on the serial monitor // 0 for all value
{                                                               
  int len = strlen_P(serialDisplayVoltage);
  for (k = 0; k < len; k++)
  {
    myChar =  pgm_read_byte_near(serialDisplayVoltage + k);
    Serial.print(myChar);
  }
  Serial.println(Vin);
}

float getVin(int sensorValue)                                                                // Get Vin value and store it in the global declaration Vin
{
  Vin = ((sensorValue) * maxControllerVoltage / 1024);                                      
  return (Vin);
}

bool verificationOvercharge(int sensorValue, int VinMax)                                     // Check for overcharge
{
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

bool verificationSetpointHigh(int sensorValue, float setpointHigh)                           // Check if sensorValue is higher than setpointHigh
{
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

bool verificationSetpointLow(int sensorValue, float setpointLow)                             // Check if sensorValue is lower than setpointLow
{
  
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

void seriesRegulator()                            // The function that does the regulation
{
  char i;
  for (i=0; i<11; i++)
  {
    delay(50);
    sensorValue = analogRead(analogInPinV);
    getVin(sensorValue);   
    controlMosfet();
    //CheckButtonA();
  } 
}

void PWMwrite(int intensity, int outputPin)       // Generate the PWM square wave with the right duty cycle
{  
  int mappedIntensity;
  mappedIntensity = map(intensity,limitSetpointLowInAnalog,limitSetpointHighInAnalog, 0, 255);
  analogWrite(outputPin, mappedIntensity);
}

void controlShuntPWM(void)                        
{
  if(verificationSetpointHigh(sensorValue,setpointHigh)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                 // Turn on the led // Overcharged
  }
  else if (verificationSetpointLow(sensorValue,setpointLow)==1) //Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, LOW);                  // Turn off the led // Not overcharged
  }
  else
  {
    PWMwrite(sensorValue, overchargeIndicatorLed);              // Hysteresis
  }
}

void controlShuntNoPWM(void)
{
  if(verificationSetpointHigh(sensorValue,setpointHigh)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                 // Turn on the led // Overcharged
  }
  else if (verificationSetpointLow(sensorValue,setpointLow)==1) //Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, LOW);                  // Turn off the led // Not overcharged
  }
  else
  {
                                                                // Hysteresis
  }
}

void controlMosfet()                                            // Selection of the control mode
{
  if (selection == 0)
  {
    controlSerieNoPWM(); 
  }
  else if (selection == 1)
  {
    controlSeriePWM(); 
  }
  else if (selection == 2)
  {
    controlShuntNoPWM(); 
  }
  else if (selection == 3)
  {
    controlShuntPWM(); 
  }
}

char getIntensityInPercent()                                    // Turn sensorValue into a percentage depending on the value of "selection"
{
  
  if (selection == 2)
  {
    if (verificationSetpointHigh(sensorValue,setpointHigh) == 1)
    {
      intensityInPercent = 100; 
    }
    else if (verificationSetpointLow(sensorValue,setpointLow) == 1)
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
    if (verificationSetpointHigh(sensorValue,setpointHigh)==1)
    {
      intensityInPercent = 0;
    }
    else if (verificationSetpointLow(sensorValue,setpointLow)==1)
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


void initLCD( int tempo )                         // Initialize the LCD
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
  delay(tempo);
  
}

void displayLCD(char selection)                   // Diplays the informations on the LCD
{ 
  char percent;
  char i;
  float mappedVinMax;
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

  percent = getIntensityInPercent();
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
  mappedVinMax=map(VinMax,0,maxControllerVoltage, 0, 25);
  u8g.drawHLine(5, 60 - mappedVinMax, 120);
  u8g.setColorIndex(0);
  for (i = 5 ; i < 124; i = i +2)
  {
    u8g.drawPixel(i , 60 - mappedVinMax );
  }
  u8g.setColorIndex(255);
  }while(u8g.nextPage());
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

void enableMenu(bool enable) // Displays the menu if you press the rotary encoder // You can change Vlim, Vmax, R1, R2, hysteresis, times and decide if you the arduino to sleep depending on the position of the encoder
{
  char y;
  char x;
  long newPos;
  char choix = 0;

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

      Serial.println(newPos);
      
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
updateValue();
}
}

bool configureSelection()                             // Changes the value of "selection" depending on the position of the encoder
{
  long newPos;
  newPos = rotEncoder.read() - posit;

  Serial.println(newPos);
  if(( newPos >= -2) & (newPos < 2))
  {
    selection = 1;
    
  }
  else if(((newPos >= 2) && (newPos < 6)) || ((newPos < -10) && (newPos >= -14)))
  {
    selection = 0;
    if(intensityInPercent != 100)
    {
      digitalWrite(overchargeIndicatorLed, LOW);
      intensityInPercent =0;
    }
  }
  else if(((newPos >= 6) && (newPos < 10)) || ((newPos < -6) && (newPos >= -10)))
  {
    selection = 2;
  }
  else if(((newPos >= 10) && (newPos < 14)) || ((newPos < -2) && (newPos >= -6)))
  {
    selection = 3;
  }
  else
  {
    posit = rotEncoder.read();
    Serial.print("oui");
  }
  
}

/***************************************************
*  Name:        enterSleep
*
*  Returns:     Nothing.
*
*  Parameters:  None.
*
*  Description: Enters the arduino into sleep mode.
*
***************************************************/
void enterSleepArduino(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();
}


void controlSerieNoPWM(void)
{
  if(verificationSetpointHigh(sensorValue,setpointHigh)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, LOW);                  // Turn on the led // Overcharged
  }
  else if (verificationSetpointLow(sensorValue,setpointLow)==1) // Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                 // Turn off the led // Not overcharged
  }
  else
  {
                                                                // Hysteresis
  }
}

void controlSeriePWM(void)
{
  if(verificationSetpointHigh(sensorValue,setpointHigh)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, LOW);                  // TURN on the led // Overcharged
  }
  else if (verificationSetpointLow(sensorValue,setpointLow)==1) //Voltage lower than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                 // Light off the led // Not overcharged
  }
  else
  {
    PWMwriteShunt(sensorValue, overchargeIndicatorLed);         // Hysteresis
  }
}


void drawCurve(byte numberArray)
{ 
  float mappedValueVin;
  byte testNumberArray;
  
  mappedValueVin=map(Vin,0,maxControllerVoltage, 0, 35);
  u8g.drawPixel(60 - mappedValueVin, numberArray);  
}

void drawBatteryLimit(void)
{ 
  
  //char positionX;
 // bool test =0;
  float mappedVinMax;
  mappedVinMax=map(VinMax,0,maxControllerVoltage, 0, 22);
  u8g.setColorIndex(128);
  u8g.drawHLine(5, 60 - mappedVinMax , 120);
  u8g.setColorIndex(255);
}

void PWMwriteShunt(int intensity, int outputPin) //Generate the PWM square wave with the right duty cycle
{ 
  int mappedIntensity;
  mappedIntensity = map(intensity,limitSetpointHighInAnalog,limitSetpointLowInAnalog, 0, 255);
  analogWrite(outputPin, mappedIntensity);
}


/***************************************************
*  Name:        ISR(WDT_vect)
*
*  Returns:     Nothing.
*
*  Parameters:  None.
*
*  Description: Watchdog Interrupt Service. This
*               is executed when watchdog timed out.
*
***************************************************/
ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1; // default 1
  }
  else
  {
             //Serial.println("WDT Overrun!!!");
  }
}


//void PinA()
//{
//  cli();                                  // Stop interrupts happening before we read pin values
//  reading = PIND & 0xC;                   // Read all eight pin values then strip away all but pinA and pinB's values
//  if(reading == B00001100 && aFlag)       // Check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
//  {  
//  encoderPos --;                          // Decrement the encoder's position count
//  bFlag = 0;                              // Reset flags for the next turn
//  aFlag = 0;                              // Reset flags for the next turn
//  }
//  else if (reading == B00000100) 
//  {
//    bFlag = 1;                            // Signal that we're expecting pinB to signal the transition to detent from free rotation
//  }
//  sei();                                  // Restart interrupts
//}
//
//void PinB(){
//  cli();                                  // Sstop interrupts happening before we read pin values
//  reading = PIND & 0xC;                   // Read all eight pin values then strip away all but pinA and pinB's values
//  if (reading == B00001100 && bFlag)      // Ccheck that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
//  {                                       
//    encoderPos ++;                        // Increment the encoder's position count
//    bFlag = 0;                            // Reset flags for the next turn
//    aFlag = 0;                            // Reset flags for the next turn
//  }
//  else if (reading == B00001000) 
//  {
//    aFlag = 1;                            // Signal that we're expecting pinA to signal the transition to detent from free rotation
//  }
//  sei();                                  // Restart interrupts
//}

void updateValue(void)                    // Update setpointHigh and setpointLow depending on VinMax and hysteresis
{
  setpointHigh = (VinMax + hysteresis);
  setpointLow = (VinMax - hysteresis) ;
}

void initEEPROMValue (void)               // initialization of VinMax, maxControllerVoltage, R1, R2, hysteresis and times with the last values stored in the EEPROM
{
  VinMax = EEPROM.readFloat(0);
  maxControllerVoltage = EEPROM.readFloat(4);
  R1 = EEPROM.read(8);
  R2 = EEPROM.read(12);
  hysteresis = EEPROM.readFloat(16);
  times = EEPROM.readInt(20);
}

void curveDisplay(void)                   // Draw the curve
{
  
  
}

bool callCurveDisplay()                   // Calls curveDisplay and drawBatteryLimit
{
  int pix = 5;
  
  if (cpt >= times)
  {
    //curveDisplay();
    drawCurve(pix);
    pix++;
    cpt=0;
  }
  else
  {
    cpt++;
  }
  
}

