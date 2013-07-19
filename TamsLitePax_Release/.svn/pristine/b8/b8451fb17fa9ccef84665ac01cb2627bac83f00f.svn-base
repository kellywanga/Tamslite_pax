
/****************************************************************************
NAME
	applib.h - 实现公共函数

DESCRIPTION
	1. 本模块内部占用定时器4#(进入模块后申请,退出模块后释放)

REFERENCE

MODIFICATION SHEET:
	MODIFIED   (YYYY.MM.DD)
	hejj		2006.09.05		- created
	shengjx		2006.09.14		- add precompile process
****************************************************************************/

#ifndef _APPLIB_H
#define _APPLIB_H

#ifndef uchar
	typedef unsigned char	uchar;
#endif

#ifndef ushort
	typedef unsigned short	ushort;
#endif

#ifndef uint
	typedef unsigned int	uint;
#endif

#ifndef ulong
	typedef unsigned long	ulong;
#endif

// #define _POS_DEBUG

#ifndef TRUE
	#define TRUE	1
	#define FALSE	0
#endif

#define TIMER_TEMPORARY			4       // 短时间的定时器(其他模块可以共用)

// macros for vDes()
#define ONE_DECRYPT     0           // 单DES解密
#define ONE_ENCRYPT     1           // 单DES加密
#define TRI_DECRYPT     2           // 三DES解密(16 bytes key)
#define TRI_ENCRYPT     3           // 三DES加密(16 bytes key)
#define TRI_DECRYPT3    4           // 三DES解密(24 bytes key)
#define TRI_ENCRYPT3    5           // 三DES加密(24 bytes key)

// macros for vCalcMac()
#define MAC_ANSIX99     0           // ANSI9.9 standard MAC algorithm
#define MAC_FAST        1           // HyperCom fast MAC algorithm

#ifndef REVER
#define REVER           0x80		// Reverse background when display
#endif

// macros for ScrSetIcon()
#define ICON_PHONE      1       // phone
#define ICON_SIGNAL     2       // wireless signal
#define ICON_PRINTER    3       // printer
#define ICON_ICCARD     4       // smart card IC
#define ICON_LOCK       5       // lock
#define ICON_SPEAKER    6       // speaker
#define ICON_UP         7       // up
#define ICON_DOWN       8       // down

#define CLOSEICON		0       // 关闭图标[针对所有图标]
#define OPENICON        1       // 显示图标[针对打印机/IC卡/锁/扬声器/上/下]
// 若需点亮摘机、挂机图标，则action的值设置如下
#define OFFHOOKICON     1       // hook off 摘机
#define ONHOOKICON      2       // hook on  挂机
// 若需点亮信号图标，则action的值设置为：INITSIGNAL+[0，5]
#define INITSIGNAL      1       // 初始信号强度[最强信号为INITSIGNAL+5]

// macros for PubGetString()
#define NUM_IN			0x01	// 数字输入
#define ALPHA_IN		0x02	// 字母数字混合输入
#define PASS_IN			0x04	// 秘密输入
#define CARRY_IN		0x08	// 有传入数据
#define ECHO_IN			0x10	// 回显传入数据,按键则清除原数据
#define AUTO_FONT		0x80	// RFU

// macros for PubGetStrParam()
#define GS_ECHOCONT         0x80        // 回车退出(回显初始值?)
#define GS_CFONT            0x40        // 大字体
#define GS_NUMBER           0x20        // 输数字
#define GS_ALPHA            0x10        // 输字符
#define GS_PASSWD           0x08        // 密码方式
#define GS_LALIGN           0x04        // 左对齐输入
#define GS_FLOAT            0x02        // 小数点
#define GS_NORMAL           0x01        // 正常显示

// macros for screen functions
#define NUM_MAXZHCHARS      16          // 每行最多字符数目(大字体)
#define NUM_MAXCOLS         128         // 每行最多列数
#define NUM_MAXCOLNO        (NUM_MAXCOLS-1)     // 最大列号
#define NUM_MAXLINES        8           // 最多行数
#define NUM_MAXLINENO       (NUM_MAXLINES-1)    // 最大行号
#define NUM_MAXZHLINES      4           // 最大中文行数
#define NUM_MAXCHARS		21

#define DISP_LINE_LEFT		0x80
#define DISP_LINE_RIGHT		0x40
#define DISP_LINE_CENTER	0x20
#define DISP_SCR_CENTER		0x10

// menu display mode
#define MENU_NOSPEC		0x00	// 自动显示
#define MENU_USESNO		0x01	// 使用菜单自己的编号(优先于MENU_AUTOSNO)
#define MENU_AUTOSNO	0x02	// 自动编号
#define MENU_CENTER		0x10	// 中间对齐显示
#define MENU_PROMPT		0x20	// 强制提示信息在底部
#define MENU_ASCII		0x40	// 强制显示小字体
#define MENU_CFONT		0x80	// 强制显示大字体(优先于MENU_ASCII)

