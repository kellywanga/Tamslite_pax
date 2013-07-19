
#include "posapi.h"
#include "applib.h"
//#include "global.h"

/********************** Internal macros declaration ************************/
#define TIMER_TEMPORARY         4       // 短时间的定时器(其他模块可以共用)
#ifdef _POS_DEBUG
	#define LEN_DBGDATA         1024    // 显示数据最大长度
	#define MAX_CHARS           5       // 每行可以格式化的字符数目
	#define CHARS_PERLINE       21      // 每行的字符数目
	#define MAX_LOG_MSG         (((LEN_DBGDATA+MAX_CHARS-1)/MAX_CHARS)*CHARS_PERLINE)
#endif /* _POS_DEBUG */

#define LEN_GETSTRING		512	// for PubGetString()

/********************** Internal structure declaration *********************/
/********************** Internal functions declaration *********************/
static void GetBaseName(uchar *pszFullPath, uchar *pszBaseName);
static void MapChar2Index(uchar ch, int *piRow, int *piCol);
static void ShowWorkBuf(uchar ucMode, uchar *pszWorkBuff, int iLen);

/********************** Internal variables declaration *********************/
#ifdef _POS_DEBUG
static uchar glszDbgInfo[MAX_LOG_MSG+128];
#endif

static uchar sgMinLineNo;

static char *pszAlphaTbl[10] =
{
	"0,*# ~`!@$%^&-+=(){}[]<>_|\\:;\"\'?/",
	"1QZ.", "2ABC", "3DEF",
	"4GHI", "5JKL", "6MNO",
	"7PRS", "8TUV", "9WXY",
};


/********************** external reference declaration *********************/

/******************>>>>>>>>>>>>>Implementations<<<<<<<<<<<<*****************/


//将源串与目标串异或后存入目标串(两个串长度相等)
//Source string XOR object string, then store the result in object string.(The two string are same length)
void PubXor(uchar *psVect1, uchar *psVect2, uint uiLength)
{
    uint   uiCnt;

    for(uiCnt = 0; uiCnt < uiLength; uiCnt++)
    {
        psVect1[uiCnt] ^= psVect2[uiCnt];
    }
}

//将二进制源串分解成双倍长度可读的16进制串,0x12AB-->"12AB"
//Convert BIN string to readable HEX string, which have double length of BIN string. 0x12AB-->"12AB"
void PubBcd2Asc(uchar *psIn, uint uiLength, uchar *psOut)
{
    static const uchar ucHexToChar[17] = {"0123456789ABCDEF"};
    uint   uiCnt;

    for(uiCnt = 0; uiCnt < uiLength; uiCnt++)
    {
        psOut[2*uiCnt]   = ucHexToChar[(psIn[uiCnt] >> 4)];
        psOut[2*uiCnt + 1] = ucHexToChar[(psIn[uiCnt] & 0x0F)];
    }
}

// 同vOneTwo()函数,并在目标串后添一 '\0'
//Similar with function PubOne2Two(),add '\0' at the end of object string
void PubBcd2Asc0(uchar *psIn, uint uiLength, uchar *pszOut)
{
    PubBcd2Asc(psIn, uiLength, pszOut);
    pszOut[2*uiLength] = 0;
}

//将可读的16进制串合并成其一半长度的二进制串,"12AB"-->0x12AB
//Convert readable HEX string to BIN string, which only half length of HEX string. "12AB"-->0x12AB
void PubAsc2Bcd(uchar *psIn, uint uiLength, uchar *psOut)
{
    uchar   tmp;
    uint    i;

    for(i = 0; i < uiLength; i += 2)
    {
        tmp = psIn[i];
        if( tmp > '9' )
        {
            tmp = (uchar)toupper((int)tmp) - 'A' + 0x0A;
        }
        else
        {
            tmp &= 0x0F;
        }
        psOut[i / 2] = (tmp << 4);

        tmp = psIn[i+1];
        if( tmp > '9' )
        {
            tmp = toupper((char)tmp) - 'A' + 0x0A;
        }else
        {
            tmp &= 0x0F;
        }
        psOut[i/2] |= tmp;
    }
}

//将数字串转变成长整数,相当于atol(),但是该函数不要求零字符结束
//Convert number string to long integer, similar to atol(). This function don't request end with '\0'.
ulong PubAsc2Long(uchar *psString, uint uiLength, ulong *pulOut)
{
    uchar    szBuff[15+1];
	ulong    ulTmp;

    sprintf((char *)szBuff, "%.*s", uiLength <= 15 ? uiLength : 15, psString);
	ulTmp =  (ulong)atol((char*)szBuff);

    if (pulOut != NULL)
	{
		*pulOut = ulTmp;
	}
    return ulTmp;
}

//整型数转换成高位在前的字符串表示(按照网络字节格式存放)
//Convert integer to string which high bit at the front. (Store it according to network byte format)
void PubLong2Char(ulong ulSource, uint uiTCnt, uchar *psTarget)
{
    uint    i;

    for(i = 0; i < uiTCnt; i++)
    {
        psTarget[i] = (uchar)(ulSource >> (8 * (uiTCnt - i - 1)));
    }
}

//高位在前的字符串表示转换为整型数
//Convert the character string with high bit in the front to integer
ulong PubChar2Long(uchar *psSource, uint uiSCnt, ulong *pulTarget)
{
    uint    i;
	ulong ulTmp;

    ulTmp = 0L;
    for(i = 0; i < uiSCnt; i++)
    {
        ulTmp |= ((ulong)psSource[i] << 8 * (uiSCnt - i - 1));
    }

	if (pulTarget != NULL)
	{
		*pulTarget = ulTmp;
	}
    return ulTmp;
}

//将整数转换为BCD格式的串存放,1234 --> 0x12 0x34
//Converted integer to BCD string,1234 --> 0x12 0x34
void PubLong2Bcd(ulong ulSource, uint uiTCnt, uchar *psTarget)
{
    uchar    szFmt[30], szBuf[30];

    sprintf((char*)szFmt, "%%0%dlu", uiTCnt*2);
    sprintf((char*)szBuf, (char *)szFmt, ulSource);
    if( psTarget != NULL )
    {
        PubAsc2Bcd(szBuf, uiTCnt * 2, psTarget);
    }
}

//转换BCD串为整数
//Convert BCD string to integer
ulong PubBcd2Long(uchar *psSource, uint uiSCnt, ulong *pulTarget)
{
    uchar   szBuf[30];
	ulong   ulTmp;

    PubBcd2Asc0(psSource, uiSCnt, szBuf);
	ulTmp = (ulong)atol((char *)szBuf);
    if( pulTarget != NULL )
    {
        *pulTarget = ulTmp;
    }

    return ulTmp;
}

//将字符串转换为大写
//Convert the character string to capital
void PubStrUpper(uchar *pszString)
{
    while( *pszString )
    {
        *pszString = toupper((char)*pszString);
        pszString++;
    }
}

//将字符串转换为小写
//Convert the character string to lowercase
void PubStrLower(uchar *pszString)
{
    while( *pszString )
    {
        *pszString = (uchar)tolower((int)*pszString);
        pszString++;
    }
}

//删除字符串中的空白字符(包括,空格/回车/换行/TAB字符)
//Delete the characters include blank,enter,newline,TAB in the string
void PubTrimStr(uchar *pszString)
{
#define ISSPACE(ch) ( ((ch) == ' ')  || ((ch) == '\t') || \
                      ((ch) == '\n') || ((ch) == '\r') )

    uchar *p, *q;

    if( !pszString || !*pszString )
    {
        return;
    }

    p = &pszString[strlen((char*)pszString) - 1];
    while(( p > pszString) && ISSPACE(*p) )
    {
        *p-- = 0;
    }
    if( (p == pszString) && ISSPACE(*p) )  *p = 0;

    for(p = pszString; *p && ISSPACE(*p); p++);
    if( p != pszString )
    {
        q = pszString;
        while( *p )   *q++ = *p++;
        *q = 0;
    }
#undef ISSPACE
}


//删除字符串两端的特定字符
//Delete the specified character in string
void PubTrimSpcStr(uchar *pszString, uchar ucSpcChar)
{
	uchar *p, *q;

	if( !pszString || !*pszString )
	{
		return;
	}

	p = &pszString[strlen((char*)pszString) - 1];
	while( (p > pszString) && ((*p) == ucSpcChar))
	{
		*p-- = 0;
	}
	if( p == pszString && ((*p) == ucSpcChar) )  *p = 0;

	for(p = pszString; *p && ((*p) == ucSpcChar); p++);
	if( p != pszString )
	{
		q = pszString;
		while( *p )   *q++ = *p++;
		*q = 0;
	}
}


