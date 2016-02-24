#include <native.h>

void Init_icanhasaudio()
{
  init_audio_mpeg_decoder();
  init_audio_mpeg_decoder_mp3data();
  init_audio_mpeg_encoder();
  init_audio_ogg_decoder();
}
