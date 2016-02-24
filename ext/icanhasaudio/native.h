#ifndef ICANHASAUDIO_H
#define ICANHASAUDIO_H

#include <ruby.h>
#ifdef HAVE_RUBY_IO_H
  #include <ruby/io.h> // Ruby 1.9
  #define OpenFile rb_io_t
  #define STDIO(fp) ((fp)->stdio_file)
#else
  #include <rubyio.h>
  #define STDIO(fp) ((fp)->f)
#endif
#include <lame/lame.h>
#include <dlfcn.h>
#include <assert.h>
#include <ogg/ogg.h>
#include <vorbis/vorbisfile.h>

#include <get_audio.h>
#include <rb_wav.h>
#include <audio_mpeg_decoder.h>
#include <audio_mpeg_decoder_mp3data.h>
#include <audio_mpeg_encoder.h>
#include <audio_ogg_decoder.h>

void Init_icanhasaudio();

#endif
