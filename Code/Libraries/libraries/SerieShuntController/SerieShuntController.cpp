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

#include "SerieShuntController.h"

Encoder rotEncoder(2, 3);                                         // Rotary encoder object declaration

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);    // OLED display declaration

LCD5110 myGLCD(12,11,10,8,9);                                     // LCD display declaration

Get get;
Value value;
Verification verif;
PWMwrite pwmwrite;
const int analogInPinV = A0;                 // Analog input pin that the potentiometer (Voltmeter) is attached to / Default = A0
const int analogInPinA = A1;                 // Analog input pin that the potentiometer (Ammeter) is attached to / Default = A1
const int buttonC = 4;                       // Digital input pin that the button of the rotary encoder is attached to / Default = 4

const int  overchargeIndicatorLed = 5;       // LedOutput  Default = 2

const char serialDisplayVoltage[] PROGMEM  = {"Actual Voltage : "};
bool testFirstSleep = HIGH;
bool testCase5 = LOW;

//OLED_Display

OLED_Display::OLED_Display(void) 
{
	
}

void OLED_Display::initOLED(void) 
{  
	u8g.begin();
	if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
	u8g.setColorIndex(255);       // white
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

void OLED_Display::displayOLED(char selection, int sensorValue, float setpointHigh, float setpointLow, float Vin, float VinMax, float maxControllerVoltage, unsigned int times, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog, unsigned char & cpt, unsigned char curveTab[], unsigned char & curveCpt)
{
	char percent;
	
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

	percent = get.intensityInPercent(selection, sensorValue, setpointHigh, setpointLow, maxControllerVoltage, limitSetpointHighInAnalog, limitSetpointLowInAnalog);
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

void OLED_Display::enableMenu(bool enable, float & VinMax, float & maxControllerVoltage, unsigned int & R1, unsigned int & R2, float & hysteresis, unsigned int & times, float setpointHigh, float setpointLow, long & newPos, long & posit)
{
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
value.updateValue(setpointHigh, setpointLow, VinMax, hysteresis);
}
}

void OLED_Display::curveDisplay(float Vin, float maxControllerVoltage, unsigned int times, unsigned char & cpt, unsigned char curveTab[], unsigned char curveCpt)
{
  char i;
  int mappedValueVin;
  if (cpt >= times)
  {
    cpt = 0;
    mappedValueVin=map(Vin,0,maxControllerVoltage, 0, 25);
    curveTab[curveCpt] = (unsigned char)mappedValueVin;
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

void OLED_Display::drawBatteryLimit(float VinMax, float maxControllerVoltage)
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

//Rotary Encoder

Encoder_control::Encoder_control() 
{
	pinMode(buttonC, INPUT_PULLUP);           
}

bool Encoder_control::testButtonC()
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

char Encoder_control::configureSelection(int sensorValue, float setpointHigh, float setpointLow, float maxControllerVoltage, long & newPos, long & posit, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)
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
    intensityInPercent = get.intensityInPercent(selection,sensorValue,setpointHigh,setpointLow, maxControllerVoltage, limitSetpointHighInAnalog, limitSetpointLowInAnalog);
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

//Values manager

Value::Value() 
{
	
}

void Value::updateValue(float & setpointHigh, float & setpointLow, float VinMax, float hysteresis)
{
  setpointHigh = (VinMax + hysteresis);
  setpointLow = (VinMax - hysteresis) ;
}

void Value::initEEPROMValue (float & VinMax, float & maxControllerVoltage, unsigned int & R1, unsigned int & R2, float & hysteresis, unsigned int & times)
{
  VinMax = EEPROM.readFloat(0);
  maxControllerVoltage = EEPROM.readFloat(4);
  R1 = EEPROM.read(8);
  R2 = EEPROM.read(12);
  hysteresis = EEPROM.readFloat(16);
  times = EEPROM.readInt(20);
}

//Verification

Verification::Verification()
{
	
}

bool Verification::overcharge(int sensorValue, int VinMax, float maxControllerVoltage) 
{
  int limitOverchargeInAnalog;
  float limitOverchargeInVolt;
  limitOverchargeInVolt = VinMax;                                                            
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

bool Verification::setpointHigh(int sensorValue, float setpointHigh, float maxControllerVoltage, int & limitSetpointHighInAnalog)                           
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

bool Verification::setpointLow(int sensorValue, float setpointLow, float maxControllerVoltage, int & limitSetpointLowInAnalog)                             
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

//Get

Get::Get()
{
	
}

float Get::vin(int sensorValue, float maxControllerVoltage)                                                                
{
  float Vin;
  Vin = ((sensorValue) * maxControllerVoltage / 1024);                                      
  return (Vin);
}

char Get::intensityInPercent(char selection, int sensorValue, float setpointHigh, float setpointLow, float maxControllerVoltage, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                                    
{
  int intensityInPercent;
  if (selection == 2)
  {
    if (verif.setpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog) == 1)
    {
      intensityInPercent = 100; 
    }
    else if (verif.setpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog) == 1)
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
    if (verif.setpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)
    {
      intensityInPercent = 0;
    }
    else if (verif.setpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1)
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

//PWMwrite

PWMwrite::PWMwrite()
{
	
}

void PWMwrite::serie(int intensity, int outputPin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog) 
{  
  int mappedIntensity;
  mappedIntensity = map(intensity,limitSetpointLowInAnalog,limitSetpointHighInAnalog, 0, 255);
  analogWrite(outputPin, mappedIntensity);
}

void PWMwrite::shunt(int intensity, int outputPin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog) 
{ 
  int mappedIntensity;
  mappedIntensity = map(intensity,limitSetpointHighInAnalog,limitSetpointLowInAnalog, 0, 255);
  analogWrite(outputPin, mappedIntensity);
}

//Controller

Control::Control() 
{
	pinMode(overchargeIndicatorLed, OUTPUT);  
}

void Control::seriesRegulator(int & sensorValue, float maxControllerVoltage, char selection, float setpointHigh, float setpointLow, float & Vin, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                            
{
  char i;
  for (i=0; i<11; i++)
  {
    delay(50);
    sensorValue = analogRead(analogInPinV);
    Vin = get.vin(sensorValue, maxControllerVoltage);   
    mosfet(selection, maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog);
  } 
}

void Control::shuntPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                        
{
  if(verif.setpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                                      // Turn on the led // Overcharged
  }
  else if (verif.setpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1) //Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, LOW);                                       // Turn off the led // Not overcharged
  }
  else
  {
    pwmwrite.serie(sensorValue, overchargeIndicatorLed, limitSetpointHighInAnalog, limitSetpointLowInAnalog);                                   // Hysteresis
  }
}

void Control::shuntNoPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)
{
  if(verif.setpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                                      // Turn on the led // Overcharged
  }
  else if (verif.setpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1) //Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, LOW);                                       // Turn off the led // Not overcharged
  }
  else
  {
                                                                                     // Hysteresis
  }
}

void Control::mosfet(char selection, float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)                                            // Selection of the control mode
{
  if (selection == 0)
  {
    serieNoPWM(maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog); 
  }
  else if (selection == 1)
  {
    seriePWM(maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog); 
  }
  else if (selection == 2)
  {
    shuntNoPWM(maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog); 
  }
  else if (selection == 3)
  {
    shuntPWM(maxControllerVoltage, sensorValue, setpointHigh, setpointLow, limitSetpointHighInAnalog, limitSetpointLowInAnalog); 
  }
}

void Control::serieNoPWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)
{
  if(verif.setpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, LOW);                  // Turn on the led // Overcharged
  }
  else if (verif.setpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1) // Voltage lower than setpointLow
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                 // Turn off the led // Not overcharged
  }
  else
  {
                                                                // Hysteresis
  }
}

void Control::seriePWM(float maxControllerVoltage, int sensorValue, float setpointHigh, float setpointLow, int limitSetpointHighInAnalog, int limitSetpointLowInAnalog)
{
  if(verif.setpointHigh(sensorValue,setpointHigh,maxControllerVoltage, limitSetpointHighInAnalog)==1)     //Voltage higher than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, LOW);                  // TURN on the led // Overcharged
  }
  else if (verif.setpointLow(sensorValue,setpointLow,maxControllerVoltage, limitSetpointLowInAnalog)==1) //Voltage lower than setpointHigh
  {
    digitalWrite(overchargeIndicatorLed, HIGH);                 // Light off the led // Not overcharged
  }
  else
  {
    pwmwrite.shunt(sensorValue, overchargeIndicatorLed, limitSetpointHighInAnalog, limitSetpointLowInAnalog);         // Hysteresis
  }
}

void Control::enterSleepArduino(void) 
{
  set_sleep_mode(SLEEP_MODE_IDLE);  
  sleep_enable();
  
  sleep_mode();

  sleep_disable(); 

  power_all_enable();
}

