//
//#include <posapi.h>
//#include <posapi_all.h>
//#include "Applib.h"
//#include "TamsLite.h"
//#include "PhyComms.h"
//#include  "PostRequest.h"
//#include  "Keys.h"
//#include  "Eod.h"
//#include  "GetParameters.h"
//#include  "CallHome.h"
//#include "EMVApplist.h"
//#include  "EmvCaKey.h"
//#include   "emvlib.h"
//#include "Emv.h"
//#include  "Hex2Bin.h"
//#include "Debug.h"
//const APPINFO AppInfo={
//	"TamsLite",
//	"PAX",
//	"1.0",
//	"ITex",
//	"Lite POS",
//	"",
//	0,
//	0,
//	0,
//	""
//};
//
//extern EMV_PARAM gxEmvParam;
//extern char gszAmount[12+1];
//extern unsigned char aucAmount[6+1];
//extern EftTransaction  MyEmvTransaction;
//void AppSetMckParam(void)
//{
//	EMV_MCKPARAM	stMckParam;
//	EMV_EXTMPARAM	stExtMckParam;
//
//	memset(&stMckParam, 0, sizeof(stMckParam));
//	stMckParam.ucBypassPin = 1;
//	stMckParam.ucBatchCapture = 1;
//
//	memset(&stExtMckParam, 0, sizeof(stExtMckParam));
//	stExtMckParam.ucUseTermAIPFlg = 1;
//	stExtMckParam.aucTermAIP[0] = 0x08;
//	stExtMckParam.ucBypassAllFlg = 1;
//
//	stMckParam.pvoid = &stExtMckParam;
//	EMVSetMCKParam(&stMckParam);
//
//    /* !!!!!!!!!!!!!!!!
//    If you encounter exception in this function, then
//    set project->settings->code generation->struct member alignment = 1.
//    */
//}
//int event_main(ST_EVENT_MSG *msg)
//{
//	SystemInit();
//	return 0;
//}
//
//
//int main(void)
//{
//	unsigned char time[7] = {0}, time2[20] = {0}, buff[20] = {0};
//	unsigned char CountryCode[5] = "0566";
//	unsigned char BinCountryCode[50] = {0};
//	/***TerminalCapabilities  **********/
//	unsigned char TerminalCapabilitiesByteOne =capMANUAL_KEY_ENTRY|capMAGNETIC_STRIPE|capICC_WITH_CONTACTS; 
//	unsigned char TerminalCapabilitiesByteTwo  = capPLAIN_PIN|capENCIPHERED_PIN_ONLINE|capSIGNATURE|capENCIPHERED_PIN_OFFLINE|capNO_CVM;
//	unsigned char TerminalCapabilitiesByteThree = acpNUMERIC_KEYS|acpALPHA_AND_SPECIAL_KEYS|acpCOMMAND_KEYS|acpFUNCTION_KEYS;
//	/*** End TerminalCapabilities  **********/
//
//	/*** AddtionalTerminalCapabilities  **********/
//	unsigned char AddtionalTerminalCapabilitiesByteOne =acpCASH|acpGOODS|acpSERVICES|acpCASHBACK|acpINQUIRY|acpTRANSFER|acpPAYMENT|acpADMINISTRATIVE; 
//	unsigned char AddtionalTerminalCapabilitiesByteTwo  = acpCASH_DEPOSIT;
//	unsigned char AddtionalTerminalCapabilitiesByteThree = capSTATIC_AUTH|capDYNAMIC_AUTH|capCARD_CAPTURE|capCOMBINED_DDA;
//	unsigned char AddtionalTerminalCapabilitiesByteFour = acpPRINT_ATTENDANT|acpPRINT_CARDHOLDER|acpDISPLAY_ATTENDANT|acpDISPLAY_CARDHOLDER;
//	unsigned char AddtionalTerminalCapabilitiesByteFive = acpNORDIC|acpTURKISH|acpHEBREW|acpGREEK|acpARABIC|acpCYRILLIC|acpLATIN4|acpLATIN3|acpLATIN2|acpLATIN1;
//	/*** End AddtionalTerminalCapabilities  **********/
//	char R[102400] = {0};
//	int iRc = 0;
//	SystemInit();
//	iRc = EMVCoreInit();
//	iRc=EMVSetPCIModeParam(1,(unsigned char*)"4,5,6",30000);
//	MagClose();
//	MagOpen();
//	MagReset(); 
//	WirelessInit();
//	BEGINAPP:
//	while (1) {
//		//TODO: Start your application
//		//PostRequest("tams/tams/devinterface/getkeys.php",NULL,"",R);
//	NewKey();
//	GetKeys();
//	GetParameters();
//    EndOfDay();
//	//CallHome();
//	//GetEmvApplist();
//	//GetEmvCaKeys();
//	memset(&gxEmvParam,0,sizeof(gxEmvParam));
//	EMVGetParameter(&gxEmvParam);
//	gxEmvParam.Capability[0] = TerminalCapabilitiesByteOne;
//	gxEmvParam.Capability[1] = TerminalCapabilitiesByteTwo;
//	gxEmvParam.Capability[2] = TerminalCapabilitiesByteThree;
//	gxEmvParam.ExCapability[0] = 0;
//	gxEmvParam.ExCapability[1]=0;
//	gxEmvParam.ExCapability[2]=0;
//	gxEmvParam.ExCapability[3] = 0;
//	gxEmvParam.ExCapability[4] = 0;
//	hex2bin((char *)CountryCode,(char *)BinCountryCode,sizeof(BinCountryCode)-1);
//	memcpy(gxEmvParam.CountryCode,   BinCountryCode, 2);
//	memcpy(gxEmvParam.TransCurrCode, BinCountryCode, 2);
//	memcpy(gxEmvParam.ReferCurrCode, "\x00\x00", 2);
//	memcpy(gxEmvParam.MerchCateCode, "\x00\x00", 2);
//	gxEmvParam.TransCurrExp  = 0x00;
//	gxEmvParam.ReferCurrExp  = 0x00;
//	gxEmvParam.ForceOnline   = 0;
//	gxEmvParam.GetDataPIN    = 1;
//	gxEmvParam.SurportPSESel = 1;
//	EMVSetParameter(&gxEmvParam);
//
//	InitTestApps();
//	InitTestKeys();
//	AppSetMckParam();
//		while (1) {
//			if (MagSwiped()==0)
//		{
//			ST_MAGCARD MAGSTRIPE;
//			memset(&MAGSTRIPE,0,sizeof(MAGSTRIPE));
//			MAGSTRIPE.RetCode=MagRead(MAGSTRIPE.track1,MAGSTRIPE.track2,MAGSTRIPE.track3);
//			 MagClose(); 
//			 if(!MAGSTRIPE.RetCode) 
//			 { 
//				PubShowTitle(TRUE,(unsigned char *)"ERROR");
//				ScrGotoxy(0, 2);
//				Lcdprintf(" Press Any Key... "); 
//				getkey();
//				goto BEGINAPP;
//			 } 
//			 if(MAGSTRIPE.RetCode & 0x02) 
//			 {
//				 //eft purc
//				 ShowMagStripeEftMenu(MAGSTRIPE);
//				 goto BEGINAPP;
//			 } if(MAGSTRIPE.RetCode & 0x04)
//			 {
//			 }
//				MagClose();
//				MagOpen();
//				MagReset();
//		}
//		if (IccDetect(0)==0)
//		{
//			
//			Beep();
//			ShowChipEftMenu();
//			//ICC processing
//			iRc=EMVClearTransLog();
//		//LOG_PRINTF(("main() -> EMVClearTransLog ret %d",iRc));
//
//        ScrCls();
//        ScrPrint(0,0,0,"PROCESSING...");
//
//
//		iRc = EMVAppSelect(0, 0);
//		//LOG_PRINTF(("main() -> EMVAppSelect ret %d",iRc));
//
//		if( iRc==EMV_DATA_ERR || iRc==ICC_RESET_ERR || iRc==EMV_NO_APP ||
//			iRc==ICC_CMD_ERR  || iRc==EMV_RSP_ERR )
//		{
//			//LOG_PRINTF(("main() EMVAppSelect -> Need Fallback!!"));
//			ShowTVR_TAC_IAC();
//			PromptRemoveICC();
//			continue;
//		}
//
//		if( iRc==EMV_TIME_OUT || iRc==EMV_USER_CANCEL )
//		{
//			//LOG_PRINTF(("main() EMVAppSelect -> User Cancel!!"));
//			ShowTVR_TAC_IAC();
//			PromptRemoveICC();
//			continue;
//		}
//
//		if( iRc!=EMV_OK )
//		{
//			//LOG_PRINTF(("main() EMVAppSelect -> Transaction Fail!!"));
//			ShowTVR_TAC_IAC();
//			PromptRemoveICC();
//			continue;
//		}
//
//		iRc = EMVReadAppData();
//		if( iRc==EMV_TIME_OUT || iRc==EMV_USER_CANCEL )
//		{
//			//LOG_PRINTF(("main() EMVReadAppData -> User Cancel!!"));
//			ShowTVR_TAC_IAC();
//			PromptRemoveICC();
//			continue;
//		}
//
//		if( iRc!=EMV_OK )
//		{
//			//LOG_PRINTF(("main() EMVReadAppData -> Transaction Fail!!"));
//			ShowTVR_TAC_IAC();
//			PromptRemoveICC();
//			continue;
//		}
//
//		EMVGetParameter(&gxEmvParam);
//		gxEmvParam.TransType = EMV_GOODS|EMV_SERVICE;
//		EMVSetTLVData(0x9C, (uchar *)"\x00", 1);
//		EMVSetParameter(&gxEmvParam);
//
//		EMVSetTLVData(0x9F02, aucAmount, 6);
//		EMVSetTLVData(0x9F53, (unsigned char*)"R", 1);
//
//    
//		iRc = EMVCardAuth();
//		//LOG_PRINTF(("main() -> EMVCardAuth() ret %d",iRc));
//		if( iRc!=EMV_OK )
//		{
//			//LOG_PRINTF(("main() EMVReadAppData -> Transaction Fail!!"));
//			ShowTVR_TAC_IAC();
//			PromptRemoveICC();
//			continue;
//		}
//
//		iRc = EMVProcTrans();
//		//LOG_PRINTF(("main() -> EMVProcTrans() ret %d",iRc));
//
//        if (iRc==0)
//        {
//            ScrCls();
//            ScrPrint(0,0,0,"TRANS APPROVED.");
//            DelayMs(2000);
//        }
//
//		ShowTVR_TAC_IAC();
//		PromptRemoveICC();
//	
//
//		}
//			GetTime(time);
//			if (!memcmp(time, time2, 6)) continue;
//			sprintf(buff, "%c%c/%c%c   %c%c:%c%c:%c%c",
//				(time[1] >> 4) + 0x30, (time[1] & 0x0f) + 0x30,
//				(time[2] >> 4) + 0x30, (time[2] & 0x0f) + 0x30,
//				(time[3] >> 4) + 0x30, (time[3] & 0x0f) + 0x30,
//				(time[4] >> 4) + 0x30, (time[4] & 0x0f) + 0x30,
//				(time[5] >> 4) + 0x30, (time[5] & 0x0f) + 0x30);
//			ScrGotoxy(0, 6);
//			Lcdprintf(buff);
//			memcpy(time2, time, 6);
//		}
//	}
//	return 0;
//}
