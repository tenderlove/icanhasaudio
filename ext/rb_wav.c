#include <icanhasaudio.h>

#define WRITE_U32(buf, x) *(buf)     = (unsigned char)((x)&0xff);\
                          *((buf)+1) = (unsigned char)(((x)>>8)&0xff);\
                          *((buf)+2) = (unsigned char)(((x)>>16)&0xff);\
                          *((buf)+3) = (unsigned char)(((x)>>24)&0xff);

#define WRITE_U16(buf, x) *(buf)     = (unsigned char)((x)&0xff);\
                          *((buf)+1) = (unsigned char)(((x)>>8)&0xff);

/* Some of this based on ao/src/ao_wav.c */
void prelim_header( VALUE self,
                    char *headbuf,
                    unsigned int size,   /* 0x7fffffff */
                    ogg_int64_t knownlength,
                    int channels,
                    int samplerate
                    ) {
  VALUE bits = rb_iv_get(self, "@bits");
  int bytespersec = channels*samplerate*NUM2INT(bits)/8;
  int align = channels*NUM2INT(bits)/8;
  int samplesize = NUM2INT(bits);
  
  if(knownlength && knownlength*NUM2INT(bits)/8*channels < size)
      size = (unsigned int)(knownlength*NUM2INT(bits)/8*channels+44) ;
  
  memcpy(headbuf, "RIFF", 4);
  WRITE_U32(headbuf+4, size-8);
  memcpy(headbuf+8, "WAVE", 4);
  memcpy(headbuf+12, "fmt ", 4);
  WRITE_U32(headbuf+16, 16);
  WRITE_U16(headbuf+20, 1); /* format */
  WRITE_U16(headbuf+22, channels);
  WRITE_U32(headbuf+24, samplerate);
  WRITE_U32(headbuf+28, bytespersec);
  WRITE_U16(headbuf+32, align);
  WRITE_U16(headbuf+34, samplesize);
  memcpy(headbuf+36, "data", 4);
  WRITE_U32(headbuf+40, size - 44);
}

void rewrite_header(char *headbuf, unsigned int written) 
{
    unsigned int length = written;

    length += 44;

    WRITE_U32(headbuf+4, length-8);
    WRITE_U32(headbuf+40, length-44);
}
