//---------------------------------------------------------------------------
// NAME: security.c
// DESC: This module implements the security on the vx670 terminal
//
//------------------------------------------------------------[ Includes ]---
//sk:


typedef short boolean;



#include <posapi.h>
#include <posapi_all.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "hex2bin.h"
#include <stdlib.h>
#include <string.h>
#include "Applib.h"
#include "Debug.h"
#include "security.h"



//----------------------------------------------------[ Extern Functions ]---


//---------------------------------------------------[ Extern References ]---

//---------------------------------------------------[ Private Functions ]---

//---------------------------------------------------[ Private variables ]---




//sk:
//------------------------------------------------------[ Des_3DES_DEA ]---
// DESC: encrypt or decrript
// ARGS:
//		in : input
//		out: output
//		key: key
//		boolEncrypt: true to encypt and false to decrypt
// RETURN:
//		none
//---------------------------------------------------------------------------
void	Des_3DES_DEA(unsigned char *in, unsigned char *out, unsigned char *key, short	boolEncrypt)
{
	unsigned char input[9],
				  output[20];
	memset(input,0,sizeof(input));
	memset(output,0,sizeof(output));
	strncpy((char *)input,(const char *)in,8);
	LOG_PRINTF(("STRING FOR ENCRIPTION IS: %s",input));
	//if encryption is required
	if(boolEncrypt){
		PubDes(TRI_ENCRYPT, input, key, output);
		
			//LOG_PRINTF(("ERROR: Encrypting..."));
	}//if
	else
	{
		PubDes(TRI_DECRYPT, input, key, output);
			
	}//else
	strcpy((char *)out,(const char *)output);
}//Des_3DES_DEA





//------------------------------------------------------[ writeDukptKey ]---
// writeDukptKey
// ARGS:
//		in : input
//		out: output
//		key: key
//		boolEncrypt: true to encypt and false to decrypt
// RETURN:
//		none
//---------------------------------------------------------------------------
int writeDukptKey(void)
{
	//char                        IPEK[16];
 //   int                         iRet=0;
 //   char                        IKSN[21];
	//int inRetVal;

 //   memset(IKSN, 0, sizeof(IKSN));
	//memset(IPEK, 0, sizeof(IPEK));
 //   iRet = GetKeyCards(IKSN, IPEK);
 //   if(iRet){

	//	DisplayMsg(MSG_ERROR,"",gettext("Security Loading Failed"));

 //       return iRet;
 //   }//if

 //   //load initial IPP keys (IKSN+IPEK) here

	//LOG_PRINTF(("--In InitializePinPad Function --"));

	//inRetVal = inInitPinPad();

	//LOG_PRINTF(("<<- Returned by inInitPinPad function = %d ->> ", inRetVal));

 //   if (inRetVal != VS_SUCCESS)
 //   {
 //       gfPPadPresent = VS_FALSE;
	//	LOG_PRINTF(("--PINpad init NOT successful --"));

 //       DisplayMsg(MSG_ERROR,"",gettext("Security Loading Failed"));

	//	return VS_SUCCESS;
 //   }
 //   else
 //   {
 //       gfPPadPresent = VS_TRUE;

	//	LOG_PRINTF(("--PINpad init successful --"));

	//}


 //   LOG_PRINTF(("--Calling PINpad Idle Prompt --"));

	//inExecLoadKeyIdlePrompt(IKSN, IPEK);
 //   recieveLoadedKeyIPPResponse();

 //   LOG_PRINTF(("--Done loading Key --"));

	//RestartApp();

	return 0;

}//SecKbTst_WriteKey



