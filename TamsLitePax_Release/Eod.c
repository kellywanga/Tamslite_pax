#include <posapi.h>
#include <posapi_all.h>
#include "TamsLite.h"
#include "PostRequest.h"
#include "Debug.h"
#include "tinyxml.h"
#include "tinyxml-dom.h"
#include "Md5.h"
#include "Eod.h"
#include "Printer.h"
extern BatchInformation MyBatchInfo;
#define NOT_FOUND -1
#define Count(ARRAY) (sizeof (ARRAY) / sizeof *(ARRAY))
int  EndOfDay()
{
	int length  = -1;
	char  Xml[10240]  = {0};
	char  RequestData[9024] = {0};
	char  AuthenticationData[200] = {0};
	char *Pdata[11] = {0};
	char T[500] = {0};
	char A[500] = {0} ;
	char BatchNo[500] = {0};
	char PC[50] = {0};
	char PV[50] = {0};
	char PRC[50] = {0};
	char PRV[50] = {0};
	char RC[50] = {0};
	char RV[50] = {0};
	char RRC[50] = {0};
	char RRV[50] = {0};

	sprintf(T,"%d",MyBatchInfo.AllTransactionCount);
	sprintf(A,"%d",MyBatchInfo.AllTransactionTotalValue);
	sprintf(BatchNo,"%d",MyBatchInfo.BatchNo);
	sprintf(PC,"%d",MyBatchInfo.PurchaseTransactionCount);
	sprintf(PV,"%d",MyBatchInfo.PurchaseTransactionTotalValue);
	sprintf(PRC,"%d",MyBatchInfo.PurchaseReversalTransactionCount);
	sprintf(PRV,"%d",MyBatchInfo.PurchaseReversakTransactionValue);
	sprintf(RC,"%d",MyBatchInfo.RefundTransactionCount);
	sprintf(RV,"%d",MyBatchInfo.RefundTransactionTotalValue);
	sprintf(RRC,"%d",MyBatchInfo.RefundReversalTransactionCount);
	sprintf(RRV,"%d",MyBatchInfo.RefundReversalTransactionTotalValue);
	
	 Pdata[0]   = T;
	 Pdata[1]	= A;
	 Pdata[2]	= BatchNo;
	 Pdata[3]	= PC;
	 Pdata[4]	= PV;
	 Pdata[5]	= PRC;
	 Pdata[6]	= PRV;
	 Pdata[7]	= RC;
	 Pdata[8]	= RV;
	 Pdata[9]	= RRC;
	 Pdata[10]	= RRV;
	 GetHash(Pdata,11,AuthenticationData);
	 sprintf(RequestData,"T=%d&A=%d&batchno=%d&PC=%d&PV=%d&PRC=%d&PRV=%d&RC=%d&RV=%d&RRC=%d&RRV=%d",
		 MyBatchInfo.AllTransactionCount,
		 MyBatchInfo.AllTransactionTotalValue,
		 MyBatchInfo.BatchNo,
		 MyBatchInfo.PurchaseTransactionCount,
		 MyBatchInfo.PurchaseTransactionTotalValue,
		 MyBatchInfo.PurchaseReversalTransactionCount,
		 MyBatchInfo.PurchaseReversakTransactionValue,
		 MyBatchInfo.RefundTransactionCount,
		 MyBatchInfo.RefundTransactionTotalValue,
		 MyBatchInfo.RefundReversalTransactionCount,
		 MyBatchInfo.RefundReversalTransactionTotalValue
		 );

	PostRequest("tams/eftpos/devinterface/efttotals.php",RequestData,AuthenticationData,Xml);
	length = strlen(Xml);
	if(length >0)
	{
		//Response Successfull
		return ProcessResponseEod(Xml);
	}else
	{
		ErrorReceipt LocalErrorReceipt;
		LocalErrorReceipt.ErrorCode = -1;
		strncpy(LocalErrorReceipt.ErrorMessage,"Connection Failure",sizeof(LocalErrorReceipt.ErrorMessage)-1);
		strncpy(LocalErrorReceipt.FunctionName,"EOD",sizeof(LocalErrorReceipt.FunctionName)-1);
		PrintErrorReceipt(LocalErrorReceipt);
		return -1;
	}
}
static int ProcessResponseEod(char *Xml)
{
	int length = 0;
	int pos = 0;
	int iRet = -1;
	char WorkingXml[10240] = {0};
	struct tx_node nodes[32]; 
	dom_node_t errorcode_nodes[1]; 
	dom_node_t errormessage_nodes[1]; 
	dom_node_t result_nodes[1];
	dom_node_t batchno_nodes[1];
	dom_node_t Domnode;
	
	size_t i = 0;
	size_t count;
	char *tail;
	char Db[200];
	pos = mystrpos(Xml,"<efttotals>");
		
		if(pos >0)
		{
			//merchantid gotten
			substring((size_t)pos,strlen(Xml),Xml,WorkingXml,strlen(WorkingXml));
			tail = tx_parse(WorkingXml, Count(nodes), nodes);
			count = dom_getElementsByTagName(nodes, "result", Count(result_nodes), result_nodes);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"N%d",count);
    	for(; i < count; ++i)
		{
			
    		Domnode = result_nodes[i];
			//strcpy((char *)MyDevice.MerchantId,Domnode->value);
			//Domnode->value;
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"ResultCode:%s",Domnode->value);
			SendDebug(Db);
		}
		count = 0;
		i=0;
		//terminal date and time
		count = dom_getElementsByTagName(nodes, "batchno", Count(batchno_nodes), batchno_nodes);
		for(; i < count; ++i)
		{
			
    		Domnode = batchno_nodes[i];
			//strcpy((char *)MyDevice.MerchantId,Domnode->value);
			//Domnode->value;
			memset(&MyBatchInfo,0,sizeof(MyBatchInfo));
			MyBatchInfo.BatchNo = atoi((void *)Domnode->value);
			SaveBatchInfo(&MyBatchInfo);
			memset(&Db[0],0,sizeof(Db));
			sprintf(&Db[0],"Batchno:%s",Domnode->value);
			SendDebug(Db);
			return 0;
		}
		
		}
		pos = 0;
		pos = mystrpos(Xml,"<error>");
		if(pos >0)
		{
			ErrorReceipt LocalErrorReceipt;

			strncpy(LocalErrorReceipt.FunctionName,"EOD",sizeof(LocalErrorReceipt.FunctionName)-1);
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
		}
	else
	{
		
		
		return iRet;
	}
	
	return iRet;	
}
