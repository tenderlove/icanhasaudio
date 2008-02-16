#include "mpeg_encoder.h"
#include <rubyio.h>

static VALUE cMpegEncoder;

static void encoder_free(lame_global_flags * gfp) {
  lame_close(gfp);
}

/*
 * call-seq:
 *    Audio::MPEG::Encoder.new
 *
 * Returns a new MPEG Encoder object.
 */
static VALUE
encoder_allocate(VALUE klass) {
  lame_global_flags * gfp = lame_init();
  id3tag_init(gfp);

  return Data_Wrap_Struct(klass, NULL, encoder_free, gfp);
}

/* Public methods. */

/*
 * call-seq:
 *    encoder.quality=
 *
 * Set the VBR quality. 0 = highest, 9 = lowest
 */
static VALUE MpegEncoder_set_vbr_quality(VALUE self, VALUE quality) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_set_VBR_q(gfp, NUM2INT(quality));
  return quality;
}

/*
 * call-seq:
 *    encoder.quality
 *
 * Get the VBR quality. 0 = highest, 9 = lowest
 */
static VALUE MpegEncoder_get_vbr_quality(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_VBR_q(gfp));
}

/*
 * call-seq:
 *    encoder.vbr_type=
 *
 * Set the type of VBR. Must be VBR_OFF, VBR_NORMAL, or VBR_FAST
 */
static VALUE MpegEncoder_set_vbr_type(VALUE self, VALUE type) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_set_VBR(gfp, NUM2INT(type));
  return type;
}

/*
 * call-seq:
 *    encoder.vbr_type
 *
 * Get the type of VBR.
 */
static VALUE MpegEncoder_get_vbr_type(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_VBR(gfp));
}

/*
 * call-seq:
 *    encoder.print_config
 *
 * Print the encoder configuration.
 */
static VALUE MpegEncoder_print_config(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_print_config(gfp);
  return Qnil;
}

/*
 * call-seq:
 *    encoder.artist=
 *
 * Set the ID3 artist.
 */
static VALUE MpegEncoder_set_artist(VALUE self, VALUE artist) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  id3tag_set_artist(gfp, StringValuePtr(artist));
  return artist;
}

/*
 * call-seq:
 *    encoder.title=
 *
 * Set the ID3 title.
 */
static VALUE MpegEncoder_set_title(VALUE self, VALUE title) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  id3tag_set_title(gfp, StringValuePtr(title));
  return title;
}

/*
 * call-seq:
 *    encoder.album=
 *
 * Set the ID3 album.
 */
static VALUE MpegEncoder_set_album(VALUE self, VALUE album) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  id3tag_set_album(gfp, StringValuePtr(album));
  return album;
}

/*
 * call-seq:
 *    encoder.year=
 *
 * Set the ID3 year.
 */
static VALUE MpegEncoder_set_year(VALUE self, VALUE year) {
  lame_global_flags * gfp;
  VALUE * year_string;

  Data_Get_Struct(self, lame_global_flags, gfp);
  year_string = rb_funcall(year, rb_intern("to_s"), 0);
  id3tag_set_year(gfp, StringValuePtr(year_string));
  return year;
}

/*
 * call-seq:
 *    encoder.track=
 *
 * Set the ID3 track.
 */
static VALUE MpegEncoder_set_track(VALUE self, VALUE track) {
  lame_global_flags * gfp;
  int track_number;
  VALUE * track_string;

  track_number = NUM2INT(track);
  if(track < 0 || track > 255)
    rb_raise(rb_eRuntimeError, "Track must be between 0 and 255.\n");

  Data_Get_Struct(self, lame_global_flags, gfp);
  track_string = rb_funcall(track, rb_intern("to_s"), 0);
  id3tag_set_track(gfp, StringValuePtr(track_string));
  return track;
}

/*
 * call-seq:
 *    encoder.genre=
 *
 * Set the ID3 genre.
 */
