#include <native.h>

static int
get_audio_common(VALUE self, VALUE musicin,
		  int buffer[2][1152], short buffer16[2][1152],
      mp3data_struct * mp3data);

int get_audio_pcm(VALUE self, VALUE musicin, int buffer[2][1152]) {
  return 0;
}

static int
read_samples_mp3(VALUE self, VALUE musicin,
    short int mpg123pcm[2][1152], int stereo, mp3data_struct * mp3data);

int
lame_decode_fromfile(VALUE file, short pcm_l[], short pcm_r[],
                     mp3data_struct * mp3data);

/*
  get_audio16 - behave as the original get_audio function, with a limited
                16 bit per sample output
*/
int
get_audio16(VALUE self, VALUE musicin, short buffer[2][1152],
    mp3data_struct * mp3data)
{
    return( get_audio_common( self, musicin, NULL, buffer, mp3data ) );
}

/************************************************************************
  get_audio_common - central functionality of get_audio*
    in: gfp
        buffer    output to the int buffer or 16-bit buffer
   out: buffer    int output    (if buffer != NULL)
        buffer16  16-bit output (if buffer == NULL) 
returns: samples read
note: either buffer or buffer16 must be allocated upon call
*/
static int
get_audio_common(VALUE self, VALUE musicin,
		  int buffer[2][1152], short buffer16[2][1152],
      mp3data_struct * mp3data)
{
  lame_global_flags *gfp;
  int     num_channels;
  short   buf_tmp16[2][1152];
  int     samples_read;
  int     framesize;
  int     samples_to_read;
  unsigned int tmp_num_samples;
  int     i;

  Data_Get_Struct(self, lame_global_flags, gfp);
  num_channels = lame_get_num_channels( gfp );
  
  /* 
   * NOTE: LAME can now handle arbritray size input data packets,
   * so there is no reason to read the input data in chuncks of
   * size "framesize".  EXCEPT:  the LAME graphical frame analyzer 
   * will get out of sync if we read more than framesize worth of data.
   */
  
  samples_to_read = framesize = lame_get_framesize(gfp);
  assert(framesize <= 1152);
  
  /* get num_samples */
  tmp_num_samples = lame_get_num_samples( gfp );
  
	if( buffer != NULL )
	    samples_read = read_samples_mp3( self, musicin,
					     buf_tmp16, num_channels, mp3data );
	else
	    samples_read = read_samples_mp3( self, musicin,
					     buffer16, num_channels, mp3data );

	if( buffer != NULL ) {
	    for( i = samples_read; --i >= 0; )
		buffer[0][i] = buf_tmp16[0][i] << (8 * sizeof(int) - 16);
	    if( num_channels == 2 ) {
		for( i = samples_read; --i >= 0; )
		    buffer[1][i] = buf_tmp16[1][i] << (8 * sizeof(int) - 16);
	    } else if( num_channels == 1 ) {
		memset( buffer[1], 0, samples_read * sizeof(int) );
	    } else
		assert(0);
	}


    /* if num_samples = MAX_U_32_NUM, then it is considered infinitely long.
       Don't count the samples */
  /*
    if ( tmp_num_samples != MAX_U_32_NUM )
        num_samples_read += samples_read;
        */

    return samples_read;
}

static int
read_samples_mp3(VALUE self, VALUE musicin,
    short int mpg123pcm[2][1152], int stereo, mp3data_struct * mp3data)
{
  int     out;
  lame_global_flags * gfp;

  Data_Get_Struct(self, lame_global_flags, gfp);

  out =
      lame_decode_fromfile(musicin, mpg123pcm[0], mpg123pcm[1],
                           mp3data);
  /*
   * out < 0:  error, probably EOF
   * out = 0:  not possible with lame_decode_fromfile() ???
   * out > 0:  number of output samples
   */
  if (out < 0) {
      memset(mpg123pcm, 0, sizeof(**mpg123pcm) * 2 * 1152);
      return 0;
  }

  if ( lame_get_num_channels( gfp ) != mp3data->stereo)
      rb_raise(rb_eRuntimeError, "Error: number of channels has changed");

    if ( lame_get_in_samplerate( gfp ) != mp3data->samplerate)
      rb_raise(rb_eRuntimeError, "Error: sample frequency has changed");

    return out;
}


/*
For lame_decode_fromfile:  return code
  -1     error
   n     number of samples output.  either 576 or 1152 depending on MP3 file.


For lame_decode1_headers():  return code
  -1     error
   0     ok, but need more data before outputing any samples
   n     number of samples output.  either 576 or 1152 depending on MP3 file.
*/
int
lame_decode_fromfile(VALUE file, short pcm_l[], short pcm_r[],
                     mp3data_struct * mp3data)
{
  int     ret = 0;
  unsigned char buf[1024];
  VALUE nread;
  
  memset(buf, 0, 1024);
  /* first see if we still have data buffered in the decoder: */
  ret = lame_decode1_headers(buf, 0, pcm_l, pcm_r, mp3data);

  if (ret!=0) return ret;
  
  /* read until we get a valid output frame */
  while (1) {
    VALUE str = rb_funcall(file, rb_intern("read"), 1, INT2NUM(1024));


  /*
  printf("tell: %d\n", NUM2INT(rb_funcall(file, rb_intern("tell"), 0)));
    */

    if (str == Qnil) {
      /* we are done reading the file, but check for buffered data */
      /*
  int j;
  printf("####################################################\n");
    for(j = 0; j < 1024; j++) {
      printf("%d\n", buf[j]);
    }
  printf("####################################################\n");
  printf("tell: %d\n", NUM2INT(rb_funcall(file, rb_intern("tell"), 0)));
  */
      ret = lame_decode1_headers(buf, 0, pcm_l, pcm_r, mp3data);
      if (ret<=0) {
        lame_decode_exit(); /* release mp3decoder memory */
        return -1;  /* done with file */
      }
      break;
    }

    nread = rb_funcall(str, rb_intern("length"), 0);
    memcpy(buf, StringValuePtr(str), NUM2INT(nread));

    ret = lame_decode1_headers(buf, NUM2INT(nread), pcm_l, pcm_r, mp3data);
    if (ret == -1) {
      printf("oh shit 2\n");
      lame_decode_exit();  /* release mp3decoder memory */
      return -1;
    }
    if(ret > 0) break;
  }
  return ret;
}

