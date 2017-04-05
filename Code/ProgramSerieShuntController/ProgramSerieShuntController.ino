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

//static int pinA = 2;                  // Our first hardware interrupt pin is digital pin 2
//static int pinB = 3;                  // Our second hardware interrupt pin is digital pin 3
//volatile byte aFlag = 0;              // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
//volatile byte bFlag = 0;              // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
//volatile byte encoderPos = 0;         // This variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
//volatile byte oldEncPos = 0;          // Stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
//volatile byte reading = 0;            // Somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent
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
    ampvalue = analogRead(analogInPinA);
    Serial.println("current:");
    Serial.println(ampvalue);
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
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);
  
  myGLCD.clrScr();
  myGLCD.print("SerieRegulator", CENTER, 0);
  myGLCD.print("RE-INNOVATION", CENTER, 20);
  myGLCD.setFont(TinyFont);
  myGLCD.print("(C)2016 by", CENTER, 36);
  myGLCD.print("Matthew Little", CENTER, 42);
  myGLCD.update();
  
  delay(tempo);
  myGLCD.clrScr();
  
}

void displayLCD(char selection)                   // Diplays the informations on the LCD
{ 
  char i;
  myGLCD.setFont(SmallFont);
  myGLCD.print("Voltage", LEFT, 0);
  
  if(selection == 0)
  {
    myGLCD.print("SER: ", 45, 0);
  }
  else if (selection == 1)
  {
    myGLCD.print("SER: ", 45, 0);
    myGLCD.print("PWM",RIGHT,0);
  }
  else if (selection == 2)
  {
    myGLCD.print("SHU: ", 45, 0);
  }
  else if (selection == 3)
  {
    myGLCD.print("SHU: ", 45, 0);
    myGLCD.print("PWM",RIGHT,0);
  }
  
  myGLCD.setFont(MediumNumbers);
  
  if(Vin < 10)
  {
    myGLCD.printNumF(Vin,1,LEFT,7);
  }
  else
  {
    myGLCD.printNumI(Vin,LEFT,7);
  }
  
  myGLCD.setFont(SmallFont);
  myGLCD.print("v",35,15);
  
  myGLCD.setFont(SmallFont);

  myGLCD.printNumI(getIntensityInPercent(),58,16,3,' ');
  myGLCD.print("%",77,15);
  
  for(i=83;i >=0 ; i--)      // Line
  {
    myGLCD.setPixel(i,47);
  }
  
  for(i=47;i > 24 ; i--)     // Column
  {
    myGLCD.setPixel(0,i);
  }
  
  myGLCD.update();
  myGLCD.clrScr();
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
      
      myGLCD.setFont(SmallFont);
      myGLCD.clrScr();
      myGLCD.print("CHOOSE :", 0, 0);
      
      newPos = rotEncoder.read() - posit;
      
      if((newPos >= -2) && (newPos < 2))
      {
        
        myGLCD.print("VLIM", 40, 0);
        myGLCD.print("VLIM =", 0, 17);
        myGLCD.printNumF(VinMax,1, 40, 17);
        myGLCD.print("PRESS BUTTON", 0, 30);
        myGLCD.print("TO SELECT", 0, 40);
        myGLCD.update();
        state= 0x00;
        //posit = 0;        
      }
      if(((newPos >= 2) && (newPos < 6)) || ((newPos < -22) && (newPos >= -26)))
      {
        
        myGLCD.print("VMAX", 40, 0);
        myGLCD.print("VMAX =", 0, 17);
        myGLCD.printNumF(maxControllerVoltage,1, 40, 17);
        myGLCD.print("PRESS BUTTON", 0, 30);
        myGLCD.print("TO SELECT", 0, 40);
        myGLCD.update();
        state= 0x01;
        //posit = 0;        
      }
      if(((newPos >= 6) && (newPos < 10)) || ((newPos < -18) && (newPos >= -22)))
      {
        
        myGLCD.print("R1", 40, 0);
        myGLCD.print("R1 =", 0, 17);
        myGLCD.printNumI(R1, 40, 17);
        myGLCD.print("PRESS BUTTON", 0, 30);
        myGLCD.print("TO SELECT", 0, 40);
        myGLCD.update();
        state= 0x02;
        //posit = 0;
      }
      if(((newPos >= 10) && (newPos < 14)) || ((newPos < -14) && (newPos >= -18)))
      {
        
        myGLCD.print("R2", 40, 0);
        myGLCD.print("R2 =", 0, 17);
        myGLCD.printNumI(R2, 40, 17);
        myGLCD.print("PRESS BUTTON", 0, 30);
        myGLCD.print("TO SELECT", 0, 40);
        myGLCD.update();
        state= 0x03;
        //posit = 0;
      }
      if(((newPos >= 14) && (newPos < 18)) || ((newPos < -10) && (newPos >= -14)))
      {
        
        myGLCD.print("HYST", 40, 0);
        myGLCD.print("HYST=", 0, 17);
        myGLCD.printNumF(hysteresis,1, 45, 17);
        myGLCD.print("PRESS BUTTON", 0, 30);
        myGLCD.print("TO SELECT", 0, 40);
        myGLCD.update();
        state= 0x04;
        //posit = 0;        
      }
      
      if(((newPos >= 18) && (newPos < 22)) || ((newPos < -6) && (newPos >= -10)))
      {
        myGLCD.print("CURVE", 40, 0);
        myGLCD.print("TIMES=", 0, 17);
        myGLCD.printNumI(times, 40, 17);
        myGLCD.print("PRESS BUTTON", 0, 30);
        myGLCD.print("TO ACCEPT", 0, 40);
        myGLCD.update();
        state= 0x06;   
        //posit = 0;
      }
      
      if(((newPos >= 22) && (newPos < 26)) || ((newPos < -2) && (newPos >= -6)))
      {
        myGLCD.print("SLEEP", 40, 0);
        myGLCD.print("GO TO SLEEP?", 0, 17);
        
        myGLCD.print("PRESS BUTTON", 0, 30);
        myGLCD.print("TO ACCEPT", 0, 40);
        myGLCD.update();
        state= 0x05; 
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
      switch (state)
      {
        case 0x00:
        {
          VinMax = EEPROM.readFloat(0);
          myGLCD.clrScr();
          myGLCD.print("CHOOSE :", 0, 0);
          myGLCD.print("VLIM", 40, 0);
          myGLCD.print("VLIM =", 0, 17);
          myGLCD.printNumF(VinMax,1, 40, 17);
          myGLCD.print("TURN BUTTON", 0, 30);
          myGLCD.print("TO CHANGE", 0, 40);
          myGLCD.update();
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
              myGLCD.clrScr();
              myGLCD.print("VLIM =", 0, 17);
              myGLCD.printNumF(VinMax,1, 40, 17);
              myGLCD.print("PRESS TO ACCEPT",LEFT,30);
              myGLCD.update(); 
            }
            else if (posit < newPos)
            {
              
              VinMax = VinMax-(newPos - posit) * 0.05;
              myGLCD.clrScr();
              myGLCD.print("VLIM =", 0, 17);
              myGLCD.printNumF(VinMax,1, 40, 17);
              myGLCD.print("PRESS TO ACCEPT",LEFT,30);
              myGLCD.update();
            }
            else
            {
              Serial.println("");
              Serial.println(posit);
              Serial.println(newPos);
              //Serial.println(encoderPos);  //nothing
            }
          
        }while(HIGH==digitalRead(buttonC));
        EEPROM.writeFloat(0, VinMax);
      }
      break;
      
      case 0x01:
      {
        maxControllerVoltage = EEPROM.readFloat(4);
        myGLCD.clrScr();
        myGLCD.print("CHOOSE :", 0, 0);
        myGLCD.print("VMAX", 40, 0);
        myGLCD.print("VMAX =", 0, 17);
        myGLCD.printNumF(maxControllerVoltage,1, 40, 17);
        myGLCD.print("TURN BUTTON", 0, 30);
        myGLCD.print("TO CHANGE", 0, 40);
        myGLCD.update();
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
            myGLCD.clrScr();
            myGLCD.print("VMAX =", 0, 17);
            myGLCD.printNumF(maxControllerVoltage,1, 40, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else if (posit < newPos)
          {
            maxControllerVoltage = maxControllerVoltage-(newPos - posit) * 0.05;
            myGLCD.clrScr();
            myGLCD.print("VMAX =", 0, 17);
            myGLCD.printNumF(maxControllerVoltage,1, 40, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else
          {
            //nothing
          }
          
        }while(HIGH==digitalRead(buttonC));
        EEPROM.writeFloat(4, maxControllerVoltage);
      }
      break;
      
      case 0x02:
      {
        R1 = EEPROM.read(8);
        myGLCD.clrScr();
        myGLCD.print("CHOOSE :", 0, 0);
        myGLCD.print("R1", 40, 0);
        myGLCD.print("R1 =", 0, 17);
        myGLCD.printNumI(R1, 40, 17);
        myGLCD.print("TURN BUTTON", 0, 30);
        myGLCD.print("TO CHANGE", 0, 40);
        myGLCD.update();
        do{
          newPos = rotEncoder.read();
          posit = newPos + 400;
          delay(200);
          newPos = rotEncoder.read() + 400;
          
          if(posit > newPos)
          {
            R1 = R1 + (posit - newPos) * 0.25;
            myGLCD.clrScr();
            myGLCD.print("R1 =", 0, 17);
            myGLCD.printNumI(R1, 40, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else if (posit < newPos)
          {
            R1 = R1 - (newPos - posit) * 0.25;
            myGLCD.clrScr();
            myGLCD.print("R1 =", 0, 17);
            myGLCD.printNumI(R1, 40, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else
          {
            //nothing
          }
          
        }while(HIGH==digitalRead(buttonC));
        EEPROM.write(8,R1);
      }
      break;
      
      case 0x03:
      {
        R2 = EEPROM.read(12);
        myGLCD.clrScr();
        myGLCD.print("CHOOSE :", 0, 0);
        myGLCD.print("R2", 40, 0);
        myGLCD.print("R2 =", 0, 17);
        myGLCD.printNumI(R2, 40, 17);
        myGLCD.print("TURN BUTTON", 0, 30);
        myGLCD.print("TO CHANGE", 0, 40);
        myGLCD.update();
        do{
          newPos = rotEncoder.read();
          posit = newPos + 400;
          delay(200);
          newPos = rotEncoder.read() + 400;
          
          if(posit > newPos)
          {
            R2 = R2 + (posit - newPos) * 0.25;
            myGLCD.clrScr();
            myGLCD.print("R2 =", 0, 17);
            myGLCD.printNumI(R2, 40, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else if (posit < newPos)
          {
            R2 = R2 - (newPos - posit) * 0.25;
            myGLCD.clrScr();
            myGLCD.print("R2 =", 0, 17);
            myGLCD.printNumI(R2, 40, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else
          {
            //nothing
          }
          
        }while(HIGH==digitalRead(buttonC));
        EEPROM.write(12,R2);
      }
      break;
      
      case 0x04:
      {
        hysteresis=EEPROM.readFloat(16);
        myGLCD.clrScr();
        myGLCD.print("CHOOSE :", 0, 0);
        myGLCD.print("HYSTERESIS", 40, 0);
        myGLCD.print("HYST=", 0, 17);
        myGLCD.printNumF(hysteresis,1, 45, 17);
        myGLCD.print("TURN BUTTON", 0, 30);
        myGLCD.print("TO CHANGE", 0, 40);
        myGLCD.update();
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
            myGLCD.clrScr();
            myGLCD.print("HYST =", 0, 17);
            myGLCD.printNumF(hysteresis,1, 45, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else if (posit < newPos)
          {
            hysteresis = hysteresis - (newPos - posit) * 0.05;
            myGLCD.clrScr();
            myGLCD.print("HYST =", 0, 17);
            myGLCD.printNumF(hysteresis,1, 45, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else
          {
            //nothing
          }
          
        }while(HIGH==digitalRead(buttonC));
        EEPROM.writeFloat(16, hysteresis);
      }
      break;
      
      case 0x05:
      { 
         myGLCD.disableSleep();
        digitalWrite(13,HIGH);
        myGLCD.clrScr();
        myGLCD.print("GOING TO", 0, 0);
        myGLCD.print("SLEEP", 0, 10);
        myGLCD.print("zzZZZZzzZZz",LEFT,30);
        myGLCD.update();
        delay(1500);
        testFirstSleep = LOW;
        myGLCD.enableSleep();
        digitalWrite(13,LOW);
        testCase5 = HIGH;
      }
      break;
      
      case 0x06:
      {
        times=EEPROM.readInt(20);
        myGLCD.clrScr();
        myGLCD.print("CHOOSE SCALE:", 0, 0);
        myGLCD.print("TIMES=", 0, 17);
        myGLCD.printNumI(times, 40, 17);
        myGLCD.print("TURN BUTTON", 0, 30);
        myGLCD.print("TO CHANGE", 0, 40);
        myGLCD.update();
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
            myGLCD.clrScr();
            myGLCD.print("TIMES=", 0, 17);
            myGLCD.printNumI(times, 40, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else if (posit < newPos)
          {
            times = times - (newPos - posit)*0.25;
            myGLCD.clrScr();
            myGLCD.print("TIMES=", 0, 17);
            myGLCD.printNumI(times, 40, 17);
            myGLCD.print("PRESS TO ACCEPT",LEFT,30);
            myGLCD.update();
          }
          else
          {
            //nothing
          }
          
        }while(digitalRead(buttonC)==HIGH);
        EEPROM.writeInt(20, times);
      }
      break;
      default:
      //nothing
      break;
    }
    delay(1000);
    
  }while(digitalRead(buttonC) == HIGH && testFirstSleep == HIGH);
  myGLCD.clrScr();
  myGLCD.print("OK", CENTER, 10);
  myGLCD.print("CHARGING VALUE",CENTER,26);
  myGLCD.update();
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
  
  mappedValueVin=map(Vin,0,maxControllerVoltage, 0, 22);
  arrayVin[numberArray]= mappedValueVin;
  for (testNumberArray=1; testNumberArray <= numberArray; testNumberArray++)
  {
    myGLCD.setPixel(testNumberArray,(46-arrayVin[testNumberArray]));
  }
}

void drawBatteryLimit(void)
{ 
  float mappedVinMax;
  char positionX;
  bool test =0;
  
  mappedVinMax=map(VinMax,0,maxControllerVoltage, 0, 22);
  for (positionX=1; positionX <= 83; positionX++)
  {
    myGLCD.setPixel(positionX,(46-mappedVinMax));
    if(test==0)
    {
      test=1;
    }
    else
    {
      myGLCD.invPixel(positionX, (46-mappedVinMax));
      test=0;
    }
  }
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
  
  if (xCurve>=82)
  {
    xCurve = 0;
    drawCurve(0);
    xCurve++;
  }
  else {
    drawCurve(xCurve);
    xCurve++;
  }
  drawBatteryLimit();
}

bool callCurveDisplay()                   // Calls curveDisplay and drawBatteryLimit
{
  if (cpt >= times)
  {
    curveDisplay();
    cpt=0;
  }
  else
  {
    cpt++;
  }
  drawBatteryLimit();
  drawCurve(xCurve);
  
}

