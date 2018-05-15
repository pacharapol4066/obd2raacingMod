/*
 *  MODIFIED BY PACHARAPOL OUMOLARN
 *  15/5/2018
 *  https://www.csselectronics.com/screen/page/simple-intro-to-can-bus/language/en
 */
#include "obd2racingMod.h"
#include <mcp_can.h>            //CAN Bus Shield Compatibility Library
#include <SPI.h>                //CAN Bus Shield SPI Pin Library            
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
      lcd.begin(16,4);
      lcd.backlight();
      showMsgLCD("Hilux Vigo Dashboard","by VT11257");
      delay(2000);
      showMsgLCD("Hilux Vigo Dashboard","booting...");
      delay(2000);
      showMsgLCD("CAN BUS Shield init ok!","");
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

    CAN.init_Filt(0, 0, 0x7e0);
    CAN.init_Mask(0, 0, 0x7f0);
    CAN.init_Filt(1, 0, 0x7e0);
    CAN.init_Mask(1, 0, 0x7f0);
    
    CAN.init_Filt(2, 0, 0x7e0);
    CAN.init_Filt(3, 0, 0x7e0);
    CAN.init_Filt(4, 0, 0x7e0);
    CAN.init_Filt(5, 0, 0x7e0);
    
    delay(10);

}

//Reload LC Display
void obd2racingmod_h::showMsgLCD(String line1,String line2)
{  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

//Reload LC Display
void obd2racingmod_h::showValLCD(String line1,INT32U line2)
{  
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print(line1);
  lcd.setCursor(0, 3);
  lcd.print(line2);
}


void obd2racingmod_h::sendOrder()
{

  INT8U len = 0;
  INT8U orBuf[8]= {0x02, m_cMode, m_cPid, 0x00, 0x00, 0x00, 0x00, 0x00};

  /*
   * sendMsgBuf(id, ext, len, buf)
   * id ：Device ID for the remote CAN device  > 0x7df
   * ext：Data transmission format             > 0 (standard data format)
   * len：size of the array                    > 8 bytes of data
   * buf：data array                           > orBuf[8]
   */
  CAN.sendMsgBuf(CAN_ID, 0, 8, orBuf);
  
  if(CAN.checkReceive()== CAN_MSGAVAIL)      // Check to see whether data is received.
  {
    len = 0;
    memcpy(m_cData, 0, 8);          // DESC : memcpy(destination,source,size); set '0' to variable
    CAN.readMsgBuf(&len, m_cData);  // DESC : readMsgBuf(read length, read data)> read data from the CAN bus
  }
}

void obd2racingmod_h::setModePid(INT8U cMode, INT8U cPid)
{
  m_cMode = cMode;
  m_cPid  = cPid;
}

INT32U char_merger(INT8U iHigh, INT8U iLow)
{
  INT32U t = 0;
  t = iHigh;
  t<<=8;
  return (t+iLow);
}

INT32U obd2racingmod_h::getRevData()
{
  setModePid(MODE1, PID_ENGREV);                             //Engine REV
  sendOrder();                                        //Send command to ECU and retrieve data by reference
  return (char_merger(m_cData[3], m_cData[4])/4);     //Return to main
}


INT32U obd2racingmod_h::getTempData()
{
  setModePid(MODE1, PID_ENGTEMP);       //Coolant temperature
  sendOrder();                  //Send command to ECU and retrieve data by reference
  return (m_cData[3] - 40);     //Return to main
}

obd2racingmod_h OBD2MOD;