//删除字符串左边连续的特定字符
//Delete the specified continuous character on the left of the string
// void PubTrimSpcStrLeft(uchar *pszString, uchar ucSpcChar)
// {
// 	ushort unStrLen;
// 	uchar *pszStrHead;
//
//     if( !pszString || !*pszString )
//     {
//         return;
//     }
//
// 	unStrLen = strlen((char*)pszString);
// 	pszStrHead = pszString;
//
// 	while(*pszString++ == ucSpcChar)
// 	{
// 		unStrLen--;
// 	}
//
// 	memmove(pszStrHead, pszString, unStrLen + 1);
// }


//删除字符串右边连续的特定字符
//Delete the specified continuous characters on the right of the string
void PubTrimTailChars(uchar *pszString, uchar ucRemoveChar)
{
	int		i, iLen;

	if( !pszString || !*pszString )
    {
        return;
    }

	iLen = strlen((char *)pszString);
	for(i=iLen-1; i>=0; i--)
	{
		if( pszString[i]!=ucRemoveChar )
		{
			break;
		}
		pszString[i] = 0;
	}
}

// 删除前导字符
// delete the specified continuous characters on the left of the string
void PubTrimHeadChars(uchar *pszString, uchar ucRemoveChar)
{
	uchar	*p;

	if( !pszString || !*pszString )
    {
        return;
    }

	for(p=pszString; *p && *p==ucRemoveChar; p++);
	if( p!=pszString )
	{
		while( (*pszString++ = *p++) );
	}
}

// 填充前导字符,以便达到uiTargetLen长度
void PubAddHeadChars( uchar *pszString, uint uiTargetLen, uchar ucAddChar )
{
	uint	uiLen;

	uiLen = (uint)strlen((char *)pszString);
	if( uiLen>=uiTargetLen )
	{
		return;
	}

	memmove(pszString+uiTargetLen-uiLen, pszString, uiLen+1);
	memset(pszString, ucAddChar, uiTargetLen-uiLen);
}

// 不区分大小写的字符串比较函数
//Compare two strings without regard to capital or lowercase
int PubStrNoCaseCmp(uchar *pszStr1, uchar *pszStr2)
{
    if( !pszStr1 || !pszStr2 )
    {
        return -1;
    }

    while( *pszStr1 && *pszStr2 )
    {
        if( toupper((char)*pszStr1) != toupper((char)*pszStr2) )
        {
            return (toupper((char)*pszStr1) - toupper((char)*pszStr2));
        }
        pszStr1++;
        pszStr2++;
    }
    if( !*pszStr1 && !*pszStr2 )
    {
        return 0;
    }
    if( !*pszStr1 )
    {
        return -1;
    }

    return 1;
}

//反转字符串
//reverse the string
void PubStrReverse(uchar *pszString)
{
    int     i, j, iLength;
    uchar    ucTmp;

    iLength = strlen((char*)pszString);
    for(i = 0,j = iLength - 1; i < iLength / 2; i++,j--)
    {
        ucTmp        = pszString[i];
        pszString[i] = pszString[j];
        pszString[j] = ucTmp;
    }
}


//取字节的高四位
//get the high 4 bit of the byte
uchar PubHigh4Bit(uchar ucInChar)
{
	return (ucInChar / 16);
}


//取字节的低四位
//get the low 4 bit of the byte
uchar PubLow4Bit(uchar ucInChar)
{
	return (ucInChar & 0x0F);
}


//两个ASC字符串相乘,结果放在psStr1里
//multiply one ASC string by another string
// void PubAscMul(uchar *psStr1, uchar ucStr1Len, uchar *psStr2, uchar ucStr2Len)
// {
// 	uchar buffer1[100],buffer2[100],i,j;
// 	ushort nFlag = 0;
//
// 	memset(buffer1, '0', sizeof(buffer1));
//
// 	for(i = 0;i < ucStr2Len;i++)
// 	{
// 		memset(buffer2,'0', sizeof(buffer2));
// 		memcpy(buffer2 + i, psStr1, ucStr1Len);
// 		for (j = '0'; j < psStr2[i]; j++)
// 		{
// 			PubAscAdd(buffer1, buffer2, (uint)(ucStr1Len + i), &nFlag);
// 		}
// 	}
// 	memcpy(psStr1, buffer1, ucStr1Len + ucStr2Len);
// }

//两个ASC字符串相乘,结果放在pszProduct里
//multiply one ASC string by another string
void PubAscMul(uchar *pszFaciend, uchar *pszMultiplier, uchar *pszProduct)
{
	uchar	*p, ucTemp, ucCarryBit, szBuff[100+1];
	uint	uiFaciLen, uiMulLen, uiProdPos, uiCnt;

	uiFaciLen = strlen((char *)pszFaciend);
	uiMulLen  = strlen((char *)pszMultiplier);

	PubASSERT( uiFaciLen+uiMulLen<=100 );
	sprintf((char *)szBuff, "%0*ld", uiFaciLen+uiMulLen, 0L);

	for(uiProdPos=0; uiFaciLen>0; uiFaciLen--,uiProdPos++)
	{
		ucCarryBit = 0;
		p = &szBuff[uiProdPos];

		for(uiCnt=uiMulLen; uiCnt>0; uiCnt--)
		{
			ucTemp = (pszFaciend[uiFaciLen-1] & 0x0F) * (pszMultiplier[uiCnt-1] & 0x0F) +
					 ucCarryBit + (*p & 0x0F);
			*p++   = (ucTemp % 10) + '0';
			ucCarryBit = ucTemp / 10;
		}
		if( ucCarryBit!=0 )
		{
			*p++ = ucCarryBit + '0';
		}
	}
	PubTrimTailChars(szBuff, '0');
	PubStrReverse(szBuff);

	if( szBuff[0]==0 )
	{
		sprintf((char *)szBuff, "0");
	}
	if( pszProduct!=NULL )
	{
		sprintf((char *)pszProduct, "%s", szBuff);
	}
}


//长度相等的两个ASC字符串相加,结果放在psStr1里
//Add one ASC string to another one, the length of two strings are same. fill 0 in the front.
// void PubAscAdd(uchar *psStr1, uchar *psStr2, uint uiStrLen, ushort *pnFlag)
// {
// 	uchar by;
// 	ushort ret;
//
// 	psStr1 += uiStrLen - 1;
// 	psStr2 += uiStrLen - 1;
// 	ret = 0xa0;
// 	while(uiStrLen-- > 0)
// 	{
// 		by = *psStr1 + ret + *psStr2--;
// 		ret = 0xa0;
// 		if(by > 9)
// 		{
// 			by += 6;
// 			ret = 0xa1;
// 		}
//
// 		*psStr1-- = by | '0';
// 	}
//
// 	*pnFlag = ret - 0xa0;
// }

//长度相等的两个ASC字符串相加
void PubAscAdd(uchar *psAddend1, uchar *psAddend2, uint uiLen, uchar *pszSum)
{
	uchar	*pszResult, ucCarryBit, ucTemp, szBuff[100];

	ucCarryBit = 0;
	pszResult  = szBuff;
	while( uiLen>0 )
	{
		ucTemp = (psAddend1[uiLen-1] & 0x0F) + (psAddend2[uiLen-1] & 0x0F) + ucCarryBit;
		*pszResult++ = (ucTemp % 10) + '0';
		ucCarryBit   = (ucTemp>9) ? 1 : 0;
		uiLen--;
	}
	if( ucCarryBit!=0 )
	{
		*pszResult++ = '1';
	}
	*pszResult = 0;

	PubStrReverse(szBuff);
	if( pszSum!=NULL )
	{
		sprintf((char *)pszSum, "%s", szBuff);
	}
}

