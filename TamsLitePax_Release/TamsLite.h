#ifndef _TAMSLITE_H
#define _TAMSLITE_H
#define TRACK1_LEN						79
#define TRACK2_LEN						41
#define HOT_PANSIZE                     25
typedef enum   {GPRS,ETHERNET,WIFI} CommunicationDeviceType;
typedef enum   {TRIDES,DUKPT}  EncType;
typedef enum   
{
PurchaseOnly = 1 ,
PurchasewithCashback = 2,
CashbackOnly = 3 ,
ReversalTransaction = 4,
Refund = 5,
AuthOnly = 6,
BalanceInquiry  = 7,
PINChange = 8  ,
MiniStatement = 9 ,
Transfer = 10,
Deposit = 11,
Rollback = 12,
PreAuthPurchase = 33 ,
PreAuthLifecycle = 34 ,
PreAuthAdjustment =35,
BillPayment = 39,
QuasiCash = 44,
WebPurchaseGoodsServices= 100

} TransactionType;
 typedef enum
 {
	 Default =0 ,
	 Savings = 1,
	 Current = 2
 }AccountType;

 typedef enum
 {
	 Pending =0,
	 Approved = 1,
	 ReverseTransaction = 2,
 }TransactionResponse;
typedef struct
{
	 char  MasterKey[50];
	unsigned char  Key0[50];
	unsigned char  Key1[50];
	unsigned char  Key2[50];
	unsigned char  Key3[50];
	unsigned char  Key4[50];
	unsigned char  Key5[50];
	unsigned char  Key6[50];
	unsigned char  Key7[50];
	unsigned char  Key8[50];
	unsigned char  Key9[50];
	
}EncKeys;
typedef struct
{
CommunicationDeviceType DeviceType;
unsigned char TMS[15];
short Port;
}Communication;
typedef struct{
	unsigned char APN[50];
	unsigned char UserName[50];
	unsigned char Password[50];
	unsigned char SignalStrength;
}GprsSettings;

typedef struct
{
	int NoOfEMVApps;
	EncType PinBlockEncryptionType;
	char TerminalId[16];
	char MerchantId[50];
	unsigned char SerialNumber[50];
	Communication DeviceCommunication;
	GprsSettings  DeviceGprsSettings;
	GprsSettings  DeviceGprsSettingsSecondary;
	EncKeys DeviceKeys;
}Device;

typedef struct
{
	int BatchNo;
	long int AllTransactionCount;
	long  AllTransactionTotalValue;
	long  PurchaseTransactionCount;
	long PurchaseTransactionTotalValue;
	long  PurchaseReversalTransactionCount;
	long PurchaseReversakTransactionValue;
	long  RefundTransactionCount;
	long  RefundTransactionTotalValue;
	long  RefundReversalTransactionCount;
	long  RefundReversalTransactionTotalValue;
}BatchInformation;

typedef struct Etransaction{
int   TransType;
int   cTranResult;    
int   iTranStatus;  
int   OperatorId;
int   BudgetPeriod;
int   AcctType;
int   ReversalTransactionType;
int   CurrencyCode;
long  BatchNo;
long  lRevSeq;  
long  SequenceNo;
long  PurchaseAmount;
long  CashBackAmount;
long  TipAmount;
long  OriginalSequenceNumber;
long  ProcessorFee;
long  AcquiringFee;
char  RefNo[21];
char  MerchantId[10];
char  CVV[10];
char  froacc[50];
char  tooacc[50];
char  balance[50];
char  transno[50];
char  available[10];
char  data[100];
char  TransacDateAndTime[10];
char  SupervisorFlag;
char  Track2[TRACK2_LEN];
char  Track1[TRACK1_LEN];
char  PinBlock[50];
char  PinSerial[50];
char  ManualFlag;
char  Offline;
char  RefCode[102];
char  OriginalTerminalId[50];
char  ReversalDateAndTime[10];
char  ReversalRefCode[102];
char  AddtionalData[102];
char  ICCData[700];
char  ResponseMessage[50];
char  AuthId[10];
char  IssuerScript[500];
}EftTransaction;
 
typedef struct {
    char    cPinblock1[17];
    char    cPinblocSerial1[21];

    char    cPinblock2[17];
    char    cPinblocSerial2[21];

    char    cPinblock3[17];
    char    cPinblocSerial3[21];
} REC_EftPin;

typedef struct {
    long    lDate;
    int     iAccount;
    char    cMsg[20];
    char    cAmount[20];
} REC_Statement;

typedef struct {
    int             iDeclineReason;
    int             iResult;
    char            cAvailable[20];
    char            cBalance[20];
    REC_Statement   tStatement[10];
//    char            cReferenceCode[20];
//    char            cAccountNo[34]; //From Account
} REC_EftReturn;
Device MyDevice;
BatchInformation MyBatchInfo;
int mystrpos(char *haystack, char *needle);
char *trim(char *s);
char *rtrim(char *s);
char *ltrim(char *s);
int uppercase ( char *sPtr );
char *substring(size_t start, size_t stop, const char *src, char *dst, size_t size);

int SaveDevice(Device * Device);
int ReadDevice(Device *Device);

int SaveBatchInfo(BatchInformation *BatchInfo);
int ReadBatchInfo(BatchInformation *BatchInfo);


int TruncateAllFiles();

int ShowMagStripeEftMenu(ST_MAGCARD MAGSTRIPE);
int ShowChipEftMenu(EftTransaction * MyEmvTransaction);
int ShowAdminMenu();	
#endif					