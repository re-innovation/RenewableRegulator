#include <LCD5110_Graph.h>

#define pwmChange 1
float constantVoltage, constantCurrent;
const int analogInPinVoltage = A5;
const int analogInPinCurrent = A4;
float maxControllerVoltage = 30;
unsigned int R1 = 10;
unsigned int R2 = 47;
float Vin;
float current;
int voltageValue;
int currentValue;
float newPower, oldPower;
int pwmValue;
LCD5110 myGLCD(12, 11, 10, 8, 9);
extern unsigned char SmallFont[];
extern unsigned char TinyFont[];
extern unsigned char MediumNumbers[];
int pwmScan=0;
int timer1_counter;
bool change = HIGH;
float pwmVariation;
float averagePower = 0;

void setup() {
  // put your setup code here, to run once:
  
  pwmVariation = pwmChange;    // Give the value of pwmChange to pwmVariation
  Serial.begin(115200);

  constantCurrent = (5000.0 / 1023.0);
  constantVoltage = (maxControllerVoltage / 1024.0) * ((R2 + R1) / R2);
  
  pinMode(5, OUTPUT);           // Select pinMode
  pinMode(13, OUTPUT);          // Select pinMode
  initLCD(1000);                 //Initialise Lcd
  scan();                        //Call the function which will find the mppt at the startup
  
   // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;

  // Set timer1_counter to the correct value for our interrupt interval
  //timer1_counter = 64911;   // preload timer 65536-16MHz/256/100Hz
 //timer1_counter = 64286;   // preload timer 65536-16MHz/256/50Hz
 timer1_counter = 34286;   // preload timer 65536-16MHz/256/2Hz
  
  TCNT1 = timer1_counter;   // preload timer
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
  interrupts();             // enable all interrupts


}


ISR(TIMER1_OVF_vect)        // interrupt service routine 
{
  TCNT1 = timer1_counter;

  // Write to the PWM to change the power level
  analogWrite(5, (pwmValue)); // Do we do this here?

  // Measure the new power. 
  getMeasure();                       //measure power
  
  newPower = averagePower;
  
 //////////////////////////////////////////////////////////////////////
 ////////// Adapt Speed of tracking/////////////////////////////////.
 /////////////////////////////////////////////////////////////////
// pwmVariation = abs(newPower - oldPower);
// //Serial.println((int)pwmVariation); 
// // Covert float to int
// if (pwmVariation <=1)
// {
//  pwmVariation =  1;
// }
 pwmVariation =  1;
 //////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////

  
  if (change == HIGH )                                            //algorithm of the mppt
  {
    if (newPower >= oldPower)
    {
      pwmValue = pwmValue + (int)pwmVariation;
      change = HIGH;
    }
    else if (newPower < oldPower)
    {
      pwmValue = pwmValue - (int)pwmVariation;
      change = LOW;
    }
  }
  else if (change == LOW)
  {
    if(newPower >= oldPower)
    {
      pwmValue = pwmValue - (int)pwmVariation;
      change = LOW;
    }
    else if (newPower < oldPower)
    {
      pwmValue = pwmValue + (int)pwmVariation;
      change = HIGH;
    }
  }
  Serial.print("Gradient:");
  Serial.println(change);
  

if (pwmValue >= 255)                          //Prevent the pwmValue to go higher than its limit
{
  pwmValue = 255;
}
else if (pwmValue <= 0)                         //Prevent the pwmValue to go lower than its limit
{
  pwmValue = 0;
  
}
 Serial.print(" nPow : ");                       //display serial information
 Serial.print(newPower); 
 Serial.print(" and ");
 Serial.print(" oPow : ");
 Serial.print(oldPower);
 Serial.print(" PWM = ");
 Serial.println(pwmValue);
// Serial.println();
// Serial.print(current);
// Serial.println();

 oldPower = newPower;                       // save the newPower as the oldPower

 displayLCD();                              // Display the information on the LCD
 
}

void loop() {
  
  
//nothing//

}

void getMeasure (void)
{ 
  digitalWrite(7,HIGH);
  float averageVoltage = 0;
  float averageCurrent = 0;

  // Taking V and I measurments
    
  for(char i = 0; i < 100 ; i++)
  {
   averageVoltage = averageVoltage + getVin();
   averageCurrent = averageCurrent + getAmp();
   averagePower = averagePower + (getVin()*getAmp());
  }

  averageVoltage = averageVoltage/100;
  averageCurrent = averageCurrent /100;
  averagePower = averagePower/100;
  
  //current = ((averageCurrent - 2493) / 183);
  Serial.print("Power:");
  Serial.println(averagePower);
  digitalWrite(7,LOW);  
}
float getVin(void)                                              // Get Vin value and store it in the global declaration Vin
{ 
  Vin = (analogRead(A5) * constantVoltage);
  return (Vin);
}

float getAmp()                  // gets the current in Amp
{
  
  current= ((constantCurrent * analogRead(A4))-2493)/183; //this is
  return (current);
}

float powerCalculation(void)      // Calculation of the power
{ float Power;
  Power = current * Vin;
  return (Power);
}



void initLCD( int tempo )       //Init the lcd and display Matthew information
{
  myGLCD.InitLCD();
  digitalWrite(13, HIGH);
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
  myGLCD.update();



}


void displayLCD(void)             // display the current, voltage , power and pwm information on the LCD
{
  myGLCD.clrScr();

  myGLCD.setFont(SmallFont);
  myGLCD.print("VOLTAGE", LEFT, 0);
  myGLCD.printNumF(Vin, 1, RIGHT, 0);

  myGLCD.print("CURRENT", LEFT, 12);
  myGLCD.printNumF(current, 1, RIGHT, 12);

  myGLCD.print("POWER", LEFT, 22);
  myGLCD.printNumF(newPower, 1, RIGHT, 22);

  myGLCD.print("PWM", LEFT, 32);
  myGLCD.printNumI(pwmValue, RIGHT, 32);

  myGLCD.update();


}

void scan(void)                                     // Used one time at the startup of the program. It will place the pwmValue at the mppt.
{ float Power = 0;
  float olderPower = 0;

  myGLCD.setFont(SmallFont);
  
  for (pwmValue = 0 ; pwmValue < 254; pwmValue ++)
  {
    analogWrite(5, pwmValue);
    //voltageValue = analogRead(analogInPinVoltage);
    //currentValue = analogRead(analogInPinCurrent);
    getMeasure();
    Power = powerCalculation();
    //Serial.println(Power);
    if ((Power - olderPower ) >= 0)
    {
      olderPower = Power;
      pwmScan = pwmValue;
    }
    else
    {
    }


  }
  pwmValue = pwmScan;



}




