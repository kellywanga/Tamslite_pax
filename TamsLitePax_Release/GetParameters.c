#include <posapi.h>
#include <posapi_all.h>
#include "TamsLite.h"
#include "PostRequest.h"
#include  "GetParameters.h"
#include "Debug.h"
#include "tinyxml.h"
#include "tinyxml-dom.h"
#include  "Md5.h"
#include  "Printer.h"

extern  Device MyDevice;
#define NOT_FOUND -1
#define Count(ARRAY) (sizeof (ARRAY) / sizeof *(ARRAY))
int  GetParameters()
{
	int length  = -1;
	char  Xml[102400]  = {0};
	char  RequestData[102400] = {0};
	char  AuthenticationData[200] = {0};
	char *Pdata[3] = {0};
	 Pdata[0] = "7.6";
	 Pdata[1] = "2345";
	 GetHash(Pdata,2,AuthenticationData);
	sprintf(RequestData,"ver=%s&serial=%s","7.6","2345");
	
	PostRequest("tams/tams/devinterface/getparams.php",RequestData,AuthenticationData,Xml);
	length = strlen(Xml);
	if(length >0)
	{
		//Response Successfull
		return ProcessResponseGp(Xml);
	}else
	{
		ErrorReceipt LocalErrorReceipt;
		LocalErrorReceipt.ErrorCode = -1;
		strncpy(LocalErrorReceipt.ErrorMessage,"Connection Failure",sizeof(LocalErrorReceipt.ErrorMessage)-1);
		strncpy(LocalErrorReceipt.FunctionName,"GETPARAMETERS",sizeof(LocalErrorReceipt.FunctionName)-1);
		PrintErrorReceipt(LocalErrorReceipt);
		return -1;
	}
}

static int ProcessResponseGp(char *Xml)
{
	int length = 0;
	int pos = 0;
	int iRet = -1;
	char WorkingXml[10240] = {0};
	struct tx_node nodes[32]; 
	dom_node_t errorcode_nodes[1]; 
	dom_node_t errormessage_nodes[1]; 
	dom_node_t merchantid_nodes[1];
	dom_node_t datetime_nodes[1];
	dom_node_t Domnode;
	dom_node_t year[1];
	dom_node_t syear[1];
	dom_node_t mon[1];
	dom_node_t day[1];
	dom_node_t hour[1];
	dom_node_t min[1];
	dom_node_t sec[1];
	size_t i = 0;
	size_t count;
	char *tail;
	char Db[200];
	pos = mystrpos(Xml,"<param>");
		
		if(pos >0)
		{
			//merchantid gotten
			substring((size_t)pos,strlen(Xml),Xml,WorkingXml,strlen(WorkingXml));
			tail = tx_parse(WorkingXml, Count(nodes), nodes);
			count = dom_getElementsByTagName(nodes, "merchantid", Count(merchantid_nodes), merchantid_nodes);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"N%d",count);
    	for(; i < count; ++i)
		{
			
    		Domnode = merchantid_nodes[i];
			strcpy((char *)MyDevice.MerchantId,Domnode->value);
			SaveDevice(&MyDevice);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"MerchantId:%s",(char *)MyDevice.MerchantId);
			SendDebug(Db);
		}
		count = 0;
		//terminal date and time
		count = dom_getElementsByTagName(nodes, "datetime", Count(datetime_nodes), datetime_nodes);
		
		if(count >0 )
		{
			
			//Year
    		Domnode = datetime_nodes[0];
			count = dom_getElementsByTagName(Domnode, "year", Count(year), year);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",year[0]->value);

			//sYear
    		//Domnode = datetime_nodes[1];
			count = dom_getElementsByTagName(Domnode, "syear", Count(syear), syear);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",syear[0]->value);


			//mon
    		//Domnode = datetime_nodes[2];
			count = dom_getElementsByTagName(Domnode, "mon", Count(mon), mon);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",mon[0]->value);


			//day
    		//Domnode = datetime_nodes[3];
			count = dom_getElementsByTagName(Domnode, "day", Count(day), day);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",day[0]->value);



			//hour
    		//Domnode = datetime_nodes[4];
			count = dom_getElementsByTagName(Domnode, "hour", Count(hour), hour);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",hour[0]->value);

			//min
    		//Domnode = datetime_nodes[5];
			count = dom_getElementsByTagName(Domnode, "min", Count(min), min);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",min[0]->value);


			//sec
    		//Domnode = datetime_nodes[4];
			count = dom_getElementsByTagName(Domnode, "sec", Count(sec), sec);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",sec[0]->value);


			//SETDATE AND TIME HERE
			//SetTime();
		}
		iRet = 0;
		
		SaveDevice(&MyDevice);
		return iRet;
		}
		pos = mystrpos(Xml,"<error>");
		if(pos >0)
		{
			ErrorReceipt LocalErrorReceipt;

			strncpy(LocalErrorReceipt.FunctionName,"GETPARAMETERS",sizeof(LocalErrorReceipt.FunctionName)-1);
			//error
			substring((size_t)pos,strlen(Xml),Xml,WorkingXml,strlen(WorkingXml));
			tail = tx_parse(WorkingXml, Count(nodes), nodes);

			//Error Code
			count = dom_getElementsByTagName(nodes, "errcode", Count(errorcode_nodes), errorcode_nodes);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"N%d",count);
    	for(; i < count; ++i)
		{
			
    		Domnode = errorcode_nodes[i];
			LocalErrorReceipt.ErrorCode = atoi((void *)Domnode->value);
			SendDebug(Domnode->value);
		}


		//Error Message
		count = dom_getElementsByTagName(nodes, "errmsg", Count(errormessage_nodes), errormessage_nodes);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"N%d",count);
    	for(; i < count; ++i)
		{
			
    		Domnode = errorcode_nodes[i];
			strncpy(LocalErrorReceipt.ErrorMessage,Domnode->value,sizeof(LocalErrorReceipt.ErrorMessage)-1);
			SendDebug(Domnode->value);
		}

		PrintErrorReceipt(LocalErrorReceipt);
		iRet=-1;
		return iRet;
		}else
		{
		iRet=-1;
		SendDebug("GETPARAMETERS:Request Failed");
		}
	return iRet;
}