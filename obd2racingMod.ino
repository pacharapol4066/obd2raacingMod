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

}
