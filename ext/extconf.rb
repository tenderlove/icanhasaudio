ENV["ARCHFLAGS"] = "-arch #{`uname -p` =~ /powerpc/ ? 'ppc' : 'i386'}"

require 'mkmf'

def exit_failure(msg)
  Logging::message msg
  message msg + "\n"
  exit(1)
end

unless respond_to?(:find_header)
  def find_header(header, *paths)
    header = cpp_include(header)
    checking_for header do
      if try_cpp(header)
        true
      else
        found = false
        paths.each do |dir|
          opt = "-I#{dir}".quote
          if try_cpp(header, opt)
            $CPPFLAGS << " " << opt
            found = true
            break
          end
        end
        found
      end
    end
  end
end

['lame/lame.h', 'ogg/ogg.h', 'vorbis/vorbisfile.h'].each do |hf|
  unless find_header(hf,
    '/opt/local/include',
    '/usr/local/include',
    '/usr/include'
  )
    exit_failure "Can't find #{hf}"
  end
end

['mp3lame', 'vorbis', 'vorbisfile'].each do |library|
  unless find_library(library, nil,
    '/opt/local/lib',
    '/usr/local/lib',
    '/usr/lib'
  )
    exit_failure "Can't find #{library}"
  end
end

dir_config('icanhasaudio')
create_makefile('icanhasaudio')
