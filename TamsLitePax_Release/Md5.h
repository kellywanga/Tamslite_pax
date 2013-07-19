//---------------------------------------------------------------------------
// NAME: rc5.h
// DESC: This module implements the rc5 algorithm which was derived from the
//		 RSA Data Security, Inc. MD5 Message-Digest Algorithm.
//
// Copyright (c) IT Experts (Pty) Ltd 2003.  All rights reserved.
//
// Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
// rights reserved.
//
// License to copy and use this software is granted provided that it
// is identified as the "RSA Data Security, Inc. MD5 Message-Digest
// Algorithm" in all material mentioning or referencing this software
// or this function.
//
// License is also granted to make and use derivative works provided
// that such works are identified as "derived from the RSA Data
// Security, Inc. MD5 Message-Digest Algorithm" in all material
// mentioning or referencing the derived work.
//
// RSA Data Security, Inc. makes no representations concerning either
// the merchantability of this software or the suitability of this
// software for any particular purpose. It is provided "as is"
// without express or implied warranty of any kind.
//
// These notices must be retained in any copies of any part of this
// documentation and/or software.
//
// $Log: md5.h,v $
// Revision 1.3  2003/09/29 07:38:24  sdebie
// Changed prototype
//
// Revision 1.2  2003/09/29 06:38:19  ecarpenter
// Fixed porting bug
//
// Revision 1.1  2003/09/27 19:46:29  ecarpenter
// Initial checkin
//
// Revision 1.1  2003/09/22 11:04:59  ecarpenter
// Initial Checkin
//
//---------------------------------------------------------------------------
#ifndef _MD5_H_
#define _MD5_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------[ Includes ]---

//-------------------------------------------------------------[ Defines ]---
#define UINT4 unsigned long

// MD5 context.
typedef struct {
	unsigned char debug1;
	UINT4 state[4];					// state (ABCD)
	UINT4 count[2];					// number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];		// input buffer
} MD5_CTX;

//----------------------------------------------------[ Global Variables ]---

//-----------------------------------------------------[ Local Variables ]---

//----------------------------------------------------------[ Prototypes ]---
void MD5Init(MD5_CTX *);
void MD5Update(MD5_CTX *, const unsigned char *, unsigned int);
void MD5Final(unsigned char [16], MD5_CTX *);
void GetHash (char *pcData[],int DataLength,char *ReturnBuffer);

#ifdef __cplusplus
}
#endif

#endif
//---------------------------------------------------------------------------
//
//                               T H E   E N D
//
//---------------------------------------------------------------------------