//长度相等的两个ASC字符串相减,结果放在psStr1里
//subtract one ASC string from another one, the length of two strings are same. fill 0 in the front.
// void PubAscSub(uchar *psStr1, uchar *psStr2, uint uiStrLen, ushort *pnFlag)
// {
//     uchar tmp;
// 	ushort ret;
//
//    	ret = 0;
//    	while(uiStrLen-- != 0)
// 	{
// 		tmp = psStr1[uiStrLen] - psStr2[uiStrLen] - ret;
// 		if (tmp > psStr1[uiStrLen])
// 		{
// 			tmp -= 0xf6;
// 			ret = 1;
// 		}
// 		else
// 		{
// 			ret = 0;
// 		}
//
// 		psStr1[uiStrLen] = tmp | '0';
//    	}
//
// 	*pnFlag = ret;
// }

//长度相等的两个ASC字符串相减(psMinuend>psSubtrahend)
void PubAscSub(uchar *psMinuend, uchar *psSubtrahend, uint uiLen, uchar *pszResult)
{
	uchar	*pszOut, ucCarryBit, ucTemp, szBuff[100];

	pszOut     = szBuff;
	ucCarryBit = 0;
	while( uiLen>0 )
	{
		ucTemp = (psMinuend[uiLen-1] & 0x0F) - (psSubtrahend[uiLen-1] & 0x0F) - ucCarryBit + 10;
		*pszOut++  = (ucTemp % 10) + '0';
		ucCarryBit = (psMinuend[uiLen-1]<psSubtrahend[uiLen-1]+ucCarryBit) ? 1 : 0;
		uiLen--;
	}
	*pszOut = 0;

	PubStrReverse(szBuff);
	if( pszResult!=NULL )
	{
		sprintf((char *)pszResult, "%s", szBuff);
	}
}

//ASC字符串减1,结果放在原串里,有返回值
//1 subtracted from the ASC string, the result store in that ASC string
uchar PubAscDec(uchar *psAscStr, uint uiStrLen)
{
	psAscStr += uiStrLen - 1;
	while(--(*psAscStr) < '0')
	{
		*psAscStr-- = '9';
		if(--uiStrLen == 0) return 1;
	}
	return 0;
}


//ASC字符串加1,结果放在原串里,有返回值
//Add 1 to the ASC string, the result store in that ASC string
uchar PubAscInc(uchar *psAscStr, uint uiStrLen)
{
	psAscStr += uiStrLen - 1;
	while(++(*psAscStr) > '9')
	{
		*psAscStr-- = '0';
		if(--uiStrLen == 0) return 1;
	}

	return 0;
}


//长度相等的两个BCD串相加，结果放在psBcdStr1里
//Add one BCD string to another one, the length of two strings are same. fill 0 in the front.
void PubBcdAdd(uchar *psAddend1, uchar *psAddend2, uint uiLen, uchar *psResult)
{
	uchar	sAdd1[100+1], sAdd2[100];

	PubASSERT( uiLen<=50 );
	PubBcd2Asc(psAddend1, uiLen, sAdd1);
	PubBcd2Asc(psAddend2, uiLen, sAdd2);
	PubAscAdd(sAdd1, sAdd2, uiLen*2, sAdd1);
	PubAsc2Bcd(sAdd1, uiLen*2, psResult);
}


//长度相等的两个BCD串相减，结果放在psAddend1里
//subtract one BCD string from another one, the length of two strings are same. fill 0 in the front.
void PubBcdSub(uchar *psMinuend, uchar *psSubtrahend, uint uiLen, uchar *psResult)
{
	uchar	sMinuend[100+1], sSubtrahend[100];
	
	PubASSERT( uiLen<=50 );
	PubBcd2Asc(psMinuend, uiLen, sMinuend);
	PubBcd2Asc(psSubtrahend, uiLen, sSubtrahend);
	PubAscSub(sMinuend, sSubtrahend, uiLen*2, sMinuend);
	PubAsc2Bcd(sMinuend, uiLen*2, psResult);
}


//BCD字符串减1,结果放在原串里,有返回值
//subtracted from the BCD string, the result store in that BCD string
uchar PubBcdDec(uchar *psBcdStr, uint uiStrLen)
{
	psBcdStr += uiStrLen - 1;
	while(--(*psBcdStr) == 0xff)
	{
		*psBcdStr = 0x99;
		psBcdStr--;
		if(--uiStrLen == 0) return 1;
	}

	if(((*psBcdStr) & 0x0f) > 9) *psBcdStr -= 0x06;
	return 0;
}


//BCD字符串加1,结果放在原串里,有返回值
//Add 1 to the BCD string, the result store in that BCD string
uchar PubBcdInc(uchar *psBcdStr, uint uiStrLen)
{
	psBcdStr += uiStrLen - 1;
	while(++(*psBcdStr) > 0x99)
	{
		*psBcdStr = 0;
		psBcdStr--;
		if((--uiStrLen) == 0) return 1;
	}

	if(((*psBcdStr) & 0x0f) > 9) *psBcdStr += 0x06;
	return 0;
}


/*********************************************************************************************/
/*********************************************************************************************/


uchar PubConvAmount(uchar *pszPrefix, uchar *pszIn, uchar ucDeciPos, uchar *pszOut, uchar ucMisc)
{
	int		ii, iInLen, iIntegerLen;
	uchar	bSeparator, bNegative, szBuff[40+1], szTemp[40];
	uchar	*pRead, *pWr;

	PubASSERT(pszIn!=NULL && pszOut!=NULL && ucDeciPos<4);

// 	iInLen = strlen((char *)pszIn);		//输入金额长度
// 	pRead  = pszIn;

	sprintf((char *)szBuff, "%.40s", pszIn);
	PubTrimHeadChars(szBuff, '0');
	iInLen = strlen((char *)szBuff);		//输入金额长度
	pRead  = szBuff;
	pWr = pszOut;

	bSeparator = ucMisc & GA_SEPARATOR;		//whether use ',' to separate huge amount
	bNegative  = ucMisc & GA_NEGATIVE;		//whether display negative amount

	if (pszPrefix!=NULL && *pszPrefix!=0)
	{
		pWr += sprintf((char *)pWr, "%.3s ", pszPrefix);
//		pWr += MIN(strlen((char *)pszPrefix), 4);
	}
	if (bNegative)
	{
		*(pWr++) = '-';
	}

	//小数点前
	if (iInLen>ucDeciPos)	//小数点前非0
	{
		iIntegerLen = iInLen - ucDeciPos;
		ii = iIntegerLen;
		while (ii--)
		{
			*(pWr++) = *(pRead++);
			if (bSeparator && (ii%3==0) && ii)
			{
				*(pWr++) = ',';
			}
		}
	}
	else
	{
		*(pWr++) = '0';
	}

	//小数点及其后
	if (ucDeciPos!=0)
	{
		sprintf((char *)szTemp, "%.*s", ucDeciPos, pRead);
		PubAddHeadChars(szTemp, ucDeciPos, '0');
		sprintf((char *)pWr, ".%s", szTemp);
//		sprintf((char *)pWr, ".%0*s", ucDeciPos, pRead);
		pWr += (ucDeciPos+1);
	}
	else
	{
		*(pWr++) = 0;
	}

	//处理后缀（如果有）
	//...

	/*
	if (ucDeciPos==0)
	{
		if (!bSeparator || iInLen<4)
		{
			strcpy((char *)pWr, (char *)pRead);
			if (iInLen==0)
			{
				strcpy((char *)pWr, "0");
			}
		}
		else	//display ',' as separater
		{
			ii = iInLen;
			while (ii--)
			{
				*(pWr++) = *(pRead++);
				if ((ii%3==0) && ii)
				{
					*(pWr++) = ',';
				}
			}
			*pWr = 0;
		}
	}
	else
	{
		if (iInLen>ucDeciPos)	//输入的金额小数点前不为0
		{
			iIntegerLen = iInLen - ucDeciPos;
			if (!bSeparator || iIntegerLen<4)
			{
				sprintf((char *)pWr, "%.*s.%.*s", iIntegerLen, (char *)pRead,
					ucDeciPos, (char *)(pRead+iIntegerLen));
			}
			else	//display ',' as separater
			{
				ii = iIntegerLen;
				while (ii--)
				{
					*(pWr++) = *(pRead++);
					if ((ii%3==0) && ii)
					{
						*(pWr++) = ',';
					}
				}
				sprintf((char *)pWr, ".%.*s", ucDeciPos, (char *)(pRead));
			}
		}
		else
		{
			sprintf((char *)pWr, "0.%0*s", ucDeciPos, (char *)pRead);
		}
	}
	*/
	return 0;
}


uchar PubKeyPressed(void)
{
	return (kbhit()==0x00);
}


