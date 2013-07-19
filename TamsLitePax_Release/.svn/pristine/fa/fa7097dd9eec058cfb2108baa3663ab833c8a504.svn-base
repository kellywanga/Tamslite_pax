#ifndef _EMVCAKEY_H
# define _EMVCAKEY_H

#include  "emvlib.h"
typedef struct
{											/* CA - Certification Authority */
  unsigned char  RID[5];                    /* Registered Application Provider Identifier */
   unsigned char CA_PKIndex;
   unsigned char CA_HashAlgoIndicator;      /* 01 = SHA-1 */
   unsigned char CA_PKAlgoIndicator;        /* 01 = RSA */

   unsigned char LengthOfCAPKModulus;
   unsigned char CAPKModulus[248];

   unsigned char LengthOfCAPKExponent;
   unsigned char CAPKExponent[3];         /* TBD 1 or 3 */

   unsigned char ChecksumHash[20];
} CAPublicKey;


typedef struct
{
	 unsigned char RID[5];
	 unsigned char CAPKIndex;
	 unsigned char Serial[3];
} EMVRevocEntry;

int  GetEmvCaKeys();
static int ProcessResponse(char *Xml);
static int AddEmvCAPK(EMV_CAPK MyLocalCAPK);
int   SaveEMVCAPKToFile (EMV_CAPK * MyCAPK);
int   ReadEMVCAPKToFile(EMV_CAPK * MyCAPK);
#endif