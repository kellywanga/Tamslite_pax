#include <posapi.h>
#include <posapi_all.h>
#include <posapi_s80.h>
#include "Debug.h"
void prepare_header (char *a_szDest, char*a_szFile, long a_lLine)
{
    char *p = (char*)NULL, *p1 = (char*)NULL;
	
    p = a_szFile;
    while (1)
    {
        p1 = strchr(p, '\\');
        if (p1 == NULL)
            break;
        p = p1+1;
    }
	
    sprintf (a_szDest, "F:%s|L:%05ld| ", p, a_lLine);
}
void SendDebug(const char *message)
{
	PortOpen(0, (unsigned char *)"115200,8,n,1");
	PortSends(0,(unsigned char *)message,strlen(message));
	PortSend(0,'\n');
	PortClose(0);
}

//void LOG_PRINTF(const char *message)
//{
//	PortOpen(0, (unsigned char *)"115200,8,n,1");
//	PortSends(0,(unsigned char *)message,strlen(message));
//	PortSend(0,'\n');
//	PortClose(0);
//}

void LOG_PRINTF(char* a_szFmt, ...)
{
 static char szMessage [2048 + 1];
    va_list xArgList;
	
    va_start (xArgList, a_szFmt);
    vsprintf (szMessage, a_szFmt, xArgList);
    va_end (xArgList);
	
    strcat (szMessage, "\r\n");
	
	PortOpen(0, (unsigned char *)"115200,8,n,1");
	PortSends(0,(unsigned char *)szMessage,strlen(szMessage));
	PortSend(0,'\n');
	PortClose(0);

}
void log_hex_printf (char*a_szFile, long a_lLine, char* a_szMessage, unsigned char* a_ucBuffer, short a_sBufferLen)
{
    char szMsg [200];
    int i;
	
    memset(szMsg, 0, sizeof (szMsg));
    prepare_header(szMsg, a_szFile, a_lLine);
    strcat (szMsg, "\r\n");
    strcat (szMsg, a_szMessage);
//    strcat (szMsg, "\r\n");
    SendDebug (szMsg);
	
    for (i = 0; i < a_sBufferLen; i++)
    {
//        if (((i + 1) % 16) == 0)
		if ((i % 16) == 0)
            SendDebug ("\r\n");
		
        memset(szMsg, 0, sizeof (szMsg));
        sprintf (szMsg, "%02X ", (char)a_ucBuffer [i]);
        SendDebug (szMsg);
    }
	
	SendDebug ("\r\n");
    return;
}