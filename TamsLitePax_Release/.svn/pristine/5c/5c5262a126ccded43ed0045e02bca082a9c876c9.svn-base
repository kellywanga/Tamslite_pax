#include <posapi.h>
#include <posapi_all.h>
#include "Applib.h"
#include "TamsLite.h"

int Bin2Hex(char *theData, int theDataLen, char *hexEncodedStr, int theHexLen)
{
int k;
int index = 0;
char hexLookUp[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
if (theHexLen <= 0)
{return 0;}
memset(hexEncodedStr,'\0',theHexLen);	

for( k=0,index=0; (k<theDataLen) && ((index=(k*2)+1) < theHexLen) ; k++)
{
hexEncodedStr[index-1]=hexLookUp[theData[k]/16];
hexEncodedStr[index]=hexLookUp[theData[k]%16];
}
return 1;
}