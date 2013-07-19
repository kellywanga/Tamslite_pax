#include <posapi.h>
#include <posapi_all.h>
#include "Applib.h"
#include "Debug.h"
#include <string.h>
#include "NetProc.h"

int Net_Open(char *remote_addr, short remote_port)
{
	int iRet, iSocket;
	NET_SOCKADDR stServer_addr;
	char remotePortChar[20] ={0};
	char DebugMessage[50] = {0};

	memset(DebugMessage,0,sizeof(DebugMessage));
	sprintf(DebugMessage,"Creating Socket:%d",0);
	SendDebug(DebugMessage);
	iRet = NetSocket(NET_AF_INET, NET_SOCK_STREAM, 0);
	if (iRet < 0)
	{
		memset(DebugMessage,0,sizeof(DebugMessage));
		sprintf(DebugMessage,"Creating Socket fail:",0);
		SendDebug(DebugMessage);
		return iRet;
	}
	iSocket = iRet;

	iRet = Netioctl(iSocket, CMD_TO_SET, 6000);
	if (iRet < 0)
	{
		NetCloseSocket(iSocket);
		return iRet;
	}
	iRet = SockAddrSet(&stServer_addr, remote_addr, remote_port);
	if (iRet != 0)
	{
		NetCloseSocket(iSocket);
		return iRet;
	}

	iRet = NetConnect(iSocket, &stServer_addr, sizeof(stServer_addr));
	if (iRet < 0)
	{
		NetCloseSocket(iSocket);
		return iRet;
	}
	return iSocket;
}
int Net_Send(int net_hd, void *buf, int size)
{
	int iRet;
	int iSendLen;
	int iSumLen;

	iRet = Netioctl(net_hd, CMD_TO_SET, 30000);
	if (iRet < 0)
	{
		return iRet;
	}

	iSumLen = 0;
	while(1)
	{
		if (size > TCPMAXSENDLEN)
		{
			iSendLen = TCPMAXSENDLEN;
			size = size - TCPMAXSENDLEN;
		}
		else
		{
			iSendLen = size;
		}	
		iRet = NetSend(net_hd, (char *)buf + iSumLen, iSendLen, 0);
		if (iRet < 0)
		{
			return iRet;
		}
		if (iRet != iSendLen)
		{
			return -1;
		}
		iSumLen = iSumLen + iSendLen;
		if (iSendLen <= TCPMAXSENDLEN)
		{
			break;
		}	
	}

	return iSumLen;
	return 0;
}
int Net_Recv(int net_hd, void *buf, int size)
{
	int iRet, iResult;
	int	 iCountTimeout = 0;
	iRet = Netioctl(net_hd, CMD_TO_SET, 20000);
	if (iRet < 0)
	{
		return iRet;
	}

	while (TRUE)
	{
		DelayMs(200);
		iRet = NetRecv(net_hd, buf, size, 0);
		if (iRet == -13)
		{
			iCountTimeout++;
			if (iCountTimeout > 2)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	return iRet;
}
int Net_Close(int net_hd)
{
	NetCloseSocket(net_hd);
	return 0;
}