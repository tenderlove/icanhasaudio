require 'md5'
require 'test/unit'
require 'icanhasaudio'
require 'tempfile'

%w(../lib ../ext).each do |path|
  $LOAD_PATH.unshift(File.expand_path(File.join(File.dirname(__FILE__), path)))
end

module ICANHASAUDIO
  class TestCase < Test::Unit::TestCase
    MP3_FILE = File.dirname(__FILE__) + "/assets/icha.mp3"
    undef :default_test
  end
end
