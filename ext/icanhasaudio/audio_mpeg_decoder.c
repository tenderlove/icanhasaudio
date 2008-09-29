/*
 * Most of this code was take from the lame front end.  So thank you to the
 * lame team!  The good parts are theirs, the bad parts are mine.
 *
 * The rest of it is (c) 2007 Aaron Patterson <aaronp@tenderlovemaking.com>
 *
 * Released under the GPL
 */
#include <audio_mpeg_decoder.h>

static void reader_mark(lame_global_flags * lgf) {}
static void reader_free(lame_global_flags * gfp) {
  lame_close(gfp);
}

/*
 * call-seq:
 *    Audio::MPEG::Decoder.new
 *
 * Returns a new MPEG Decoder object.
 */
static VALUE
reader_allocate(VALUE klass) {
  int i = 0;
  lame_global_flags * gfp = lame_init();
  lame_set_decode_only(gfp, 1);
  i = lame_init_params(gfp);

  if(i < 0) {
    rb_raise(rb_eRuntimeError, "Fatal error during initialization.\n");
  }
  lame_decode_init();
  return Data_Wrap_Struct(klass, reader_mark, reader_free, gfp);
}

/*
 * call-seq:
 *    Audio::MPEG::Decoder.lame_version
 *
 * Returns the version of lame you are using.
 */
static VALUE method_lame_version(VALUE klass) {
  const char * version = get_lame_version();
  return rb_str_new(version, strlen(version));
}

static VALUE decode_headers_for(VALUE self, VALUE rb_buffer)
{
  int enc_delay;
  int enc_padding;
  mp3data_struct * mp3data;

  unsigned char * buf = (unsigned char *)StringValuePtr(rb_buffer);
  int len = NUM2INT(rb_funcall(rb_buffer, rb_intern("length"), 0));
  short int pcm_l[1152], pcm_r[1152];

  VALUE rb_mp3data = rb_funcall(self, rb_intern("mp3data"), 0);
  Data_Get_Struct(rb_mp3data, mp3data_struct, mp3data);

  int ret = lame_decode1_headersB(buf,
                                  len,
                                  pcm_l,
                                  pcm_r,
                                  mp3data,
                                  &enc_delay,
                                  &enc_padding
  );

  if(ret == -1)
    rb_raise(rb_eRuntimeError, "Decode headers failed.\n");

  return rb_mp3data;
}

/*
 * call-seq:
 *    native_decode(input_io, output_io)
 *
 * Decode the input IO and write it to the output IO.
 */
static VALUE native_decode(VALUE self, VALUE infile, VALUE outf) {
  lame_global_flags * gfp;
  short int Buffer[2][1152];
  int     iread;
  double  wavsize;
  int     i;
  int tmp_num_channels;
  int skip;
  VALUE raw;
  mp3data_struct * mp3data;

  VALUE rb_mp3data = rb_funcall(self, rb_intern("mp3data"), 0);
  Data_Get_Struct(rb_mp3data, mp3data_struct, mp3data);

  raw = rb_iv_get(self, "@raw");

  Data_Get_Struct(self, lame_global_flags, gfp);
  tmp_num_channels = lame_get_num_channels( gfp );
  lame_set_num_samples(gfp, MAX_U_32_NUM);

  skip = lame_get_encoder_delay(gfp)+528+1;

  rb_iv_set(self, "@bits", INT2NUM(16));

  wavsize = -skip;
  if(lame_get_num_samples(gfp) == MAX_U_32_NUM) {
    rb_funcall(self, rb_intern("determine_samples_for"), 1, infile);
  }
  mp3data->totalframes = mp3data->nsamp / mp3data->framesize;

  assert(tmp_num_channels >= 1 && tmp_num_channels <= 2);

  do {
    char BitBuffer16[1152 * 4];
    int bit_16_i = 0;
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

  rb_iv_set(self, "@wavsize", INT2NUM(wavsize));
  return Qnil;
}

/*
 * call-seq:
 *    num_channels
 *
 * Get the number of channels
 */
static VALUE get_num_channels(VALUE self)
{
  lame_global_flags * gfp;
  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_num_channels(gfp));
}

/*
 * call-seq:
 *    num_samples=(number)
 *
 * Set the number of samples
 */
static VALUE set_num_samples(VALUE self, VALUE sample_count)
{
  lame_global_flags * gfp;
  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_set_num_samples(gfp, NUM2INT(sample_count));
  return sample_count;
}

/*
 * call-seq:
 *    in_samplerate
 *
 * Get the input samplerate
 */
static VALUE get_in_samplerate(VALUE self)
{
  lame_global_flags * gfp;
  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_in_samplerate(gfp));
}

void init_audio_mpeg_decoder() {
  VALUE rb_mAudio = rb_define_module("Audio");
  VALUE rb_mMpeg = rb_define_module_under(rb_mAudio, "MPEG");
  VALUE rb_cDecoder = rb_define_class_under(rb_mMpeg, "Decoder", rb_cObject);

  rb_define_singleton_method(
      rb_cDecoder,
      "lame_version",
      method_lame_version,
      0
  );

  rb_define_alloc_func(rb_cDecoder, reader_allocate);
  rb_define_method(rb_cDecoder, "num_samples=", set_num_samples, 1);
  rb_define_method(rb_cDecoder, "in_samplerate", get_in_samplerate, 0);
  rb_define_private_method(rb_cDecoder, "native_decode", native_decode, 2);
  rb_define_private_method(rb_cDecoder, "decode_headers_for", decode_headers_for, 1);
  rb_define_private_method(rb_cDecoder, "num_channels", get_num_channels, 0);
}
