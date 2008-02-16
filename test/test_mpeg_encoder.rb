require 'test/unit'
require 'icanhasaudio'

class MPEGEncoderTest < Test::Unit::TestCase
  WAV_FILE = File.dirname(__FILE__) + "/assets/testcase.wav"
  AIFF_FILE = File.dirname(__FILE__) + "/assets/cow.aiff"

  def setup
    @encoder = Audio::MPEG::Encoder.new
    assert File.exists?(WAV_FILE)
  end

  def test_encoder_initialize
    assert Audio::MPEG::Encoder.new
  end
  
  def test_encode_aiff
    File.open('/tmp/out_aiff.mp3', 'wb+') { |outfile|
      @encoder.encode(File.open(AIFF_FILE, 'rb'), outfile)
    }
  end

  def test_encode
    File.open('/tmp/out.mp3', 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_title
    @encoder.title = 'tenderlovemaking.com'

    File.open('/tmp/title.mp3', 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_artist
    @encoder.artist = 'Aaron Patterson'

    File.open('/tmp/artist.mp3', 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_album
    @encoder.album = 'Some Album'

    File.open('/tmp/album.mp3', 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_year
    @encoder.year = 1999

    File.open('/tmp/year.mp3', 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_track
    @encoder.track = 1

    File.open('/tmp/track.mp3', 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end

  def test_set_genre
    @encoder.genre = 'Porn Groove'

    File.open('/tmp/genre.mp3', 'wb+') { |outfile|
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
    File.open('/tmp/all.mp3', 'wb+') { |outfile|
      @encoder.encode(File.open(WAV_FILE, 'rb'), outfile)
    }
  end
end
