#include <posapi.h>
#include <posapi_all.h>
#include <posapi_s80.h>
#include <string.h>
#include "rc4.h"
#include "Debug.h"
#include "TamsLite.h"
#include  "PostRequest.h"
#include  "Applib.h"
#include "tinyxml.h"
#include "tinyxml-dom.h"
#include "Hex2Bin.h"
#include "Keys.h"
#include "Printer.h"
#include "Bin2Hex.h"


extern  Device MyDevice;
#define NOT_FOUND -1
#define Count(ARRAY) (sizeof (ARRAY) / sizeof *(ARRAY))
int NewKey()
{
	int length  = -1;
	char  Xml[102400]  = {0};
	PostRequest("tams/tams/devinterface/newkey.php","","",Xml);
	length = strlen(Xml);
	if(length >0)
	{
		//Response Successfull
		return ProcessNewKeyResponse(Xml);
	}else
	{
		ErrorReceipt LocalErrorReceipt;
		LocalErrorReceipt.ErrorCode = -1;
		strncpy(LocalErrorReceipt.ErrorMessage,"Connection Failure",sizeof(LocalErrorReceipt.ErrorMessage)-1);
		strncpy(LocalErrorReceipt.FunctionName,"NEWKEY",sizeof(LocalErrorReceipt.FunctionName)-1);
		PrintErrorReceipt(LocalErrorReceipt);
		return -1;
	}
		
}
int GetKeys()
{

	int length  = -1;
	char  Xml[102400]  = {0};
	PostRequest("tams/tams/devinterface/getkeys.php","","",Xml);
	length = strlen(Xml);
	if(length >0)
	{
		//Response Successfull
		return ProcessGetKeysResponse(Xml);
	}else
	{
		ErrorReceipt LocalErrorReceipt;
		LocalErrorReceipt.ErrorCode = -1;
		strncpy(LocalErrorReceipt.ErrorMessage,"Connection Failure",sizeof(LocalErrorReceipt.ErrorMessage)-1);
		strncpy(LocalErrorReceipt.FunctionName,"GETKEYS",sizeof(LocalErrorReceipt.FunctionName)-1);
		PrintErrorReceipt(LocalErrorReceipt);
		return -1;
	}
}

