module Audio
  module MPEG
    class Encoder
      WAV_ID_RIFF = 0x52494646
      WAV_ID_WAVE = 0x57415645 # "WAVE"
      WAV_ID_FMT  = 0x666d7420 # "fmt "
      WAV_ID_DATA = 0x64617461 # "data"

      IFF_ID_FORM = 0x464f524d # "FORM"
      IFF_ID_AIFF = 0x41494646 # "AIFF"
      IFF_ID_AIFC = 0x41494643 # "AIFC"
      IFF_ID_COMM = 0x434f4d4d # "COMM"
      IFF_ID_SSND = 0x53534e44 # "SSND"

      IFF_ID_NONE = 0x4e4f4e45 # "NONE" AIFF-C data format
      IFF_ID_2CBE = 0x74776f73 # "twos" AIFF-C data format
      IFF_ID_2CLE = 0x736f7774 # "sowt" AIFF-C data format


      VBR_OFF     = 0
      VBR_NORMAL  = 2
      VBR_FAST    = 4

      MODE_NAMES = [
        [ 'stereo', 'j-stereo', 'dual-ch', 'single-ch' ],
        [ 'stereo', 'force-ms', 'dual-ch', 'single-ch' ],
      ]

      attr_accessor :pcmbitwidth
      attr_accessor :logger
      def initialize
        @pcmbitwidth = 16
        @logger = nil
        self.vbr_quality = 2
        self.vbr_type    = VBR_NORMAL
      end

      def init
        init_params
      end

      def encode(infile, outfile)
        raise "Out file must be a FILE.  :-(" unless outfile.is_a?(File)

        parse_header(infile)

        init
        encode_io(infile) do |data|
          outfile.write(data)
        end
        flush_io do |data|
          outfile.write(data)
        end

        write_vbr_tags(outfile) if write_vbr_tag?
      end

      def encode_io(in_io)
        num_samples = 0xFFFFFFFF

        logger.debug(encoding_info) if logger

        num_samples_read = 0

        sw = case @pcmbitwidth
             when 8
               32 - 8
             when 16
               32 - 16
             when 24
               32 - 24
             end
        while !in_io.eof?
          tmp_num_samples = num_samples()
          samples_to_read = framesize()
          remaining = tmp_num_samples - [tmp_num_samples, num_samples_read].min
          if remaining < framesize && 0 != tmp_num_samples
            samples_to_read = remaining
          end

          read_size = num_channels * samples_to_read * (pcmbitwidth / 8)

          samples = in_io.read(read_size)
          samples_read = samples.length / num_channels

          buffers = [[], []]
          samples.unpack('v*').each_with_index do |b,i|
            (buffers[(i % 2)]) << (b << sw)
          end

          data = encode_buffer(buffers[0], buffers[1])

          yield data
        end
      end

      def flush_io
        data = flush()
        yield data
      end

      private
      def encoding_info
        sprintf("Encoding as %g kHz ", 1e-3 * out_samplerate) +
        sprintf("VBR(q=%i)", vbr_quality) +
        sprintf(" %s MPEG-%u%s Layer III (%s%gx) qval=%i\n",
               MODE_NAMES[force_ms][mpeg_mode],
               2 - mpeg_version,
               out_samplerate < 16000 ? ".5" : '',
               '',
               0.1 * (10.0 * compression_ratio + 0.5).to_i,
               mpeg_quality)
      end

      def parse_header(file)
        header = file.read(4).unpack('N').first
        info =  case header
                when WAV_ID_RIFF # Wave file
                  info = self.class.parse_wave_header(file)
                when IFF_ID_FORM # AIFF file
                  info = self.class.parse_aiff_header(file)
                else
                  $stderr.puts "Assuming RAW PCM"
                  file.rewind
                  nil
                end
        if info
          self.num_channels   = info[:num_channels]
          self.in_samplerate  = info[:in_samplerate]
          self.num_samples    = info[:num_samples]
          self.pcmbitwidth    = info[:bit_width]
        end
      end

      public
      class << self
        def parse_aiff_header(file)
          chunk_size  = file.read(4).unpack('N').first
          type_id     = file.read(4).unpack('N').first

          raise unless [IFF_ID_AIFF, IFF_ID_AIFC].any? { |x| type_id == x }

          sub_size = 0
          sample_type = nil
          sample_size = nil
          num_channels = nil
          block_size = nil
          sample_rate = nil
          is_aiff = false
          while chunk_size > 0
            type = file.read(4).unpack('N').first
            chunk_size -= 4

            case type
            when IFF_ID_COMM
              sub_size = file.read(4).unpack('N').first
              chunk_size -= 4

              num_channels = file.read(2).unpack('n').first
              sub_size -= 2
              num_sample_frames = file.read(4).unpack('N').first
              sub_size -= 4
              sample_size = file.read(2).unpack('n').first
              sub_size -= 2
              sample_rate = unpack_ieee(file.read(10))
              sub_size -= 10

              if type_id == IFF_ID_AIFC
                data_type = file.read(4).unpack('N').first
                sub_size -=4

                raise unless [  IFF_ID_2CLE,
                                IFF_ID_2CBE,
                                IFF_ID_NONE,
                ].any? { |x| data_type == x }

                #if sample_size == 16
                # swap bytes....
              end

              file.read(sub_size)
            when IFF_ID_SSND
              sub_size = file.read(4).unpack('N').first
              chunk_size -= sub_size

              block_offset = file.read(4).unpack('N').first
              sub_size -= 4
              block_size = file.read(4).unpack('N').first
              sub_size -= 4

              sample_type = IFF_ID_SSND
              file.read(block_offset)
              is_aiff = true
              break
            else
              sub_size = file.read(4).unpack('N').first
              chunk_size -= sub_size
              file.read(sub_size)
            end
          end

          # Validate the header
          if is_aiff
            raise "Sound data is not PCM" unless sample_type == IFF_ID_SSND
            raise "Sound data is not 16 bits" unless sample_size == 16
            unless num_channels == 1 || num_channels == 2
              raise "Sound data is not mono or stereo"
            end
            raise "Block size is not 0 bytes" unless block_size == 0
          end

          {
            :num_channels   => num_channels,
            :in_samplerate  => simple_rate.to_i,
            :num_samples    => num_sample_frames,
            :bit_width      => sample_size,
          }
        end

        def unpack_ieee(data)
          (expon, hi_mant, lo_mant) = data.unpack('nNN')
          expon -= 16383
          hi_mant * (2 ** (expon -= 31)) + lo_mant * (2 ** (expon -= 32))
        end

        def parse_wave_header(file)
          format_tag        = nil
          channels          = nil
          samples_per_sec   = nil
          avg_bytes_per_sec = nil
          block_align       = nil
          bits_per_sample   = nil
          is_wav            = false
          data_length       = 0

          file_length = file.read(4).unpack('N').first
          raise "Corrupt wave" if file.read(4).unpack('N').first != WAV_ID_WAVE
          20.times {
            type = file.read(4).unpack('N').first
            case type
            when WAV_ID_FMT
              sub_size = file.read(4).unpack('V').first
              raise "Corrupt wave" if sub_size < 16

              ( format_tag,
                channels,
                samples_per_sec,
                avg_bytes_per_sec,
                block_align, bits_per_sample) = *(file.read(16).unpack('vvVVvv'))
              sub_size -= 16

              file.read(sub_size) if sub_size > 0
            when WAV_ID_DATA
              sub_size = file.read(4).unpack('V').first
              data_length = sub_size
              is_wav = true
              break;
            else
              sub_size = file.read(4).unpack('V').first
              file.read(sub_size)
            end
          }
          raise "Unsupported format" unless format_tag == 1
          raise unless is_wav

          {
            :num_channels   => channels,
            :in_samplerate  => samples_per_sec,
            :bytes_in_seconds => data_length,
            :milliseconds   => (data_length/2)/(samples_per_sec/1000)/2,
            :num_samples    => data_length / (channels*((bits_per_sample+7)/8)),
            :bit_width      => bits_per_sample,
          }
        end
      end
    end
  end
end