//--------------------------------------------------------------[ GetPin ]---
// DESC: Get The Pin from the User
// ARGS:
//		pcSerial    - Return buffer for the Serial number
//      pcPinBlock  - Return pinblock form the sec
//      cPan        - The PAN of the Transaction
// RETURN:
//		none
//----------------------------------------------------------------------------
int GetPin(char *pcSerial, char *pcPinBlock, char *cPan, char *cMsg, int iKeyIndex)
{
	//char cTemp[100];
	//int ret = 0;

	//if(iKeyIndex == 2){ //3DES
	//   ret = GetPin2(pcSerial, pcPinBlock, cPan, cMsg);
	//   if (ret){
	//	   return ret;
	//   }
	//}
	//else if(iKeyIndex == 1){ //DUKPT

 //   memset(pcSerial, 0, sizeof(pcSerial));
	//memset(pcPinBlock, 0, sizeof(pcPinBlock));

	//clrscr();
	//if( strlen(cMsg) )
	//{
	//	write_at(cMsg,strlen(cMsg),1,2);
	//}//if
	//else
	//{
	//	write_at("PIN REQUIRED",strlen("PIN REQUIRED"),1,2);
	//}

	//gotoxy(1,3);

	//set_cursor(1);
	//ret = inInitializePinPad(pcSerial, pcPinBlock, cPan);
	//set_cursor(0);

	//if(ret == 0)
	//	LOG_PRINTF(("SUCCESS: PIN ENTRY"));

	//if(ret != 0)
	//	LOG_PRINTF(("ERROR: PIN ENTRY"));

	//if(ret)
	//	return -7;

	//}

	return 0;
}

//--------------------------------------------------------------[ GetPin ]---
// DESC: Get The Pin from the User
// ARGS:

//      pcPinBlock  - Return pinblock form the sec
//      cPan        - The PAN of the Transaction
// RETURN:
//		none
//---------------------------------------------------------------------------
int TriDesGetPin(char *pcPinBlock, char *cPan, char *cMsg)
{
    int             iRet=0;
    char            cTemp[15];
    char            pindata[15];
    char            B1[17],B2[17];
    char            B1XORB2[9];
    char            tmpCARD1[17];
    char            tmpCARD2[17];
    char            cTmpClearKey[17];
	int             i;
	//unsigned char   Temp[9];
	char            Temp[9];
	//char            hex[5];
	//char            pinLen[15];
	char                    cTmpPinBlock[9];
    int                     iCount;
    char	                acBuffer[30];
	char	                *pcBuffer;
    char                    *pcTemp;
	unsigned char	        ucCh;
	int                     panlen;

	memset(pcPinBlock, 0, sizeof(pcPinBlock));

    //ISO Format-0 (ISO standard 9564 to generate the pin block)

    //Prompt for PIN
    for(;;){
        memset(cTemp, 0, sizeof(cTemp));
		PubShowTitle(TRUE,(unsigned char *)"ENTER PIN");
        if (PubGetString(ALPHA_IN|ECHO_IN|PASS_IN,4,10,(unsigned char *)cTemp,60) !=0)
		{
            return -1;//ERR_USERCANCEL;
        }//if
		
        if(strlen(cTemp) > 0)
		{
            break;
        }
        else
		{
            PubShowTitle(TRUE,(unsigned char *)"INVALID PIN RETRY");
            continue;
            //return -7;
        }
    }//if

    memset(B1, 0, sizeof(B1));
    memset(B2, 0, sizeof(B2));
	memset(pindata,0,sizeof(pindata));

    strcat(B1,"04");
    strcat(B1,cTemp);
    strcat(B1,"FFFFFFFFFF");
    strcat(B2,"0000");

    panlen = strlen(cPan);

	LOG_PRINTF(("cPan:"));
	LOG_PRINTF((cPan));

    if(panlen > 12){
      strncpy(pindata, &cPan[panlen - 13], 12);
      strcat(B2,pindata);
    }
    else{
      strcat(B2,cPan);
    }

	LOG_PRINTF(("B1:"));
	LOG_PRINTF((B1));

	LOG_PRINTF(("B2:"));
	LOG_PRINTF((B2));

    memset(tmpCARD1, 0, sizeof(tmpCARD1));
    memset(tmpCARD2, 0, sizeof(tmpCARD2));

    hex2bin(B1, tmpCARD1, 8);
    hex2bin(B2, tmpCARD2, 8);

    memset(B1XORB2, 0, sizeof(B1XORB2));

    for ( i = 0; i < 8; i++ )
    {
        B1XORB2[i] = tmpCARD1[i] ^ tmpCARD2[i];
    }

    memset(tmpCARD1, 0, sizeof(tmpCARD1));
    memset(tmpCARD2, 0, sizeof(tmpCARD2));

    // Extract Security key form Track 1
    hex2bin("10101010101010101010101010101010", tmpCARD1, 16);
    hex2bin("01010101010101010101010101010101", tmpCARD2, 16);

    memset(cTmpClearKey, 0, sizeof(cTmpClearKey));

    for ( i = 0; i < 16; i++ )
    {
        cTmpClearKey[i] = tmpCARD1[i] ^ tmpCARD2[i];
    }

    memset(Temp,0,sizeof(Temp));

	Des_3DES_DEA((unsigned char *)B1XORB2, (unsigned char  *)Temp, (unsigned char  *)cTmpClearKey, TRUE);
	//Des_3DES_DEA((byte *)B1XORB2[8], (byte *)Temp[8], (byte *)cTmpClearKey, TRUE);

	memset(acBuffer, 0, sizeof (acBuffer));
    memset(cTmpPinBlock, 0, sizeof (cTmpPinBlock));
    memcpy(cTmpPinBlock, (char *) Temp, 8);
    pcTemp = cTmpPinBlock;
    pcBuffer = acBuffer;
    for (iCount = 0; iCount < 8; iCount++) {
            ucCh = *pcTemp;
            pcBuffer += sprintf(pcBuffer, "%02X", (int) ucCh);
            pcTemp++;
    }//while
	memset(pcPinBlock,0,sizeof(pcPinBlock));
    strncpy(pcPinBlock, acBuffer, strlen(acBuffer));

	//strcpy(pcPinBlock, "948A6910BEE54D99");

	LOG_PRINTF(("pcPinBlock:"));
	LOG_PRINTF((pcPinBlock));


    return 0;
}//GetPin


