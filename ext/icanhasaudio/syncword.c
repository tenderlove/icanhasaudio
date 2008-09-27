#include <native.h>

/*
 * Taken from lame, get_audio.c
 */

int is_syncword_mp123(const void *const headerptr)
{
    const unsigned char *const p = headerptr;
    static const char abl2[16] =
        { 0, 7, 7, 7, 0, 7, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8 };

    if ((p[0] & 0xFF) != 0xFF)
        return 0;       /* first 8 bits must be '1' */
    if ((p[1] & 0xE0) != 0xE0)
        return 0;       /* next 3 bits are also */
    if ((p[1] & 0x18) == 0x08)
        return 0;       /* no MPEG-1, -2 or -2.5 */        
    if ((p[1] & 0x06) == 0x00)
        return 0;       /* no Layer I, II and III */
#ifndef USE_LAYER_1
    if ((p[1] & 0x06) == 0x03*2)
	return 0; /* layer1 is not supported */
#endif
#ifndef USE_LAYER_2
    if ((p[1] & 0x06) == 0x02*2)
	return 0; /* layer1 is not supported */
#endif
    /*
    if (!(((p[1] & 0x06) == 0x03*2 && input_format == sf_mp1)
	  || ((p[1] & 0x06) == 0x02*2 && input_format == sf_mp2)
	  || ((p[1] & 0x06) == 0x01*2 && input_format == sf_mp3)))
    */
	  if(!(p[1] & 0x06) == 0x01*2)
	return 0; /* imcompatible layer with input file format */
    if ((p[2] & 0xF0) == 0xF0)
        return 0;       /* bad bitrate */
    if ((p[2] & 0x0C) == 0x0C)
        return 0;       /* no sample frequency with (32,44.1,48)/(1,2,4)     */
    if ((p[1] & 0x18) == 0x18 && (p[1] & 0x06) == 0x04
	&& abl2[p[2] >> 4] & (1 << (p[3] >> 6)))
	return 0;
    if ((p[3] & 3) == 2)
	return 0;       /* reserved enphasis mode */
    return 1;
}

