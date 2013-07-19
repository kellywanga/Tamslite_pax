#include <posapi.h>
#include <posapi_all.h>
#include "Applib.h"
#include <string.h>
#include "PhyComms.h"
#include "NetProc.h"
#include "Debug.h"
#include "TamsLite.h"
static unsigned char AppWaitKey(int wsec)
{
	unsigned char ucKeyval;

	TimerSet(0,(ushort)(wsec*10));
	ucKeyval=NOKEY;
	while (TimerCheck(0))
	{
		if (kbhit()==OK)
		{
			ucKeyval=getkey();
			break;
		}
	}
	return ucKeyval;
}

int PostRequest(char *Purl, char*RequestData, char * AuthenticationData,char* RcvData)
{
			int               iRet=1,iSize=102400,iTimeoutNum;
			int	              ii;
			char PortNo[5];
			int ContentLength=0;
			char DebugMessage[50] = {0};
			unsigned char sAPN[50] ="ETISALAT"; //{0};
			unsigned char sUser[50] = {0};
			unsigned char sPwd[50] = {0};
			char sIP[50]           ="41.58.130.139";// "";//{0};
			short ucPort = 80;
			int pos;
			Device  MyLocalDevice = {0};
			ReadDevice(&MyLocalDevice);
			
			strncpy((char *)sAPN,(char *)MyLocalDevice.DeviceGprsSettings.APN,sizeof(sAPN)-1);
			strncpy((char *)sUser,(char *)MyLocalDevice.DeviceGprsSettings.UserName,sizeof(sUser)-1);
			strncpy((char *)sPwd,(char *)MyLocalDevice.DeviceGprsSettings.Password,sizeof(sPwd)-1);
			strncpy((char *)sIP,(char *)MyLocalDevice.DeviceCommunication.TMS,sizeof(sIP)-1);
			ucPort = MyLocalDevice.DeviceCommunication.Port;
			sprintf(DebugMessage,"Logon %s %s %s",sAPN,sUser,sPwd);
			SendDebug(DebugMessage);
			iRet = WirelessDial(sAPN,sUser,sPwd);
			memset(DebugMessage,0,sizeof(DebugMessage));
			sprintf(DebugMessage,"PPP Logon:%d",iRet);
			SendDebug(DebugMessage);
			if (iRet!=0) 
			{
				
				return -1;
			}			
			
			for (ii=0; ii<3; ii++)
			{
				int    iNCRet,iTemp,jj,iNSRet;
				uchar  ucTmpSend[102400],ucTmpRcv[102400];
				iNSRet = Net_Open(sIP, ucPort);
				if (iNSRet < 0)
				{
					memset(DebugMessage,0,sizeof(DebugMessage));
					sprintf(DebugMessage,"NetSocket:%d", iNSRet);
					SendDebug(DebugMessage);
					return -1;
					break;
				}
				memset(ucTmpSend, 0, sizeof(ucTmpSend));
				memset(PortNo,0,sizeof(PortNo));
				sprintf(PortNo,"%d",ucPort);
				if(RequestData != NULL)
				{
					ContentLength = strlen(RequestData);
				}
				
				sprintf((char *)ucTmpSend, 
						"POST /%s HTTP/1.1\r\n" \
						"Host: %s:%s\r\n" \
						"User-Agent: PAX/%s\r\n" \
						"Accept: application/xml\r\n" \
						"Content-Type: application/x-www-form-urlencoded\r\n" \
						"Terminal: %s\r\n" \
						"Content-Length: %d\r\n"\
						"Sign: %s\r\n\r\n",						
						Purl,sIP, PortNo, "4.00",MyLocalDevice.TerminalId,ContentLength,AuthenticationData						 
						);


				if(RequestData != NULL)
				{
				strcat((char *)ucTmpSend,RequestData);
				}
				
				SendDebug((char *)ucTmpSend);
					iNCRet = Net_Send(iNSRet,ucTmpSend,strlen((char *)ucTmpSend));
					memset(DebugMessage,0,sizeof(DebugMessage));
					sprintf(DebugMessage,"TCP Send:%d ",iNCRet);
					SendDebug(DebugMessage);
				if (iNCRet<0)
				{
					Net_Close(iNSRet);
					return -1;
					break;
				}
				
				iTemp = 0;
				iTimeoutNum = 0;
				memset(ucTmpRcv, 0, sizeof(ucTmpRcv));
				
				while (1)
				{
					
					iNCRet = Net_Recv(iNSRet,ucTmpRcv+iTemp,10*1000);
					if (iNCRet<0)
					{
						if (iNCRet==-13)
						{
							iTimeoutNum++;
							if (iTimeoutNum<3)
							{
								continue;
							}
							else
							{
								 break;
							}
						}
						else
						{
							break;
						}
				
						break ;
					}
					
					
					iTemp+=iNCRet;
					if (iTemp>=iSize)break;
				
					if (kbhit()==0)
					{
						if (getkey()==KEYCANCEL) break;
					}
				}
				pos = mystrpos((char *)ucTmpRcv,"\r\n\r\n");
				substring((size_t)pos,strlen((char *)ucTmpRcv),(char *)ucTmpRcv ,RcvData,strlen(RcvData));
				//strcpy(RcvData,(char *)ucTmpRcv);
				memset(DebugMessage,0,sizeof(DebugMessage));
				sprintf(DebugMessage,"TCP Recv:%d Rlen:%d ",iNCRet,iTemp);
				SendDebug(DebugMessage);
				 Net_Close(iNSRet);
				WirelessHangUp();
				PortOpen(0, (unsigned char *)"115200,8,n,1");
				PortSends(0,(unsigned char *)ucTmpRcv,sizeof((char *)ucTmpRcv));
				PortClose(0);
				
				break;
			}
			
			return 0;
}