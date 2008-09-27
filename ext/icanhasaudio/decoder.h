#ifndef ICANHASAUDIO_LAME_DECODER_H
#define ICANHASAUDIO_LAME_DECODER_H

#include <native.h>

int
lame_decoder(VALUE self, VALUE infile, VALUE outf, mp3data_struct * mp3data);

#endif
