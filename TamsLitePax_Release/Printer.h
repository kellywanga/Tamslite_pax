#ifndef _PRINTER_H

#define _PRINTER_H

#define DOTTEDLINE		"------------------------"

#define DOUBLELINE		"========================"
typedef struct 
{

	int ErrorCode;

	char ErrorMessage[200];
	char FunctionName[50];
}ErrorReceipt;

/*
typedef struct
{
}TransactionReceipt;

*/
int PrintErrorReceipt(ErrorReceipt _ErrorReceipt);


 int RePrintTransaction(int Control);

void PrintSlipHeader(const char *pcSlipHeader);


void PrintSlipFooter();


void printEftTranSlip(EftTransaction *Transaction);


void PrintEftError(EftTransaction *Transaction);
#endif