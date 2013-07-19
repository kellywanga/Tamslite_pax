#include <posapi.h>
#include <posapi_all.h>
#include "MagStripe.h"
#include "TamsLite.h"
int ParseTrackTwo(char *TrackTwoData,TrackTwo *OutPut,int IsChip)
{
	char PAN[50] = {0};
	char EXP[10] = {0};
	char SRV[10] = {0};
	int StartPos =-1;
	int SepPos = -1;
	if(IsChip == 1)
	{
	StartPos = mystrpos(TrackTwoData,";");
	SepPos =  mystrpos(TrackTwoData,"D");
	}else
	{
		StartPos = mystrpos(TrackTwoData,";");
		SepPos =  mystrpos(TrackTwoData,"=");
	}
	if(strlen(TrackTwoData) >0)
	{
	
	substring((StartPos+1),SepPos,TrackTwoData,PAN,strlen(PAN));
	substring(SepPos,(SepPos+5),TrackTwoData,EXP,strlen(EXP));
	substring((SepPos+4),(SepPos+7),TrackTwoData,SRV,strlen(SRV));
	}
	strcpy(OutPut->Pan,PAN);
	strcpy(OutPut->ExpiryDate,EXP);
	strcpy(OutPut->SRV,SRV);
}