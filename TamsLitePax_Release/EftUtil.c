#include <posapi.h>
#include <posapi_all.h>
#include "PostRequest.h"
#include "Debug.h"
#include "Applib.h"
#include "TamsLite.h"
#include "EftUtil.h"

int getEftTransaction(long lRecordNo, EftTransaction *tranToGet)
{
	int    iRecords=0;
	int     iFd;
	long   lfilesize ;

	iFd = open(FILE_EFTTRANSACTION, O_RDWR);

	if (iFd == -1){
		LOG_PRINTF(("ERROR: open(FILE_EFTTRANSACTION, O_RDONLY)"));
 		return -1;
	}//if
	else{
		lfilesize  = filesize(FILE_EFTTRANSACTION);
		iRecords = lfilesize / sizeof(EftTransaction);
	}//else


	LOG_PRINTF(("ERROR: lRecordNo =%d : iRecords =%d",lRecordNo ,iRecords));

	//Invalid Record Or end of File
	/*if (lRecordNo >= iRecords){
		close(iFd);
		LOG_PRINTF(("ERROR: if (lRecordNo >= iRecords)"));
		return 0;
	}//if*/

	seek(iFd, lRecordNo*sizeof(EftTransaction), SEEK_SET);
	if (read(iFd, (void *)tranToGet, sizeof(EftTransaction)) != sizeof(EftTransaction)) {
		close(iFd);
		LOG_PRINTF(("ERROR: if (lp_read(iFd, tranToGet, sizeof(REC_EftAuthTran)) != sizeof(REC_EftAuthTran))"));
		return -1;
	}//if

	close(iFd);
	return iRecords;
	
}
int EftSaveTransaction(EftTransaction *SavTran)
{
	int iFd;

    iFd = open(FILE_EFTTRANSACTION, O_CREATE|O_RDWR);
	if (iFd==-1) {
		//DisplayMsg(MSG_ERROR,"",gettext("Error creating/opening 'trneft' file. %s"),strerror(errno));
		return 1;
	}//if
	//sk:
	/*
    #ifdef IOCTL_RECSIZE
    ioctl(iFd, IOCTL_RECSIZE, sizeof(REC_EftAuthTran));
    #endif
	*/
    if (write(iFd, (void *)SavTran, sizeof(EftTransaction)) == -1) {
        //DisplayMsg(MSG_ERROR,"",gettext("Error writing Transaction File. %s"),strerror(errno));
        close(iFd);
        return 1;
    }//if
    close(iFd);

    return 0;
}
int changeEftTranStatus(int iRecord, EftTransaction *tranToSave)
{
	int iFd;

    iFd = open(FILE_EFTTRANSACTION, O_RDWR);
	if (iFd==-1) {
		//DisplayMsg(MSG_ERROR,"",gettext("Error creating/opening 'trneft' file. %s"),strerror(errno));
		return 1;
	}//if

    seek(iFd, iRecord*sizeof(EftTransaction), SEEK_SET);
    if (write(iFd, (void *)tranToSave, sizeof(EftTransaction)) == -1) {
       // DisplayMsg(MSG_ERROR,"",gettext("Error writing Transaction File. %s"),strerror(errno));
        close(iFd);
        return 1;
    }//if
    close(iFd);

    return 0;
}
int reprintLast(void)
{
	return 0;
}