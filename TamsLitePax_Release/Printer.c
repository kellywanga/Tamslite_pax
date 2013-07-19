#include <posapi.h>
#include <posapi_all.h>
#include "TamsLite.h"
#include "Printer.h"
#include "Applib.h"
#include "emvlib.h"
#include "Emv.h"
#include "Bin2Ascii.h"
#include "MagStripe.h"
#include "EftUtil.h"

int PrintErrorReceipt(ErrorReceipt _ErrorReceipt)
{
	int iRet;
	char LineOne[50] = {0};
	char LineTwo[50] = {0};
	char LineThree[50] = {0};
	//12spaces
	sprintf(LineOne,"%s           %s\n" ,_ErrorReceipt.FunctionName,"FAILED");
	sprintf(LineTwo,"%s\n",_ErrorReceipt.ErrorMessage);
	sprintf(LineThree,"  Trace Code(%d)  \n", _ErrorReceipt.ErrorCode);
	iRet = PrnInit();
	if(iRet != 0)
	{
		return -1;
	}
	PrnFontSet(3,3);
	PrnStr(LineOne);
	PrnStr(LineTwo);
	PrnStr(LineThree);

	iRet = PrnStart();
			if (iRet)
			{
				return -1;
			}
	return 0;
}


int RePrintTransaction(int Control)
{
	BatchInformation _MyLocalBatchInfo;
	EftTransaction  * _MyLocalTransaction;
	
	ReadBatchInfo(&_MyLocalBatchInfo);

	switch(Control)
	{
	case 1:
		//LAST TRANSACTION
		getEftTransaction(_MyLocalBatchInfo.AllTransactionCount, _MyLocalTransaction);
		if(_MyLocalTransaction  !=  NULL)
		{
			if(_MyLocalTransaction->cTranResult == 1)
			{
				printEftTranSlip(_MyLocalTransaction);
			}else
			{
				PrintEftError(_MyLocalTransaction);
			}
		}
		break;
	case 2:
		//SPECIFIC TRANSACTION
		{
			unsigned char BatchNo[50] = {0};
			unsigned char SequenceNo[50] = {0};
			long BatchN;
			long SequenceN;
			int i=0;

			PubShowTitle(TRUE,(unsigned char *)"ENTER BATCH NO");
			PubGetString(ALPHA_IN|ECHO_IN,0,50,BatchNo,60) ;
			PubShowTitle(TRUE,(unsigned char *)"ENTER SEQ NO");
			PubGetString(ALPHA_IN|ECHO_IN,0,50,SequenceNo,60) ;

			BatchN = atol(BatchNo);
			SequenceN = atol(SequenceNo);

			for(i = 1;1<=_MyLocalBatchInfo.AllTransactionCount;i++)
			{
				 _MyLocalTransaction = (EftTransaction*)NULL;
				getEftTransaction(_MyLocalBatchInfo.AllTransactionCount, _MyLocalTransaction);
				if(_MyLocalTransaction  !=  NULL)
				{
					if(_MyLocalTransaction->BatchNo == BatchN && _MyLocalTransaction->SequenceNo == SequenceN)
					{
						if(_MyLocalTransaction->cTranResult == 1)
						{
							printEftTranSlip(_MyLocalTransaction);
						}else
						{
							PrintEftError(_MyLocalTransaction);
						}

					}


				}

		  }

		}

		break;
	case 3:
		//ALL TRANSACTION
		{
			int i=0;

			for( i = 1;1<=_MyLocalBatchInfo.AllTransactionCount;i++)
			{
				 _MyLocalTransaction = (EftTransaction*)NULL;
				getEftTransaction(_MyLocalBatchInfo.AllTransactionCount, _MyLocalTransaction);
				if(_MyLocalTransaction  !=  NULL)
				{
					if(_MyLocalTransaction->cTranResult == 1)
					{
						printEftTranSlip(_MyLocalTransaction);
					}else
					{
						PrintEftError(_MyLocalTransaction);
					}
				}
			}
		}

		break;

	}
	return 0;
}