// types for PubGetMenu()
typedef void (*MenuFunc)(void);
typedef struct _tagMenuItem{
	uchar		bVisible;						// TRUE: show it, FALSE: hide
    uchar       szMenuName[NUM_MAXCHARS+1];		// menu name
    MenuFunc    pfMenuFunc;						// callback function(if set)
}MenuItem;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/****************************************************************************
 Function:      Calculate the offset of the struct member
 Param In:
    type        struct type
    member      struct member name
 Param Out:
    none
 Return Code:   the offset of the struct member
****************************************************************************/
/****************************************************************************
 功    能:      计算结构成员的偏移
 输入参数:
    type        结构体类型
    member      结构体成员名称
 输出参数:
    none
 返 回 值:      该结构体成员的偏移量
****************************************************************************/
#define OFFSET(type, member)    ( (ulong)(&((type *)0)->member) )

/****************************************************************************
 Function:      Calculate the maximum(minimum) of two int number/expression
 Param In:
    a,b         number/expression need to be compare
 Param Out:
    none
 Return Code:   maximum(minimum)
****************************************************************************/
/****************************************************************************
 功    能:      计算两个整数数字/表达式最大(小)值
 输入参数:
    a,b         待比较的整数/表达式
 输出参数:
    none
 返 回 值:      最大(小)值
****************************************************************************/
#define MAX(a, b)       ( (a)>=(b) ? (a) : (b) )
#define MIN(a, b)       ( (a)<=(b) ? (a) : (b) )

/****************************************************************************
 Function:      Calculate the absolute value of int number/expression
 Param In:
    a           int number/expression
 Param Out:
    none
 Return Code:   absolute value
****************************************************************************/
/****************************************************************************
 功    能:      计算整数数字/表达式绝对值
 输入参数:
    a           整数数字/表达式
 输出参数:
    none
 返 回 值:      绝对值
****************************************************************************/
#define ABS(a)      ( (a)>=0 ? (a) : (-1)*(a) )

/****************************************************************************
 Function:      Source string XOR object string, then store the result in
 	              object string.(The two string are same length)
 Param In:
    psVect1     Object string
    psVect2     source string
    nLength     string length
 Param Out:
    psVect1     Object string after XOR
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:      将源串与目标串异或后存入目标串(两个串长度相等)
 输入参数:
    psVect1     目标串
    psVect2     源串
    nLength     串长度
 输出参数:
    psVect1     异或后的目标串
 返 回 值:      无
****************************************************************************/
void PubXor(uchar *psVect1, uchar *psVect2, uint uiLength);

/****************************************************************************
 Function:      Convert BIN string to readable HEX string, which have double
 	              length of BIN string. 0x12AB-->"12AB"
 Param In:
    psIn        source string
    nLength     the length of Object string
 Param Out:
    psOut       Object string
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:      将二进制源串分解成双倍长度可读的16进制串,0x12AB-->"12AB"
 输入参数:
    psIn        源串
    nLength     源串长度
 输出参数:
    psOut       目标串
 返 回 值:      无
****************************************************************************/
void PubBcd2Asc(uchar *psIn, uint uiLength, uchar *psOut);

/****************************************************************************
 Function:      Similar with function PubOne2Two(),add '\0' at the end of
 	              object string
 Param In:
    psIn        source string
    nLength     the length of source string
 Param Out:
    psOut       object string
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:      同vOneTwo()函数,并在目标串后添一 '\0'
 输入参数:
    psIn        源串
    nLength     源串长度
 输出参数:
    psOut       目标串
 返 回 值:      无
****************************************************************************/
void PubBcd2Asc0(uchar *psIn, uint uiLength, uchar *pszOut);

/****************************************************************************
 Function:      Convert readable HEX string to BIN string, which only half
 	              length of HEX string. "12AB"-->0x12AB
 Param In:
    psIn        source string
    nLength     the length of source string
 Param Out:
    psOut       object string
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:      将可读的16进制串合并成其一半长度的二进制串,"12AB"-->0x12AB
 输入参数:
    psIn        源串
    nLength     源串长度
 输出参数:
    psOut       目标串
 返 回 值:      无
****************************************************************************/
void PubAsc2Bcd(uchar *psIn, uint uiLength, uchar *psOut);

/****************************************************************************
 Function:      Convert number string to long integer, similar to atol(). This
 	              function don't request end with '\0'.
 Param In:
    sString     number string,must be legal number, don't request end with '\0'
    nLength     the length of number string
 Param Out:
    pulOut:     converted long integer
 Return Code:   converted long integer
****************************************************************************/
/****************************************************************************
 功    能:      将数字串转变成长整数,相当于atol(),但是该函数不要求零字符结束
 输入参数:
    sString     数字串,必须为合法的数字,不需要'\0'结尾
    nLength     数字串长度
 输出参数:
    pulOut:     转变后的长整数
 返 回 值:      转变后的长整数
****************************************************************************/
ulong PubAsc2Long(uchar *psString, uint uiLength, ulong *pulOut);

/****************************************************************************
 Function:      Convert integer to string which high bit at the front. (Store
 	              it according to network byte format)
 Param In:
    ulSource    the integer need to be converted
    uiTCnt      the size of object buffer(byte number)
 Param Out:
    psTarget    converted string
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:      整型数转换成高位在前的字符串表示(按照网络字节格式存放)
 输入参数:
    ulSource    待转换的整数
    uiTCnt      目标缓冲区的大小(字节数)
 输出参数:
    psTarget    转换后的串
 返 回 值:      无
****************************************************************************/
void PubLong2Char(ulong ulSource, uint uiTCnt, uchar *psTarget);