uchar PubWaitKey(short iWaitTime)
{
	uchar   ucKey, bChkTimer;

	if (iWaitTime>=0)
	{
		bChkTimer = TRUE;
		TimerSet(TIMER_TEMPORARY, (short)(iWaitTime*10));
	}
	else
	{
		bChkTimer = FALSE;
	}

	while (1)
	{
		if (PubKeyPressed())
		{
			ucKey = getkey();
			break;
		}
		else if (bChkTimer && TimerCheck(TIMER_TEMPORARY)==0)
		{
			ucKey = NOKEY;
			break;
		}
	}   // end of while (1)

	return ucKey;
}

// display menu
int PubGetMenu(uchar *pszPrompt, MenuItem *pstMenu, uchar ucMode, ushort nTimeOut)
{
	static uchar *pszDefPrompt = (uchar *)"PLS INPUT:";
	uchar   ucKey, ucMaxLen, ucLen, ucCol, ucKeepChars, ucMenuFont, ucRever;
	uchar   ucStartLineNo, ucLineNo, ucMenuLines, ucEndLineNo;
	uchar   ucMenuNum, ucCurItem, ucPageNum, ucPageNo;
	uchar	bOneKeySel, bAutoSno, szInput[2+1], ucInputCnt, ucMenuCnt;
	uchar	ucSelNo, bBottomPrompt, bUseSno, ucCnt, ucShowItemNum;
	uchar	*pDotSign, szMenuNo[2+1], bMenuOk, ucActNum;
	short	iCnt;

	PubASSERT( pstMenu!=NULL );
	kbflush();

	// Analyze menu content
	if( pstMenu==NULL )
	{
		return -1;
	}
	bUseSno = (ucMode & MENU_USESNO);
	for(ucShowItemNum=ucMaxLen=ucMenuNum=0; pstMenu[ucMenuNum].szMenuName[0]; ucMenuNum++)
	{
		ucLen	 = strlen((char *)pstMenu[ucMenuNum].szMenuName);
		ucMaxLen = MAX(ucLen, ucMaxLen);
		if( pstMenu[ucMenuNum].bVisible )
		{
			ucShowItemNum++;
		}
		if( !bUseSno )
		{
			continue;
		}
		pDotSign = (uchar *)strchr((char *)pstMenu[ucMenuNum].szMenuName, '.');
		if( pDotSign==NULL )
		{
			return -2;
		}
		if( pDotSign-pstMenu[ucMenuNum].szMenuName==0 ||
			pDotSign-pstMenu[ucMenuNum].szMenuName>2 )
		{
			return -2;
		}
		// 暂时不检查菜单自带动编号是否为数字
	}
	if( ucMenuNum==0 || ucShowItemNum==0 )
	{
		return -3;
	}

	// process options
	ucMenuNum     = MIN(ucMenuNum, 99);			// 最多99个菜单项可以被显示
	bBottomPrompt = bUseSno  ? TRUE  : (ucMode & MENU_PROMPT);
	bAutoSno      = bUseSno  ? FALSE : (ucMode & MENU_AUTOSNO);
	bOneKeySel    = bAutoSno ? (ucMenuNum<=9) : FALSE;	// 一键选择
	ucKeepChars   = 0;
	if( ucMode & MENU_AUTOSNO )
	{
		ucKeepChars = (ucMenuNum<=9) ? 2 : 3;	// 计算用于显示编号的保留字符数目
	}

	// analyze menu font
	ucMenuFont = (ucMaxLen+ucKeepChars>NUM_MAXZHCHARS) ? ASCII: CFONT;
	if( (ucMode & MENU_CFONT) || (ucMode & MENU_ASCII) )
	{
		ucMenuFont = (ucMode & MENU_CFONT) ? CFONT : ASCII;
	}

	// calculate max. chars per lines and the start column
	if( ucMenuFont==ASCII )
	{
		ucMaxLen = MIN(ucMaxLen, NUM_MAXCHARS-ucKeepChars);
		ucCol    = (ucMode & MENU_CENTER) ? (6*(NUM_MAXCHARS-ucKeepChars-ucMaxLen)/2)   : 0;
	}
	else
	{
		ucMaxLen = MIN(ucMaxLen, NUM_MAXZHCHARS-ucKeepChars);
		ucCol    = (ucMode & MENU_CENTER) ? (8*(NUM_MAXZHCHARS-ucKeepChars-ucMaxLen)/2) : 0;
	}

	// analyze menu start/end lines
	ucStartLineNo = 0;
	if( pszPrompt==NULL || *pszPrompt==0 )
	{
		if( bUseSno )
		{
			pszPrompt   = pszDefPrompt;
			ucMenuLines = (ucMenuFont==ASCII) ? NUM_MAXLINES-2 : NUM_MAXZHLINES-1;
		}
		else if( bAutoSno )
		{
			if( ucMenuFont==ASCII )
			{
				ucMenuLines = bOneKeySel ? NUM_MAXLINES : NUM_MAXLINES-2;
			}
			else
			{
				ucMenuLines = bOneKeySel ? NUM_MAXZHLINES : NUM_MAXZHLINES-1;
			}
			pszPrompt = bOneKeySel ? NULL : pszDefPrompt;
		}
		else
		{
			ucMenuLines = (ucMenuFont==ASCII) ? NUM_MAXLINES : NUM_MAXZHLINES;
			pszPrompt   = NULL;
		}
		if( pszPrompt!=NULL )
		{
			ucStartLineNo = bBottomPrompt ? 0 : 2;
		}
	}
	else
	{
		if( !bBottomPrompt )
		{
			ucStartLineNo = (bOneKeySel || !bAutoSno) ? 2 : 0;
		}
		ucMenuLines = (ucMenuFont==ASCII) ? NUM_MAXLINES-2 : NUM_MAXZHLINES-1;
	}
	ucEndLineNo = (ucMenuFont==ASCII) ? (ucStartLineNo+ucMenuLines-1) : (ucStartLineNo+ucMenuLines*2-1);
	ucPageNum   = (ucShowItemNum+ucMenuLines-1)/ucMenuLines;

	// show title/prompt message
	if( ucStartLineNo>0 )
	{
		PubShowTitle(TRUE, pszPrompt);
	}
	else if( pszPrompt!=NULL )
	{
		PubDispString(pszPrompt, 6|DISP_LINE_LEFT);
	}

	for(ucMenuCnt=0; ucMenuCnt<ucMenuNum; ucMenuCnt++)
	{
		if( pstMenu[ucMenuCnt].bVisible )
		{
			break;
		}
	}
	ucSelNo = ucMenuCnt;
	// Do menu loops
	ucPageNo = ucInputCnt = 0;
	memset(szInput, 0, sizeof(szInput));
	while( 1 )
	{
		// Light/Close up & down icon
		if( bUseSno || bAutoSno )
		{
			ScrSetIcon(ICON_UP,   (uchar)(ucPageNo>0		   ? OPENICON : CLOSEICON));
			ScrSetIcon(ICON_DOWN, (uchar)(ucPageNo<ucPageNum-1 ? OPENICON : CLOSEICON));
		}
		else
		{
			ScrSetIcon(ICON_UP,   (uchar)(ucSelNo>0		      ? OPENICON : CLOSEICON));
			ScrSetIcon(ICON_DOWN, (uchar)(ucSelNo<ucMenuNum-1 ? OPENICON : CLOSEICON));
		}

		if( ucStartLineNo==0 && pszPrompt!=NULL )
		{
			ScrPrint(112, 6, CFONT, "%2s", szInput);
		}

		for(ucActNum=ucMenuCnt=0; ucMenuCnt<ucMenuNum; ucMenuCnt++)
		{
			if( !pstMenu[ucMenuCnt].bVisible )
			{
				continue;
			}
			if( ucActNum>=ucPageNo*ucMenuLines )
			{
				ucCurItem = ucMenuCnt;
				break;
			}
			ucActNum++;
		}
//		ucCurItem = ucPageNo * ucMenuLines;
		// Display menu
		ScrClrLine(ucStartLineNo, ucEndLineNo);
		for(ucMenuCnt=0,ucLineNo=ucStartLineNo; ucMenuCnt<ucMenuLines;)
		{
			if( pstMenu[ucCurItem].bVisible )
			{
				if( bAutoSno )
				{
					ScrPrint(ucCol, ucLineNo, ucMenuFont, "%*d.%.*s",
							(int)(ucKeepChars>0 ? ucKeepChars-1 : 0),
							(int)(ucCurItem+1), ucMaxLen, pstMenu[ucCurItem].szMenuName);
				}
				else
				{
					ucRever = 0;
					if( !bUseSno )
					{
						ucRever = (ucCurItem==ucSelNo) ? REVER: 0;
					}
					ScrPrint(ucCol, ucLineNo, (uchar)(ucMenuFont|ucRever), "%.*s",
							ucMaxLen, pstMenu[ucCurItem].szMenuName);
				}
				ucLineNo += ((ucMenuFont==ASCII) ? 1 : 2);
				ucMenuCnt++;
			}
			ucCurItem++;
			if( ucCurItem>=ucMenuNum )
			{
				break;
			}
		}

		ucKey = PubWaitKey(nTimeOut);		// Waiting for user input
		ScrSetIcon(ICON_UP,   CLOSEICON);
		ScrSetIcon(ICON_DOWN, CLOSEICON);
		if( ucKey==KEYCANCEL || ucKey==NOKEY )
		{
			return -4;
		}
		else if( ucKey==KEYUP )
		{
			if( bUseSno || bAutoSno )
			{
				ucPageNo = (ucPageNo==0) ? (ucPageNum-1) : (ucPageNo-1);
			}
			else
			{
				ucSelNo  = (ucSelNo>0) ? ucSelNo-1 : ucMenuNum-1;
				for(iCnt=(int)ucSelNo; iCnt>=0; iCnt--)
				{
					if( pstMenu[iCnt].bVisible )
					{
						break;
					}
				}
				if( iCnt<0 )
				{
					for(iCnt=(int)(ucMenuNum-1); iCnt>=0; iCnt--)
					{
						if( pstMenu[iCnt].bVisible )
						{
							break;
						}
					}
				}
				ucSelNo = (uchar)iCnt;
				for(ucActNum=ucMenuCnt=0; ucMenuCnt<ucSelNo; ucMenuCnt++)
				{
					if( pstMenu[ucMenuCnt].bVisible )
					{
						ucActNum++;
					}
				}
				ucPageNo = ucActNum/ucMenuLines;
			}
		}
		else if( ucKey==KEYDOWN )
		{
			if( bUseSno || bAutoSno )
			{
				ucPageNo = (ucPageNo+1>=ucPageNum) ? 0 : ucPageNo+1;
			}
			else
			{
				ucSelNo  = (ucSelNo+1<ucMenuNum) ? ucSelNo+1 : 0;
				for(ucMenuCnt=ucSelNo; ucMenuCnt<ucMenuNum; ucMenuCnt++)
				{
					if( pstMenu[ucMenuCnt].bVisible )
					{
						break;
					}
				}
				if( ucMenuCnt>=ucMenuNum )
				{
					for(ucMenuCnt=0; ucMenuCnt<ucMenuNum; ucMenuCnt++)
					{
						if( pstMenu[ucMenuCnt].bVisible )
						{
							break;
						}
					}
				}
				ucSelNo = ucMenuCnt;
				for(ucActNum=ucMenuCnt=0; ucMenuCnt<ucSelNo; ucMenuCnt++)
				{
					if( pstMenu[ucMenuCnt].bVisible )
					{
						ucActNum++;
					}
				}
				ucPageNo = ucActNum/ucMenuLines;
			}
		}
		else if( (bUseSno || bAutoSno) && ucKey>=KEY0 && ucKey<=KEY9 )
		{
			if( bUseSno )
			{
				if( ucInputCnt<2 )
				{
					szInput[ucInputCnt++] = ucKey;
					szInput[ucInputCnt]   = 0;
				}
			}
			else if( bOneKeySel )
			{
				if( ucKey>=KEY1 && ucKey<ucMenuNum+KEY1 )
				{
					ucSelNo = (uchar)(ucKey - KEY1);
					break;
				}
			}
			else if( ucInputCnt<(uchar)(ucKeepChars-1) )
			{
				szInput[ucInputCnt++] = ucKey;
				szInput[ucInputCnt]   = 0;
				ucSelNo = (uchar)atoi((char *)szInput);
				if( ucSelNo>ucMenuNum )
				{
					szInput[--ucInputCnt] = 0;
				}
			}
		}
#ifdef _P80_
		else if( (bUseSno || bAutoSno) && ucKey==KEYCLEAR )
		{
			szInput[0] = 0;
			ucInputCnt = 0;
		}
		else if( (bUseSno || bAutoSno) && ucKey==KEYF5 )
#else
		else if( (bUseSno || bAutoSno) && ucKey==KEYCLEAR )
#endif
		{
			if( ucInputCnt>0 )
			{
				szInput[--ucInputCnt] = 0;
			}
		}
		else if( ucKey==KEYENTER )
		{
			if( !bAutoSno && !bUseSno )
			{
				break;
			}

			if( bOneKeySel || ucInputCnt==0 )
			{
				ucPageNo = (ucPageNo>=ucPageNum-1) ? 0 : ucPageNo+1;
			}
			else if( ucInputCnt>0 )
			{
				ucSelNo = (uchar)atoi((char *)szInput);
				if( bUseSno )
				{
					for(bMenuOk=FALSE,ucCnt=0; ucCnt<ucMenuNum; ucCnt++)
					{
						pDotSign = (uchar *)strchr((char *)pstMenu[ucCnt].szMenuName, '.');
						PubASSERT( pDotSign!=NULL );
						sprintf((char *)szMenuNo, "%.*s",
								(int)MIN(2, pDotSign-pstMenu[ucCnt].szMenuName),
								pstMenu[ucCnt].szMenuName);
						if( atoi((char *)szMenuNo)==(int)ucSelNo )
						{
							ucSelNo = ucCnt;
							bMenuOk = TRUE;
							break;
						}
					}
					if( bMenuOk )
					{
						break;
					}
					szInput[0] = 0;
					ucInputCnt = 0;
				}
				else
				{
					if( ucSelNo>0 && ucSelNo<=ucMenuNum )
					{
						ucSelNo--;
						break;
					}
				}
			}
		}
	}	// end of while( 1

	if( pstMenu[ucSelNo].pfMenuFunc!=NULL )
	{
		(*pstMenu[ucSelNo].pfMenuFunc)();
	}

	return (int)ucSelNo;
}