void PrintSlipHeader(const char *pcSlipHeader)
{
	int iRet;
	Device LocalDevice;
	char LineOne[50] = {0};
	char LineTwo[50] = {0};
	char LineThree[50] = {0};
	unsigned char time[7], time2[20], buff[20];


	PubShowTitle(TRUE,(unsigned char *)"Printing");
	ScrGotoxy(0, 2);
	Lcdprintf("Please wait"); 

	ReadDevice(&LocalDevice);

			GetTime(time);
			if (!memcmp(time, time2, 6))
			{
				return;
			}
			

	sprintf(LineOne,"   %s   \n",pcSlipHeader);
	sprintf(LineTwo,"%s          %s\n",LocalDevice.MerchantId,LocalDevice.TerminalId);
	sprintf(LineThree, "%c%c/%c%c   %c%c:%c%c:%c%c\n",
				(time[1] >> 4) + 0x30, (time[1] & 0x0f) + 0x30,
				(time[2] >> 4) + 0x30, (time[2] & 0x0f) + 0x30,
				(time[3] >> 4) + 0x30, (time[3] & 0x0f) + 0x30,
				(time[4] >> 4) + 0x30, (time[4] & 0x0f) + 0x30,
				(time[5] >> 4) + 0x30, (time[5] & 0x0f) + 0x30);

	iRet = PrnInit();
	if(iRet != 0)
	{
		return;
	}
	PrnFontSet(3,3);
	PrnStr(LineTwo);
	PrnStr(LineThree);
	PrnStr(LineOne);
	PrnStr(DOTTEDLINE);
	iRet = PrnStart();
			if (iRet)
			{
				return;
			}
	return;
}



void PrintSlipFooter()
{
	int iRet;
	Device LocalDevice;
	char LineOne[50] = {0};
	char LineTwo[50] = {0};
	char LineThree[50] = {0};
	char LineFour[50] = {0};
	char LineFive[50] = {0};		
	sprintf(LineOne,"   %s     ","Please Call Again");
	sprintf(LineTwo,"%s",DOTTEDLINE);
	sprintf(LineThree,"    %s    ","Thank You");
	sprintf(LineFour,"%s","Version:Tamslite");
	sprintf(LineFive,"%s","www.iisysgroup.com|018720390");
	iRet = PrnInit();
	if(iRet != 0)
	{
		return;
	}
	PrnFontSet(3,3);

	PrnStr(LineOne);
	PrnStr(LineTwo);
	PrnStr(LineThree);
	PrnStr(LineFour);
	PrnStr(LineFive);

	iRet = PrnStart();
			if (iRet)
			{
				return;
			}
	return;
}