static VALUE MpegEncoder_set_genre(VALUE self, VALUE genre) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  id3tag_set_genre(gfp, StringValuePtr(genre));
  return genre;
}

/* Private methods. */

static VALUE MpegEncoder_write_vbr_tag(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  if(lame_get_bWriteVbrTag(gfp))
    return Qtrue;
  return Qfalse;
}

static VALUE MpegEncoder_print_internals(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_print_internals(gfp);
  return Qnil;
}

static VALUE MpegEncoder_get_mpeg_quality(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_quality(gfp));
}

static VALUE MpegEncoder_get_compression_ratio(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return rb_float_new(lame_get_compression_ratio(gfp));
}

static VALUE MpegEncoder_get_mpeg_version(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_version(gfp));
}

static VALUE MpegEncoder_get_mpeg_mode(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_mode(gfp));
}

static VALUE MpegEncoder_get_force_ms(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_force_ms(gfp));
}

static VALUE MpegEncoder_get_out_samplerate(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_out_samplerate(gfp));
}

static VALUE MpegEncoder_set_in_samplerate(VALUE self, VALUE samplerate) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_set_in_samplerate(gfp, NUM2INT(samplerate));
  return samplerate;
}

static VALUE MpegEncoder_set_num_samples(VALUE self, VALUE num_samples) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_set_num_samples(gfp, NUM2ULONG(num_samples));
  return num_samples;
}

static VALUE MpegEncoder_get_num_samples(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return ULONG2NUM(lame_get_num_samples(gfp));
}

static VALUE MpegEncoder_set_num_channels(VALUE self, VALUE num_channels) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_set_num_channels(gfp, NUM2INT(num_channels));
  return num_channels;
}

static VALUE MpegEncoder_get_num_channels(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_num_channels(gfp));
}

static VALUE MpegEncoder_get_framesize(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  return INT2NUM(lame_get_framesize(gfp));
}

static VALUE MpegEncoder_encoder_buffer(VALUE self, VALUE left, VALUE right) {
  unsigned char mp3buffer[LAME_MAXMP3BUFFER];
  int * buffer_left;
  int * buffer_right;
  int length, i;
  int imp3;
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);

  length = NUM2INT(rb_funcall(left, rb_intern("length"), 0));
  buffer_left = calloc(length, sizeof(int));
  buffer_right = calloc(length, sizeof(int));

  for(i = 0; i < length; i++) {
    buffer_left[i] = NUM2UINT(rb_funcall(left, rb_intern("[]"), 1, INT2NUM(i)));
    buffer_right[i] = NUM2UINT(rb_funcall(right, rb_intern("[]"), 1,INT2NUM(i)));
  }

  imp3 = lame_encode_buffer_int(gfp, buffer_left, buffer_right, length,
    mp3buffer, sizeof(mp3buffer));

  free(buffer_left);
  free(buffer_right);

  if(imp3 < 0) {
    if(imp3 == -1)
      rb_raise(rb_eRuntimeError, "Mp3 buffer is not big enough.\n");
    else
      rb_raise(rb_eRuntimeError, "internal error.\n");
  }
  return rb_str_new(mp3buffer, imp3);
}

static VALUE MpegEncoder_flush(VALUE self) {
  unsigned char mp3buffer[LAME_MAXMP3BUFFER];
  int imp3;
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);
  imp3 = lame_encode_flush(gfp, mp3buffer, sizeof(mp3buffer));
  if(imp3 < 0) {
    if(imp3 == -1)
      rb_raise(rb_eRuntimeError, "Mp3 buffer is not big enough.\n");
    else
      rb_raise(rb_eRuntimeError, "internal error.\n");
  }
  return rb_str_new(mp3buffer, imp3);
}

static VALUE MpegEncoder_write_vbr_tags(VALUE self, VALUE outfile) {
  OpenFile *fp;
  lame_global_flags * gfp;

  GetOpenFile(outfile, fp);

  Data_Get_Struct(self, lame_global_flags, gfp);
  lame_mp3_tags_fid(gfp, fp->f);
  return Qnil;
}

