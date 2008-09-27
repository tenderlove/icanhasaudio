#include <native.h>

int
lame_decoder(VALUE self, VALUE infile, VALUE outf, mp3data_struct * mp3data)
{
  lame_global_flags * gfp;
  short int Buffer[2][1152];
  int     iread;
  double  wavsize;
  int     i;
  int tmp_num_channels;
  int skip;
  char headbuf[44];
  VALUE raw;

  raw = rb_iv_get(self, "@raw");
  if(raw == Qnil) {
    raw = Qfalse;
    rb_iv_set(self, "@raw", Qfalse);
  }

  Data_Get_Struct(self, lame_global_flags, gfp);
  tmp_num_channels = lame_get_num_channels( gfp );

  skip = lame_get_encoder_delay(gfp)+528+1;

  if(!raw) {
    rb_iv_set(self, "@bits", INT2NUM(16));
    prelim_header(  self,
                    headbuf,
                    0x7FFFFFFF,
                    0,
                    tmp_num_channels,
                    lame_get_in_samplerate( gfp )
                    );
    rb_funcall(outf, rb_intern("write"), 1, rb_str_new(headbuf, 44));
  }

  wavsize = -skip;
  mp3data->totalframes = mp3data->nsamp / mp3data->framesize;

  assert(tmp_num_channels >= 1 && tmp_num_channels <= 2);

  do {
    char BitBuffer16[1152 * 4];
    int bit_16_i = 0;
    int total = 0;
    iread = get_audio16(self, infile, Buffer, mp3data);
    mp3data->framenum += iread / mp3data->framesize;
    wavsize += iread;

    memset(&BitBuffer16, 0, 1152 * 4);

    skip -= (i = skip < iread ? skip : iread); /* 'i' samples are to skip in this frame */

    for (; i < iread; i++) {
      /* Write the 0 channel */
      BitBuffer16[bit_16_i] = Buffer[0][i] & 0xFF;
      BitBuffer16[bit_16_i + 1] = ((Buffer[0][i] >> 8) & 0xFF);

      if (tmp_num_channels == 2) {
        BitBuffer16[bit_16_i + 2] = Buffer[1][i] & 0xFF;
        BitBuffer16[bit_16_i + 3] = ((Buffer[1][i] >> 8) & 0xFF);
        bit_16_i += 4;
      } else {
        bit_16_i += 2;
      }
    }
    rb_funcall(outf, rb_intern("write"), 1, rb_str_new(BitBuffer16, bit_16_i));
  } while (iread);

  i = (16 / 8) * tmp_num_channels;
  assert(i > 0);
  if (wavsize <= 0) {
      wavsize = 0;
  }
  else if (wavsize > 0xFFFFFFD0 / i) {
      wavsize = 0xFFFFFFD0;
  }
  else {
      wavsize *= i;
  }

  if(!raw && rb_funcall(self, rb_intern("attempt_rewind"), 1, outf)) {
    rewrite_header(headbuf, (int)wavsize);
    rb_funcall(outf, rb_intern("write"), 1, rb_str_new(headbuf, 44));
  }
  return 0;
}

