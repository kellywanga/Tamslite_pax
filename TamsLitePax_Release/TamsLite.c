#include <posapi.h>
#include <posapi_all.h>
#include <Ctype.h>
#include "Applib.h"
#include "TamsLite.h"
#include "Debug.h"
#include "Purchase.h"
#include "Security.h"
#include "MagStripe.h"
#include "BalanceEnq.h"
#include "Hex2Bin.h"
#include  "Keys.h"
#include  "Eod.h"
#include  "GetParameters.h"
#include  "CallHome.h"
#include "EftUtil.h"
#include  "Printer.h"
extern char gszAmount[12+1];
extern unsigned char aucAmount[6+1];

 int mystrpos(char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;
}
 char *substring(size_t start, size_t stop, const char *src, char *dst, size_t size)
{
   int count = stop - start;
   if ( count >= --size )
   {
      count = size;
   }
   sprintf(dst, "%.*s", count, src + start);
   return dst;
}
 
 char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}

int uppercase ( char *sPtr )
{
  while ( *sPtr != '\0' ) {
    *sPtr = toupper ( ( unsigned char ) *sPtr );
    ++sPtr;
  }
}
 int SaveDevice(Device * MyDevice)
 {
	 int iRet;
	iRet  = PubFileWrite((unsigned char *)"DEVICE",0,(void *)MyDevice,sizeof(Device));
	 return iRet;
 }
int ReadDevice(Device *MyDevice)
{
	int iRet;
	iRet  = PubFileRead((unsigned char *)"DEVICE",0,(void *)MyDevice,sizeof(Device));
	switch(iRet)
	{
	case 0:
	case 1:
	case 2:
		return iRet;
	case 3:
	memset(MyDevice,0,sizeof(MyDevice));
	return iRet;
	}
	return iRet;
}

int SaveBatchInfo(BatchInformation *BatchInfo)
{
	 int iRet;
	iRet  = PubFileWrite((unsigned char *)"BATCHINFO",0,(void *)BatchInfo,sizeof(BatchInformation));
	 return iRet;
}
int ReadBatchInfo(BatchInformation *BatchInfo)
{
	int iRet;
	iRet  = PubFileRead((unsigned char *)"BATCHINFO",0,(void *)BatchInfo,sizeof(BatchInformation));
	switch(iRet)
	{
	case 0:
	case 1:
	case 2:
		return iRet;
	case 3:
	memset(BatchInfo,0,sizeof(BatchInfo));
	return iRet;
	}
	return iRet;
}

