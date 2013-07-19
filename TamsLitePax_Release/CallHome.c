#include <posapi.h>
#include <posapi_all.h>
#include "Applib.h"
#include "TamsLite.h"
#include "Md5.h"
#include "PostRequest.h"
#include  "CallHome.h"
#include   "Printer.h"

int CallHome()
{
	int length  = -1;
	char  Xml[102400]  = {0};
	char  RequestData[102400] = {0};
	char  AuthenticationData[200] = {0};
	char  CallHomeData[200] = {0};
	char *Pdata[1] = {0};
	sprintf(CallHomeData,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s","22222","12345678","","","","","","","","","","","","","","","","","","","","","","","");
	 Pdata[0] = CallHomeData;
	 GetHash(Pdata,1,AuthenticationData);
	sprintf(RequestData,"T[]=%s",CallHomeData);
	
	PostRequest("tams/tams/devinterface/callhome.php",RequestData,AuthenticationData,Xml);
	length = strlen(Xml);
	if(length >0)
	{
		//Response Successfull
		return ProcessResponse(Xml);
	}else
	{
		ErrorReceipt LocalErrorReceipt;
		LocalErrorReceipt.ErrorCode = -1;
		strncpy(LocalErrorReceipt.ErrorMessage,"Connection Failure",sizeof(LocalErrorReceipt.ErrorMessage)-1);
		strncpy(LocalErrorReceipt.FunctionName,"CALLHOME",sizeof(LocalErrorReceipt.FunctionName)-1);
		PrintErrorReceipt(LocalErrorReceipt);
		return -1;
	}
}
static int ProcessResponse(char *Xml)
{
	return 0;
}