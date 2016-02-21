#include "pid.h"
#include "tmp.h"
#include "pcr.h"
#include "bluetooth_pcr.h"

//unsigned long ulSendTime=0;
unsigned long ulPreSendTime=0;

void setup() {
  init_bluetooth();
  tmp_init();
  pid_init();
  iCurTemp=iTemp;
  ulPreSendTime=millis();
  Serial.println("start pcr"); 
}

void loop() {
  ReadTemp();
  iCurTemp=iTemp;
  if((millis()-ulPreSendTime)>5000)
  {
    SendData();
    ulPreSendTime=millis();
  }
  switch(eCurState)
  {
    case eWAIT:
      ReadSetCmd();
      //SendData();
      if(sPcr_Set.bRun==true)
      {
        eCurState=eRUN;
        sPcr_Set.iCycle=4;
        //iStartTime=millis();
        iStartPoint=0;
        iCurRunTemp=eTemp1;
      }
      break;
    case eRUN:
      ReadSetCmd();
      //SendData();
      if(sPcr_Set.bRun==false)
      {
        eCurState=eWAIT;
      }
      else
      {
        iPreTemp=iCurTemp;
        iCurTemp=iTemp;
        SetPid();
      }
      break;
    default:
      break;
  }
}




