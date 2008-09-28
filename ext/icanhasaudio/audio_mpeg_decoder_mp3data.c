#include <audio_mpeg_decoder_mp3data.h>

static VALUE header_parsed_eh(VALUE self)
{
  mp3data_struct * mp3data;
  Data_Get_Struct(self, mp3data_struct, mp3data);

  if(mp3data->header_parsed == 1) return Qtrue;
  return Qfalse;
}

static VALUE bitrate(VALUE self)
{
  mp3data_struct * mp3data;
  Data_Get_Struct(self, mp3data_struct, mp3data);
  return INT2NUM(mp3data->bitrate);
}

static VALUE total_frames(VALUE self)
{
  mp3data_struct * mp3data;
  Data_Get_Struct(self, mp3data_struct, mp3data);
  return INT2NUM(mp3data->totalframes);
}

static VALUE get_nsamp(VALUE self)
{
  mp3data_struct * mp3data;
  Data_Get_Struct(self, mp3data_struct, mp3data);
  return LONG2NUM(mp3data->nsamp);
}

static VALUE set_nsamp(VALUE self, VALUE num)
{
  mp3data_struct * mp3data;
  Data_Get_Struct(self, mp3data_struct, mp3data);
  mp3data->nsamp = NUM2LONG(num);
  return num;
}

static VALUE deallocate(mp3data_struct * mp3data)
{
  free(mp3data);
}

static VALUE allocate(VALUE klass)
{
  mp3data_struct * mp3data = calloc(1, sizeof(mp3data_struct));
  return Data_Wrap_Struct(klass, 0, deallocate, mp3data);
}

void init_audio_mpeg_decoder_mp3data()
{
  VALUE rb_mAudio = rb_define_module("Audio");
  VALUE rb_mMpeg = rb_define_module_under(rb_mAudio, "MPEG");
  VALUE rb_cDecoder = rb_define_class_under(rb_mMpeg, "Decoder", rb_cObject);
  VALUE klass = rb_define_class_under(rb_cDecoder, "MP3Data", rb_cObject);

  rb_const_set(klass, rb_intern("MAX_U_32_NUM"), INT2NUM(MAX_U_32_NUM));
  rb_define_method(klass, "header_parsed?", header_parsed_eh, 0);
  rb_define_method(klass, "bitrate", bitrate, 0);
  rb_define_method(klass, "total_frames", total_frames, 0);
  rb_define_method(klass, "nsamp", get_nsamp, 0);
  rb_define_method(klass, "nsamp=", set_nsamp, 1);
  rb_define_alloc_func(klass, allocate);
}
