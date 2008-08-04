require 'hoe'

$LOAD_PATH.unshift File.join(File.dirname(__FILE__), "lib")

kind = Config::CONFIG["DLEXT"]

Hoe.new('icanhasaudio', '0.1.1') do |p|
  p.rubyforge_name  = 'seattlerb'
  p.author          = 'Aaron Patterson'
  p.email           = 'aaronp@rubyforge.org'
  p.summary         = "icanhasaudio is a lame/vorbis wrapper for decoding ur mp3s and ur oggs."
  p.description     = p.paragraphs_of('README.txt', 3..6).join("\n\n")
  p.url             = p.paragraphs_of('README.txt', 1).first.strip
  p.spec_extras     = { :extensions => ['ext/extconf.rb'] }
  p.clean_globs     = ["ext/Makefile", "ext/*.{o,so,bundle,log}"]
end

Rake::Task[:test].prerequisites << :extension

desc "I can haz binary"
task :extension => ["ext/icanhasaudio.#{kind}"]

file "ext/Makefile" => "ext/extconf.rb" do
  Dir.chdir("ext") { ruby "extconf.rb" }
end

file "ext/icanhasaudio.#{kind}" => FileList["ext/Makefile", "ext/*.{c,h}"] do
  Dir.chdir("ext") { sh "make" }
end
