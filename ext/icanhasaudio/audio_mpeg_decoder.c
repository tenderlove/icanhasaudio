/*
 * Most of this code was take from the lame front end.  So thank you to the
 * lame team!  The good parts are theirs, the bad parts are mine.
 *
 * The rest of it is (c) 2007 Aaron Patterson <aaronp@tenderlovemaking.com>
 *
 * Released under the GPL
 */
#include <native.h>

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

static int lame_decode_initfile(VALUE file, mp3data_struct * mp3data) {
  unsigned char * buf;
  VALUE str;
  int len = 4;
  int enc_delay;
  int enc_padding;
  int ret;
  short int pcm_l[1152], pcm_r[1152];

  str = rb_funcall(file, rb_intern("read"), 1, INT2NUM(len));
  buf = (unsigned char *)StringValuePtr(str);
  if(buf[0] == 'I' && buf[1] == 'D' && buf[2] == '3') {
    len = 6;
    str = rb_funcall(file, rb_intern("read"), 1, INT2NUM(len));
    buf = (unsigned char *)StringValuePtr(str);

    buf[2] &= 127; buf[3] &= 127; buf[4] &= 127; buf[5] &= 127;
    len = (((((buf[2] << 7) + buf[3]) << 7) + buf[4]) << 7) + buf[5];

    rb_funcall( file,
                rb_intern("read"),
                1,
                INT2NUM(len));

    len = 4;
    str = rb_funcall(file, rb_intern("read"), 1, INT2NUM(len));
    buf = (unsigned char *)StringValuePtr(str);
  }

  /* Check for Album ID */
  if(0 == rb_str_cmp(str, rb_str_new2("AiD\1"))) {
    /* FIXME */
    rb_raise(rb_eRuntimeError, "Found Album ID.\n");
  }

  while (!is_syncword_mp123(buf)) {
    int i;
    for(i = 0; i < len - 1; i++) {
      buf[i] = buf[i + 1];
    }
    buf[len - 1] = NUM2INT(rb_funcall(file, rb_intern("getc"), 0));
  }

  if ((buf[2] & 0xF0) == 0) {
    printf("Input file is freeformat\n");
  }

  ret = lame_decode1_headersB(buf, len, pcm_l, pcm_r, mp3data, &enc_delay, &enc_padding);
  if(ret == -1)
    rb_raise(rb_eRuntimeError, "Decode headers failed.\n");

  while(!mp3data->header_parsed) {
    str = rb_funcall(file, rb_intern("read"), 1, INT2NUM(100));
    buf = (unsigned char *)StringValuePtr(str);
    ret = lame_decode1_headersB(buf, 100, pcm_l, pcm_r, mp3data,&enc_delay,&enc_padding);
    if(ret == -1)
      rb_raise(rb_eRuntimeError, "Decode headers failed.\n");
  }

  if(mp3data->totalframes > 0) {
  } else {
    mp3data->nsamp = MAX_U_32_NUM;
  }
  return 0;
}

/*
 * call-seq:
 *    native_decode(input_io, output_io)
 *
 * Decode the input IO and write it to the output IO.
 */
static VALUE native_decode(VALUE self, VALUE file, VALUE outf) {
  mp3data_struct mp3data;
  lame_global_flags * gfp;

  memset(&mp3data, 0, sizeof(mp3data_struct));
  lame_decode_initfile(file, &mp3data);

  Data_Get_Struct (self, lame_global_flags, gfp);

  rb_iv_set(self, "@stereo", INT2NUM(mp3data.stereo));
  rb_iv_set(self, "@samplerate", INT2NUM(mp3data.samplerate));
  rb_iv_set(self, "@bitrate", INT2NUM(mp3data.bitrate));
  rb_iv_set(self, "@mode", INT2NUM(mp3data.mode));
  rb_iv_set(self, "@mode_ext", INT2NUM(mp3data.mode_ext));
  rb_iv_set(self, "@framesize", INT2NUM(mp3data.framesize));

  lame_decoder(self, file, outf, &mp3data);

  return Qnil;
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
}
