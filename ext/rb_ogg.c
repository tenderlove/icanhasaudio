#include <icanhasaudio.h>

size_t rb_ogg_read(void *ptr, size_t size, size_t nmemb, void *datasource) {
  VALUE file = (VALUE)datasource;
  VALUE str;
  size_t length;
  
  str = rb_funcall(file, rb_intern("read"), 1, INT2NUM(size * nmemb));
  if(str == Qnil) return 0;

  length = NUM2INT(rb_funcall(str, rb_intern("length"), 0));

  memcpy(ptr, StringValuePtr(str), length);
  return length;
}

int rb_ogg_seek(void *datasource, ogg_int64_t offset, int whence) {
  VALUE file = (VALUE)datasource;
  if(rb_respond_to(file, rb_intern("seek")) == Qtrue) {
    return NUM2INT(rb_funcall( file,
                rb_intern("seek"),
                2,
                INT2NUM(offset),
                INT2NUM(whence)));
  }
  return -1;
}

int rb_ogg_close(void *datasource) {
  VALUE file = (VALUE)datasource;
  rb_funcall(file, rb_intern("close"), 0);
  return 0;
}

long rb_ogg_tell(void *datasource) {
  VALUE file = (VALUE)datasource;
  return NUM2LONG(rb_funcall(file, rb_intern("tell"), 0));
}

/*
 * call-seq:
 *    decoder.decode(input_io, output_io)
 *
 * Decode the input IO and write it to the output IO.
 */
VALUE method_ogg_decode(VALUE self, VALUE infile, VALUE outf) {
  OggVorbis_File vf;
  ov_callbacks callbacks;
  int bs = 0;
  char buf[8192];
  int buflen = 8192;
  unsigned int written = 0;
  int ret;
  ogg_int64_t length = 0;
  int seekable = 0;
  VALUE raw;
  char headbuf[44];
  VALUE bits;
  VALUE endian;
  VALUE sign;

  raw = rb_iv_get(self, "@raw");
  if(raw == Qnil) {
    raw = Qfalse;
    rb_iv_set(self, "@raw", Qfalse);
  }

  bits = rb_iv_get(self, "@bits");
  if(bits == Qnil) {
    bits = INT2NUM(16);
    rb_iv_set(self, "@bits", INT2NUM(16));
  }

  endian = rb_iv_get(self, "@endian");
  if(endian == Qnil) {
    rb_iv_set(self, "@endian", INT2NUM(0));
    endian = INT2NUM(0);
  }

  sign = rb_iv_get(self, "@sign");
  if(sign == Qnil) {
    rb_iv_set(self, "@sign", INT2NUM(1));
    sign = INT2NUM(1);
  }

  callbacks.read_func   = rb_ogg_read;
  callbacks.seek_func   = rb_ogg_seek;
  callbacks.close_func  = rb_ogg_close;
  callbacks.tell_func   = rb_ogg_tell;

  if(ov_open_callbacks((void *)infile, &vf, NULL, 0, callbacks) < 0) {
    rb_raise(rb_eRuntimeError, "Failed to open input as vorbis.\n");
  }

  if(ov_seekable(&vf)) {
    seekable = 1;
    length = ov_pcm_total(&vf, 0);
  }

  if(!raw) {
    prelim_header(  self,
                    headbuf,
                    0x7fffffff,
                    length,
                    ov_info(&vf,0)->channels,
                    ov_info(&vf,0)->rate );

    rb_funcall(outf, rb_intern("write"), 1, rb_str_new(headbuf, 44));
  }

  while((ret = ov_read(&vf, buf, buflen, NUM2INT(endian), NUM2INT(bits)/8, NUM2INT(sign), &bs)) != 0) {
    if(bs !=0) {
      rb_raise(rb_eRuntimeError,
          "Only one logical bitstream currently supported.\n");
    }

    if(ret) {
      rb_funcall(outf, rb_intern("write"), 1, rb_str_new(buf, ret));
      written += ret;
    }
  }

  if(seekable && !raw) {
    rewrite_header(headbuf, written);
    rb_funcall( outf,
                rb_intern("seek"),
                2,
                INT2NUM(0),
                rb_const_get(rb_cIO, rb_intern("SEEK_SET")) );
    rb_funcall(outf, rb_intern("write"), 1, rb_str_new(headbuf, 44));
  }

  ov_clear(&vf);

  return Qnil;
}
