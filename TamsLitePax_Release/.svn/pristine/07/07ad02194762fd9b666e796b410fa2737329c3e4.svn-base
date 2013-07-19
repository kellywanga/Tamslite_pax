//---------------------------------------------------------------------------
// NAME: arcfour.h
// DESC: This include file for arcfour
//
// Copyright (c) IT Experts (Pty) Ltd 2003.  All rights reserved.
//
// $Log: arcfour.h,v $
// Revision 1.1  2003/09/29 06:37:59  ecarpenter
// initial checkin
//
// Revision 1.1  2003/09/27 19:46:29  ecarpenter
// Initial checkin
//
//---------------------------------------------------------------------------
#ifndef ARCFOUR_H
#define ARCFOUR_H

//------------------------------------------------------------[ Includes ]---

//-------------------------------------------------------------[ Defines ]---
typedef struct _arc4_key
{
	unsigned char state[256];
	unsigned char x;
	unsigned char y;
} arc4_key;

//----------------------------------------------------[ Global Variables ]---

//-----------------------------------------------------[ Local Variables ]---

//----------------------------------------------------------[ Prototypes ]---
#ifdef __cplusplus
extern "C" {
#endif

void ARC4_Init(const char *pcKeyData, int iKeyDataLen, arc4_key *pKey);
void ARC4_Update(char *pcBuffer, int iBufLen, arc4_key *pKey);


#ifdef __cplusplus
}
#endif
//---------------------------------------------------------------------------
//
//                               T H E   E N D
//
//---------------------------------------------------------------------------
#endif