void PrintEftError(EftTransaction *Transaction)
{
	char            acStr[55];
    char            cBuf[40];
    char            cReadPan[21];
    char            cExpiry[5];
	char			printBuf[70];
	uchar           tagValueBin[100];
    char            tagValueStr[100];
    char            name[20];
    int             len = 0;
	int				iRet;
	unsigned short  valuelen = 0;
	char            pan[10];
	unsigned char	ApplicationIdentication[50] = {0};
	unsigned char	ApplicationLabel[50] = {0};
	unsigned char TerminalVerificationResult[50] = {0};
	unsigned char   TSI[50] = {0};
	unsigned char   HexApplicationIdentication[50]={0};
	unsigned char   HexApplicationLabel[50] = {0};
	unsigned char   HexTSI[50] = {0};
	unsigned char   HexTerminalVerificationResult[50]={0};
	int ApplicationIdenticationLen,ApplicationLabelLen,TSILen,TerminalVerificationResultLen;



	char LineOne[50] = {0};
	char LineTwo[50] = {0};
	char LineThree[50] = {0};
	char LineFour[50] = {0};
	char LineFive[50] = {0};
	char LineSix[50] = {0};
	char LineSeven[50] = {0};
	char LineEight[50] = {0};
	char LineNine[50] = {0};
	char LineTen[50] = {0};
	char LineEleven[50] = {0};
	char LineTwelve[50] = {0};

	PubShowTitle(TRUE,(unsigned char *)"TRANSACTION");
	ScrGotoxy(0, 2);
	Lcdprintf("DECLINED");

	DelayMs(2000);
	switch(Transaction->TransType)
	{
        case PurchaseOnly:
            strncpy(cBuf, "PURCHASE" ,sizeof(cBuf));
            break;
		case BalanceInquiry:
			strncpy(cBuf, "BALANCE INQUIRY",sizeof(cBuf));
			break;
		case ReversalTransaction:
			strncpy(cBuf, "REVERSAL",sizeof(cBuf));
			break;
	}

	PrintSlipHeader(cBuf);

	sprintf(LineOne,"%s","      **DECLINED**      \n");
	if(strlen(Transaction->ICCData) > 0)
	{
		//Chip Card Transaction
		TrackTwo MyTrackTwo;
		EMVGetTLVData(tagAPPL_LABEL,ApplicationLabel,&ApplicationLabelLen);
		EMVGetTLVData(tagAID_CARD,ApplicationIdentication,&ApplicationIdenticationLen);
		EMVGetTLVData(tagTVR,TerminalVerificationResult,&TerminalVerificationResultLen);
		EMVGetTLVData(tagTSI,TSI,&TSILen);

		bin2ascii(ApplicationIdentication,ApplicationIdenticationLen,(char *)HexApplicationIdentication);
		bin2ascii(ApplicationLabel,ApplicationLabelLen,(char *)HexApplicationLabel);
		bin2ascii(TerminalVerificationResult,TerminalVerificationResultLen,(char *)HexTerminalVerificationResult);
		bin2ascii(TSI,TSILen,(char *)HexTSI);


	sprintf(LineTwo,"AID        %s\n",HexApplicationIdentication);
	sprintf(LineThree,"LABEL        %s\n",HexApplicationLabel);
	sprintf(LineFour, "PAN			 EXPIRY\n");
	memset(&MyTrackTwo,0,sizeof(TrackTwo));
	ParseTrackTwo(Transaction->Track2,&MyTrackTwo,1);
	sprintf(LineFive,"%s          %s\n",MyTrackTwo.Pan,MyTrackTwo.ExpiryDate);
	sprintf(LineSix,"BatchNo         %d\n",Transaction->BatchNo);
	sprintf(LineSeven,"Seq No         %d\n",Transaction->SequenceNo);
	sprintf(LineEight, "TRANSACTION NOT DONE!!!\n");

	sprintf(LineNine,   "%s\n",Transaction->ResponseMessage);

	sprintf(LineTen,"Trace Code(%d)\n",Transaction->cTranResult);

	sprintf(LineEleven,"TVR          %s\n",HexTerminalVerificationResult);
	sprintf(LineTwelve,"TSI          %s\n",HexTSI);

	iRet = PrnInit();
	if(iRet != 0)
	{
		return;
	}
	PrnFontSet(3,3);

	PrnStr(LineOne);
	PrnStr(LineTwo);
	PrnStr(LineThree);
	PrnStr(LineFour);
	PrnStr(LineFive);
	PrnStr(LineSix);
	PrnStr(LineSeven);
	PrnStr(LineEight);
	PrnStr(LineNine);
	PrnStr(LineTen);
	PrnStr(LineEleven);
	PrnStr(LineTwelve);
	PrnStart();

	}else
	{
		//Magstripe Card
		TrackTwo MyTrackTwo;
		ParseTrackTwo(Transaction->Track2,&MyTrackTwo,0);

		sprintf(LineFive,"%s          %s\n",MyTrackTwo.Pan,MyTrackTwo.ExpiryDate);
		sprintf(LineSix,"BatchNo         %d\n",Transaction->BatchNo);
		sprintf(LineSeven,"Seq No         %d\n",Transaction->SequenceNo);
		sprintf(LineEight, "TRANSACTION NOT DONE!!!\n");
		sprintf(LineNine,   "%s\n",Transaction->ResponseMessage);
		sprintf(LineTen,"Trace Code(%d)\n",Transaction->iTranStatus);
		PrnFontSet(3,3);
		PrnStr(LineFive);
		PrnStr(LineSix);
		PrnStr(LineSeven);
		PrnStr(LineEight);
		PrnStr(LineNine);
		PrnStr(LineTen);
		PrnStart();
	}

	PrintSlipFooter();

}


