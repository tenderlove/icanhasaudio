#include <icanhasaudio.h>

void prelim_header( VALUE self,
                    char *headbuf,
                    unsigned int size,   /* 0x7fffffff */
                    ogg_int64_t knownlength,
                    int channels,
                    int samplerate
                    );
void rewrite_header(char *headbuf, unsigned int written);
