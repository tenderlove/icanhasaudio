ENV["ARCHFLAGS"] = "-arch #{`uname -p` =~ /powerpc/ ? 'ppc' : 'i386'}"

require 'mkmf'

find_header 'lame/lame.h'

pkg_config 'ogg'
find_header 'ogg/ogg.h'
find_header 'vorbis/vorbisfile.h'

find_library 'mp3lame', 'lame_init'
find_library 'vorbis', 'ov_open_callbacks'

create_makefile('icanhasaudio')
