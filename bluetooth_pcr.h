#ifndef _BLUETOOTH_PCR_H_
#define _BLUETOOTH_PCR_H_
#include "pcr.h"

char cGetCmd[30]={0};
int i=0;

void init_bluetooth()
{
  Serial.begin(115200);
}
void setCmd(char* cGetData)
{
  double dTemTemp1=0;
  double dTemTemp2=0;
  double dTemTemp3=0;
  double dTemTemp4=0;
  int iTemTime1=0;
  int iTemTime2=0;
  int iTemTime3=0;
  int iTemTime4=0;
  int iRight=0;
  int j=0;
  
  if(cGetData[j]=='A')
  {
    j++;
    while((cGetData[j]>='0')&&(cGetData[j]<='9'))
    {
      dTemTemp1=dTemTemp1*10+cGetData[j]-48;
      j++;
    }
    if(cGetData[j]=='T')
    {
      j++;
      while((cGetData[j]>='0')&&(cGetData[j]<='9'))
      {
        iTemTime1=iTemTime1*10+cGetData[j]-48;
        j++;
      }
      iRight=1;
    }
    if(iRight==1)
    {
      iRight=0;
      if(cGetData[j]=='B')
      {
        j++;
        while((cGetData[j]>='0')&&(cGetData[j]<='9'))
        {
          dTemTemp2=dTemTemp2*10+cGetData[j]-48;
          j++;
        }
        if(cGetData[j]=='T')
        {
          j++;
          while((cGetData[j]>='0')&&(cGetData[j]<='9'))
          {
            iTemTime2=iTemTime2*10+cGetData[j]-48;
            j++;
          }
          iRight=1;
        }
      }
    }
    if(iRight==1)
    {
      iRight=0;
      if(cGetData[j]=='C')
      {
        j++;
        while((cGetData[j]>='0')&&(cGetData[j]<='9'))
        {
          dTemTemp3=dTemTemp3*10+cGetData[j]-48;
          j++;
        }
        if(cGetData[j]=='T')
        {
          j++;
          while((cGetData[j]>='0')&&(cGetData[j]<='9'))
          {
            iTemTime3=iTemTime3*10+cGetData[j]-48;
            j++;
          }
          iRight=1;
        }
      }
    }
    if(iRight==1)
    {
      iRight=0;
      if(cGetData[j]=='D')
      {
        j++;
        while((cGetData[j]>='0')&&(cGetData[j]<='9'))
        {
          dTemTemp4=dTemTemp4*10+cGetData[j]-48;
          j++;
        }
        if(cGetData[j]=='T')
        {
          j++;
          while((cGetData[j]>='0')&&(cGetData[j]<='9'))
          {
            iTemTime4=iTemTime4*10+cGetData[j]-48;
            j++;
          }
          iRight=1;
        }
      }
    }
    if(iRight==1)
    {
      while(cGetData[j]!='\n')
      {
        j++;
      }
      sPcr_Set.sSet1.dTemp=dTemTemp1;
      sPcr_Set.sSet2.dTemp=dTemTemp2;
      sPcr_Set.sSet3.dTemp=dTemTemp3;
      sPcr_Set.sSet4.dTemp=dTemTemp4;
      sPcr_Set.sSet1.iTime=iTemTime1*1000;
      sPcr_Set.sSet2.iTime=iTemTime2*1000;
      sPcr_Set.sSet3.iTime=iTemTime3*1000;
      sPcr_Set.sSet4.iTime=iTemTime4*1000;
    }
  }
  else if(cGetData[j]=='Y')
  {
    int iTemCycle=sPcr_Set.iCycle;
    sPcr_Set.iCycle=0;
    j++;
    while(cGetData[j]!='\n')
    {
      sPcr_Set.iCycle=sPcr_Set.iCycle*10+cGetData[j]-48;
      j++;
    }
  }
  else if(cGetData[j]=='O')
  {
    int iTemRun=sPcr_Set.bRun;
    sPcr_Set.iCycle=0;
    j++;
    if(cGetData[j]=='N')
    {
      sPcr_Set.bRun=true;
    }
    else if(cGetData[j]=='F')
    {
      sPcr_Set.bRun=false;
    }
    j++;
    while(cGetData[j]!='\n')
    {
      j++;
    }
  }
}

void ReadSetCmd()
{
  if(Serial.available())
  {
    cGetCmd[i]=Serial.read();

    if(cGetCmd[i]=='\n')
    {
      i=0;
      //Serial.println("enter");
      //Serial.println(cGetCmd);
      setCmd(cGetCmd);
      Serial.print("CMD:"); 
      Serial.println(cGetCmd);
      //sPcr_Set.sSet4.iTime=sPcr_Set.sSet4.iTime*1000*60;
    }
    else
    {
      i++;
      if(i>=30)
      {
        i=0;
      }
    }
  }
}

void SendData()
{
  Serial.print("TEMP"); 
  Serial.println((int)iCurTemp); 
  //Serial.print("TIME"); 
  //Serial.println((unsigned long)((iCurTime-iStartTime)/1000)); 
}
#endif