/****************************************************************************
 Function:      Convert the character string with high bit in the front to
 	              integer
Param In:
    psSource    the string need to be converted
    uiSCnt      the length of source string
 Param Out:
    *pulTarget  converted integer
 Return Code:   converted integer
****************************************************************************/
/****************************************************************************
 功    能:      高位在前的字符串表示转换为整型数
 输入参数:
    psSource    待转换的串
    uiSCnt      源串的长度
 输出参数:
    *pulTarget  转换后整数
 返 回 值:      转换后整数
****************************************************************************/
ulong PubChar2Long(uchar *psSource, uint uiSCnt, ulong *pulTarget);

/****************************************************************************
Function:       Converted integer to BCD string,1234 --> 0x12 0x34
Param In:
    ulSource    the integer need to be converted
    uiTCnt      the size of object buffer(byte number)
Param Out:
    psTarget    object buffer
Return:         none
****************************************************************************/
/****************************************************************************
功    能:       将整数转换为BCD格式的串存放,1234 --> 0x12 0x34
输入参数:
    ulSource    待转换的整数
    uiTCnt      目标缓冲区的大小(字节数)
输出参数:
    psTarget    目标缓冲区
返 回 值:       无
****************************************************************************/
void PubLong2Bcd(ulong ulSource, uint uiTCnt, uchar *psTarget);

/****************************************************************************
Function:       Convert BCD string to integer
Param In:
    psSource    BCD format string need to be converted
    uiSCnt      the length of BCD string
 Param Out:
    *pulTarget  converted integer
Return:         converted integer
****************************************************************************/
/****************************************************************************
功    能:       转换BCD串为整数
输入参数:
    psSource    待转换的BCD数据格式的串
    uiSCnt      源串的长度
输出参数:
    *pulTarget  转换后整数
返 回 值:       转换后整数
****************************************************************************/
ulong PubBcd2Long(uchar *psSource, uint uiSCnt, ulong *pulTarget);

/****************************************************************************
Function:       Convert the character string to capital
Param in:
   pszString    the string need to be converted, ended with '\0'
Param out:
   pszString    converted string, ended with '\0'
Return Code:    none
****************************************************************************/
/****************************************************************************
功    能:       将字符串转换为大写
输入参数:
   pszString    待转换的字符串,以'\0'结束
输出参数:
   pszString    转换后的字符串,以'\0'结束
返 回 值:       无
****************************************************************************/
void PubStrUpper(uchar *pszString);

/****************************************************************************
Function:       Convert the character string to lowercase
Param in:
   pszString    the string need to be converted, ended with '\0'
Param out:
   pszString    converted string, ended with '\0'
Return Code:    none
****************************************************************************/
/****************************************************************************
功    能:       将字符串转换为小写
输入参数:
   pszString    待转换的字符串,以'\0'结束
输出参数:
   pszString    转换后的字符串,以'\0'结束
返 回 值:       无
****************************************************************************/
void PubStrLower(uchar *pszString);

/****************************************************************************
 Function:      Delete the characters include blank,enter,newline,TAB in the
 	              string
 Param In:
    pszString   the string need to be converted, ended with '\0'
 Param Out:
    pszString   converted string, ended with '\0'
 Return Code:   无
****************************************************************************/
/****************************************************************************
 功    能:      删除字符串中的空白字符(包括,空格/回车/换行/TAB字符)
 输入参数:
    pszString   待处理的字符串,以'\0'结束
 输出参数:
    pszString   处理后的字符串,以'\0'结束
 返 回 值:   none
****************************************************************************/
void PubTrimStr(uchar *pszString);

/****************************************************************************
 Function:      Delete the specified character in string
 Param In:
    pszString   the string need to be converted, ended with '\0'
 Param Out:
    pszString   converted string, ended with '\0'
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:      删除字符串中的特定字符
 输入参数:
    pszString   待处理的字符串,以'\0'结束
    ucSpcChar   特定字符
 输出参数:
    pszString   处理后的字符串,以'\0'结束
 返 回 值:      无
****************************************************************************/
void PubTrimSpcStr(uchar *pszString, uchar ucSpcChar);

/****************************************************************************
 Function:      Delete the specified continuous character on the left of the
 	              string
 Param In:
    pszString   the string need to be converted, ended with '\0'
 Param Out:
    pszString   converted string, ended with '\0'
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:      删除字符串左边连续的特定字符
 输入参数:
    pszString   待处理的字符串,以'\0'结束
    ucSpcChar   特定字符
 输出参数:
    pszString   处理后的字符串,以'\0'结束
 返 回 值:      无
****************************************************************************/
//void PubTrimSpcStrLeft(uchar *pszString, uchar ucSpcChar);

/****************************************************************************
 Function:      Delete the specified continuous characters on the right of the
 	              string
 Param In:
    pszString   the string need to be converted, ended with '\0'
 Param Out:
    pszString   converted string, ended with '\0'
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:        删除字符串右边连续的特定字符
 输入参数:
    pszString     待处理的字符串,以'\0'结束
    ucRemoveChar  待删除字符
 输出参数:
    pszString     处理后的字符串,以'\0'结束
 返 回 值:        无
****************************************************************************/
void PubTrimTailChars(uchar *pszString, uchar ucRemoveChar);

