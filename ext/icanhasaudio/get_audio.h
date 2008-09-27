#ifndef GET_AUDIO_H
#define GET_AUDIO_H

#include <native.h>

#define MAX_U_32_NUM 0xFFFFFFFF
int
get_audio16(VALUE self, VALUE musicin, short buffer[2][1152],
    mp3data_struct * mp3data);

int get_audio_pcm(VALUE self, VALUE musicin, int buffer[2][1152]);

#endif
