= Audio::MPEG

  http://seattlerb.rubyforge.org/

== DESCRIPTION.  LULZ

Hai! icanhasaudio? is an interface to lame for decoding ur MP3s.  I iz in ur
computer. Decodin ur mp3s.  Whatevs!  I also decodin ur OGGz!

== SYNOPSYS ROFLOL

  require 'icanhasaudio'

  reader = Audio::MPEG::Decoder.new
  File.open(ARGV[0], 'rb') { |input_lol|
    File.open(ARGV[1], 'wb') { |output_lol|
      reader.decode(input_lol, output_lol)
    }
  }

Or even smaller:

  reader = Audio::OGG::Decoder.new
  reader.decode(File.open(ARGV[0], 'rb'), File.open(ARGV[1], 'wb'))

== PROBLEMS

Currently only decodes MP3/OGG data.  Plus many other problems....  YMMV. LOL.
Not laugh plz!

== DEPENDENSEEZ

Make sure lame is installed on ur 'puter.  Also ogg and vorbisfile!

== CREDITZ

Thanx Ryan for mah name!  Also, most of this code was taken from the lame
front end.  So thank you to the lame team!  THX VORBIS!

== LICENSE

GPL.  See LICENSE.txt

KTHX BAI!
