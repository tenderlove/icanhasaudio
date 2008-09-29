module Audio
  module WAV
    class File
      attr_accessor :bits
      def initialize io_or_path, mode = 'wb'
        @io = io_or_path.is_a?(IO) ? io_or_path : File.open(io_or_path, mode)
        @bits = 16
        if block_given?
          yield self
          close
        end
      end

      def write_header size, known_length, channels, samplerate
        if known_length != 0 && known_length * bits / 8 * channels < size
          size = known_length * bits / 8 * channels + 44
        end

        bytespersec = channels * samplerate * bits / 8
        align = channels * bits / 8

        header = [
          'RIFF',
          u32(size - 8),
          'WAVE',
          'fmt ',
          u32(16),
          u16(1),
          u16(channels),
          u32(samplerate),
          u32(bytespersec),
          u16(align),
          u16(bits),
          'data',
          u32(size - 44),
        ].join
        @io.write header
      end

      def write *args
        @io.write(*args)
      end

      def close *args
        @io.close(*args)
      end

      def seek *args
        @io.seek(*args)
      end

      private
      def u32 num
        [0, 8, 16, 24].map { |x| (num >> x) & 0xFF }.pack('C4')
      end

      def u16 num
        [0, 8].map { |x| (num >> x) & 0xFF }.pack('C2')
      end
    end
  end
end
