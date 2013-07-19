#include <string.h>
#include <posapi.h>
#include <posapi_all.h>
#include <posapi_s80.h>
#include "Bin2Ascii.h"
#include "TamsLite.h"

  char * bin2ascii(const void *data, size_t len ,char *dest) {
    char *s = dest;
    const unsigned char *src = (const unsigned char *) data;
    for (; len > 0; --len, s += 2)
        sprintf(s, "%02X", *src++);
    *s = 0;
    return dest;
}
int  Bin2Ascii(unsigned char Source[], int SourceLength, unsigned char Destination[])
{
	 //Destination[(SourceLength * 2)+1] = '\0';
	//bin2ascii((char *)Destination,Source,(SourceLength * 2)+1);
       /* Destination[SourceLength * 2] = '\0';
        while(SourceLength != 0)
        {
                Destination[SourceLength * 2 - 2] = Source[SourceLength - 1];
                Destination[SourceLength * 2 - 1] = Source[SourceLength - 1];
                Destination[SourceLength * 2 - 2] &= 0x0f;
                Destination[SourceLength * 2 - 1] &= 0xf0;
                Destination[SourceLength * 2 - 1] >>= 0x4;
                if(Destination[SourceLength * 2 - 2] <= 0x9)
                        Destination[SourceLength * 2 - 2] |= 0x30;
                else
                {
                        Destination[SourceLength * 2 - 2] |= 0x60;
                        Destination[SourceLength * 2 - 2] -= 0x9;
                }
                if(Destination[SourceLength * 2 - 1] <= 0x9)
                        Destination[SourceLength * 2 - 1] |= 0x30;
                else
                {
                        Destination[SourceLength * 2 - 1] |= 0x60;
                        Destination[SourceLength * 2 - 1] -= 0x9;
                }
                SourceLength--;
        }
		uppercase((char *)Destination);*/
		return 0;
}