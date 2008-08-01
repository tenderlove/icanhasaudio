require 'icanhasaudio/mpeg/encoder'

class Audio::MPEG::Decoder
  attr_reader :stereo, :samplerate, :bitrate, :mode, :mode_ext, :framesize

  # Number of bits, 8 or 16
  attr_accessor :bits

  def initialize
    @bits = 16
    @raw = nil
    yield self if block_given?
  end

  private
  def attempt_rewind(outf)
    begin
      outf.seek(0, IO::SEEK_SET)
      true
    rescue
      false
    end
  end
end