uchar PubYesNo(ushort nWaitTime)
{
	uchar   ucTmp;

	while (1)
	{
		ucTmp = PubWaitKey(nWaitTime);
		if (ucTmp==KEYENTER)
		{
			return  TRUE;
		}
		if (ucTmp==KEYCANCEL || ucTmp==NOKEY)
		{
			return  FALSE;
		}
	}
}


uchar PubGetStrParam(uchar ucType,	uchar ucPromptLine,	uchar *pszPrompt,
					uchar ucMinLen,	uchar ucMaxLen,	uchar *pszData)
{
	uchar   ucRet, szBuff[256];

	if (pszPrompt!=NULL && *pszPrompt!=0)
	{
		ScrClrLine(2, NUM_MAXLINENO);
		ScrPrint(0, ucPromptLine, CFONT, "%.*s", NUM_MAXZHCHARS, (char *)pszPrompt);
	}

	ScrClrLine(NUM_MAXLINENO-1, NUM_MAXLINENO);
	ucType |= (GS_ECHOCONT|GS_CFONT|GS_NORMAL);
	sprintf((char *)szBuff, "%.*s", ucMaxLen, pszData);
	while (1)
	{
		ucRet = GetString(szBuff, ucType, ucMinLen, ucMaxLen);
		if (ucRet==0)
		{
			sprintf((char *)pszData, "%.*s", szBuff[0], &szBuff[1]);
			break;
		}
		else if (ucRet==0x0D)
		{
			if (ucMinLen==0)
			{
				*pszData = 0;
				break;
			}
		}
		else if (ucRet)
		{
			return 0xFF;
		}
	}

	return 0;
}


