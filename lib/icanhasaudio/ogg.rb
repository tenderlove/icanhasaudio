class Audio::OGG::Decoder
  # Set to true for no WAV header
  attr_accessor :raw

  # Number of bits, 8 or 16
  attr_accessor :bits

  # Endianness
  attr_accessor :endian

  # Signedness
  attr_accessor :sign

  def initialize
    @raw    = false
    @bits   = 16
    @endian = 0
    @sign   = 1
    yield self if block_given?
  end
end