void printEftTranSlip(EftTransaction *Transaction)
{
	char            acStr[55];
    char            cBuf[40];
    char            cReadPan[21];
    char            cExpiry[5];
	char			printBuf[70];
	uchar           tagValueBin[100];
    char            tagValueStr[100];
    char            name[20];
    int             len = 0;
	int				iRet;
	unsigned short  valuelen = 0;
	char            pan[10];
	unsigned char	ApplicationIdentication[50] = {0};
	unsigned char	ApplicationLabel[50] = {0};
	unsigned char TerminalVerificationResult[50] = {0};
	unsigned char Cryptogram[50] = {0};
	unsigned char   TSI[50] = {0};
	unsigned char   HexApplicationIdentication[50]={0};
	unsigned char   HexApplicationLabel[50] = {0};
	unsigned char   HexTSI[50] = {0};
	unsigned char   HexTerminalVerificationResult[50]={0};
	unsigned char	HexCryptogram[50] = {0};
	int ApplicationIdenticationLen,ApplicationLabelLen,TSILen,TerminalVerificationResultLen,CryptogramLen;



	char LineOne[50] = {0};
	char LineTwo[50] = {0};
	char LineThree[50] = {0};
	char LineFour[50] = {0};
	char LineFive[50] = {0};
	char LineSix[50] = {0};
	char LineSeven[50] = {0};
	char LineEight[50] = {0};
	char LineNine[50] = {0};
	char LineTen[50] = {0};
	char LineEleven[50] = {0};
	char LineTwelve[50] = {0};
	char LineThirteen[50] = {0};
	char LineFourtheen[50] = {0};
	char LineFifteen[50] = {0};
	char LineSixtheen[50] = {0};

	PubShowTitle(TRUE,(unsigned char *)"APPROVED");
	ScrGotoxy(0, 2);
	Lcdprintf("MERCHANTS COPY");

	DelayMs(2000);
	switch(Transaction->TransType)
	{
        case PurchaseOnly:
            strncpy(cBuf, "PURCHASE" ,sizeof(cBuf));
            break;
		case BalanceInquiry:
			strncpy(cBuf, "BALANCE INQUIRY",sizeof(cBuf));
			break;
		case ReversalTransaction:
			strncpy(cBuf, "REVERSAL",sizeof(cBuf));
			break;
	}

	PrintSlipHeader(cBuf);

	strncpy(LineOne,"      **APPROVED**      \n", sizeof(LineOne));
	strncpy(LineTwo,"      ***MERCHANT COPY***      \n", sizeof(LineTwo));
	if(strlen(Transaction->ICCData) > 0)
	{
		//Chip Card Transaction
		TrackTwo MyTrackTwo;
		EMVGetTLVData(tagAPPL_LABEL,ApplicationLabel,&ApplicationLabelLen);
		EMVGetTLVData(tagAID_CARD,ApplicationIdentication,&ApplicationIdenticationLen);
		EMVGetTLVData(tagTVR,TerminalVerificationResult,&TerminalVerificationResultLen);
		EMVGetTLVData(tagTSI,TSI,&TSILen);
		EMVGetTLVData(tagAPPL_CRYPTOGRAM,Cryptogram,&CryptogramLen);

		bin2ascii(ApplicationIdentication,ApplicationIdenticationLen,(char *)HexApplicationIdentication);
		bin2ascii(ApplicationLabel,ApplicationLabelLen,(char *)HexApplicationLabel);
		bin2ascii(TerminalVerificationResult,TerminalVerificationResultLen,(char *)HexTerminalVerificationResult);
		bin2ascii(TSI,TSILen,(char *)HexTSI);
		bin2ascii(Cryptogram,CryptogramLen,(char *)HexCryptogram);


	sprintf(LineThree,"AID        %s\n",HexApplicationIdentication);
	sprintf(LineFour,"LABEL        %s\n",HexApplicationLabel);
	sprintf(LineFive, "PAN			 EXPIRY\n");
	memset(&MyTrackTwo,0,sizeof(TrackTwo));
	ParseTrackTwo(Transaction->Track2,&MyTrackTwo,1);
	sprintf(LineSix,"%s          %s\n",MyTrackTwo.Pan,MyTrackTwo.ExpiryDate);
	sprintf(LineSeven,"BatchNo         %d\n",Transaction->BatchNo);
	sprintf(LineEight,"Seq No         %d\n",Transaction->SequenceNo);
	sprintf(LineNine, "RefNo          %s",Transaction->RefNo);

	sprintf(LineTen,   "Auth ID          %s\n",Transaction->AuthId);

	sprintf(LineEleven,"Account			 %s\n",Transaction->AcctType==Current?"CURRENT":"SAVINGS");
																																						//update this to reverasal amount in the future								
	sprintf(LineTwelve,"Total			 %d\n",Transaction->TransType==PurchaseOnly?Transaction->PurchaseAmount:Transaction->TransType==BalanceInquiry?0:Transaction->PurchaseAmount);

	sprintf(LineThirteen,"TVR          %s\n",HexTerminalVerificationResult);
	sprintf(LineFourtheen,"TSI          %s\n",HexTSI);
	sprintf(LineFifteen,"EXP DATE          %s\n",MyTrackTwo.ExpiryDate);
	sprintf(LineSixtheen,"CRTM          %s\n\n",HexCryptogram);

	iRet = PrnInit();
	if(iRet != 0)
	{
		return;
	}
	PrnFontSet(3,3);

	PrnStr(LineOne);
	PrnStr(LineTwo);
	PrnStr(LineThree);
	PrnStr(LineFour);
	PrnStr(LineFive);
	PrnStr(LineSix);
	PrnStr(LineSeven);
	PrnStr(LineEight);
	PrnStr(LineNine);
	PrnStr(LineTen);
	PrnStr(LineEleven);
	PrnStr(LineTwelve);
	PrnStr(LineThirteen);
	PrnStr(LineFourtheen);
	PrnStr(LineFifteen);
	PrnStr(LineSixtheen);
	PrnStart();

	}else
	{
		//Magstripe Card
		TrackTwo MyTrackTwo;
		ParseTrackTwo(Transaction->Track2,&MyTrackTwo,0);

		sprintf(LineSix,"%s          %s\n",MyTrackTwo.Pan,MyTrackTwo.ExpiryDate);
		sprintf(LineSeven,"BatchNo         %d\n",Transaction->BatchNo);
		sprintf(LineEight,"Seq No         %d\n",Transaction->SequenceNo);
		sprintf(LineNine, "RefNo          %s",Transaction->RefNo);

		sprintf(LineTen,   "Auth ID          %s\n",Transaction->AuthId);

		sprintf(LineEleven,"Account			 %s\n",Transaction->AcctType==Current?"CURRENT":"SAVINGS");
																																						//update this to reverasal amount in the future								
		sprintf(LineTwelve,"Total			 %d\n",Transaction->TransType==PurchaseOnly?Transaction->PurchaseAmount:Transaction->TransType==BalanceInquiry?0:Transaction->PurchaseAmount);


		PrnFontSet(3,3);
		PrnStr(LineSix);
		PrnStr(LineSeven);
		PrnStr(LineEight);
		PrnStr(LineNine);
		PrnStr(LineTen);
		PrnStr(LineEleven);
		PrnStr(LineTwelve);
		PrnStart();
	}

	PrintSlipFooter();

	getkey();
	PubShowTitle(TRUE,(unsigned char *)"APPROVED");
	ScrGotoxy(0, 2);
	Lcdprintf("CUSTOMERS COPY");

	DelayMs(2000);



	PrintSlipHeader(cBuf);

	strncpy(LineOne,"      **APPROVED**      \n", sizeof(LineOne));
	strncpy(LineTwo,"      ***CUSTOMERS COPY***      \n", sizeof(LineTwo));
	if(strlen(Transaction->ICCData) > 0)
	{
		//Chip Card Transaction
		TrackTwo MyTrackTwo;
		EMVGetTLVData(tagAPPL_LABEL,ApplicationLabel,&ApplicationLabelLen);
		EMVGetTLVData(tagAID_CARD,ApplicationIdentication,&ApplicationIdenticationLen);
		EMVGetTLVData(tagTVR,TerminalVerificationResult,&TerminalVerificationResultLen);
		EMVGetTLVData(tagTSI,TSI,&TSILen);
		EMVGetTLVData(tagAPPL_CRYPTOGRAM,Cryptogram,&CryptogramLen);

		bin2ascii(ApplicationIdentication,ApplicationIdenticationLen,(char *)HexApplicationIdentication);
		bin2ascii(ApplicationLabel,ApplicationLabelLen,(char *)HexApplicationLabel);
		bin2ascii(TerminalVerificationResult,TerminalVerificationResultLen,(char *)HexTerminalVerificationResult);
		bin2ascii(TSI,TSILen,(char *)HexTSI);
		bin2ascii(Cryptogram,CryptogramLen,(char *)HexCryptogram);


	sprintf(LineThree,"AID        %s\n",HexApplicationIdentication);
	sprintf(LineFour,"LABEL        %s\n",HexApplicationLabel);
	sprintf(LineFive, "PAN			 EXPIRY\n");
	memset(&MyTrackTwo,0,sizeof(TrackTwo));
	ParseTrackTwo(Transaction->Track2,&MyTrackTwo,1);
	sprintf(LineSix,"%s          %s\n",MyTrackTwo.Pan,MyTrackTwo.ExpiryDate);
	sprintf(LineSeven,"BatchNo         %d\n",Transaction->BatchNo);
	sprintf(LineEight,"Seq No         %d\n",Transaction->SequenceNo);
	sprintf(LineNine, "RefNo          %s",Transaction->RefNo);

	sprintf(LineTen,   "Auth ID          %s\n",Transaction->AuthId);

	sprintf(LineEleven,"Account			 %s\n",Transaction->AcctType==Current?"CURRENT":"SAVINGS");
																																						//update this to reverasal amount in the future								
	sprintf(LineTwelve,"Total			 %d\n",Transaction->TransType==PurchaseOnly?Transaction->PurchaseAmount:Transaction->TransType==BalanceInquiry?0:Transaction->PurchaseAmount);

	sprintf(LineThirteen,"TVR          %s\n",HexTerminalVerificationResult);
	sprintf(LineFourtheen,"TSI          %s\n",HexTSI);
	sprintf(LineFifteen,"EXP DATE          %s\n",MyTrackTwo.ExpiryDate);
	sprintf(LineSixtheen,"CRTM          %s\n",HexCryptogram);

	iRet = PrnInit();
	if(iRet != 0)
	{
		return;
	}
	PrnFontSet(3,3);

	PrnStr(LineOne);
	PrnStr(LineTwo);
	PrnStr(LineThree);
	PrnStr(LineFour);
	PrnStr(LineFive);
	PrnStr(LineSix);
	PrnStr(LineSeven);
	PrnStr(LineEight);
	PrnStr(LineNine);
	PrnStr(LineTen);
	PrnStr(LineEleven);
	PrnStr(LineTwelve);
	PrnStr(LineThirteen);
	PrnStr(LineFourtheen);
	PrnStr(LineFifteen);
	PrnStr(LineSixtheen);
	PrnStart();

	}else
	{
		//Magstripe Card
		TrackTwo MyTrackTwo;
		ParseTrackTwo(Transaction->Track2,&MyTrackTwo,0);

		sprintf(LineSix,"%s          %s\n",MyTrackTwo.Pan,MyTrackTwo.ExpiryDate);
		sprintf(LineSeven,"BatchNo         %d\n",Transaction->BatchNo);
		sprintf(LineEight,"Seq No         %d\n",Transaction->SequenceNo);
		sprintf(LineNine, "RefNo          %s",Transaction->RefNo);

		sprintf(LineTen,   "Auth ID          %s\n",Transaction->AuthId);

		sprintf(LineEleven,"Account			 %s\n",Transaction->AcctType==Current?"CURRENT":"SAVINGS");
																																						//update this to reverasal amount in the future								
		sprintf(LineTwelve,"Total			 %d\n",Transaction->TransType==PurchaseOnly?Transaction->PurchaseAmount:Transaction->TransType==BalanceInquiry?0:Transaction->PurchaseAmount);


		PrnFontSet(3,3);
		PrnStr(LineSix);
		PrnStr(LineSeven);
		PrnStr(LineEight);
		PrnStr(LineNine);
		PrnStr(LineTen);
		PrnStr(LineEleven);
		PrnStr(LineTwelve);
		PrnStart();
	}

	PrintSlipFooter();
}