/****************************************************************************
 Function:      Delete the specified continuous characters on the left of the
 	              string
 Param In:
    pszString   the string need to be converted, ended with '\0'
 Param Out:
    pszString   converted string, ended with '\0'
 Return Code:   none
****************************************************************************/
/****************************************************************************
 功    能:        删除字符串左边连续的特定字符
 输入参数:
    pszString     待处理的字符串,以'\0'结束
    ucRemoveChar  待删除字符
 输出参数:
    pszString     处理后的字符串,以'\0'结束
 返 回 值:        无
****************************************************************************/
void PubTrimHeadChars(uchar *pszString, uchar ucRemoveChar);

/****************************************************************************
Function:       Compare two strings without regard to capital or lowercase
Param In:
    pszStr1     string 1
    pszStr2     string 2
Param Out:      none
Return:
    0               ok
    <0              pszStr1 < pszStr2
    >0              pszStr1 > pszStr2
****************************************************************************/
/****************************************************************************
功    能:       不区分大小写的字符串比较函数
输入参数:
    pszStr1     待比较的字符串1
    pszStr2     待比较的字符串2
输出参数:       无
返 回 值:
    0               ok
    <0              pszStr1 < pszStr2
    >0              pszStr1 > pszStr2
****************************************************************************/
int PubStrNoCaseCmp(uchar *pszStr1, uchar *pszStr2);

// 填充前导字符,以便达到uiTargetLen长度
void PubAddHeadChars(uchar *pszString, uint uiTargetLen, uchar ucAddChar);

/****************************************************************************
Function:       reverse the string
Param In:
    pszString   the string ended with '\0' to be reversed
Param Out:
    pszString   the reversed string ended with '\0'
Return:         none
****************************************************************************/
/****************************************************************************
功    能:       反转字符串
输入参数:
    pszString   待反转的字符串,以'\0'结束
输出参数:
    pszString   反转后的字符串,以'\0'结束
返 回 值:       无
****************************************************************************/
void PubStrReverse(uchar *pszString);

/****************************************************************************
Function:       get the high 4 bit of the byte
Param In:
    ucInChar    the byte
Param Out:
Return:         the high 4 bit of the byte
****************************************************************************/
/****************************************************************************
功    能:       取字节的高四位
输入参数:
    ucInChar    待取高四位的字节
输出参数:
返 回 值:       高四位的字节
****************************************************************************/
uchar PubHigh4Bit(uchar ucInChar);

/****************************************************************************
Function:       get the low 4 bit of the byte
Param In:
    ucInChar    the byte
Param Out:
Return:         the low 4 bit of the byte
****************************************************************************/
/****************************************************************************
功    能:       取字节的低四位
输入参数:
    ucInChar    待取低四位的字节
输出参数:
返 回 值:       低四位的字节
****************************************************************************/
uchar PubLow4Bit(uchar ucInChar);

/****************************************************************************
Function:       multiply one ASC string by another string
Param In:
    psStr1      multiplicand ASC string
    ucStr1Len   the length of psStr1
    psStr2      multiplier ASC string
    ucStr2Len   the length of psStr2
Param Out:
    psStr1      the result of multiplication,length will be ucStr1Len+ucStr2Len
Return:         none
****************************************************************************/
/****************************************************************************
功    能:       两个ASC字符串相乘
输入参数:
    psStr1      被乘数ASC字符串
    ucStr1Len   psStr1的长度
    psStr2      乘数ASC字符串
    ucStr2Len   psStr2的长度
输出参数:
    psStr1      相乘的结果,长度为ucStr1Len+ucStr2Len
返 回 值:       无
****************************************************************************/
//void PubAscMul(uchar *psStr1, uchar ucStr1Len, uchar *psStr2, uchar ucStr2Len);
void PubAscMul(uchar *pszFaciend, uchar *pszMultiplier, uchar *pszProduct);

/****************************************************************************
Function:       Add one ASC string to another one, the length of two strings
	              are same. fill 0 in the front.
Param In:
    psStr1      summand ASC string
    psStr2      addend ASC string
    nStrLen     the length of the two string
    pnFlag      the flag whether there are carry
Param Out:
    psStr1      the result of the addition
    pnFlag      0  no carry
                1  carry
Return:         none
****************************************************************************/
/****************************************************************************
功    能:       长度相等的两个ASC字符串相加，前补0
输入参数:
    psAddend1   被加数ASC字符串
    psAddend2   加数ASC字符串
    uiLen       字符串的长度
输出参数:
    pszSum      相加的结果
返 回 值:       无
****************************************************************************/
void PubAscAdd(uchar *psAddend1, uchar *psAddend2, uint uiLen, uchar *pszSum);
#define SafeAscAdd(a,b,c)	PubAscAdd(a,b,c,a)

//void PubAscAdd(uchar *psStr1, uchar *psStr2, uint uiStrLen, ushort *pnFlag);

