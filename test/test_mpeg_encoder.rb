require File.expand_path(File.join(File.dirname(__FILE__), 'helper'))

class MPEGEncoderTest < ICANHASAUDIO::TestCase
  include Audio::MPEG

  WAV_FILE = File.dirname(__FILE__) + "/assets/testcase.wav"
  #AIFF_FILE = File.dirname(__FILE__) + "/assets/cow.aiff"

  def setup
    @encoder = Audio::MPEG::Encoder.new
    assert File.exists?(WAV_FILE)
    #assert File.exists?(AIFF_FILE)
  end

  def test_encoder_initialize
    assert Audio::MPEG::Encoder.new
  end
  
  def test_encode
    File.open("#{Dir::tmpdir}/out.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_bitrate
    @encoder.bitrate = 128
    assert_equal(128, @encoder.bitrate)
    assert_equal(128, @encoder.vbr_min_bitrate)
  end

  def test_vbr_type
    @encoder.vbr_type = Encoder::VBR_OFF
    assert_equal(Encoder::VBR_OFF, @encoder.vbr_type)
    File.open("#{Dir::tmpdir}/no_vbr.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_min_vbr_bitrate
    @encoder.vbr_min_bitrate = 128
    assert_equal(128, @encoder.vbr_min_bitrate)
  end

  def test_set_vbr_max_bitrate
    @encoder.vbr_max_bitrate = 128
    assert_equal(128, @encoder.vbr_max_bitrate)
    @encoder.vbr_max_bitrate = 256
    assert_equal(256, @encoder.vbr_max_bitrate)
  end

  def test_set_vbr_hard_min
    assert_equal(false, @encoder.vbr_hard_min?)
    @encoder.vbr_hard_min = true
    assert_equal(true, @encoder.vbr_hard_min?)
    @encoder.vbr_hard_min = false
    assert_equal(false, @encoder.vbr_hard_min?)
  end

  def test_set_title
    @encoder.title = 'tenderlovemaking.com'

    File.open("#{Dir::tmpdir}/title.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_artist
    @encoder.artist = 'Aaron Patterson'

    File.open("#{Dir::tmpdir}/artist.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_album
    @encoder.album = 'Some Album'

    File.open("#{Dir::tmpdir}/album.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_year
    @encoder.year = 1999

    File.open("#{Dir::tmpdir}/year.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_track
    @encoder.track = 1

    File.open("#{Dir::tmpdir}/track.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_genre
    @encoder.genre = 'Porn Groove'

    File.open("#{Dir::tmpdir}/genre.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_all
    @encoder.title = 'tenderlovemaking.com'
    @encoder.artist = 'Aaron Patterson'
    @encoder.album = 'ICANHASAUDIO'
    @encoder.year = 2008
    @encoder.track = 1
    @encoder.genre = 'Rock'
    File.open("#{Dir::tmpdir}/all.mp3", 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end
end