int ShowMagStripeEftMenu(ST_MAGCARD MAGSTRIPE)
{
	
	long double Ammount;
	char inputAmmt[10] = {0};
	EftTransaction MyEftTransaction;
	char PAN[50] = {0};
	char ServiceCode[3] = {0};
	TrackTwo OutPut;
	int iRet = -1;
	SendDebug("Menu Entered");
	memset(&MyEftTransaction,0,sizeof(MyEftTransaction));
	while(1)
	{
	PubShowTitle(TRUE,(unsigned char *)"TAMSLITE");
	ScrGotoxy(0, 2);
	Lcdprintf("Purchase::::::::: "); 
	ScrGotoxy(0, 4);
	Lcdprintf("Balance ::::::::: "); 
	ScrGotoxy(0, 6);
	Lcdprintf("Reversal:::::::: "); 
	switch(getkey())
	{
	case KEYATM2:
		//PURCHASE
		//SELECT ACCOUNT TYPE
		PubShowTitle(TRUE,(unsigned char *)"ACCOUNT TYPE");
		ScrGotoxy(0, 2);
		Lcdprintf("Default ::::::: "); 
		ScrGotoxy(0, 4);
		Lcdprintf("Savings ::::::::: "); 
		ScrGotoxy(0, 6);
		Lcdprintf("Current :::::: "); 
		switch(getkey())
		{
			case KEYATM2:
				MyEftTransaction.AcctType = Default;
				break;
			case KEYATM3:
				MyEftTransaction.AcctType =  Savings;
				break;
			case KEYATM4:
				MyEftTransaction.AcctType =  Current;
				break;
			default:
				MyEftTransaction.AcctType =  Default;
			break;
		}
		MyEftTransaction.TransType = PurchaseOnly;

		PubShowTitle(TRUE,(unsigned char *)"ENTER AMOUNT");
		if (PubGetAmount((unsigned char *)"=N=",0,1, 7, (unsigned char *)inputAmmt,60, GA_SEPARATOR) == 0)
		{
		MyEftTransaction.PurchaseAmount = atol(inputAmmt);
		MyEftTransaction.BatchNo = MyBatchInfo.BatchNo;
		MyBatchInfo.AllTransactionCount++;
		MyBatchInfo.AllTransactionTotalValue  += MyEftTransaction.PurchaseAmount;
		MyBatchInfo.PurchaseTransactionCount++;
		MyBatchInfo.PurchaseTransactionTotalValue +=MyEftTransaction.PurchaseAmount;
		SaveBatchInfo(&MyBatchInfo);
		MyEftTransaction.SequenceNo = 12;//MyBatchInfo.AllTransactionCount;
		strncpy(MyEftTransaction.MerchantId,MyDevice.MerchantId,sizeof(MyEftTransaction.MerchantId));
		MyEftTransaction.CurrencyCode = 566;
		MyEftTransaction.OperatorId = 1;
		MyEftTransaction.SupervisorFlag = 0;
		GetTime((unsigned char *)MyEftTransaction.TransacDateAndTime);
		MyEftTransaction.CashBackAmount = 0.0L;
		MyEftTransaction.TipAmount = 0.0L;
		MyEftTransaction.Offline = 0;
		MyEftTransaction.ManualFlag = 0;
		MyEftTransaction.OriginalSequenceNumber = 0;
		MyEftTransaction.BudgetPeriod = 0;
		strcpy (MyEftTransaction.Track1,(char *)MAGSTRIPE.track1);
		strcpy (MyEftTransaction.Track2,(char *)MAGSTRIPE.track2);
		strcpy (MyEftTransaction.CVV,"");
		strcpy (MyEftTransaction.PinSerial,"");
		MyEftTransaction.OriginalSequenceNumber = 0.0L;
		ParseTrackTwo(MyEftTransaction.Track2,&OutPut,0);


		//PROCESS SERVICE CODE
		

		if(MyDevice.PinBlockEncryptionType == TRIDES)
		{
			//PIN BLOCK ENC
			TriDesGetPin(MyEftTransaction.PinBlock, OutPut.Pan, "ENTER PIN");
		}else
		{
		}
		Purchase(&MyEftTransaction);
		}
		break;
	case KEYATM3:
		//BALANCE ENQ
		//SELECT ACCOUNT TYPE
		PubShowTitle(TRUE,(unsigned char *)"ACCOUNT TYPE");
		ScrGotoxy(0, 2);
		Lcdprintf("Default ::::::: "); 
		ScrGotoxy(0, 4);
		Lcdprintf("Savings ::::::: "); 
		ScrGotoxy(0, 6);
		Lcdprintf("Current :::::: "); 
		switch(getkey())
		{
			case KEYATM2:
				MyEftTransaction.AcctType = Default;
				break;
			case KEYATM3:
				MyEftTransaction.AcctType =  Savings;
				break;
			case KEYATM4:
				MyEftTransaction.AcctType =  Current;
				break;
			default:
				MyEftTransaction.AcctType =  Default;
			break;
		}
		MyEftTransaction.TransType = BalanceInquiry;

		MyEftTransaction.PurchaseAmount = 0L;
		MyBatchInfo.AllTransactionCount++;
		SaveBatchInfo(&MyBatchInfo);
		MyEftTransaction.BatchNo = MyBatchInfo.BatchNo;
		MyEftTransaction.SequenceNo =MyBatchInfo.AllTransactionCount;
		strncpy(MyEftTransaction.MerchantId,MyDevice.MerchantId,sizeof(MyEftTransaction.MerchantId));
		MyEftTransaction.CurrencyCode = 566;
		MyEftTransaction.OperatorId = 1;
		MyEftTransaction.SupervisorFlag = 0;
		GetTime((unsigned char *)MyEftTransaction.TransacDateAndTime);
		MyEftTransaction.CashBackAmount = 0.0L;
		MyEftTransaction.TipAmount = 0.0L;
		MyEftTransaction.Offline = 0;
		MyEftTransaction.ManualFlag = 0;
		MyEftTransaction.OriginalSequenceNumber = 0;
		MyEftTransaction.BudgetPeriod = 0;
		strcpy (MyEftTransaction.Track1,(char *)MAGSTRIPE.track1);
		strcpy (MyEftTransaction.Track2,(char *)MAGSTRIPE.track2);
		strcpy (MyEftTransaction.CVV,"");
		strcpy (MyEftTransaction.PinSerial,"");
		ParseTrackTwo(MyEftTransaction.Track2,&OutPut,0);
		if(MyDevice.PinBlockEncryptionType == TRIDES)
		{
			TriDesGetPin(MyEftTransaction.PinBlock, OutPut.Pan, "ENTER PIN");
		}else
		{
		}
		BalanceEnq(&MyEftTransaction);
		
		break;
		break;
	case KEYATM4:
		//REVERSAL
		break;
	case KEYMENU:
		return 0;
	case KEYCANCEL:
		return 0;
	}

	}
}