/****************************************************************************
Function:       subtract one ASC string from another one, the length of two
	              strings are same. fill 0 in the front.
Param In:
    psStr1      minuend ASC string
    psStr2      subtrahend ASC string
    nStrLen     the length of the two string
    pnFlag      the flag whether there are carry
Param Out:
    psStr1      the result of the subtraction
    pnFlag      0  no borrow
                1  borrow
Return:         none
****************************************************************************/
/****************************************************************************
功    能:       长度相等的两个ASC字符串相减，前补0
输入参数:
    psMinuend    被减数ASC字符串
    psSubtrahend 减数ASC字符串
    uiLen        字符串的长度
输出参数:
    pszResult   结果
返 回 值:       无
****************************************************************************/
void PubAscSub(uchar *psMinuend, uchar *psSubtrahend, uint uiLen, uchar *pszResult);
#define SafeAscSub(a,b,c)	PubAscSub(a,b,c,a)
//void PubAscSub(uchar *psStr1, uchar *psStr2, uint uiStrLen, ushort *pnFlag);

/****************************************************************************
Function:       1 subtracted from the ASC string, the result store in that ASC
	              string
Param In:
    psAscStr    the ASC string
    nStrLen     the length of psAscStr
Param Out:
    psAscStr    the ASC string which store the result
Return:         0  successful
                1  failed
****************************************************************************/
/****************************************************************************
功    能:       ASC字符串减1,结果放在原串里
输入参数:
    psAscStr    需减1的ASC字符串
    nStrLen     psAscStr的长度
输出参数:
    psAscStr    减1后的ASC字符串
返 回 值:       0  成功
                1  失败
****************************************************************************/
uchar PubAscDec(uchar *psAscStr, uint uiStrLen);

/****************************************************************************
Function:       Add 1 to the ASC string, the result store in that ASC string
Param In:
    psAscStr    the ASC string
    nStrLen     the length of psAscStr
Param Out:
    psAscStr    the ASC string which store the result
Return:         0  successful
                1  failed
****************************************************************************/
/****************************************************************************
功    能:       ASC字符串加1,结果放在原串里
输入参数:
    psAscStr    需加1的ASC字符串
    nStrLen     psAscStr的长度
输出参数:
    psAscStr    加1后的ASC字符串
返 回 值:       0  成功
                1  失败
****************************************************************************/
uchar PubAscInc(uchar *psAscStr, uint uiStrLen);

/****************************************************************************
Function:       Add one BCD string to another one, the length of two strings
	              are same. fill 0 in the front.
Param In:
    psAddend1   summand BCD string
    psAddend2   addend BCD string
    uiLen       the length of the two string
Param Out:
    psResult    the result of the addition
Return:         none
****************************************************************************/
/****************************************************************************
功    能:       长度相等的两个BCD字符串相加，前补0
输入参数:
    psAddend1   被加数BCD字符串
    psAddend2   加数BCD字符串
    uiLen       字符串的长度
输出参数:
    psResult     相加的结果
返 回 值:       无
****************************************************************************/
void PubBcdAdd(uchar *psAddend1, uchar *psAddend2, uint uiLen, uchar *psResult);
#define SafeBcdAdd(a,b,c)		PubBcdAdd(a, b, c, a)

/****************************************************************************
Function:       subtract one BCD string from another one, the length of two
	              strings are same. fill 0 in the front.
Param In:
    psMinuend   minuend BCD string
    psAddend2   subtrahend BCD string
    uiLen       the length of the two string
Param Out:
    psResult     the result of the subtraction
Return:         none
****************************************************************************/
/****************************************************************************
功    能:       长度相等的两个BCD字符串相减，前补0
输入参数:
    psMinuend      被减数BCD字符串
    psSubtrahend   减数BCD字符串
    uiLen          字符串的长度
输出参数:
    psResult      相减的结果
返 回 值:       无
****************************************************************************/
void PubBcdSub(uchar *psAddend1, uchar *psAddend2, uint uiLen, uchar *psResult);
#define SafeBcdSub(a,b,c)		PubBcdSub(a, b, c, a)

/****************************************************************************
Function:       1 subtracted from the BCD string, the result store in that BCD
	              string
Param In:
    psAscStr    the BCD string
    nStrLen     the length of psAscStr
Param Out:
    psAscStr    the BCD string which store the result
Return:         0  successful
                1  failed
****************************************************************************/
/****************************************************************************
功    能:       BCD字符串减1,结果放在原串里
输入参数:
    psBcdStr    需减1的BCD字符串
    nStrLen     psBcdStr的长度
输出参数:
    psBcdStr    减1后的BCD字符串
返 回 值:         0  成功
                1  失败
****************************************************************************/
uchar PubBcdDec(uchar *psBcdStr, uint uiStrLen);

/****************************************************************************
Function:       Add 1 to the BCD string, the result store in that BCD string
Param In:
    psAscStr    the BCD string
    nStrLen     the length of psAscStr
Param Out:
    psAscStr    the BCD string which store the result
Return:         0  successful
                1  failed
****************************************************************************/
/****************************************************************************
功    能:       BCD字符串加1,结果放在原串里
输入参数:
    psBcdStr    需加1的BCD字符串
    nStrLen     psBcdStr的长度
输出参数:
    psBcdStr    加1后的BCD字符串
返 回 值:       0  成功
                1  失败
****************************************************************************/
uchar PubBcdInc(uchar *psBcdStr, uint uiStrLen);

