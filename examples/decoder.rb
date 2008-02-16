require 'icanhasaudio'

reader = Audio::MPEG::Decoder.new
File.open(ARGV[0], 'rb') { |mp3|
  File.open(ARGV[1], 'wb') { |out|
    reader.decode(mp3, out);
  }
}
