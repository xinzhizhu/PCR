#ifndef _PID_H_
#define _PID_H_
#include "pcr.h"
#include "tmp.h"
#define PID_OUTA 4
#define PID_OUTB 2
#define PID_PWM 9

#define ERROR_TEMP 1

#define CYCLE_START_TOLERANCE 0.2
#define LID_START_TOLERANCE 1.0

#define PLATE_PID_INC_NORM_P 1000
#define PLATE_PID_INC_NORM_I 250
#define PLATE_PID_INC_NORM_D 250

#define PLATE_PID_INC_LOW_THRESHOLD 40
#define PLATE_PID_INC_LOW_P 600
#define PLATE_PID_INC_LOW_I 200
#define PLATE_PID_INC_LOW_D 400

#define PLATE_PID_DEC_HIGH_THRESHOLD 70
#define PLATE_PID_DEC_HIGH_P 800
#define PLATE_PID_DEC_HIGH_I 700
#define PLATE_PID_DEC_HIGH_D 300

#define PLATE_PID_DEC_NORM_P 500
#define PLATE_PID_DEC_NORM_I 400
#define PLATE_PID_DEC_NORM_D 200

#define PLATE_PID_DEC_LOW_THRESHOLD 35
#define PLATE_PID_DEC_LOW_P 2000
#define PLATE_PID_DEC_LOW_I 100
#define PLATE_PID_DEC_LOW_D 200

struct SPIDTuning {
  int maxValueInclusive;
  double kP;
  double kI;
  double kD;
};

enum ThermalDirection {
    OFF,
    HEAT,
    COOL
};

enum CYCLE_STATE {
  eTemp1,
  eTemp2,
  eTemp3,
  eTemp4,
  eEnd
};

int iCurDir=OFF;
int iCurRunTemp=eTemp1;

void SetPidTemp(int dir, int pwm) {
  if (dir == COOL) {
    digitalWrite(PID_OUTA, HIGH);
    digitalWrite(PID_OUTB, LOW);
    //digitalWrite(FAN_OUT,HIGH);
    //Serial.println("cool"); 
  } else if (dir == HEAT) {
    digitalWrite(PID_OUTA, LOW);
    digitalWrite(PID_OUTB, HIGH);
    //digitalWrite(FAN_OUT,LOW);
    //Serial.println("heat"); 
  } else {
    digitalWrite(PID_OUTA, LOW);
    digitalWrite(PID_OUTB, LOW);
    //digitalWrite(FAN_OUT,LOW);
    //Serial.println("off"); 
  }
  
  analogWrite(PID_PWM, pwm);
}

void SetTempState(double dTemp)
{
  if(iStartPoint==0)
  {
    if((iCurTemp>=(dTemp-ERROR_TEMP))&&(iCurTemp<=(dTemp+ERROR_TEMP)))
    {
      iStartPoint=1;
      iStartTime=millis();
      iCurTime=millis();
      Serial.print("start temp:"); 
      Serial.println(".");
    }
  }
  if(iCurTemp<=(dTemp-ERROR_TEMP))
  {
    SetPidTemp(HEAT, 255);
    
    //Serial.println("heat");
  }
  else if(iCurTemp>=(dTemp+ERROR_TEMP))
  {
    //digitalWrite(FAN_OUT,HIGH);
    SetPidTemp(COOL, 255);
    //Serial.println("out");
    //Serial.println("cool");
  }
  else
  {
    SetPidTemp(OFF, 0);
    //digitalWrite(FAN_OUT,LOW);
    //Serial.println("off");
    //iCurTime=millis();
  }
}

void SetPid()
{
  
  if(iStartPoint==0)
  {
      iStartTime=millis();
  }
  iCurTime=millis();
  if(iCurRunTemp==eTemp1)
  {
    //Serial.println("in temp1");  
    if(sPcr_Set.sSet1.iTime>(iCurTime-iStartTime))
    {
      //Serial.println("in work");
      SetTempState(sPcr_Set.sSet1.dTemp);
    }
    else
    {
      iCurRunTemp=eTemp2;
      iStartTime=millis();
      iStartPoint=0;
      Serial.println("out");
    }
   //SetPidTemp(dir, pwm);
  }
  else if(iCurRunTemp==eTemp2)
  {
    if(sPcr_Set.sSet2.iTime>(iCurTime-iStartTime))
    {
      SetTempState(sPcr_Set.sSet2.dTemp);
    }
    else
    {
      iCurRunTemp=eTemp3;
      //iStartTime=millis();
      iStartPoint=0;
      Serial.println("out");
    }   
  }
  else if(iCurRunTemp==eTemp3)
  {
   if(sPcr_Set.sSet3.iTime>(iCurTime-iStartTime))
    {
      SetTempState(sPcr_Set.sSet3.dTemp);
    }
    else
    {
      iCurRunTemp=eTemp4;
      //iStartTime=millis();
      iStartPoint=0;
      Serial.println("out");
    }
  }
  else if(iCurRunTemp==eTemp4)
  {
    if(sPcr_Set.sSet4.iTime>(iCurTime-iStartTime))
    {
      SetTempState(sPcr_Set.sSet4.dTemp);
    }
    else
    {
      iCurRunTemp=eEnd;
      //iStartTime=millis();
      iStartPoint=0;
      sPcr_Set.iCycle--;
      Serial.println("out");
    }
  }
  else if(iCurRunTemp==eEnd)
  {
    if(sPcr_Set.iCycle>0)
    {
      iCurRunTemp=eTemp1;
      //iStartTime=millis();
      iStartPoint=0;
      Serial.println("out");;
    }
    else
    {
      eCurState = eWAIT;
      sPcr_Set.bRun=false;
      iStartPoint=0;
      Serial.println("end");
    }
  }
}

void pid_init()
{
  // Peltier PWM
  TCCR1A |= (1<<WGM11) | (1<<WGM10);
  TCCR1B = _BV(CS21);

  iCurDir=OFF;
  pinMode(PID_OUTA,OUTPUT);
  pinMode(PID_OUTB,OUTPUT);
  SetPidTemp(iCurDir, 0);
  //digitalWrite(PID_OUTA,LOW);
  //digitalWrite(PID_OUTB,LOW);
  //analogWrite(9, 0);
}

#endif
