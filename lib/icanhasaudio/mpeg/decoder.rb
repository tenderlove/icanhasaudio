module Audio
  module MPEG
    class Decoder
      attr_reader :stereo, :samplerate, :bitrate, :mode, :mode_ext, :framesize

      # Number of bits, 8 or 16
      attr_accessor :bits

      def initialize
        @stereo     = nil
        @samplerate = nil
        @bitrate    = nil
        @mode       = nil
        @mode_ext   = nil
        @framesize  = nil
        @bits       = 16
        @raw        = nil
        yield self if block_given?
      end

      def decode input, output
        native_decode(input, output)
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

      def determine_samples_for infile
        length = File.stat(infile.path).size
        total_seconds = length * 8.0 / (1000.0 * @bitrate)
        self.num_samples = (total_seconds * in_samplerate).to_i
      end
    end
  end
end


