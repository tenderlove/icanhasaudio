module Audio
  module MPEG
    class Decoder
      # Number of bits, 8 or 16
      attr_accessor :bits
      attr_reader :mp3data

      def initialize
        @bits       = 16
        @raw        = false
        @mp3data    = MP3Data.new
        yield self if block_given?
      end

      def decode input, output
        buf = skip_id3_header(input)

        decode_headers_for(buf)
        while !mp3data.header_parsed?
          decode_headers_for(input.read(100))
        end
        mp3data.nsamp = MP3Data::MAX_U_32_NUM unless mp3data.total_frames > 0
        wav = WAV::File.new(output)
        wav.write_header(0x7FFFFFFF, 0, num_channels, in_samplerate) if !@raw
        native_decode(input, wav)
        if !@raw && attempt_rewind(wav)
          wav.write_header(@wavsize + 44, 0, num_channels, in_samplerate)
        end
      end

      private
      ID3 = [73, 68, 51, 3].pack('C*')
      AID = [65, 105, 68, 1].pack('C*')

      def skip_id3_header input
        header = input.read(4)
        if header == ID3
          puts "asdfadsf"
          id3_len = input.read(6).unpack('C*')[2..-1].map { |chr|
            chr & 127
          }.inject(0) { |total,chr|
            (total + chr) << 7
          } >> 7
          input.read(id3_len) # skip the ID3 tag
          header = input.read(4)
        end
        raise "Found AiD header" if header == AID

        while !syncword_mp123?(header)
          header = header.slice(1..-1) + input.getc
        end
        header
      end

      def syncword_mp123? header
        alb2 = [0, 7, 7, 7, 0, 7, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8]
        p_h = header.unpack('C*')
        return false if p_h[0] & 0xFF != 0xFF # first 8 bits must be '1'
        return false if p_h[1] & 0xE0 != 0xE0 # next 3 bits are also
        return false if p_h[1] & 0x18 == 0x08 # no MPEG-1, -2, or -2.5
        return false if p_h[1] & 0x06 == 0x00 # no Layer I, II and III
        return false if p_h[1] & 0x06 == 0x03 * 2 # layer1 not supported
        return false if p_h[1] & 0x06 == 0x02 * 2 # layer1 not supported
        return false unless p_h[1] & 0x06 == 0x01 * 2 # incompatible layer
        return false if p_h[2] & 0xF0 == 0xF0 # bad bitrate
        return false if p_h[2] & 0x0C == 0x0C # no sample frequency

        if( (p_h[1] & 0x18 == 0x18) && (p_h[1] & 0x06 == 0x04) )
          if(abl2[p_h[2] >> 4] & (1 << (p_h[3] >> 6)) != 0)
            return false
          end
        end
        return false if p_h[3] & 3 == 2
        true
      end

      def attempt_rewind outf
        begin
          outf.seek(0, IO::SEEK_SET)
          true
        rescue
          false
        end
      end

      def determine_samples_for infile
        length = File.stat(infile.path).size
        total_seconds = length * 8.0 / (1000.0 * mp3data.bitrate)
        self.num_samples = (total_seconds * in_samplerate).to_i
      end
    end
  end
end


