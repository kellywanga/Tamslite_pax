
#include <posapi.h>
#include <posapi_all.h>
#include "Emv.h"
#include "Applib.h"
#include "TamsLite.h"
#include "PhyComms.h"
#include "Logo.h"
#include "Hex2Bin.h"
#include "EftUtil.h"
#include "Bin2Hex.h"
const APPINFO AppInfo={
	"TamsLite",
	"PAX",
	"1.0",
	"ITex",
	"Lite POS",
	"",
	0,
	0,
	0,
	""
};
extern EMV_PARAM gxEmvParam;
extern char gszAmount[12+1];
extern unsigned char aucAmount[6+1];
extern EftTransaction MyEmvTransaction;
void AppSetMckParam(void)
{
	EMV_MCKPARAM	stMckParam;
	EMV_EXTMPARAM	stExtMckParam;

	memset(&stMckParam, 0, sizeof(stMckParam));
	stMckParam.ucBypassPin = 1;
	stMckParam.ucBatchCapture = 1;

	memset(&stExtMckParam, 0, sizeof(stExtMckParam));
	stExtMckParam.ucUseTermAIPFlg = 1;
	stExtMckParam.aucTermAIP[0] = 0x08;
	stExtMckParam.ucBypassAllFlg = 1;

	stMckParam.pvoid = &stExtMckParam;
	EMVSetMCKParam(&stMckParam);

    /* !!!!!!!!!!!!!!!!
    If you encounter exception in this function, then
    set project->settings->code generation->struct member alignment = 1.
    */
}
int event_main(ST_EVENT_MSG *msg)
{
	SystemInit();
	return 0;
}

