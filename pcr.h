#ifndef _PCR_H_
#define _PCR_H_

enum PCRSTATE {
  eWAIT = 0,
  eRUN,
  eFINISH
};

struct set_par{
  double dTemp;
  unsigned long iTime;
};

struct pcr_set_point{
  bool bRun;
  int iCycle;
  set_par sSet1;
  set_par sSet2;
  set_par sSet3;
  set_par sSet4;
};

int readPhone(pcr_set_point set)
{
  set.sSet1.iTime=1;
  return 1;
}
//TEST
pcr_set_point sPcr_Set={0,4,{0,0},{0,0},{0,0},{0,0}};//4 times for cycle
//pcr_set_point sPcr_Set={1,4,{30,5000},{60,5000},{40,5000},{30,5000}};

PCRSTATE eCurState = eWAIT;
double iCurTemp=0;
double iPreTemp=0;
unsigned long iStartTime=0;
unsigned long iCurTime=0;
int iStartPoint=0;
#endif
