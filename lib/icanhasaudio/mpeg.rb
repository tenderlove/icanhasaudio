require 'icanhasaudio/mpeg/encoder'

class Audio::MPEG::Decoder
  attr_reader :stereo, :samplerate, :bitrate, :mode, :mode_ext, :framesize

  # Number of bits, 8 or 16
  attr_accessor :bits

  def initialize
    @bits = 16
    yield self if block_given?
  end
end

