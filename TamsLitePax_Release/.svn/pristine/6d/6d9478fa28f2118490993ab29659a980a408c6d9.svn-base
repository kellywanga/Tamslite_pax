//---------------------------------------------------------------------------
// NAME: security.h
// DESC: This header file for the security on vx670
//
// Copyright (c) IT Experts (Pty) Ltd 2003.  All rights reserved.
//
// $Log: security.h,v $
// Revision 1.5  2005/11/21 09:41:01  sdebie
// Added function to check if DUKPT keys was loaded
//
// Revision 1.4  2005/04/15 08:44:55  sdebie
// Change DUKPT function
//
// Revision 1.3  2005/03/11 08:22:21  sdebie
// Added Functions for KTK
//
// Revision 1.2  2005/03/02 07:41:25  sdebie
// Added Getpin
//
// Revision 1.1  2005/02/22 06:26:11  sdebie
// Initial Checkin
//
//---------------------------------------------------------------------------
#ifndef SECURITY_H
#define SECURITY_H

//------------------------------------------------------------[ Includes ]---

//-------------------------------------------------------------[ Defines ]---
#define	DUKPT_KEY	2
#define	KTK_KEY		1

//----------------------------------------------------[ Global Variables ]---

//-----------------------------------------------------[ Local Variables ]---

//----------------------------------------------------------[ Prototypes ]---
int		writeDukptKey(void);
int		GetPin(char *pcSerial, char *pcPinBlock, char *cPan, char *cMsg, int KeyIndex);
int		TriDesGetPin(char *pcPinBlock, char *cPan, char *cMsg);
int		GetKeyCards(char *pcIKSN, char *IPEK);
int		checkDuckputKeys(void);
void	Des_3DES_DEA(unsigned char *in, unsigned char *out, unsigned char *key, short	boolEncrypt);
int KeysLdr_WriteCipheredKey(void *avoidPtr, unsigned char *dukptKey, short	avoidShort);
//---------------------------------------------------------------------------
//
//                               T H E   E N D
//
//---------------------------------------------------------------------------
#endif

