//Libraries
#include "obd2racingMod.h"
#include "obd2racingmod_def.h"


//Setup Configuration
void setup()
{
#if DEBUGMODE
  Serial.begin(115200);
#endif
  OBD2MOD.init();
}

//Loop
void loop()
{
    OBD2MOD.showValLCD("REV  : ",OBD2MOD.getRevData());
    OBD2MOD.showValLCD("TEMP : ",OBD2MOD.getTempData());
}