int ShowChipEftMenu(EftTransaction * MyLocalEmvTransaction)
{
	

	long double Ammount;
	char inputAmmt[10] = {0};
	//EftTransaction MyEftTransaction;
	char PAN[50] = {0};
	char ServiceCode[3] = {0};
	int iRet = -1;
	//SendDebug("Menu Entered");



	while(1)
	{
		
	PubShowTitle(TRUE,(unsigned char *)"TAMSLITE");
	ScrGotoxy(0, 2);
	Lcdprintf("Purchase::::::::: "); 
	ScrGotoxy(0, 4);
	Lcdprintf("Balance ::::::::: "); 
	ScrGotoxy(0, 6);
	Lcdprintf("Reversal:::::::: "); 
	switch(getkey())
	{
	case KEYATM2:
		//Purchase Transaction
		//SELECT ACCOUNT TYPE
		PubShowTitle(TRUE,(unsigned char *)"ACCOUNT TYPE");
		ScrGotoxy(0, 2);
		Lcdprintf("Default ::::::: "); 
		ScrGotoxy(0, 4);
		Lcdprintf("Savings ::::::: "); 
		ScrGotoxy(0, 6);
		Lcdprintf("Current :::::: "); 
		switch(getkey())
		{
			case KEYATM2:
				MyLocalEmvTransaction->AcctType =  Default;
				break;
			case KEYATM3:
				MyLocalEmvTransaction->AcctType =  Savings;
				break;
			case KEYATM4:
				MyLocalEmvTransaction->AcctType =  Current;
				break;
			default:
				MyLocalEmvTransaction->AcctType =  Default;
			break;
		}
		MyLocalEmvTransaction->TransType = PurchaseOnly;

		PubShowTitle(TRUE,(unsigned char *)"ENTER AMOUNT");
		if (PubGetAmount((unsigned char *)"=N=",0,1, 7, (unsigned char *)inputAmmt,60, GA_SEPARATOR) == 0)
		{
		strncpy(gszAmount,inputAmmt,sizeof(gszAmount)-1);
		//hex2bin(gszAmount,(char *)aucAmount,sizeof(aucAmount));
		memcpy(aucAmount,gszAmount,sizeof(aucAmount));
		MyLocalEmvTransaction->PurchaseAmount = atol(inputAmmt);
		MyLocalEmvTransaction->BatchNo = MyBatchInfo.BatchNo;
		MyBatchInfo.AllTransactionCount++;
		MyBatchInfo.AllTransactionTotalValue  += MyLocalEmvTransaction->PurchaseAmount;
		MyBatchInfo.PurchaseTransactionCount++;
		MyBatchInfo.PurchaseTransactionTotalValue +=MyLocalEmvTransaction->PurchaseAmount;
		SaveBatchInfo(&MyBatchInfo);
		MyLocalEmvTransaction->SequenceNo = MyBatchInfo.AllTransactionCount;
		strncpy(MyLocalEmvTransaction->MerchantId,MyDevice.MerchantId,sizeof(MyLocalEmvTransaction->MerchantId));
		MyLocalEmvTransaction->CurrencyCode = 566;
		MyLocalEmvTransaction->OperatorId = 1;
		MyLocalEmvTransaction->SupervisorFlag = 0;
		GetTime((unsigned char *)MyLocalEmvTransaction->TransacDateAndTime);
		MyLocalEmvTransaction->CashBackAmount = 0.0L;
		MyLocalEmvTransaction->TipAmount = 0.0L;
		MyLocalEmvTransaction->Offline = 0;
		MyLocalEmvTransaction->ManualFlag = 0;
		MyLocalEmvTransaction->OriginalSequenceNumber = 0;
		MyLocalEmvTransaction->BudgetPeriod = 0;
		//strcpy (MyEmvTransaction->Track1,(char *)MAGSTRIPE.track1);
		//strcpy (MyEmvTransaction->Track2,(char *)MAGSTRIPE.track2);
		strcpy (MyLocalEmvTransaction->CVV,"");
		strcpy (MyLocalEmvTransaction->PinSerial,"");
		MyLocalEmvTransaction->OriginalSequenceNumber = 0.0L;
		
		return 0;
		
		//Purchase(MyEftTransaction);
		}
		break;
	case KEYATM3:
		//BALANCE ENQ
		PubShowTitle(TRUE,(unsigned char *)"ACCOUNT TYPE");
		ScrGotoxy(0, 2);
		Lcdprintf("Default ::::::: "); 
		ScrGotoxy(0, 4);
		Lcdprintf("Savings ::::::: "); 
		ScrGotoxy(0, 6);
		Lcdprintf("Current :::::: "); 
		switch(getkey())
		{
			case KEYATM2:
				MyLocalEmvTransaction->AcctType =  Default;
				break;
			case KEYATM3:
				MyLocalEmvTransaction->AcctType =  Savings;
				break;
			case KEYATM4:
				MyLocalEmvTransaction->AcctType =  Current;
				break;
			default:
				MyLocalEmvTransaction->AcctType =  Default;
			break;
		}
		MyLocalEmvTransaction->TransType = BalanceInquiry;


		strncpy(gszAmount,"00",sizeof(gszAmount)-1);
		hex2bin(gszAmount,(char *)aucAmount,6);
		MyLocalEmvTransaction->PurchaseAmount = 0L;
		MyLocalEmvTransaction->BatchNo = MyBatchInfo.BatchNo;
		MyBatchInfo.AllTransactionCount++;
		SaveBatchInfo(&MyBatchInfo);
		MyLocalEmvTransaction->SequenceNo = MyBatchInfo.AllTransactionCount;
		strncpy(MyLocalEmvTransaction->MerchantId,MyDevice.MerchantId,sizeof(MyLocalEmvTransaction->MerchantId));
		MyLocalEmvTransaction->CurrencyCode = 566;
		MyLocalEmvTransaction->OperatorId = 1;
		MyLocalEmvTransaction->SupervisorFlag = 0;
		GetTime((unsigned char *)MyLocalEmvTransaction->TransacDateAndTime);
		MyLocalEmvTransaction->CashBackAmount = 0.0L;
		MyLocalEmvTransaction->TipAmount = 0.0L;
		MyLocalEmvTransaction->Offline = 0;
		MyLocalEmvTransaction->ManualFlag = 0;
		MyLocalEmvTransaction->OriginalSequenceNumber = 0;
		MyLocalEmvTransaction->BudgetPeriod = 0;
		//strcpy (MyEmvTransaction->Track1,(char *)MAGSTRIPE.track1);
		//strcpy (MyEmvTransaction->Track2,(char *)MAGSTRIPE.track2);
		strcpy (MyLocalEmvTransaction->CVV,"");
		strcpy (MyLocalEmvTransaction->PinSerial,"");
		MyLocalEmvTransaction->OriginalSequenceNumber = 0.0L;
		
		return 0;

		break;
	case KEYATM4:
		break;
	case KEYMENU:
		return 0;
	case KEYCANCEL:
		return 0;
	}

	}

}