static int ProcessNewKeyResponse(char *Xml)
{
	int length = 0;
	int pos = 0;
	int iRet = -1;
	char WorkingXml[10240] = {0};
	struct tx_node nodes[32]; 
	dom_node_t errorcode_nodes[1];
	dom_node_t errormessage_nodes[1];
	dom_node_t masterkey_nodes[1];
	dom_node_t Domnode;
	size_t i = 0;
	size_t count;
	char *tail;
	char Db[200];
	pos = mystrpos(Xml,"<newkey>");
		
		if(pos >0)
		{
			//Newkey gotten
			substring((size_t)pos,strlen(Xml),Xml,WorkingXml,strlen(WorkingXml));
			tail = tx_parse(WorkingXml, Count(nodes), nodes);
			count = dom_getElementsByTagName(nodes, "masterkey", Count(masterkey_nodes), masterkey_nodes);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"N%d",count);
    	for(; i < count; ++i)
		{
			
    		Domnode = masterkey_nodes[i];
			strcpy((char *)MyDevice.DeviceKeys.MasterKey,Domnode->value);
			SaveDevice(&MyDevice);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"MasterKey:%s",(char *)MyDevice.DeviceKeys.MasterKey);
			SendDebug(Db);
		}
		iRet = 0;
		
		
		return iRet;
		}
		pos = 0;
		pos = mystrpos(Xml,"<error>");
		if(pos >0)
		{
			ErrorReceipt LocalErrorReceipt;

			strncpy(LocalErrorReceipt.FunctionName,"NEWKEY",sizeof(LocalErrorReceipt.FunctionName)-1);
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
		SendDebug("NewKey:Request Failed");
		}
	return iRet;
}
static int ProcessGetKeysResponse(char * Xml)
{
	
	int length = 0;
	int pos = 0;
	int iRet=-1;
	char WorkingXml[102400]  = {0};
	struct tx_node nodes[200];
	dom_node_t errorcode_nodes[1];
	dom_node_t errormessage_nodes[1];
	dom_node_t cipher_nodes[10];
	dom_node_t Domnode;
	dom_node_t Key0Node[1];
	dom_node_t Key1Node[1];
	dom_node_t Key2Node[1];
	dom_node_t Key3Node[1];
	dom_node_t Key4Node[1];
	dom_node_t Key5Node[1];
	dom_node_t Key6Node[1];
	dom_node_t Key7Node[1];
	dom_node_t Key8Node[1];
	dom_node_t Key9Node[1];
	size_t i = 0;
	size_t count;
	char *tail;
	char Db[33];
	length = strlen(Xml);
	if(length >0)
	{

		//Response Successfull
		
		pos = mystrpos(Xml,"<getkeys>");
		
		if(pos >0)
		{
			
			substring((size_t)pos,strlen(Xml),Xml,WorkingXml,strlen(WorkingXml));
			tail = tx_parse(WorkingXml, Count(nodes), nodes);
			count = dom_getElementsByTagName(nodes, "cipher", Count(cipher_nodes), cipher_nodes);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"N%d",count);
			SendDebug(Db);
    	if(count >0 )
		{
			
			//Key0
    		Domnode = cipher_nodes[0];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key0Node), Key0Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key0Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key0[0],0,sizeof(MyDevice.DeviceKeys.Key0));
			strcpy((char *)&MyDevice.DeviceKeys.Key0[0],Db);
			SendDebug(Db);

			//Key1
    		Domnode = cipher_nodes[1];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key1Node), Key1Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key1Node[0]->value);
			SendDebug(Db);
			
			sprintf(&Db[0],"%s",Key1Node[0]->value);
			SendDebug(Db);
			
			memset((char *)&MyDevice.DeviceKeys.Key1[0],0,sizeof(MyDevice.DeviceKeys.Key1));
			strcpy((char *)&MyDevice.DeviceKeys.Key1[0],Db);
			//key2
			memset(&Domnode,0,sizeof(Domnode));
			Domnode = cipher_nodes[2];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key2Node), Key2Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"F%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key2Node[0]->value);
			SendDebug(Db);
			
			memset((char *)&MyDevice.DeviceKeys.Key2[0],0,sizeof(MyDevice.DeviceKeys.Key2));
			//strcpy((char *)&MyDevice.DeviceKeys.Key2[0],Db);
			
			//key 3
			Domnode = cipher_nodes[3];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key3Node), Key3Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key3Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key3[0],0,sizeof(MyDevice.DeviceKeys.Key3));
			strcpy((char *)&MyDevice.DeviceKeys.Key3[0],Db);
			SendDebug(Db);

			/*
			//key 4

			Domnode = cipher_nodes[4];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key4Node), Key4Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key4Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key4[0],0,sizeof(MyDevice.DeviceKeys.Key4));
			strcpy((char *)&MyDevice.DeviceKeys.Key4[0],Db);
			SendDebug(Db);
			//end
			*/
			//key 4

			Domnode = cipher_nodes[4];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key4Node), Key4Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key4Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key4[0],0,sizeof(MyDevice.DeviceKeys.Key4));
			strcpy((char *)&MyDevice.DeviceKeys.Key4[0],Db);
			SendDebug(Db);
			//end

			//key 5

			Domnode = cipher_nodes[5];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key5Node), Key5Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key5Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key5[0],0,sizeof(MyDevice.DeviceKeys.Key5));
			strcpy((char *)&MyDevice.DeviceKeys.Key5[0],Db);
			SendDebug(Db);
			//end

			//key 6

			Domnode = cipher_nodes[6];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key6Node), Key6Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key6Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key6[0],0,sizeof(MyDevice.DeviceKeys.Key6));
			strcpy((char *)&MyDevice.DeviceKeys.Key6[0],Db);
			SendDebug(Db);
			//end

			//key 7

			Domnode = cipher_nodes[7];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key7Node), Key7Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key7Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key7[0],0,sizeof(MyDevice.DeviceKeys.Key7));
			strcpy((char *)&MyDevice.DeviceKeys.Key7[0],Db);
			SendDebug(Db);
			//end

			//key 8

			Domnode = cipher_nodes[8];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key8Node), Key8Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key8Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key8[0],0,sizeof(MyDevice.DeviceKeys.Key8));
			strcpy((char *)&MyDevice.DeviceKeys.Key8[0],Db);
			SendDebug(Db);
			//end

			//key 9

			Domnode = cipher_nodes[9];
			count = dom_getElementsByTagName(Domnode, "key", Count(Key9Node), Key9Node);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"K%d",count);
			SendDebug(Db);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"%s",Key9Node[0]->value);

			memset((char *)&MyDevice.DeviceKeys.Key9[0],0,sizeof(MyDevice.DeviceKeys.Key9));
			strcpy((char *)&MyDevice.DeviceKeys.Key9[0],Db);
			SendDebug(Db);
			//end
			SaveDevice(&MyDevice);
			iRet = 0;
		}
		
		return iRet;
		}
		pos = 0;
		pos = mystrpos(Xml,"<error>");
		if(pos >0)
		{
			ErrorReceipt LocalErrorReceipt;

			strncpy(LocalErrorReceipt.FunctionName,"GETKEYS",sizeof(LocalErrorReceipt.FunctionName)-1);
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
			iRet = -1;
			SendDebug("GetKey:Request Failed");
		}
	}else
	{
		iRet = -1;
		SendDebug("GetKey:Request Failed");
		return iRet;
	}
	
	return iRet;	
}

