#include "obd2racingMod.h"
#include <mcp_can.h>            //CAN Bus Shield Compatibility Library
#include <SPI.h>                //CAN Bus Shield SPI Pin Library
#include <LiquidCrystal_I2C.h>  //Libraries for controlling LC Displays               
#include <LiquidCrystal.h>
#include <Arduino.h>            //String support

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);  // Set CS pin

// Class init
void obd2racingmod_h::init(void)
{
  m_cMode      = 0;
  m_cPid       = 0;

  // CAN bus INIT 500k
  if(CAN.begin(CAN_500KBPS) == CAN_OK)
  {
#if DEBUGMODE
      //Initialize LCD
      lcd.begin(16,3);
      lcd.backlight();
      showLCD("Hilux Vigo Dashboard","by VT11257");
      delay(2000);
      showLCD("Hilux Vigo Dashboard","booting...");
      showLCD("CAN BUS Shield init ok!");
  }
#else
     ;
#endif
    }
  else 
    {
#if DEBUGMODE
      Serial.println("CAN INIT FAIL!!");
#else
      ;
#endif
    }
    
    CAN.init_Mask(0, 0, 0x7f0);
    CAN.init_Mask(1, 0, 0x7f0);
    CAN.init_Filt(0, 0, 0x7e0); 
    CAN.init_Filt(1, 0, 0x7e0);
    CAN.init_Filt(2, 0, 0x7e0);
    CAN.init_Filt(3, 0, 0x7e0);
    CAN.init_Filt(4, 0, 0x7e0);
    CAN.init_Filt(5, 0, 0x7e0);
    
    delay(10);

}

//Reload LC Display
void obd2racingmod_h::showTextLCD(String line1,String line2)
{  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}


void obd2racingmod_h::sendOrder()
{

  INT8U len = 0;
  INT8U orBuf[8]= {0x02, m_cMode, m_cPid, 0x00, 0x00, 0x00, 0x00, 0x00};
  CAN.sendMsgBuf(0x7df, 0, 8, orBuf);
  
  while(CAN.checkReceive() == CAN_NOMSG);
  while(CAN.checkReceive() == CAN_NOMSG);
  while(CAN.checkReceive() == CAN_NOMSG);

  len = 0;
  memcpy(m_cData, 0, 8);
  CAN.readMsgBuf(&len, m_cData);  
}

void obd2racingmod_h::setModePid(INT8U cMode, INT8U cPid)
{
  m_cMode = cMode;
  m_cPid  = cPid;
}


obd2racingmod_h OBD2MOD;
