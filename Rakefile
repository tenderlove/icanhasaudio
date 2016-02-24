require 'hoe'
gem 'rake-compiler', '>= 0.4.1'
require "rake/extensiontask"

Hoe.plugin :doofus, :git, :gemspec

Hoe.spec 'icanhasaudio' do
  developer 'Aaron Patterson', 'tenderlove@ruby-lang.org'
  spec_extras     = {
    :extensions => ['ext/icanhasaudio/extconf.rb']
  }
  extra_dev_deps << ['rake-compiler', '>= 0.4.1']
  extra_dev_deps << ['minitest', '~> 5.0']

  Rake::ExtensionTask.new "icanhasaudio", spec do |ext|
    ext.lib_dir = File.join(*['lib', ENV['FAT_DIR']].compact)
  end
end

Rake::Task[:test].prerequisites << :compile