//GA_SEPARATOR:	0x80
uchar PubGetAmount(uchar *pszPrefix, uchar ucDeciPos,
				  uchar ucMinLen, uchar ucMaxLen, uchar *pszData,
				  uchar ucTimeout, uchar ucMisc)
{
	uchar	*pszNowChar, bSeparator, bNegative, ucKeyVal;
	uchar	szBuff[20], szDispBuf[20], szDispBuf2[30];//, szTrunBuf[10];	//buffer to display
	int		iTotalLen;

	if (ucDeciPos>5 ||
		ucMinLen>ucMaxLen || ucMaxLen>12 ||
		pszData==NULL)
	{
		return 0xFE;		//parameter illegal
	}

	memset(szBuff, 0, sizeof(szBuff));
	memset(szDispBuf, 0, sizeof(szDispBuf));
	memset(szDispBuf2, 0, sizeof(szDispBuf2));

	bSeparator = ucMisc & GA_SEPARATOR;		//whether use ',' to separate huge amount
	bNegative  = ucMisc & GA_NEGATIVE;		//whether display negative amount
	pszNowChar = szBuff;

	while (1)
	{
		/* 显示 */
		memset(szDispBuf2, 0, sizeof(szDispBuf2));
		if (pszPrefix!=NULL && strlen((char *)pszPrefix)>0)
		{
			sprintf((char *)szDispBuf2, "%.4s", (char *)pszPrefix);
		}

		//protocol
		//PubConvAmount(uchar *pszPrefix, uchar *pszIn, uchar *pszOut,
		//				uchar ucDeciPos, uchar ucMisc)
		PubConvAmount(NULL, szBuff, ucDeciPos, szDispBuf, ucMisc);

		ScrClrLine(6, 7);
		iTotalLen = strlen((char *)szDispBuf) + strlen((char *)szDispBuf2);
		if (iTotalLen < 17)
		{
			strcat((char *)szDispBuf2, (char *)szDispBuf);
			ScrPrint(0, 6, CFONT, "%16.16s", szDispBuf2);
		}
		else
		{
			ScrPrint(0, 6, CFONT, "%s..%s", szDispBuf2, &szDispBuf[iTotalLen-16+2]);
		}

		/* 处理按键 */
		ucKeyVal = PubWaitKey(ucTimeout);
		switch(ucKeyVal)
		{
			case KEY0:
				if (pszNowChar==szBuff || pszNowChar>=szBuff+ucMaxLen)
				{
					PubBeepErr();
				}
				else
				{
					*pszNowChar++ = ucKeyVal;
					*pszNowChar   = 0;
				}
				break;
			case KEY1:
			case KEY2:
			case KEY3:
			case KEY4:
			case KEY5:
			case KEY6:
			case KEY7:
			case KEY8:
			case KEY9:
				if (pszNowChar>=szBuff+ucMaxLen)
				{
					PubBeepErr();
				}
				else
				{
					*pszNowChar++ = ucKeyVal;
					*pszNowChar   = 0;
				}
				break;
#ifdef _P80_
			case KEYCLEAR:
				pszNowChar  = szBuff;
				*pszNowChar = 0;
				break;

			case KEYF5:	// backspace
#else
			case KEYCLEAR:
#endif
				if (pszNowChar<=szBuff)
				{
					PubBeepErr();
				}
				else
				{
					*(--pszNowChar) = 0;
				}
				break;

			case KEYENTER:
				if (pszNowChar>=szBuff+ucMinLen)
				{
					*pszData = 0;
					strcpy((char *)pszData, (char *)szBuff);
					if (strlen((char *)szBuff)==0)
					{
						strcpy((char *)pszData, "0");
					}
					return 0;
				}
				else
				{
					PubBeepErr();
				}
				break;
			case KEYCANCEL:
			case NOKEY:
				return 0xFF;
			default:
				PubBeepErr();
				break;
		}
	}
}


uchar PubGetBoolParam(uchar ucLine, uchar *pszPrompt, uchar *pucResult)
{
	uchar	ucKey, ucFlag;

	if (pszPrompt!=NULL && *pszPrompt!=0)
	{
		ScrClrLine(2, NUM_MAXLINENO);
		ScrPrint(0, ucLine, CFONT, "%.*s", NUM_MAXZHCHARS, (char *)pszPrompt);
	}

	ucFlag = *pucResult;
	while(1)
	{
		PubShowMsg(NUM_MAXLINENO-1, (uchar *)(ucFlag ? "[YES]" : "[NO]"));
		ucKey = PubWaitKey(60);
		if (ucKey==KEYENTER)
		{
			*pucResult = ucFlag;
			return 0;
		}
		if (ucKey==KEYCANCEL || ucKey==NOKEY)
		{
			return 0xFF;
		}
		if ((ucKey==KEYCLEAR)
			|| (ucKey==KEYALPHA)
			|| (ucKey>=KEY0 && ucKey<=KEY9))
		{
			ucFlag = !ucFlag;
		}
	}   // end of while(1)
}

void MapChar2Index(uchar ch, int *piRow, int *piCol)
{
	int		i;
	char	*p;

	for(i=0; i<sizeof(pszAlphaTbl)/sizeof(pszAlphaTbl[0]); i++)
	{
		for(p=pszAlphaTbl[i]; *p; p++)
		{
			if( *p==toupper(ch) )
			{
				*piRow = i;
				*piCol = p-pszAlphaTbl[i];
				break;
			}
		}
	}
}

void ShowWorkBuf(uchar ucMode, uchar *pszWorkBuff, int iLen)
{
	int		iLineNum, iCnt, iLeftBytes;
	uchar	ucLineNo, sTemp[NUM_MAXZHCHARS];

	iLineNum = (iLen+NUM_MAXZHCHARS-1)/NUM_MAXZHCHARS;
	if( iLineNum>=NUM_MAXZHLINES )
	{
		ScrCls();
	}
	else
	{
		sgMinLineNo = MIN(sgMinLineNo, (NUM_MAXLINENO-iLineNum*2+1));
		ScrClrLine(sgMinLineNo, NUM_MAXLINENO);
	}

	memset(sTemp, '*', sizeof(sTemp));
	ucLineNo = NUM_MAXLINENO-1;
	for(iCnt=1; iCnt<=3; iCnt++)
	{
		if( (iCnt*NUM_MAXZHCHARS)<=iLen )
		{
			iLeftBytes = NUM_MAXZHCHARS;
			if( ucMode & PASS_IN )
			{
				ScrPrint(0, ucLineNo, CFONT, "%.*s", iLeftBytes, sTemp);
			}
			else
			{
				ScrPrint(0, ucLineNo, CFONT, "%.*s", iLeftBytes, pszWorkBuff + iLen - iCnt*NUM_MAXZHCHARS);
			}
		}
		else
		{
			iLeftBytes = iLen - (iCnt-1)*NUM_MAXZHCHARS;
			if( iLeftBytes<=0 && iCnt>1 )
			{
				break;
			}
			if( ucMode & PASS_IN )
			{
				ScrPrint(0, ucLineNo, CFONT, "%*.*s", NUM_MAXZHCHARS, iLeftBytes, sTemp);
			}
			else
			{
				ScrPrint(0, ucLineNo, CFONT, "%*.*s", NUM_MAXZHCHARS, iLeftBytes, pszWorkBuff);
			}
			break;
		}
		ucLineNo -= 2;
	}
}

