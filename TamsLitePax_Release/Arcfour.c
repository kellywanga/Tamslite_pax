//---------------------------------------------------------------------------
// NAME: arcfour.c
// DESC: This module implements the arcfour encrypt/decrypt functions
//

//------------------------------------------------------------[ Includes ]---
#include "arcfour.h"

//-------------------------------------------------------------[ Defines ]---
#define swap_byte(x,y) tmp = (x); (x) = (y); (y) = tmp

//----------------------------------------------------[ Extern Functions ]---

//---------------------------------------------------[ Extern References ]---

//---------------------------------------------------[ Private Functions ]---

//---------------------------------------------------[ Private variables ]---

//-----------------------------------------------------------[ ARC4_Init ]---
// DESC: Initialize the ARC4 chipher
// ARGS:
//		pcKeyData	- The Keydata to initialise the chiper with
//		iKeyDataLen	- The length of the key data
//		pKey		- ARC4 state buffer
//
// RETURN:
//		none
//---------------------------------------------------------------------------
void ARC4_Init(const char *pcKeyData, int iKeyDataLen, arc4_key *pKey)
{
	register int tmp;
	register int j;
	register int i;
	unsigned char *state = pKey->state;

	for (i = 0; i < 256; i++) {
		state[i] = i;
	}//for

	pKey->x = 0;
	pKey->y = 0;

	for (j = i = 0; iKeyDataLen > 0 && i < 256; i++) {
		j += state[i] + pcKeyData[i % iKeyDataLen];
		j &= 0xff;
		swap_byte(state[i], state[j]);
	}//for
}//RC4_Init

//----------------------------------------------------------[ ARC4_Update ]---
// DESC: Function to add more data to be encrypted/decrypted
// ARGS:
//		pcBuffer	- Pointer to the data to be processed
//		iBufLen		- The length of the data
//		pKey		- ARC4 state buffer                                                                           
//
// RETURN:
//		none
//---------------------------------------------------------------------------
void ARC4_Update(char *pcBuffer, int iBufLen, arc4_key *pKey)
{
	register int tmp;
	register int counter;
	register int i = pKey->x;
	register int j = pKey->y;
	unsigned char *state = pKey->state;

	for (counter = 0; counter < iBufLen; counter++) {
		i++;
		i &= 0xff;
		j += state[i];
		j &= 0xff;

		/* swap state[i] and state[j] */
		swap_byte(state[i], state[j]);
		pcBuffer[counter] ^= state[(state[i] + tmp) & 0xff];
	}//for
	pKey->y = j;
	pKey->x = i;
}//ARC4_Update

//---------------------------------------------------------------------------
//
//                               T H E   E N D
//
//---------------------------------------------------------------------------
