#include <posapi.h>
#include <posapi_all.h>
#include "TamsLite.h"
#include "PostRequest.h"
#include  "Purchase.h"
#include  "md5.h"
#include "tinyxml.h"
#include "tinyxml-dom.h"
#include  "Debug.h"
#include "EftUtil.h"
#include "Printer.h"

#define Count(ARRAY) (sizeof (ARRAY) / sizeof *(ARRAY))

int Purchase( EftTransaction *  MyEftTransaction)
{
	int length  = -1;
	char  Xml[1024]  = {0};
	char  RequestData[70000] = {0};
	char  AuthenticationData[200] = {0};
	char  PurchaseData[50000] = {0};
	char *Pdata[1] = {0};
	char TransactionType[10] = {0};
	char BatchNo[10] = {0};
	char SequenceNo[10] = {0};
	char MerchantId[10] = {0};
	char TransactionDate[10] = {0};
	char TrackTwo[102] = {0};
	char Cvv[10] = {0};
	char Operator[10] = {0};
	char Supervisor[10] = {0};
	char Amount[10] = {0};
	char CashBack[10] = {0};
	char Tip[10] = {0};
	char Budget[10] = {0};
	char AccountType[10] = {0};
	char PinBlock[50] = {0};
	char KSN[50] = {0};
	char Offline[5] = {0};
	char Manual[5] = {0};
	char OriginalSequeneNo[10] = {0};
	char RSVD[2] = "";
	char RefrenceCode[10] = {0};
	char OriginalTerminalID[16] = {0};
	char ReversalDateAndTime[10] = {0};
	char ReversalTransactionType[10] = {0};
	char ReversalReferenceCode[10] = {0};
	char CurrencyCode[5] = {0};
	char ProcessorFee[10] = {0};
	char AcquiringFee[10] = {0};
	char AdditionalData[20] ={0};
	char ICCData[10240] = {0};
	char AuthId[10] = {0};
	BatchInformation LocalBatchInfo;
	sprintf(TransactionType,"%d",MyEftTransaction->TransType);
	sprintf(BatchNo,"%d",MyEftTransaction->BatchNo);
	sprintf(SequenceNo,"%d",MyEftTransaction->SequenceNo);
	sprintf(MerchantId,"%s",MyEftTransaction->MerchantId);
	sprintf(TransactionDate,"%s",MyEftTransaction->TransacDateAndTime);
	sprintf(TrackTwo,"%s",MyEftTransaction->Track2);
	sprintf(Cvv,"%s",MyEftTransaction->CVV);
	sprintf(Operator,"%d",MyEftTransaction->OperatorId);
	sprintf(Supervisor,"%s",MyEftTransaction->SupervisorFlag==0?"N":"Y" );
	sprintf(Amount,"%d",MyEftTransaction->PurchaseAmount);
	sprintf(CashBack,"%d",MyEftTransaction->CashBackAmount);
	sprintf(Tip,"%d",MyEftTransaction->TipAmount);
	sprintf(Budget,"%d",MyEftTransaction->BudgetPeriod);
	sprintf(AccountType,"%d|%d",MyEftTransaction->AcctType,0);
	if(strlen(MyEftTransaction->PinBlock) > 0)
	{
	sprintf(PinBlock,"%s",MyEftTransaction->PinBlock);
	}else
	{
		sprintf(PinBlock,"%s","OFFLINE");
	}
	sprintf(KSN,"%s",MyEftTransaction->PinSerial);
	sprintf(Offline,"%s",MyEftTransaction->Offline==0?"N":"Y" );
	sprintf(Manual,"%s",MyEftTransaction->ManualFlag==0?"N":"Y" );
	sprintf(OriginalSequeneNo,"%s","");
	sprintf(RefrenceCode,"%s",MyEftTransaction->RefCode);
	sprintf(OriginalTerminalID,"%s","");
	sprintf(ReversalDateAndTime,"%s","");
	sprintf(ReversalTransactionType,"%s","");
	sprintf(ReversalReferenceCode,"%s","");
	sprintf(CurrencyCode,"%s","566");
	sprintf(ProcessorFee,"%s","");
	sprintf(AcquiringFee,"%s","");
	sprintf(AdditionalData,"%s","");
	sprintf(ICCData,"%s",MyEftTransaction->ICCData);
	sprintf(AuthId,"%s","");
	sprintf(PurchaseData,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
		TransactionType,
		BatchNo,
		SequenceNo,
		MerchantId,
		TransactionDate,
		TrackTwo,
		Cvv,
		Operator,
		Supervisor,
		Amount,
		CashBack,
		Tip,
		Budget,
		AccountType,
		PinBlock,
		KSN,
		Offline,
		Manual,
		OriginalSequeneNo,
		RSVD,
		RefrenceCode,
		OriginalTerminalID,
		ReversalDateAndTime,
		ReversalTransactionType,
		ReversalReferenceCode,
		CurrencyCode,
		ProcessorFee,
		AcquiringFee,
		AdditionalData,
		RSVD,
		RSVD,
		RSVD,
		RSVD,
		ICCData,
		AuthId
		);
	 Pdata[0] = PurchaseData;
	 GetHash(Pdata,1,AuthenticationData);
	sprintf(RequestData,"T[]=%s",PurchaseData);
	EftSaveTransaction(MyEftTransaction);
	PostRequest("/tams/eftpos/devinterface/transaction.php",RequestData,AuthenticationData,Xml);
	length = strlen(Xml);
	if(length >0)
	{
		//Response Successfull
		int iResponseValue;
		
		iResponseValue = 	ProcessResponsePur(Xml,MyEftTransaction);
		return iResponseValue;
	}else
	{	
		//ReadBatchInfo(&LocalBatchInfo);
		//MyEftTransaction.MyResponse = ReverseTransaction;
		//changeEftTranStatus(LocalBatchInfo.PurchaseReversalTransactionCount,&MyEftTransaction);

		ErrorReceipt LocalErrorReceipt;
		LocalErrorReceipt.ErrorCode = -1;
		strncpy(LocalErrorReceipt.ErrorMessage,"Connection Failure",sizeof(LocalErrorReceipt.ErrorMessage)-1);
		strncpy(LocalErrorReceipt.FunctionName,"EOD",sizeof(LocalErrorReceipt.FunctionName)-1);
		PrintErrorReceipt(LocalErrorReceipt);
		return -1;
	}



}