int GetKey(int Key ,unsigned char KeyBin[50])
{
	Device  MyLocalDevice = {0};
			ReadDevice(&MyLocalDevice);
switch(Key)
{
case 1:
	{
	char MasterKey[50]  ={0};
	char TempTerminalId[50] = {0};
	char TerminalId[50] = {0};
	//char TerminalId[]="30303030303030303132333435363738";
	unsigned char TerminalConcat[200] = {0}, BinTerminalConat[100] = {0};
	unsigned char Key0[50] = {0}, binKey0[50]={0};
	unsigned char Key1[50]={0}, binKey1[50]={0};
	unsigned char Key2[50] = {0}, binKey2[50]={0};
	rc4_key myRc4Key = {0};
	sprintf(TempTerminalId,"%016s",MyLocalDevice.TerminalId);
	Bin2Hex(TempTerminalId,sizeof(TempTerminalId),TerminalId,sizeof(TerminalId));
	strncpy((char *)MasterKey,MyDevice.DeviceKeys.MasterKey,(sizeof(MasterKey)-1));
	sprintf((char*)TerminalConcat,"%s%s",MasterKey,TerminalId);
	hex2bin((char*)TerminalConcat,(char*)BinTerminalConat,strlen((char*)TerminalConcat));
	prepare_key(BinTerminalConat,32,&myRc4Key);
	
		
	strncpy(Key0,MyDevice.DeviceKeys.Key0,(sizeof(Key0)-1));
					
  strncpy(Key1,MyDevice.DeviceKeys.Key1,(sizeof(Key1)-1));
				
 strncpy(Key2,MyDevice.DeviceKeys.Key2,(sizeof(Key2)-1));
	hex2bin(Key0,binKey1,strlen(Key0));
	rc4(binKey0,16,&myRc4Key);

	hex2bin(Key1,KeyBin,strlen(Key1));
	rc4(KeyBin,16,&myRc4Key);
				//ARC4_Update(binKey1,16,&ARC4Key);
				//DumpMem(binKey1,16);
				hex2bin(Key2,binKey2,strlen(Key2));
				rc4(binKey2,16,&myRc4Key);
				//ARC4_Update(binKey2,16,&ARC4Key);

				//DumpMem(binKey2,16);
		
	return 0;
	}
	
default:
		return -1;
}
}