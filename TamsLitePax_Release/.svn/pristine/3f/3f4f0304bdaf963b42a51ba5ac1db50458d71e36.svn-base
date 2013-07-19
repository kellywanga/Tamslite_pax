#ifndef _EMVAPPLIST_H
#define _EMVAPPLIST_H

#include "emvlib.h"
//typedef struct{ 
//  int  Index;
//  char AID[20];
//  int  Match;
//  int  IRN;
//  char Name[50];
//  char AppVersion[15];
//  int Priority;
//  char DDOL[50];
//  char TDOL[50];
//  char TFLDOM[10];
//  char TFLINT[10];
//  char THRESHOLDDOM[10];
//  char THRESHOLDINT[10];
//  char MAXTARGETDOM[5];
//  char MAXTARGETINT[5];
//  char TARGETPERCENTDOM[5];
//  char TARGETPERCENTINT[5];
//  char TACDEFAULT[15];
//  char TACDENIAL[15];
//  char TACONLINE[15];
//}EMVApp;

int   GetEmvApplist();
int   SaveEMVApplistToFile (EMV_APPLIST * MyEmvApplist);
int   ReadEMVApplistToFile(EMV_APPLIST * MyEmvApplist);

static int ProcessResponse(char* Xml);
static int AddEmvApplication(EMV_APPLIST MyEmvApplication);

#endif