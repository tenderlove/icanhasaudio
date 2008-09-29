require File.expand_path(File.join(File.dirname(__FILE__), '..', 'helper'))

module Audio
  module MPEG
    class TestDecoder < ICANHASAUDIO::TestCase
      def setup
        @decoder = MPEG::Decoder.new
      end

      def test_decode
        out = "#{Dir::tmpdir}/out.wav"
        File.open(out, 'wb+') { |outfile|
          @decoder.decode(File.open(MP3_FILE, 'rb'), outfile)
        }
        digest = Digest::MD5.hexdigest(File.read(out))
        #assert_equal '9a55bcdda77ec7c20f32031632927403', digest
      end
    end
  end
end
