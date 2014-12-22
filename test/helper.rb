if RUBY_VERSION.to_f < 1.9
  require 'md5'
else
  require 'digest/md5'
end
require 'test/unit'
require 'icanhasaudio'
require 'tempfile'

%w(../lib ../ext).each do |path|
  $LOAD_PATH.unshift(File.expand_path(File.join(File.dirname(__FILE__), path)))
end

module ICANHASAUDIO
  class TestCase < Test::Unit::TestCase
    MP3_FILE = File.dirname(__FILE__) + "/assets/icha.mp3"
    undef :default_test if RUBY_VERSION.to_f < 1.9
  end
end