/****************************************************************************
 Function:      Convert a numeric string(no dot) to the format of amount.
 Param In:
    pszPrefix   Prefix to the amount output, such as "HKD" in "HKD*****.**",
                   allow 4 character at most. if NULL, no prefix will be attached.
	pszIn		Pointer to input amount string (without dot), e.g "314277".
	ucDeciPos   Decimal position of the numeric input string. should be less than 4.
	               e.g. if ucDeciPos==2, "14277" may output "142.77"
				   if ucDeciPos==0, "14277" may output "14,277" or "14277"
    ucMisc      Misc parameter,
                   GA_SEPARATOR : use ',' to separater large amount: "12,345.67"
				   GA_NEGATIVE	: display negative sign: "SGD-123,45.67"
 Param Out:
	pszOut      Pointer to output amount string, the result may be like
	               "SGD$-3,142.77". (depending on call parameter)
 Return Code:
    TRUE        Keyboard is pressed.
    FALSE       No key is pressed.
****************************************************************************/
uchar PubConvAmount(uchar *pszPrefix, uchar *pszIn, uchar ucDeciPos, uchar *pszOut, uchar ucMisc);

/****************************************************************************
 Function:      Decide if keyboard is pressed.
 Param In:
    none
 Param Out:
    none
 Return Code:
    TRUE        Keyboard is pressed.
    FALSE       No key is pressed.
****************************************************************************/
uchar PubKeyPressed(void);

/****************************************************************************
 Function:      Wait key press in iWaitTime.
 Param In:
    iWaitTime   >=0: Seconds to wait before user input.
                <0:  Infinite.
 Param Out:
    none
 Return Code:
                Key value user pressed or NOKEY.
****************************************************************************/
uchar PubWaitKey(short iWaitTime);

/****************************************************************************
 Function:      Display menu & call menu process function, the max # of menu
                item is 99. Max characters of menu item name is 19 bytes.
 Param In:
    pszPrompt   Title/prompt of menu group(optional), NULL or zero length
                means none. string should end with '\0'.
    pstMenu     Array of menu structure, null string of menu name means end
                of menu items.
    ucMode      Mode for show menu:MENU_NOSPEC,MENU_CENTERR,MENU_AUTOSNO,
                MENU_USESNO,MENU_PROMPT,MENU_CFONT,MENU_ASCII
    nTimeOut    >0: Seconds before user select menu.
                =0:  Infinite.
 Param Out:
    none
 Return Code:
    >=0         Index of menu user selected. In the most time, user only need
                to set the callback function member and let PubGetMenu() call
                it automatically. But in some other unusual time user need set
                callback function member to NULL, and use this returned index
                to call the process function. This is useful for dynamically
                generated menus or applications which need pass parameters to
                the process function. For example, application manager can
                read information of applications and build menu, show it, then
                run application by the returned index of the menu.
    -1          NULL menu pointer.
    -2          Invalid menu content.
    -3          No menu content.
    -4          User cancel or timeout.
****************************************************************************/
int PubGetMenu(uchar *pszPrompt, MenuItem *pstMenu, uchar ucMode, ushort nTimeOut);

/****************************************************************************
 Function:      Wait ENTER or CANCEL key is pressed in iWaitTime.
 Param In:
    nWaitTime   >0: Seconds to wait before user input.
                =0:  Infinite.
 Param Out:
    none
 Return Code:
    TRUE        ENTER is pressed.
    FALSE       CANCEL is pressed.
****************************************************************************/
uchar PubYesNo(ushort nWaitTime);

/****************************************************************************
 Function:      Read boolean parameter value.
 Param In:
    ucLine      Line number where to display prompt message.
    pszPrompt   Prompt message.
 Param Out:
    pucResult   Pointer of boolean parameter value.
 Return Code:
    0           OK.
    0xFF        User cancelled.
****************************************************************************/
uchar PubGetBoolParam(uchar ucLine, uchar *pszPrompt, uchar *pucResult);

/****************************************************************************
 Function:      Read string/number/password parameter value.
 Param In:
    ucType       GS_NUMBER: number string
                 GS_ALPHA:  number & Alpha string
                 GS_PASSWD: password
                 GS_LALIGN: left-aligned(default is right-aligned)
                 GS_FLOAT: float point string
    ucPromptLine Line number where to display prompt message.
    pszPrompt    Prompt message.
    ucMinLen     Min length of parameter string.
    ucMaxLen     Max length of parameter string.
 Param Out:
    pszData     Pointer of parameter string value.
 Return Code:
    0           OK.
    0xFF        User cancelled.
****************************************************************************/
uchar PubGetStrParam(uchar ucType,	uchar ucPromptLine,	uchar *pszPrompt,
					uchar ucMinLen,	uchar ucMaxLen,	uchar *pszData);