static VALUE MpegEncoder_init_params(VALUE self) {
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);

  if(lame_init_params(gfp) < 0) {
    rb_raise(rb_eRuntimeError, "Fatal error during initialization.\n");
  }

  return Qnil;
}

void init_MpegEncoder(VALUE rb_mMpeg) {
  /*
  rb_mAudio = rb_define_module("Audio");
  rb_mMpeg = rb_define_module_under(rb_mAudio, "MPEG");
  */
  cMpegEncoder = rb_define_class_under(rb_mMpeg, "Encoder", rb_cObject);
  rb_define_alloc_func(cMpegEncoder, encoder_allocate);

  /* Public Methods */
  rb_define_method(cMpegEncoder, "vbr_quality=",MpegEncoder_set_vbr_quality, 1);
  rb_define_method(cMpegEncoder, "vbr_quality", MpegEncoder_get_vbr_quality, 0);
  rb_define_method(cMpegEncoder, "vbr_type=", MpegEncoder_set_vbr_type, 1);
  rb_define_method(cMpegEncoder, "vbr_type", MpegEncoder_get_vbr_type, 0);
  rb_define_method(cMpegEncoder, "print_config", MpegEncoder_print_config, 0);
  rb_define_method(cMpegEncoder, "title=", MpegEncoder_set_title, 1);
  rb_define_method(cMpegEncoder, "artist=", MpegEncoder_set_artist, 1);
  rb_define_method(cMpegEncoder, "album=", MpegEncoder_set_album, 1);
  rb_define_method(cMpegEncoder, "year=", MpegEncoder_set_year, 1);
  rb_define_method(cMpegEncoder, "track=", MpegEncoder_set_track, 1);
  rb_define_method(cMpegEncoder, "genre=", MpegEncoder_set_genre, 1);

  rb_define_private_method(cMpegEncoder, "init_params", MpegEncoder_init_params, 0);
  rb_define_private_method(cMpegEncoder, "num_channels=", MpegEncoder_set_num_channels, 1);
  rb_define_private_method(cMpegEncoder, "num_channels", MpegEncoder_get_num_channels, 0);
  rb_define_private_method(cMpegEncoder, "in_samplerate=", MpegEncoder_set_in_samplerate, 1);
  rb_define_private_method(cMpegEncoder, "num_samples=", MpegEncoder_set_num_samples, 1);
  rb_define_private_method(cMpegEncoder, "num_samples", MpegEncoder_get_num_samples, 0);
  rb_define_private_method(cMpegEncoder, "out_samplerate", MpegEncoder_get_out_samplerate, 0);
  rb_define_private_method(cMpegEncoder, "framesize", MpegEncoder_get_framesize, 0);
  rb_define_private_method(cMpegEncoder, "encode_buffer", MpegEncoder_encoder_buffer, 2);
  rb_define_private_method(cMpegEncoder, "flush", MpegEncoder_flush, 0);
  rb_define_private_method(cMpegEncoder, "write_vbr_tags", MpegEncoder_write_vbr_tags, 1);
  rb_define_private_method(cMpegEncoder, "force_ms", MpegEncoder_get_force_ms, 0);
  rb_define_private_method(cMpegEncoder, "mpeg_mode", MpegEncoder_get_mpeg_mode, 0);
  rb_define_private_method(cMpegEncoder, "mpeg_version", MpegEncoder_get_mpeg_version, 0);
  rb_define_private_method(cMpegEncoder, "compression_ratio", MpegEncoder_get_compression_ratio, 0);
  rb_define_private_method(cMpegEncoder, "mpeg_quality", MpegEncoder_get_mpeg_quality, 0);
  rb_define_private_method(cMpegEncoder, "print_internals", MpegEncoder_print_internals, 0);
  rb_define_private_method(cMpegEncoder, "write_vbr_tag?", MpegEncoder_write_vbr_tag, 0);
}