int GetKeyCards(char *pcIKSN, char *IPEK)
{
 //   //int             iRet;
 //   //REC_CardInfo    SecCard1Info;
 //   //REC_CardInfo    SecCard2Info;

 //   char            cTmpIKSN[21];// = {0xff,0xff,0x98,0x76,0x54,0x32,0x10,0xE0,0x00,0x00};
 //   char            cDervKey[16] = {0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0x00};
 //   char            tmpCARD1[16];
 //   char            tmpCARD2[16];
 //   char            cTmpClearKey[16];
 //   char            cTmpBuf[35];
 //   char            cTmp[10];
 //   int             iCount;

	//char            KDUKPT[16];
 //   char            tmpIPEK[16];

 //   char	        acBuffer[30];
	//char	        *pcBuffer;
 //   char            *pcTemp;
	//unsigned char	ucCh;

	//char            IKSN[10];

 //   /*for(;;){
 //       memset(&SecCard1Info, 0, sizeof(SecCard1Info));
 //       iRet = getCardSwipe(true, true, &SecCard1Info, true, gettext("Security Card 1"));
 //       if (iRet == ERR_MANUAL){
 //           if (Prompt(gettext("Component 1"), SecCard1Info.cTrack1, INPUT_ALPHA, 32, false)){
 //               return ERR_USERCANCEL;
 //           }//if
 //           if (Prompt(gettext("Card No"), SecCard1Info.cPAN, INPUT_ALPHA, 2, false)){
 //               return ERR_USERCANCEL;
 //           }//if
 //           iRet = 0;
 //       }//if
 //       if (!iRet){
 //           //Save First Track 1
 //           memset(&SecCard2Info, 0, sizeof(SecCard2Info));

 //           iRet = getCardSwipe(true, true, &SecCard2Info, true, gettext("Security Card 2"));
 //           if (iRet == ERR_MANUAL){
 //               if (Prompt(gettext("Component 2"), SecCard2Info.cTrack1, INPUT_ALPHA, 32, false)){
 //                   return ERR_USERCANCEL;
 //               }//if
 //               if (Prompt(gettext("Card No"), SecCard2Info.cPAN, INPUT_ALPHA, 2, false)){
 //                   return ERR_USERCANCEL;
 //               }//if
 //               iRet = 0;
 //           }//if
 //           if (iRet){
 //               return iRet;
 //           }//if
 //           else{
 //               if (!strcmp(SecCard1Info.cTrack1, SecCard2Info.cTrack1)){
 //                   iRet = DisplayMsg(MSG_CONFIRM,gettext("Retry"), gettext("Cards Cannot be the same"));
 //                   //Cancel
 //                   if (iRet == 1) {
 //                       return ERR_USERCANCEL;
 //                   }//if
 //                   continue;
 //               }//if
 //               else{
 //                   if (strcmp(SecCard1Info.cPAN, SecCard2Info.cPAN)){
 //                       iRet = DisplayMsg(MSG_CONFIRM,gettext("Retry"), gettext("Card Set Dont Match"));
 //                       //Cancel
 //                       if (iRet == 1) {
 //                           return ERR_USERCANCEL;
 //                       }//if
 //                       continue;
 //                   }//if
 //                   break;
 //               }//else
 //           }//else
 //       }//if
 //       else{
 //           return iRet;
 //       }//else
 //   }//for*/

 //   memset(cTmpIKSN, 0, sizeof(cTmpIKSN));
 //   memset(cTmpBuf, 0, sizeof(cTmpBuf));
 //   memset(cTmp, 0, sizeof(cTmp));

 //   strcpy(cTmpIKSN, "FFFF");
 //   //strncat(cTmpIKSN, SecCard1Info.cPAN, 2);
	//strncat(cTmpIKSN, "02", 2);
 //   //GetTermId(cTmpBuf);
 //  // getTerminalSerial(cTmpBuf);
	////LOG_PRINTF(("cTmpBuf=%s",cTmpBuf));

	//strncpy(cTmp, &cTmpBuf[strlen(cTmpBuf)-8], 8);
	////strncpy(cTmp, "66522993", 8);

 //   //TermId 8
 //   strncat(cTmpIKSN, cTmp, 8);
 //   strcat(cTmpIKSN, "E00000");

	//LOG_PRINTF(("KSN=%s",cTmpIKSN));

	//strcpy(pcIKSN,cTmpIKSN);

 //   hex2bin(cTmpIKSN, IKSN, 10);

 //   memset(tmpCARD1, 0, sizeof(tmpCARD1));
 //   memset(tmpCARD2, 0, sizeof(tmpCARD2));

 //   //Extract Security key form Track 2
 //   //hex2bin(SecCard1Info.cTrack1, tmpCARD1, 16);
 //   //hex2bin(SecCard2Info.cTrack1, tmpCARD2, 16);

	//hex2bin("39E8ADFF72DAA3093052A5CCC48B0C65",tmpCARD1,16);
 //   hex2bin("9F99D49F784DE1D8C90E48471FCBC67E",tmpCARD2,16);

 //   memset(cTmpClearKey, 0, sizeof(cTmpClearKey));
 //   for(iCount=0;iCount<=16;iCount++){
 //       cTmpClearKey[iCount] = tmpCARD1[iCount] ^ tmpCARD2[iCount];
	//	cDervKey[iCount] = cDervKey[iCount] ^ cTmpClearKey[iCount];
 //   }//for

	//Des_3DES_DEA((byte *)IKSN, (byte *)KDUKPT, (byte *)cTmpClearKey, TRUE);
 //   Des_3DES_DEA((byte *)IKSN, (byte *)&KDUKPT[8], (byte *)cDervKey, TRUE);

	//memset(acBuffer, 0, sizeof(acBuffer));
 //   memset(tmpIPEK, 0, sizeof(tmpIPEK));
 //   memcpy(tmpIPEK, (char *)KDUKPT, 16);
 //   pcTemp = tmpIPEK;
 //   pcBuffer = acBuffer;
 //   for(iCount=0;iCount<16;iCount++){
 //       ucCh = *pcTemp;
 //       pcBuffer += sprintf(pcBuffer, "%02X",(int)ucCh);
 //       pcTemp++;
	//}//while
 //   strcpy(IPEK, acBuffer);

	//LOG_PRINTF(("IPEK:"));
	//LOG_PRINTF((IPEK));

    return 0;
}//GetKeyCards


//---------------------------------------------------------------------------
//
//                               T H E   E N D
//
//---------------------------------------------------------------------------