unsigned char TechnicalMenuPin[6];
const unsigned char *TECHINCALMENUPIN="4839";
int main(void)
{
	unsigned char time[7], time2[20], buff[20];
	int CallHomeCounter,iRet ;
	unsigned char CountryCode[5] = "0566";
	unsigned char BinCountryCode[50] = {0};
	/***TerminalCapabilities  **********/
	unsigned char TerminalCapabilitiesByteOne =capMANUAL_KEY_ENTRY|capMAGNETIC_STRIPE|capICC_WITH_CONTACTS; 
	unsigned char TerminalCapabilitiesByteTwo  = capPLAIN_PIN|capENCIPHERED_PIN_ONLINE|capSIGNATURE|capENCIPHERED_PIN_OFFLINE|capNO_CVM;
	unsigned char TerminalCapabilitiesByteThree = acpNUMERIC_KEYS|acpALPHA_AND_SPECIAL_KEYS|acpCOMMAND_KEYS|acpFUNCTION_KEYS;
	/*** End TerminalCapabilities  **********/

	/*** AddtionalTerminalCapabilities  **********/
	unsigned char AddtionalTerminalCapabilitiesByteOne =acpCASH|acpGOODS|acpSERVICES|acpCASHBACK|acpINQUIRY|acpTRANSFER|acpPAYMENT|acpADMINISTRATIVE; 
	unsigned char AddtionalTerminalCapabilitiesByteTwo  = acpCASH_DEPOSIT;
	unsigned char AddtionalTerminalCapabilitiesByteThree = capSTATIC_AUTH|capDYNAMIC_AUTH|capCARD_CAPTURE|capCOMBINED_DDA;
	unsigned char AddtionalTerminalCapabilitiesByteFour = acpPRINT_ATTENDANT|acpPRINT_CARDHOLDER|acpDISPLAY_ATTENDANT|acpDISPLAY_CARDHOLDER;
	unsigned char AddtionalTerminalCapabilitiesByteFive = acpNORDIC|acpTURKISH|acpHEBREW|acpGREEK|acpARABIC|acpCYRILLIC|acpLATIN4|acpLATIN3|acpLATIN2|acpLATIN1;
	/*** End AddtionalTerminalCapabilities  **********/
	int iRc = 0;
	SystemInit();
	iRc = EMVCoreInit();
	iRc=EMVSetPCIModeParam(1,(unsigned char*)"4,5,6",30000);

	/*Initialize Application */
	PubShowTitle(TRUE,(unsigned char *)"TamsLite.....");
	ScrPrint(0,4,1,"Initializing Device.....");
	if(ReadDevice(&MyDevice) != 0 || ReadBatchInfo(&MyBatchInfo) != 0 )
	{
		PubShowTitle(TRUE,(unsigned char *)"TamsLite.....");
		ScrPrint(0,4,1,"Device Initializion Failed..  ");
		getkey();
	}
	PubShowTitle(TRUE,(unsigned char *)"TamsLite.....");
	ScrPrint(0,4,1,"Initializing Comms.....");
	iRet=WirelessInit();
	if (iRet != 0 && iRet!=-212)
	{
	PubShowTitle(TRUE,(unsigned char *)"TamsLite.....");
	ScrPrint(0,4,1,"Comm Initializing Failed..  ");
	getkey();
	}		

	BEGINAPP:
	ScrCls();
	ScrGotoxy(0, 0);
    ScrDrLogo(ItexLogo);
	CallHomeCounter = 10000;
	MagClose();
	MagOpen();
	MagReset(); 
	while (1) {
		//TODO: Start your application
		memset(&gxEmvParam,0,sizeof(gxEmvParam));
	EMVGetParameter(&gxEmvParam);
	gxEmvParam.Capability[0] = TerminalCapabilitiesByteOne;
	gxEmvParam.Capability[1] = TerminalCapabilitiesByteTwo;
	gxEmvParam.Capability[2] = TerminalCapabilitiesByteThree;
	gxEmvParam.ExCapability[0] = 0;
	gxEmvParam.ExCapability[1]=0;
	gxEmvParam.ExCapability[2]=0;
	gxEmvParam.ExCapability[3] = 0;
	gxEmvParam.ExCapability[4] = 0;
	hex2bin((char *)CountryCode,(char *)BinCountryCode,sizeof(BinCountryCode)-1);
	memcpy(gxEmvParam.CountryCode,   BinCountryCode, 2);
	memcpy(gxEmvParam.TransCurrCode, BinCountryCode, 2);
	memcpy(gxEmvParam.ReferCurrCode, "\x00\x00", 2);
	memcpy(gxEmvParam.MerchCateCode, "\x00\x00", 2);
	gxEmvParam.TransCurrExp  = 0x00;
	gxEmvParam.ReferCurrExp  = 0x00;
	gxEmvParam.ForceOnline   = 0;
	gxEmvParam.GetDataPIN    = 1;
	gxEmvParam.SurportPSESel = 1;
	EMVSetParameter(&gxEmvParam);

	InitTestApps();
	InitTestKeys();
	AppSetMckParam();
	
		while (1) 
		{

				if (kbhit()==0)
					{
					 if(getkey()==KEYFN)
						{
							int iRet  =-1;
							PubShowTitle(TRUE,(unsigned char *)"Admin");
							ScrGotoxy(0, 2);
							Lcdprintf("Enter Pin.."); 
							ScrGotoxy(0, 4);
							memset(TechnicalMenuPin,0,sizeof(TechnicalMenuPin));
							iRet = PubGetString(ALPHA_IN|ECHO_IN|PASS_IN,0,4,TechnicalMenuPin,60);
							if(iRet == 0)
							{
								char Tst[20];
								iRet = strcmp((char *)&TECHINCALMENUPIN[0],(char *)&TechnicalMenuPin[0]);
								if(iRet == 0)
								{
						
						
								ScrSetIcon (ICON_DOWN, CLOSEICON);
								ScrSetIcon (ICON_UP, CLOSEICON);
								AdminMenuLabel:
								switch(ShowAdminMenu())
								{
								case KEYCANCEL:
								goto BEGINAPP;
								default:
								goto AdminMenuLabel;
								}
						
				
								}else
								{
								ScrCls();
								ScrGotoxy(0, 0);
								ScrAttrSet(1);
								Lcdprintf("  Admin  "); 
								ScrAttrSet(0);
								ScrGotoxy(0, 2);
								Lcdprintf("Wrong Pin..");  
								getkey();
								goto BEGINAPP;
					}
				}else if(iRet ==1)
				{
					
					goto BEGINAPP;
				}else
				{
					
					goto BEGINAPP;
				}

			

			}
			}

		if (MagSwiped()==0)
		{
			ST_MAGCARD MAGSTRIPE;
			memset(&MAGSTRIPE,0,sizeof(MAGSTRIPE));
			MAGSTRIPE.RetCode=MagRead(MAGSTRIPE.track1,MAGSTRIPE.track2,MAGSTRIPE.track3);
			 MagClose(); 
			 if(!MAGSTRIPE.RetCode) 
			 { 
				PubShowTitle(TRUE,(unsigned char *)"ERROR");
				ScrGotoxy(0, 2);
				Lcdprintf(" Press Any Key... "); 
				getkey();
				goto BEGINAPP;
			 } 
			 if(MAGSTRIPE.RetCode & 0x02) 
			 {
				 //eft purc
				 ShowMagStripeEftMenu(MAGSTRIPE);
				 goto BEGINAPP;
			 } if(MAGSTRIPE.RetCode & 0x04)
			 {
			 }
				MagClose();
				MagOpen();
				MagReset();
		}

		if (IccDetect(0)==0)
		{
			
			Beep();
			memset(&MyEmvTransaction,0,sizeof(EftTransaction));
			ShowChipEftMenu(&MyEmvTransaction);
			//ICC processing
			iRc=EMVClearTransLog();
		//LOG_PRINTF(("main() -> EMVClearTransLog ret %d",iRc));

        ScrCls();
        ScrPrint(0,0,0,"PROCESSINGA...");


		iRc = EMVAppSelect(0, 0);
		//LOG_PRINTF(("main() -> EMVAppSelect ret %d",iRc));

		if( iRc==EMV_DATA_ERR || iRc==ICC_RESET_ERR || iRc==EMV_NO_APP ||
			iRc==ICC_CMD_ERR  || iRc==EMV_RSP_ERR )
		{
			//LOG_PRINTF(("main() EMVAppSelect -> Need Fallback!!"));
			ShowTVR_TAC_IAC();
			PromptRemoveICC();
			continue;
		}

		if( iRc==EMV_TIME_OUT || iRc==EMV_USER_CANCEL )
		{
			//LOG_PRINTF(("main() EMVAppSelect -> User Cancel!!"));
			ShowTVR_TAC_IAC();
			PromptRemoveICC();
			continue;
		}

		if( iRc!=EMV_OK )
		{
			//LOG_PRINTF(("main() EMVAppSelect -> Transaction Fail!!"));
			ShowTVR_TAC_IAC();
			PromptRemoveICC();
			continue;
		}

		iRc = EMVReadAppData();
		if( iRc==EMV_TIME_OUT || iRc==EMV_USER_CANCEL )
		{
			//LOG_PRINTF(("main() EMVReadAppData -> User Cancel!!"));
			ShowTVR_TAC_IAC();
			PromptRemoveICC();
			continue;
		}

		if( iRc!=EMV_OK )
		{
			//LOG_PRINTF(("main() EMVReadAppData -> Transaction Fail!!"));
			ShowTVR_TAC_IAC();
			PromptRemoveICC();
			continue;
		}

		EMVGetParameter(&gxEmvParam);
		gxEmvParam.TransType = EMV_GOODS|EMV_SERVICE;
		EMVSetTLVData(0x9C, (uchar *)"\x00", 1);
		EMVSetParameter(&gxEmvParam);

		EMVSetTLVData(0x9F02, aucAmount, 6);
		EMVSetTLVData(0x9F53, (unsigned char*)"R", 1);

    
		iRc = EMVCardAuth();
		//LOG_PRINTF(("main() -> EMVCardAuth() ret %d",iRc));
		if( iRc!=EMV_OK )
		{
			//LOG_PRINTF(("main() EMVReadAppData -> Transaction Fail!!"));
			ShowTVR_TAC_IAC();
			PromptRemoveICC();
			continue;
		}

		iRc = EMVProcTrans();
		//LOG_PRINTF(("main() -> EMVProcTrans() ret %d",iRc));

        if (iRc==0)
        {
            ScrCls();
            ScrPrint(0,0,0,"TRANS APPROVED.");
            DelayMs(2000);
        }

		ShowTVR_TAC_IAC();
		PromptRemoveICC();
	

		goto BEGINAPP;
		}


			GetTime(time);
			if (!memcmp(time, time2, 6)) continue;
			sprintf(buff, "%c%c/%c%c   %c%c:%c%c:%c%c",
				(time[1] >> 4) + 0x30, (time[1] & 0x0f) + 0x30,
				(time[2] >> 4) + 0x30, (time[2] & 0x0f) + 0x30,
				(time[3] >> 4) + 0x30, (time[3] & 0x0f) + 0x30,
				(time[4] >> 4) + 0x30, (time[4] & 0x0f) + 0x30,
				(time[5] >> 4) + 0x30, (time[5] & 0x0f) + 0x30);
			ScrGotoxy(0, 6);
			Lcdprintf(buff);
			memcpy(time2, time, 6);
		}
	}
	return 0;
}