// 输入字母/数字
uchar PubGetString(uchar ucMode, uchar ucMinlen, uchar ucMaxlen, uchar *pszOut, uchar ucTimeOut)
{
	uchar	ucKey, bClearInData, szWorkBuff[LEN_GETSTRING+1];
	int		iLen, iIndex, iLastKey;

	PubASSERT( (ucMode & NUM_IN) || (ucMode & PASS_IN) || (ucMode & ALPHA_IN) );
	PubASSERT( !((ucMode & CARRY_IN) && (ucMode & ECHO_IN)) );
	if( (ucMode & CARRY_IN) || (ucMode & ECHO_IN) )
	{
		sprintf((char *)szWorkBuff, "%.*s", LEN_GETSTRING, pszOut);
	}
	else
	{
		memset(szWorkBuff, 0, sizeof(szWorkBuff));
	}

	iLen = strlen((char *)szWorkBuff);
	if( iLen>0 )
	{
		MapChar2Index(szWorkBuff[iLen-1], &iLastKey, &iIndex);
	}
	else
	{
		iLastKey = -1;
		iIndex   = 0;
	}

	sgMinLineNo  = 0xFF;
	bClearInData = (ucMode & ECHO_IN);
	while( 1 )
	{
		ShowWorkBuf(ucMode, szWorkBuff, iLen);
		ucKey = PubWaitKey(ucTimeOut);
		if( ucKey==KEYCANCEL ||ucKey==NOKEY )
		{
			return 1;
		}
		else if( ucKey==KEYENTER )
		{
			bClearInData = FALSE;
			if( iLen<(int)ucMinlen )
			{
				continue;
			}
			sprintf((char *)pszOut, "%s", szWorkBuff);
			break;
		}
		else if( ucKey>=KEY0 && ucKey<=KEY9 )
		{
			if( bClearInData )
			{	// clear in buffer
				szWorkBuff[0] = 0;
				iLen          = 0;
				iLastKey      = -1;
				bClearInData  = FALSE;
			}
			// save key in data
			if( iLen<(int)ucMaxlen )
			{
				szWorkBuff[iLen++] = ucKey;
				szWorkBuff[iLen]   = 0;
				iLastKey = ucKey - KEY0;
				iIndex   = 0;
			}
		}
#ifdef _P80_
		else if( ucKey==KEYCLEAR )
		{
			szWorkBuff[0] = 0;
			iLen          = 0;
			iLastKey      = -1;
		}
		else if( ucKey==KEYF5 )	// backspace
#else
		else if( ucKey==KEYCLEAR )
#endif
		{
			bClearInData = FALSE;
			if( iLen<=0 )
			{
				continue;
			}
			szWorkBuff[--iLen] = 0;
			if( iLen>0 )
			{
				MapChar2Index(szWorkBuff[iLen-1], &iLastKey, &iIndex);
			}
			else
			{
				iLastKey = -1;
				iIndex   = 0;
			}
		}
		else if( ucKey==KEYALPHA || ucKey == KEYFN)
		{
			bClearInData = FALSE;
			if( !(ucMode & ALPHA_IN) || iLastKey<0 || iLen<1 )
			{
				continue;
			}
			iIndex = (iIndex+1)%strlen(pszAlphaTbl[iLastKey]);
			szWorkBuff[iLen-1] = pszAlphaTbl[iLastKey][iIndex];
		}
	}

	return 0;
}

////////////////////////////////////// 显示输出 //////////////////////////////////////


void PubShowTitle(uchar bClrScreen, uchar *pszTitle)
{
	int	 iLen;

	PubASSERT(pszTitle!=NULL);
	if (pszTitle==NULL)
	{
		return;
	}

	if (bClrScreen)
	{
		ScrCls();
	}

	iLen = strlen((char *)pszTitle);
	if (iLen > NUM_MAXZHCHARS)
	{
		iLen = NUM_MAXZHCHARS;
	}

	ScrPrint(0, 0, CFONT|REVER, "%*s", NUM_MAXZHCHARS, "");
	ScrPrint((uchar)((NUM_MAXCOLS-8*iLen)/2), 0, CFONT|REVER,
		"%.*s", iLen, (char *)pszTitle);
}


void PubShowMsg(uchar ucLine, uchar *pszMsg)
{
	int		iLen;
	uchar	ucMode;

	PubASSERT(pszMsg!=NULL);
	if (pszMsg==NULL)
	{
		return;
	}

	ucMode =  (ucLine & REVER) | CFONT;
	ucLine &= ~REVER;
	ScrClrLine(ucLine, (uchar)(ucLine+1));

	iLen = strlen((char *)pszMsg);
	if (iLen > NUM_MAXZHCHARS)
	{
		iLen = NUM_MAXZHCHARS;
	}

	ScrPrint((uchar)((NUM_MAXCOLS-8*iLen)/2), ucLine, ucMode,
			"%.*s", iLen, (char *)pszMsg);
}


void PubShowTwoMsg(uchar ucLine, uchar *pszMsg1, uchar *pszMsg2)
{
	if (ucLine>4)
	{
		ucLine = 4;
	}
	PubShowMsg(ucLine, pszMsg1);
	PubShowMsg((uchar)(ucLine+2), pszMsg2);
}

//api.c--apiDispString
//...
void PubDispString(void *pszStr, uchar ucPosition)
{
	uchar	mode, x, y, ucLen, buffer[60];

	mode  = ucPosition & 0xf0;
	y     = ucPosition & 0x0f;
	ucLen = strlen(pszStr);
	if (ucLen > NUM_MAXZHCHARS)
	{
		ucLen = NUM_MAXZHCHARS;
	}

	switch(mode)
	{
		case DISP_SCR_CENTER:
			ScrCls();
			x = (64 - ucLen * 4);
			y = 3;
			break;

		case DISP_LINE_CENTER:
			ScrClrLine(y, (uchar) (y + 1));
			x = (64 - ucLen * 4);
			break;

		case DISP_LINE_RIGHT:
			x = (128 - ucLen * 8);
			break;

		case DISP_LINE_LEFT:
		default:
			x = 0;
			break;
	}

	memset(buffer, 0, sizeof(buffer));
	memcpy(buffer, pszStr, ucLen);
	ScrClrLine(y, (uchar)(y+1));
	ScrPrint(x, y, CFONT, "%s", (char *)buffer);
}


//...
void PubHalt(uchar *pszfile, uint uiLine)
{
	int iErr;

	ScrCls();
	iErr = GetLastError();

	ScrPrint(0, 0, ASCII, "File:%s,\nLine:%d,\nRet:%d", (char *)pszfile, uiLine, iErr);
	ScrPrint(0, 4, CFONT, "PLS RECORD");
	ScrPrint(0, 4, CFONT, "THEN RESTART POS");

	while (1)
	{
	};
}

////////////////////////////////////// 声音 //////////////////////////////////////

void PubLongBeep(void)
{
	Beef(6, 800);
}


void PubBeepOk(void)
{
	uchar	i;

	for (i=0; i<3; i++)
	{
		Beef(6, 60);
		DelayMs(80);
	}
}


void PubBeepErr(void)
{
	Beef(1, 200);
	DelayMs(200);
}

////////////////////////////////////// 文件 //////////////////////////////////////


int PubFReadN(int fd, void *psData, int iNeedBytes)
{
	int		iReadBytes, iLeftBytes;
	uchar	*psTmp;

	PubASSERT( fd>=0 && psData!=NULL && iNeedBytes>=0 );
	psTmp      = (uchar *)psData;
	iLeftBytes = iNeedBytes;
	while(iLeftBytes>0)
	{
		iReadBytes = read(fd, psTmp, iLeftBytes);
		if (iReadBytes<=0)
		{
			PubTRACE1("errno:%d", errno);
			break;
		}
		iLeftBytes -= iReadBytes;
		psTmp	   += iReadBytes;
	}

	return (iNeedBytes-iLeftBytes);
}


uint PubFileRead(uchar *pszFileName, long lOffset, void *psData, int iLen)
{
	int		iRet, fd, iReadBytes;

	PubASSERT( pszFileName!=NULL&& *pszFileName!=0 && iLen>0 );
	fd = open((char *)pszFileName, O_RDWR);
	if (fd<0)
	{
		PubTRACE1("open:%s", pszFileName);
		PubTRACE1("errno:%d", errno);
		return 1;
	}

	iRet = seek(fd, lOffset, SEEK_SET);
	if (iRet<0)
	{
		PubTRACE2("seek:%s,%ld", pszFileName, lOffset);
		PubTRACE1("errno:%d", errno);
		return 2;
	}

	iReadBytes = PubFReadN(fd, psData, iLen);
	close(fd);
	if (iReadBytes!=iLen)
	{
		return 3;
	}

	return 0;
}


int PubFWriteN(int fd, void *psData, int iNeedBytes)
{
	int		iWriteBytes, iLeftBytes;
	uchar   *psTmp;

	PubASSERT( fd>=0 && psData!=NULL && iNeedBytes>=0 );
	psTmp      = (uchar *)psData;
	iLeftBytes = iNeedBytes;

	while(iLeftBytes>0)
	{
		iWriteBytes = write(fd, psTmp, iLeftBytes);
		if (iWriteBytes<=0)
		{
			PubTRACE2("FWN:%d,%d", iWriteBytes, iLeftBytes);
			PubTRACE1("errno:%d", errno);
			break;
		}
		iLeftBytes -= iWriteBytes;
		psTmp	   += iWriteBytes;
	}

	return (iNeedBytes-iLeftBytes);
}