int ShowAdminMenu()
{
	unsigned char TempPort[5];
	BeginFunctionShowAdmin:
	while(1)
	{
	ScrCls();
	ScrSetIcon (ICON_DOWN, OPENICON);
	ScrAttrSet(1);
	Lcdprintf("  ADMIN  "); 
	ScrAttrSet(0);
	ScrGotoxy(0, 2);
	Lcdprintf("PREP TERMINAL"); 
	ScrGotoxy(0, 4);
	Lcdprintf("GPRS "); 
	ScrGotoxy(0, 6);
	Lcdprintf("TMS"); 
	switch(getkey())
	{
	case KEYMENU:
		return KEYMENU;
	case KEYCANCEL:
		//goto BeginFunctionShowAdmin ;
		return KEYCANCEL;
		case KEYATM2:
			{
				int iReturnValue = -1;
				//PrepTerminal
				TruncateAllFiles();
				PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
				ScrFontSet(0);
				ScrPrint(0*6, 2,ASCII,"%s","NEW KEY");
				iReturnValue = NewKey();
					if(iReturnValue <0)
					{
						ScrCls();
						PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
						ScrFontSet(0);
						ScrPrint(0*6, 2,ASCII,"%s","NEW KEY FAILED");
						getkey();
						return KEYCANCEL;
					}else
					{
						iReturnValue  = - 1;
						ScrCls();
						PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
						ScrFontSet(0);
						ScrPrint(0*6, 2,ASCII,"%s","GET KEYS");
						iReturnValue = GetKeys();

						if(iReturnValue <0)
						{
							ScrCls();
							PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
							ScrFontSet(0);
							ScrPrint(0*6, 2,ASCII,"%s","GET KEYS FAILED");
							getkey();
							return KEYCANCEL;
						}else
						{
							iReturnValue =  - 1;
							ScrCls();
							PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
							ScrFontSet(0);
							ScrPrint(0*6, 2,ASCII,"%s","GETPARAMTERS");
							iReturnValue = GetParameters();

							if(iReturnValue <0)
							{
							ScrCls();
							PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
							ScrFontSet(0);
							ScrPrint(0*6, 2,ASCII,"%s","GETPARAMTERS FAILED");
							getkey();
							return KEYCANCEL;
							}else
							{
								iReturnValue =  - 1;
								ScrCls();
								PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
								ScrFontSet(0);
								ScrPrint(0*6, 2,ASCII,"%s","EOD");
								iReturnValue = EndOfDay();

								if(iReturnValue < 0 )
								{
									ScrCls();
									PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
									ScrFontSet(0);
									ScrPrint(0*6, 2,ASCII,"%s","EOD FAILED");
									getkey();
									return KEYCANCEL;
								}else
								{
									iReturnValue = -1;
									ScrCls();
									PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
									ScrFontSet(0);
									ScrPrint(0*6, 2,ASCII,"%s","CALLHOME");
									iReturnValue = CallHome();

									if(iReturnValue < 0)
									{
									ScrCls();
									PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
									ScrFontSet(0);
									ScrPrint(0*6, 2,ASCII,"%s","CALLHOME FAILED");
									getkey();
									return KEYCANCEL;
									}else
									{
									ScrCls();
									PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
									ScrFontSet(0);
									ScrPrint(0*6, 2,ASCII,"%s","PREP COMPLETE");
									getkey();
									return KEYCANCEL;
									}
									
								}

							}

						}
					}

			}
		break;
		case KEYDOWN:
			KEYDOWNLABEL:
			while(1)
			{
			ScrCls();
			ScrSetIcon (ICON_UP, OPENICON);
			ScrAttrSet(1);
			Lcdprintf("  ADMIN  "); 
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("EOD"); 
			ScrGotoxy(0, 4);
			Lcdprintf("REPRINT "); 
			ScrGotoxy(0, 6);
			Lcdprintf("PIN BLOCK"); 
			switch(getkey())
			{
				case KEYUP:
					goto BeginFunctionShowAdmin;
					break;
				case KEYATM2:
					//DO EOD
					{

					int iReturnValue =  - 1;
								ScrCls();
								PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
								ScrFontSet(0);
								ScrPrint(0*6, 2,ASCII,"%s","EOD");
								iReturnValue = EndOfDay();

								if(iReturnValue < 0 )
								{
									ScrCls();
									PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
									ScrFontSet(0);
									ScrPrint(0*6, 2,ASCII,"%s","EOD FAILED");
									getkey();
									goto KEYDOWNLABEL;
								}else
								{
									ScrCls();
									PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
									ScrFontSet(0);
									ScrPrint(0*6, 2,ASCII,"%s","EOD DONE");
									getkey();
									goto KEYDOWNLABEL;
								}
					}
			break;

				case KEYATM3:
					//REPRINT
					REPRINTLABEL:
					ScrSetIcon (ICON_DOWN, CLOSEICON);
					ScrSetIcon (ICON_UP, CLOSEICON);
					while(1)
					{
						Lcdprintf("  REPRINT  "); 
						ScrAttrSet(0);
						ScrGotoxy(0, 2);
						Lcdprintf("LAST TRANSACTION"); 
						ScrGotoxy(0, 4);
						Lcdprintf("SPECIFIC TRANSACTION"); 
						ScrGotoxy(0, 6);
						Lcdprintf("ALL TRANSACTIONS"); 
						switch(getkey())
						{
						case KEYCANCEL:
						case KEYMENU:
							goto KEYDOWNLABEL;
							break;
						case KEYATM2:
							//LASTTRANSACTION
							if (RePrintTransaction(1) == 0)
							{

							}else
							{
										PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
										ScrAttrSet(0);
										ScrGotoxy(0, 2);
										Lcdprintf("ERROR PRINTING "); 
										getkey();
							}
							goto REPRINTLABEL;
							break;
						case KEYATM3:
							//SPECIFIC TRANSACTION
							if (RePrintTransaction(2) == 0)
							{

							}else
							{
										PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
										ScrAttrSet(0);
										ScrGotoxy(0, 2);
										Lcdprintf("ERROR PRINTING "); 
										getkey();
							}
							goto REPRINTLABEL;
							break;
						case KEYATM4:
							//ALL TRANSACTION
							if (RePrintTransaction(3) == 0)
							{

							}else
							{
										PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
										ScrAttrSet(0);
										ScrGotoxy(0, 2);
										Lcdprintf("ERROR PRINTING "); 
										getkey();
							}
							goto REPRINTLABEL;
							break;
						}

					}
					break;
				case KEYATM4:
					//PIN BLOCK

					PINBLOCKLABEL:
					ScrSetIcon (ICON_DOWN, CLOSEICON);
					ScrSetIcon (ICON_UP, CLOSEICON);
					while(1)
					{
						Lcdprintf("  PIN BLOCK  "); 
						ScrAttrSet(0);
						ScrGotoxy(0, 2);
						Lcdprintf("3DES"); 
						ScrGotoxy(0, 4);
						Lcdprintf("DUKPT"); 
						ScrGotoxy(0, 6);
						Lcdprintf("INJECT DUKPT KEYS"); 
						switch(getkey())
						{
						case KEYCANCEL:
						case KEYMENU:
							goto KEYDOWNLABEL;
							break;
						case KEYATM2:
							//3DES
							if(ReadDevice(&MyDevice)!= 0)
							{
								//Error Reading Configuration Information
							}
							MyDevice.PinBlockEncryptionType = TRIDES;

							 if(SaveDevice(&MyDevice)!=0)
								{
										//Error Saving Configuration Information
										PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
										ScrAttrSet(0);
										ScrGotoxy(0, 2);
										Lcdprintf("ERROR UPDATING "); 
										getkey();
								}
							 else{
										//Error Saving Configuration Information
										PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
										ScrAttrSet(0);
										ScrGotoxy(0, 2);
										Lcdprintf("UPDATED "); 
										getkey();
								}
							 goto PINBLOCKLABEL;
							break;
						case KEYATM3:
							//DUKPT
							if(ReadDevice(&MyDevice)!= 0)
							{
								//Error Reading Configuration Information
							}
							MyDevice.PinBlockEncryptionType = DUKPT;

							 if(SaveDevice(&MyDevice)!=0)
								{
										//Error Saving Configuration Information
										PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
										ScrAttrSet(0);
										ScrGotoxy(0, 2);
										Lcdprintf("ERROR UPDATING "); 
										getkey();
								}
							 else{
										//Error Saving Configuration Information
										PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
										ScrAttrSet(0);
										ScrGotoxy(0, 2);
										Lcdprintf("UPDATED "); 
										getkey();
								}
							  goto PINBLOCKLABEL;
							break;
						case KEYATM4:
							//INJECT DUKT KEYS
							//Error Saving Configuration Information
										PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
										ScrAttrSet(0);
										ScrGotoxy(0, 2);
										Lcdprintf("INJECTING DUKPT KEYS "); 
										getkey();
										goto PINBLOCKLABEL;
							break;
						}

					}
					break;
			}

			}
			break;
		case KEYATM3:
		//GPRS Settings
		GPRSSETTINGSMENU:
	while(1)
	{
	ScrCls();
	ScrAttrSet(1);
	Lcdprintf("  GPRS  "); 
	ScrAttrSet(0);
	ScrGotoxy(0, 2);
	Lcdprintf("PRIMARY"); 
	ScrGotoxy(0, 4);
	Lcdprintf("SECONDARY"); 
	switch(getkey())
	{
		case KEYMENU:
		goto BeginFunctionShowAdmin;
		case KEYCANCEL:
		goto BeginFunctionShowAdmin;
		break;
		case KEYATM2:
			//Primary SIM SETTINGS
			PRIMARYSIMMENU:
			while(1)
			{
				ScrCls();
	ScrAttrSet(1);
	Lcdprintf("  PRIMARY  "); 
	ScrAttrSet(0);
	ScrGotoxy(0, 2);
	Lcdprintf("APN"); 
	ScrGotoxy(0, 4);
	Lcdprintf("Username"); 
	ScrGotoxy(0, 6);
	Lcdprintf("Password ");
switch(getkey())
	{
	case KEYMENU:
	
		goto GPRSSETTINGSMENU;
	case KEYCANCEL:
		
		goto GPRSSETTINGSMENU;
		case KEYATM2:
		//APN
		if(ReadDevice(&MyDevice)!= 0)
		{
			//Error Reading Configuration Information
		}
		PubShowTitle(TRUE,(unsigned char *)"ENTER APN");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
		 switch(PubGetString(ALPHA_IN|ECHO_IN,0,50,MyDevice.DeviceGprsSettings.APN,60) )
		 {
		 case 0:
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING "); 
			getkey();
			goto PRIMARYSIMMENU;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("UPDATED"); 
			getkey();
			goto PRIMARYSIMMENU;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
		//PubGetStrParam(GS_ALPHA,0,(unsigned char *)"APN NAME",0,50,MainDevice.DeviceGprsSettings.APN);
		break;
		case KEYATM3:
		//USERNAME
			if(ReadDevice(&MyDevice)!=0)
			{
			//Error Reading Configuration Information
			}
		PubShowTitle(TRUE,(unsigned char *)"ENTER UserName");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
		 switch(PubGetString(ALPHA_IN|ECHO_IN,0,50,MyDevice.DeviceGprsSettings.UserName,60) )
		 {
		 case 0:
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING "); 
			getkey();
			goto PRIMARYSIMMENU;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("UPDATED"); 
			getkey();
			goto PRIMARYSIMMENU;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
			break;
		case KEYATM4:
		//password
			if(ReadDevice(&MyDevice)!=0)
			{
			//Error Reading Configuration Information
			}
		PubShowTitle(TRUE,(unsigned char *)"ENTER PASSWORD");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
		switch(PubGetString(ALPHA_IN|ECHO_IN,0,50,MyDevice.DeviceGprsSettings.Password,60) )
		 {
		 case 0:
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING  "); 
			getkey();
			goto PRIMARYSIMMENU;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf(" UPDATED"); 
			getkey();
			goto PRIMARYSIMMENU;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
		break;
	}
			}
		break;
		case KEYATM3:
			//SECONDARY
SECONDARYSIMMENU:

			while(1)
			{
				ScrCls();
	ScrAttrSet(1);
	Lcdprintf("  GPRS  "); 
	ScrAttrSet(0);
	ScrGotoxy(0, 2);
	Lcdprintf("APN"); 
	ScrGotoxy(0, 4);
	Lcdprintf("Username"); 
	ScrGotoxy(0, 6);
	Lcdprintf("Password ");
switch(getkey())
	{

	case KEYMENU:
	
		goto GPRSSETTINGSMENU;
	case KEYCANCEL:
		
		goto GPRSSETTINGSMENU;
		case KEYATM2:
		//APN
		if(ReadDevice(&MyDevice)!= 0)
		{
			//Error Reading Configuration Information
		}
		PubShowTitle(TRUE,(unsigned char *)"ENTER APN");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
		 switch(PubGetString(ALPHA_IN|ECHO_IN,0,50,MyDevice.DeviceGprsSettingsSecondary.APN,60) )
		 {
		 case 0:
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING "); 
			getkey();
			goto SECONDARYSIMMENU;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("UPDATED"); 
			getkey();
			goto SECONDARYSIMMENU;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
		//PubGetStrParam(GS_ALPHA,0,(unsigned char *)"APN NAME",0,50,MainDevice.DeviceGprsSettings.APN);
		break;
		case KEYATM3:
		//USERNAME
			if(ReadDevice(&MyDevice)!=0)
			{
			//Error Reading Configuration Information
			}
		PubShowTitle(TRUE,(unsigned char *)"ENTER UserName");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
		 switch(PubGetString(ALPHA_IN|ECHO_IN,0,50,MyDevice.DeviceGprsSettingsSecondary.UserName,60) )
		 {
		 case 0:
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING "); 
			getkey();
			goto SECONDARYSIMMENU;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("UPDATED"); 
			getkey();
			goto SECONDARYSIMMENU;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
			break;
		case KEYATM4:
		//password
			if(ReadDevice(&MyDevice)!=0)
			{
			//Error Reading Configuration Information
			}
		PubShowTitle(TRUE,(unsigned char *)"ENTER PASSWORD");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
		switch(PubGetString(ALPHA_IN|ECHO_IN,0,50,MyDevice.DeviceGprsSettingsSecondary.Password,60) )
		 {
		 case 0:
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING  "); 
			getkey();
			goto SECONDARYSIMMENU;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf(" UPDATED"); 
			getkey();
			goto SECONDARYSIMMENU;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
		break;
	}
			}
		break;
	}

	}
		break;
		case KEYATM4:
		//TMS SETTINGS
			TAMSETTINGSLABEL:
			while(1)
			{
			PubShowTitle(TRUE,(unsigned char *)"TMS");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("TerminalId"); 
			ScrGotoxy(0, 4);
			Lcdprintf("IP Adress"); 
			ScrGotoxy(0, 6);
			Lcdprintf("Port"); 
			switch(getkey())
			{
			case KEYMENU:
			goto BeginFunctionShowAdmin;
			case KEYCANCEL:
			goto BeginFunctionShowAdmin;
			case KEYATM2:
				//TerminalID
				if(ReadDevice(&MyDevice)!=0)
					{
					//Error Reading Configuration Information
					}
					PubShowTitle(TRUE,(unsigned char *)"ENTER TERMINALID");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
		switch(PubGetString(ALPHA_IN|ECHO_IN,0,50,(unsigned char *)MyDevice.TerminalId,60) )
		 {
		 case 0:
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING  "); 
			getkey();
			goto TAMSETTINGSLABEL;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf(" UPDATED"); 
			getkey();
			goto TAMSETTINGSLABEL;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
		break;
			case KEYATM3:
				//Ip Adress
				if(ReadDevice(&MyDevice)!=0)
					{
					//Error Reading Configuration Information
					}
					PubShowTitle(TRUE,(unsigned char *)"ENTER IPADRESS");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
					switch(PubGetString(ALPHA_IN|ECHO_IN,0,15,(unsigned char *)MyDevice.DeviceCommunication.TMS,60) )
		 {
		 case 0:
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING  "); 
			getkey();
			goto TAMSETTINGSLABEL;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf(" UPDATED"); 
			getkey();
			goto TAMSETTINGSLABEL;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
				break;
			case KEYATM4:
				//Port
				if(ReadDevice(&MyDevice)!=0)
					{
					//Error Reading Configuration Information
					}
					PubShowTitle(TRUE,(unsigned char *)"ENTER PORT");
		//iRet = PubGetString(ALPHA_IN|ECHO_IN,0,4,TechnicalMenuPin,60);
					memset(TempPort,0,sizeof(TempPort));
					sprintf((char *)TempPort,"%d",MyDevice.DeviceCommunication.Port);
		switch(PubGetString(NUM_IN|ECHO_IN,1,5,(unsigned char *)TempPort,60) )
		 {
		 case 0:
			 MyDevice.DeviceCommunication.Port =  atoi((char *)TempPort);
			 if(SaveDevice(&MyDevice)!=0)
			{
			//Error Saving Configuration Information
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf("ERROR UPDATING  "); 
			getkey();
			goto TAMSETTINGSLABEL;
			}
			PubShowTitle(TRUE,(unsigned char *)"INFORMATION");
			ScrAttrSet(0);
			ScrGotoxy(0, 2);
			Lcdprintf(" UPDATED"); 
			getkey();
			goto TAMSETTINGSLABEL;
		 case 0xFE:
			 break;
		 case 0xFF:
			 break;

		 }
				break;
			}
		 
			break;
			
			}

		break;
		

	}

	}
}

int TruncateAllFiles()
{
	
	int fd ;
	/*
	fd = open((unsigned char *)"DEVICE", O_RDWR|O_CREATE);
	truncate(fd,0);
	close(fd);
	

	fd = open((unsigned char *)"BATCHINFO", O_RDWR|O_CREATE);
	truncate(fd,0);
	close(fd);
	*/

	fd = open((unsigned char *)"TRANDT", O_RDWR|O_CREATE);
	truncate(fd,0);
	close(fd);
}