/****************************************************************************
 Function:      Input amount value.
 Notice:        This function will use whole line6-7 and the last 4 character
                  of line4-5 to display the amount string and prefix, so do not
				  place prompt information at these places.
 Param In:
    pszPrefix   Pointer to the prefix string. e.g "HKD", "SGD$". max allow
                  4 bytes. if NULL of empty content, prefix won't be displayed.
    ucDeciPos   decimal position of the amount, should be same with tims/Protims para.
                  allow 0 to 5.
    ucMinLen    minimal amount length(digits).
    ucMaxLen    Maximal amount length(digits).
    ucTimeout   waiting timeout(seconds).
    ucMisc      Misc parameter,
                   GA_SEPARATOR : use ',' to separater large amount: "HKD$12,345.67"
				   GA_NEGATIVE	: display negative sign: "SGD-123,45.67"
 Param Out:
	pszData     Pointer to output string (end with '\0')
    pszData     Pointer of parameter string value.
 Return Code:
    0           OK.
    0xFE        Illegal parameter.
    0xFF        User cancelled.
****************************************************************************/
#define GA_SEPARATOR	0x80
#define GA_NEGATIVE		0x40
uchar PubGetAmount(uchar *pszPrefix, uchar ucDeciPos,
				  uchar ucMinLen, uchar ucMaxLen, uchar *pszData,
				  uchar ucTimeout, uchar ucMisc);

// get string of number/password/alpha
uchar PubGetString(uchar ucMode, uchar ucMinlen, uchar ucMaxlen, uchar *pszOut, uchar ucTimeOut);

//显示
/****************************************************************************
 Function:      Show title at line zero & Clear screen first, if needed.
 Param In:
    bClrScreen  TRUE:  Clear screen before show title.
                FALSE: Keep original screen.
    pszTitle    Pointer of title to be displayed. string should end with '\0'.
 Param Out:
    none
 Return Code:   none
****************************************************************************/
void PubShowTitle(uchar bClrScreen, uchar *pszTitle);	//

/****************************************************************************
 Function:      Show message(CFONT) at ucLine, center-aligned. If the text
                is out of right-border, extra characters will be truncated.
 Param In:
    ucLine      Line number to show message. If ucLine & REVER is true, the
                background of the text is reversed.
    pszMsg      Pointer of string to be displayed. string should end with '\0'.
 Param Out:
    none
 Return Code:   none
****************************************************************************/
void PubShowMsg(uchar ucLine, uchar *pszMsg);			//

/****************************************************************************
 Function:      Show two message(CFONT) at ucLine and ucLine+2, center-aligned.
                If each text is out of right-border, extra characters will be
                truncated.
 Param In:
    ucLine      Line number to show first message. If ucLine & REVER is true,
                the background of the text is reversed. Since 2 lines of CFONT will
                be displayed, ucLine should be 0 to 4.
    pszMsg1     Pointer of 1st string to be displayed. string should end with '\0'.
    pszMsg2     Pointer of 2nd string to be displayed. string should end with '\0'.
 Param Out:
    none
 Return Code:   none
****************************************************************************/
void PubShowTwoMsg(uchar ucLine, uchar *pszMsg1, uchar *pszMsg2);		//


/****************************************************************************
 Function:      Display a string(CFONT) at specified line and backgroud.
 Param In:
    pszStr      Pointer of string to be displayed. string should end with '\0'.
	            get rid of bRever
    ucPosition  the value : (dispMode & 0x0F) indicates the line where to display,
                while the value : (dispMode & 0x0F) indicates the align rule.
                Line number should be 0 to 6, and align value should be DISP_SCR_CENTER,
                DISP_LINE_CENTER, DISP_LINE_RIGH and DISP_LINE_LEFT
 Param Out:
    none
 Return Code:   none
 Example:
                PubDispString("This is a sample", TRUE, DISP_LINE_LEFT+3);
                  will display the string in CFONT with black background, left align
                on the 4th line (use 0x00 to 0x07 to indicate line 1 to 8)
****************************************************************************/
void PubDispString(void *pszStr, uchar ucPosition);

/****************************************************************************
 Function:      APP halt when error happen.
 Param In:
    pszfile     Pointer to file name string, end with '\0'.
    uiLine      Line No. where the error happen.
 Param Out:     None
 Return Code:   None
****************************************************************************/
void PubHalt(uchar *pszfile, uint uiLine);

/****************************************************************************
 Function:      Beep 800ms at frequency 6.
 Param In:      None.
 Param Out:     None
 Return Code:   None
****************************************************************************/
void PubLongBeep(void);	//

/****************************************************************************
 Function:      Beep 60ms at frequency 6 for 3 times, with 80ms interval.
 Param In:      None.
 Param Out:     None
 Return Code:   None
****************************************************************************/
void PubBeepOk(void);		//

/****************************************************************************
 Function:      Beep 200ms at frequency 6,then delay 200ms.
 Param In:      None.
 Param Out:     None
 Return Code:   None
****************************************************************************/
void PubBeepErr(void);		//


//文件
/****************************************************************************
 Function:      Read data from file. Should use Seek() to set write pointer first.
 Param In:
    fd          file handle to be read from.
	psData      data pointer to place data read
    iNeedByte   number of bytes to be read out.
 Param Out:
    none.
 Return Code:
    total read out bytes.
****************************************************************************/
int PubFReadN(int fd, void *psData, int iNeedBytes);

