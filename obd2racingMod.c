//Libraries
#include <mcp_can.h>            //CAN Bus Shield Compatibility Library
#include <SPI.h>                //CAN Bus Shield SPI Pin Library
#include <Wire.h>               //Extension Library for measuring current
#include <LCD.h>
#include <LiquidCrystal_I2C.h>  //Libraries for controlling LC Displays               
#include <LiquidCrystal.h>


//Definition
#define lo8(x) ((int)(x)&0xff)
#define hi8(x) ((int)(x)>>8)

//Variables LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Addr, En, Rw, Rs, d4, d5, d6, d7, backlightpin, polarity

/Variables CAN BUS
/* the cs pin of the version after v1.1 is default to D9
v0.9b and v1.0 is default D10 */
const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);  // Set CS pin

//Setup Configuration
void setup()
{
    //Initialize LCD
    lcd.begin(16,2);
    lcd.backlight();
    showLCD("Hilux Vigo Dashboard","by VT11257");
    delay(2000);

    showLCD("Hilux Vigo Dashboard","booting...");

    //Begin with Serial Connection
    Serial.begin(115200);

    //Begin with CAN Bus Initialization
  START_INIT:

    if(CAN_OK == CAN.begin(CAN_500KBPS))                   // init can bus : baudrate = 500k
    {
        Serial.println("CAN BUS Shield init ok!");
    }
    else
    {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
        goto START_INIT;
    }
}

//Reload LC Display
void showLCD(String line1,String line2)
{  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

//Send CAN Command (short version)
void CanSend(short address, byte a, byte b, byte c, byte d, byte e, byte f, byte g, byte h)
{
  unsigned char DataToSend[8] = {a,b,c,d,e,f,g,h};
  CAN.sendMsgBuf(address, 0, 8, DataToSend);
}

//Loop
void loop()
{

}