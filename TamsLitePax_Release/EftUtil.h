#ifndef _EFTUTIL_H

#define _EFTUTIL_H

#define FILE_EFTTRANSACTION  "TRANDT"



int getEftTransaction(long lRecordNo, EftTransaction *tranToGet);
int EftSaveTransaction(EftTransaction *SavTran);
int changeEftTranStatus(int iRecord, EftTransaction *tranToSave);
int reprintLast(void);
#endif