/****************************************************************************
 Function:      Read data from file pointed by pszFileName.
 Param In:
    pszFileName Name of the file to be read.
    lOffset     Offset to read data.
    iLen        Length of the data to be read.
 Param Out:
    pszData     Output pointer of read dada.
 Return Code:
    0:          OK.
    1:          open file failed.
    2:          seek file failed.
    3:          read file failed.
****************************************************************************/
uint PubFileRead(uchar *pszFileName, long lOffset, void *psData, int iLen);		//

/****************************************************************************
 Function:      Write data into file. Should use Seek() to set write pointer first.
 Param In:
    fd          file handle to be written into
	psData      pointer of data source to write into file
    iNeedBytes  number of bytes to write
 Param Out:
    none
 Return Code:
    total write bytes.
****************************************************************************/
int PubFWriteN(int fd, void *psData, int iNeedBytes);

/****************************************************************************
 Function:      Write data to file pointed by pszFileName.
 Param In:
    pszFileName Name of the file to be written.
    lOffset     Offset to write data.
    pszData     Pointer of dada to be written to.
    iLen        Length of the data.
 Param Out:
    none
 Return Code:
    0:          OK.
    1:          open file failed.
    2:          seek file failed.
    3:          write file failed.
****************************************************************************/
uint PubFileWrite(uchar *pszFileName, long lOffset, void *psData, int iLen);	//

//计算

/****************************************************************************
 Function:      多功能DES加密/解密运算
 Param In:
    ucMode      DES加密/解密模式:
                ONE_ENCRYPT --> 单DES加密
                ONE_DECRYPT --> 单DES解密
                TRI_ENCRYPT --> 三DES加密
                TRI_DECRYPT --> 三DES解密
    psData:     待加密/解密的数据(8字节)
    psKey:      DES运算的密钥(单倍长度或者两倍长度,根据ucMode确定)
 Param Out:
    psResult    DES运算的结果(8字节)
 Return Code:   none
****************************************************************************/
void PubDes(uchar ucMode, uchar *psData, uchar *psKey, uchar *psResult);	//

/****************************************************************************
 Function:      报文MAC运算函数
 Param In:
    ucMode      报文MAC运算模式:
                MAC_FAST    --> 快速MAC运算(HyperCom快速方式)
                MAC_ANSIX99 --> ANSI x9.9 方式
    psKey:      MAC运算的密钥(8字节)
    psMsg:      计算MAC的数据
    uiLength:   计算MAC的数据的长度
 Param Out:
    psMAC       报文的MAC(8字节)
 Return Code:   none
****************************************************************************/
void PubCalcMac(uchar ucMode, uchar *psKey, uchar *psMsg, uint uiLen, uchar *psMAC);	//


// 计算LRC
uchar PubCalcLRC(uchar *psData, ushort uiLength, uchar ucInit);



#ifdef _POS_DEBUG

/****************************************************************************
 Function:      Show HEX message on screen.
 Param In:
    pszTitle    Title of the message.
    psMsg       Pointer of message to be displayed.
    uiMsgLen    Bytes of the message.
    nTimeOut    Seconds between user interaction.
 Param Out:
    none
 Return Code:   none
****************************************************************************/
void DispHexMsg(uchar *pszTitle, uchar *psMsg, uint uiMsgLen, short nTimeOut);

// print a line as hexadecimal format
int DispHexLine(uchar *pszBuff, uint uiOffset, uchar *psMsg, uint uiMsgLen);

// For Debug use, disp file name and line
void DispAssert(uchar *pszFileName, ulong ulLineNo);

    // debug macro for boolean expression
	#define _POS_DEBUG_WAIT		15
    #define PubASSERT(expr)                                           \
                if( !(expr) ){                                      \
                    DispAssert((uchar *)__FILE__, (ulong)(__LINE__));       \
                }

    // print string debug information
    #define PubTRACE0(sz)                                             \
            {                                                       \
                ScrCls();                                           \
                ScrPrint(0, 0, CFONT, "%s", sz);                    \
                PubWaitKey(_POS_DEBUG_WAIT);							\
            }

    #define PubTRACE1(sz, p1)                                         \
            {                                                       \
                ScrCls();                                           \
                ScrPrint(0, 0, CFONT, sz, p1);                      \
                PubWaitKey(_POS_DEBUG_WAIT);							\
            }

    #define PubTRACE2(sz, p1, p2)                                     \
            {                                                       \
                ScrCls();                                           \
                ScrPrint(0, 0, CFONT, sz, p1, p2);                  \
                PubWaitKey(_POS_DEBUG_WAIT);							\
            }

    #define PubTRACE3(sz, p1, p2, p3)                                 \
            {                                                       \
                ScrCls();                                           \
                ScrPrint(0, 0, CFONT, sz, p1, p2, p3);              \
                PubWaitKey(_POS_DEBUG_WAIT);							\
            }

    #define PubTRACEHEX(t, s, l)    DispHexMsg((t), (s), (l), _POS_DEBUG_WAIT)

#else /* _POS_DEBUG */

    #define PubASSERT(expr)
    #define PubTRACE0(sz)
    #define PubTRACE1(sz, p1)
    #define PubTRACE2(sz, p1, p2)
    #define PubTRACE3(sz, p1, p2, p3)
    #define PubTRACEHEX(t, s, l)

#endif /* _POS_DEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* _UTIL_H */
