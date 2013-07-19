#ifndef  _MAGSTRIPE_H

#define  _MAGSTRIPE_H
typedef struct
{
	char Pan[50];
	char ExpiryDate[50];
	char Cvv[50];
	char SRV[50];
}TrackTwo;
int ParseTrackTwo(char *TrackTwoData,TrackTwo *OutPut,int IsChip);

#endif