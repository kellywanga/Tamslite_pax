#ifndef  _KEYS_H
#define _KEYS_H
#define KEY_FUNCNR		3
#define KEY_SIZE		16
#define NRKEYS			10
#define PIN_SIZE 		10
#define TERMID_SIZE		16

#define KTKKEY_SIZE     16

#define KEY_OK			0
#define KEY_WRONG_PIN	1
#define KEY_NOMASTER    2
#define KEY_NOKEYS	    3
#define KEY_ERROR		4
#define KEY_CANCELLED	5

int NewKey();
int GetKeys();
int GetKey(int Key,unsigned char KeyBin[50]);
static int ProcessNewKeyResponse(char *Xml);
static int ProcessGetKeysResponse(char * Xml);

#endif