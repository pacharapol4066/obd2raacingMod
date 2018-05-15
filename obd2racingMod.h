#ifndef _OBD_II_RC_H_
#define _OBD_II_RC_H_
#include <Arduino.h>
#include "obd2racingmod_def.h"

class obd2racingmod_h
{
  private:
  INT8U m_cMode;
  INT8U m_cPid;
  INT8U m_cData[8];
  
  private:
  void sendOrder();
  void setModePid(INT8U cMode, INT8U cPid);
  void showTextLCD(String line1,String line2);
  
  public:
  void init(void);
  //INT32U getData(INT8U type);

};

extern obd2racingmod_h OBD2MOD;
#endif