static int ProcessResponsePur(char *Xml,EftTransaction  * MyEftTransaction)
{
	int length = 0;
	int pos = 0;
	int iRet = -1;
	char WorkingXml[10240] = {0};
	struct tx_node nodes[1000]; 
	dom_node_t errorcode_nodes[1]; 
	dom_node_t errormessage_nodes[1]; 
	dom_node_t trans_nodes[1];
	dom_node_t Domnode;
	dom_node_t result_dom[1];
	dom_node_t message_dom[1];
	dom_node_t status_dom[1];
	dom_node_t refno_dom[1];
	dom_node_t tranno_dom[1];
    dom_node_t batchno_dom[1];
	dom_node_t fromacc_dom[1];
    dom_node_t toacc_dom[1];
	dom_node_t balance_dom[1];
	dom_node_t available_dom[1];
    dom_node_t data_dom[1];
    dom_node_t iccresponse_dom[1];
	dom_node_t authid_dom[1];
	int result = -1;
	char message[50] = {0};
	char status[50] = {0};
	char refno[50] ={0};
	char tranno[50] = {0};
	int batchno = -1;
	char fromacc[50] = {0};
	char toacc[50] = {0};
	char balance[50] = {0};
	char available[50] = {0};
	char data[500] = {0};
	unsigned char iccresponse[500] = {0};
	char authid[20]  = {0};
	size_t i = 0;
	size_t count;
	size_t transcount;
	char *tail;
	char Db[500];
	pos = mystrpos(Xml,"<efttran>");
	
	if(pos >0)
	{
	substring((size_t)pos,strlen(Xml),Xml,WorkingXml,strlen(WorkingXml));
	memset(nodes,0,sizeof(nodes));
	tail = tx_parse(WorkingXml, Count(nodes), nodes);

	transcount = dom_getElementsByTagName(nodes,"<tran>", Count(trans_nodes), trans_nodes);
	memset(&Db[0],0,sizeof(Db));
	sprintf(&Db[0],"COUNT:%d",transcount);

	if(transcount> 0)
	{
		//TRansaction
		Domnode = trans_nodes[0];

		//result
		count = dom_getElementsByTagName(Domnode, "<result>", Count(result_dom), result_dom);
		if(count > 0)
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",result_dom[0]->value);
		result = atoi(&Db[0]);
		MyEftTransaction->cTranResult = result;
		count = 0;
		}


		//message
		count = dom_getElementsByTagName(Domnode, "<message>", Count(message_dom), message_dom);
		if(count > 0)
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",message_dom[0]->value);
		sprintf(message,"%s",message_dom[0]->value);
		strncpy(MyEftTransaction->ResponseMessage,message,sizeof(MyEftTransaction->ResponseMessage)-1);
		count = 0;
		}



		//status
		count = dom_getElementsByTagName(Domnode, "<status>", Count(status_dom), status_dom);
		if(count > 0)
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",status_dom[0]->value);
		sprintf(status,"%s",status_dom[0]->value);
		MyEftTransaction->iTranStatus = atoi(status);
		count = 0;
		}


		//refno
		count = dom_getElementsByTagName(Domnode, "<refno>", Count(refno_dom), refno_dom);
		if(count > 0)
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",refno_dom[0]->value);
		sprintf(refno,"%s",refno_dom[0]->value);
		strncpy(MyEftTransaction->RefNo,refno,sizeof(MyEftTransaction->RefNo)-1);
		count = 0;
		}

		//tranno
		count = dom_getElementsByTagName(Domnode, "<tranno>", Count(tranno_dom), tranno_dom);
		if(count >0 )
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",tranno_dom[0]->value);
		sprintf(tranno,"%s",tranno_dom[0]->value);
		strncpy(MyEftTransaction->transno,tranno,sizeof(MyEftTransaction->transno));
		count = 0;
		}



		//batchno
		count = dom_getElementsByTagName(Domnode, "<batchno>", Count(batchno_dom), batchno_dom);
		if(count >0 )
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",batchno_dom[0]->value);
		batchno = atoi(&Db[0]);
		count = 0;
		}


		//fromacc
		count = dom_getElementsByTagName(Domnode, "<fromacc>", Count(fromacc_dom), fromacc_dom);
		if(count > 0 )
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",fromacc_dom[0]->value);
		sprintf(fromacc,"%s",fromacc_dom[0]->value);
		sprintf(MyEftTransaction->froacc,"%s",fromacc);
			
		count = 0;
		}

		//toacc
		count = dom_getElementsByTagName(Domnode, "<toacc>", Count(toacc_dom), toacc_dom);
		if(count >0 )
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",toacc_dom[0]->value);
		sprintf(toacc,"%s",toacc_dom[0]->value);
		sprintf(MyEftTransaction->tooacc,"%s",toacc);
		count  = 0;
		}

		//balance
		count = dom_getElementsByTagName(Domnode, "<balance>", Count(balance_dom), balance_dom);
		if(count > 0)
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",balance_dom[0]->value);
		sprintf(balance,"%s",balance_dom[0]->value);
		sprintf(MyEftTransaction->balance,"%s",balance);
		count = 0;
		}

		//available
		count = dom_getElementsByTagName(Domnode, "<available>", Count(available_dom), available_dom);
		if(count >0)
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",available_dom[0]->value);
		sprintf(available,"%s",available_dom[0]->value);
		sprintf(MyEftTransaction->available,"%s",available);
		count  = 0;
		}


		//data
		count = dom_getElementsByTagName(Domnode, "<data>", Count(data_dom), data_dom);
		if(count > 0 )
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",data_dom[0]->value);
		sprintf(data,"%s",data_dom[0]->value);
		sprintf(MyEftTransaction->data,"%s",data);
		count   = 0;
		}

		

		//iccresponse
		count = dom_getElementsByTagName(Domnode, "<iccresponse>", Count(iccresponse_dom), iccresponse_dom);
		if(count >0 )
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",iccresponse_dom[0]->value);
		sprintf((char *)iccresponse,"%s",iccresponse_dom[0]->value);
		strncpy(MyEftTransaction->IssuerScript,(char *)iccresponse,sizeof(MyEftTransaction->IssuerScript)-1);
		count  = 0;
		}


		//Authid
		count = dom_getElementsByTagName(Domnode, "<authid>", Count(authid_dom), authid_dom);
		if(count >0 )
		{
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"K%d",count);
		SendDebug(Db);
		memset(&Db[0],0,sizeof(Db));
		sprintf(&Db[0],"%s",data_dom[0]->value);
		sprintf((char *)authid,"%s",data_dom[0]->value);
		strncpy(MyEftTransaction->AuthId,(char *)authid,sizeof(MyEftTransaction->AuthId)-1);
		count  = 0;
		}

		return 0;

	}else
		{
		
		return -1;
		}

	
	}
		pos = 0;
		pos = mystrpos(Xml,"<error>");
		if(pos >0)
		{
			ErrorReceipt LocalErrorReceipt;

			strncpy(LocalErrorReceipt.FunctionName,"PURCHASE",sizeof(LocalErrorReceipt.FunctionName)-1);
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
		{
				iRet=-1;
				SendDebug("Purchase:Request Failed");
		}
	return iRet;
}