uint PubFileWrite(uchar *pszFileName, long lOffset, void *psData, int iLen)
{
	int	 iRet, fd, iWriteBytes;

	PubASSERT( pszFileName!=NULL && *pszFileName!=0 && iLen>=0 );
	fd = open((char *)pszFileName, O_RDWR|O_CREATE);
	if (fd<0)
	{
		PubTRACE1("open:%s", pszFileName);
		PubTRACE1("errno:%d", errno);
		return 1;
	}

	iRet = seek(fd, lOffset, SEEK_SET);
	if (iRet<0)
	{
		PubTRACE2("seek:%s,%ld", pszFileName, lOffset);
		PubTRACE1("errno:%d", errno);
		return 2;
	}

	iWriteBytes = PubFWriteN(fd, psData, iLen);
	close(fd);
	if (iWriteBytes!=iLen)
	{
		PubTRACE2("FW:%d,%d", iWriteBytes, iLen);
		return 3;
	}

	return 0;
}

////////////////////////////////////// 计算 //////////////////////////////////////


void PubDes(uchar ucMode, uchar *psData, uchar *psKey, uchar *psResult)
{
	uchar   sTmp[8];

	PubASSERT(ucMode==ONE_ENCRYPT  || ucMode==ONE_DECRYPT ||
			ucMode==TRI_ENCRYPT  || ucMode==TRI_DECRYPT ||
			ucMode==TRI_ENCRYPT3 || ucMode==TRI_DECRYPT3);

	switch(ucMode)
	{
		case ONE_ENCRYPT:
			des(psData, psResult, psKey, ENCRYPT);
			break;

		case ONE_DECRYPT:
			des(psData, psResult, psKey, DECRYPT);
			break;

		case TRI_ENCRYPT:
			des(psData,   psResult, psKey,   ENCRYPT);
			des(psResult, sTmp,     psKey+8, DECRYPT);
			des(sTmp,     psResult, psKey,   ENCRYPT);
			break;

		case TRI_DECRYPT:
			des(psData,   psResult, psKey,   DECRYPT);
			des(psResult, sTmp,     psKey+8, ENCRYPT);
			des(sTmp,     psResult, psKey,   DECRYPT);
			break;

		case TRI_ENCRYPT3:
			des(psData,   psResult, psKey,    ENCRYPT);
			des(psResult, sTmp,     psKey+8,  DECRYPT);
			des(sTmp,     psResult, psKey+16, ENCRYPT);
			break;

		case TRI_DECRYPT3:
			des(psData,   psResult, psKey+16, DECRYPT);
			des(psResult, sTmp,     psKey+8,  ENCRYPT);
			des(sTmp,     psResult, psKey,    DECRYPT);
			break;
	}
}


void PubCalcMac(uchar ucMode, uchar *psKey, uchar *psMsg, uint uiLen, uchar *psMac)
{
	uchar   sOutMac[8];
	uint	uiOffset, i;

	PubASSERT(ucMode==MAC_FAST || ucMode==MAC_ANSIX99);
	memset(sOutMac, 0, sizeof(sOutMac));
	uiOffset = 0;

	while(uiLen>uiOffset)
	{
		if (uiLen-uiOffset<=8)
		{
			for (i=0; i<uiLen-uiOffset; i++)
			{
				sOutMac[i] ^= psMsg[uiOffset+i];
			}
			break;
		}
		for (i=0; i<8; i++)
		{
			sOutMac[i] ^= psMsg[uiOffset+i];
		}
		if (ucMode==MAC_ANSIX99)
		{
			PubDes(ONE_ENCRYPT, sOutMac, psKey, sOutMac);
		}
		uiOffset += 8;
	}

	PubDes(ONE_ENCRYPT, sOutMac, psKey, psMac);
}

// 计算LRC
uchar PubCalcLRC(uchar *psData, ushort uiLength, uchar ucInit)
{
	while( uiLength>0 )
	{
		ucInit ^= *psData++;
		uiLength--;
	}
	
	return ucInit;
}




/*********************************************************************************************/
/*********************************************************************************************/



/***************************************************************************************
    For Debug use
***************************************************************************************/


#ifdef _POS_DEBUG
void DispHexMsg(uchar *pszTitle, uchar *psMsg, uint uiMsgLen, short nTimeOut)
{
	uint	i, iLineNum, iPageNo, iPageNum, iDispLine;
	uchar	*pszBuff;
	uchar   ucKey, ucStartLine;

	// Format message
	uiMsgLen = MIN(uiMsgLen, LEN_DBGDATA);
	glszDbgInfo[0] = 0;
	for (pszBuff=glszDbgInfo,i=0; i<uiMsgLen; i+=MAX_CHARS)
	{
		if (uiMsgLen-i<MAX_CHARS)
		{
			pszBuff += DispHexLine(pszBuff, i, psMsg+i, uiMsgLen-i);
		}
		else
		{
			pszBuff += DispHexLine(pszBuff, i, psMsg+i, MAX_CHARS);
		}
	}   // end of for (pszBuff=

	// Display message
	if (pszTitle!=NULL && *pszTitle!=0)
	{
		PubShowTitle(TRUE, pszTitle);
		iDispLine   = NUM_MAXLINES-2;
		ucStartLine = 2;
	}
	else
	{
		iDispLine   = NUM_MAXLINES;
		ucStartLine = 0;
	}

	iLineNum = (strlen((char *)glszDbgInfo)+CHARS_PERLINE-1)/CHARS_PERLINE;
	iPageNum = (iLineNum+iDispLine-1)/iDispLine;
	iPageNo  = 0;

	while(1)
	{
		// Light/Close up & down icon
		ScrSetIcon(ICON_UP,   (uchar)(iPageNo>0		  ? OPENICON : CLOSEICON));
		ScrSetIcon(ICON_DOWN, (uchar)(iPageNo<iPageNum-1 ? OPENICON : CLOSEICON));

		ScrClrLine(ucStartLine, NUM_MAXLINENO);
		ScrPrint(0, ucStartLine, ASCII, "%.*s", CHARS_PERLINE*iDispLine,
				 &glszDbgInfo[CHARS_PERLINE*iDispLine*iPageNo]);

		ucKey = PubWaitKey(nTimeOut);
		ScrSetIcon(ICON_UP,   CLOSEICON);
		ScrSetIcon(ICON_DOWN, CLOSEICON);

		if (ucKey==KEYUP)
		{
			iPageNo = (iPageNo==0) ? iPageNum-1 : iPageNo-1;
		}
		else if (ucKey==KEYDOWN || ucKey==KEYENTER)
		{
			iPageNo = (iPageNo>=iPageNum-1) ? 0 : iPageNo+1;
		}
		else if (ucKey==KEYCANCEL || ucKey==NOKEY)
		{
			return;
		}
	}   // end of while(1
}

// print a line as hexadecimal format
int DispHexLine(uchar *pszBuff, uint uiOffset, uchar *psMsg, uint uiMsgLen)
{
	uint	i;
	uchar	*p = pszBuff;

	// Print line information
	pszBuff += sprintf((char *)pszBuff, "%04Xh:", uiOffset);

	for (i=0; i<uiMsgLen; i++)
	{
		pszBuff += sprintf((char *)pszBuff, " %02X", psMsg[i]);
	}
	for (; i<MAX_CHARS; i++)
	{   // append blank spaces, if needed
		pszBuff += sprintf((char *)pszBuff, "   ");
	}

	return (pszBuff-p);
}

// Debug 使用, 功能：显示文件名与行数
void DispAssert(uchar *pszFileName, ulong ulLineNo)
{
	uchar	szFName[30];

	PubShowTitle(TRUE, (uchar *)"Assert Failure");
	GetBaseName(pszFileName, szFName);
	ScrPrint(0, 2, CFONT, "FILE:%.11s", szFName);
	ScrPrint(0, 4, CFONT, "LINE:%ld", ulLineNo);
	PubShowMsg(6, (uchar *)"PRESS ENTER EXIT");
	PubLongBeep();

	while(!PubYesNo(60));
}

// get basename of a full path name
void GetBaseName(uchar *pszFullPath, uchar *pszBaseName)
{
	uchar	*pszTmp;

	*pszBaseName = 0;
	if (!pszFullPath || !*pszFullPath)
	{
		return;
	}

	pszTmp = &pszFullPath[strlen((char *)pszFullPath)-1];
	while( pszTmp>=pszFullPath && *pszTmp!='\\' && *pszTmp!='/' )
	{
		pszTmp--;
	}
	sprintf((char *)pszBaseName, "%s", (char *)(pszTmp+1));
}

#else /* _POS_DEBUG */

#endif /* _POS_DEBUG */

