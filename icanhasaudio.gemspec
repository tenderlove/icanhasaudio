# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{icanhasaudio}
  s.version = "0.1.3"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = [%q{Aaron Patterson}]
  s.date = %q{2011-11-15}
  s.description = %q{Hai! icanhasaudio? is an interface to lame for decoding ur MP3s.  I iz in ur
computer. Decodin ur mp3s.  Whatevs!  I also decodin ur OGGz!  I kin also
encodin' ur WAV and AIFF to mp3z!

== SYNOPSYS ROFLOL

  require 'icanhasaudio'

  reader = Audio::MPEG::Decoder.new
  File.open(ARGV[0], 'rb') { |input_lol|
    File.open(ARGV[1], 'wb') { |output_lol|
      reader.decode(input_lol, output_lol)
    }
  }}
  s.email = %q{aaronp@rubyforge.org}
  s.extensions = [%q{ext/icanhasaudio/extconf.rb}]
  s.extra_rdoc_files = [%q{History.txt}, %q{LICENSE.txt}, %q{Manifest.txt}, %q{README.txt}]
  s.files = [%q{History.txt}, %q{LICENSE.txt}, %q{Manifest.txt}, %q{README.txt}, %q{Rakefile}, %q{examples/decoder.rb}, %q{examples/encoder.rb}, %q{examples/kexp.rb}, %q{ext/icanhasaudio/audio_mpeg_decoder.c}, %q{ext/icanhasaudio/audio_mpeg_decoder.h}, %q{ext/icanhasaudio/audio_mpeg_decoder_mp3data.c}, %q{ext/icanhasaudio/audio_mpeg_decoder_mp3data.h}, %q{ext/icanhasaudio/audio_mpeg_encoder.c}, %q{ext/icanhasaudio/audio_mpeg_encoder.h}, %q{ext/icanhasaudio/audio_ogg_decoder.c}, %q{ext/icanhasaudio/audio_ogg_decoder.h}, %q{ext/icanhasaudio/extconf.rb}, %q{ext/icanhasaudio/get_audio.c}, %q{ext/icanhasaudio/get_audio.h}, %q{ext/icanhasaudio/native.c}, %q{ext/icanhasaudio/native.h}, %q{ext/icanhasaudio/rb_wav.c}, %q{ext/icanhasaudio/rb_wav.h}, %q{lib/icanhasaudio.rb}, %q{lib/icanhasaudio/mpeg.rb}, %q{lib/icanhasaudio/mpeg/decoder.rb}, %q{lib/icanhasaudio/mpeg/encoder.rb}, %q{lib/icanhasaudio/ogg.rb}, %q{lib/icanhasaudio/ogg/decoder.rb}, %q{lib/icanhasaudio/version.rb}, %q{lib/icanhasaudio/wav.rb}, %q{lib/icanhasaudio/wav/file.rb}, %q{test/assets/icha.mp3}, %q{test/assets/testcase.wav}, %q{test/helper.rb}, %q{test/mpeg/test_decoder.rb}, %q{test/test_mpeg_encoder.rb}]
  s.homepage = %q{http://icanhasaudio.com/
  http://github.com/tenderlove/icanhasaudio/tree/master
  http://seattlerb.rubyforge.org/}
  s.rdoc_options = [%q{--main}, %q{README.txt}]
  s.require_paths = [%q{lib}]
  s.rubyforge_project = %q{seattlerb}
  s.rubygems_version = %q{1.8.6}
  s.summary = %q{icanhasaudio is a lame/vorbis wrapper for decoding ur mp3s and ur oggs.}
  s.test_files = [%q{test/mpeg/test_decoder.rb}, %q{test/test_mpeg_encoder.rb}]

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<hoe>, ["~> 2.12"])
    else
      s.add_dependency(%q<hoe>, ["~> 2.12"])
    end
  else
    s.add_dependency(%q<hoe>, ["~> 2.12"])
  end
end
