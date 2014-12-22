require 'hoe'

$LOAD_PATH.unshift File.join(File.dirname(__FILE__), "lib")

kind = Config::CONFIG["DLEXT"]

Hoe.spec 'icanhasaudio' do |p|
  p.rubyforge_name  = 'seattlerb'
  p.author          = 'Aaron Patterson'
  p.email           = 'aaronp@rubyforge.org'
  p.summary         = "icanhasaudio is a lame/vorbis wrapper for decoding ur mp3s and ur oggs."
  p.description     = p.paragraphs_of('README.txt', 3..6).join("\n\n")
  p.url             = p.paragraphs_of('README.txt', 1).first.strip
  p.spec_extras     = {
    :extensions => ['ext/icanhasaudio/extconf.rb']
  }
  p.clean_globs     = [
    "ext/icanhasaudio/Makefile",
    "ext/icanhasaudio/*.{o,so,bundle,log}"
  ]
end

Rake::Task[:test].prerequisites << :extension

desc "I can haz binary"
task :extension => ["ext/icanhasaudio/native.#{kind}"]

file "ext/icanhasaudio/Makefile" => "ext/icanhasaudio/extconf.rb" do
  Dir.chdir("ext/icanhasaudio") { ruby "extconf.rb" }
end

file "ext/icanhasaudio/native.#{kind}" => FileList["ext/icanhasaudio/Makefile", "ext/icanhasaudio/*.{c,h}"] do
  Dir.chdir("ext/icanhasaudio") { sh "make" }
end

file "ext/icanhasaudio/native.#{kind}" => FileList["ext/icanhasaudio/Makefile", "ext/icanhasaudio/*.{c,h}"] do
  Dir.chdir("ext/icanhasaudio") { sh "make install" }
end