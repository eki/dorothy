require File.expand_path("../lib/dorothy/version", __FILE__)

Gem::Specification.new do |s|
  s.name = 'dorothy'
  s.version = Dorothy::VERSION
  s.summary = 'A Z-Machine interpreter library.'
  s.description = 'A Z-Machine interpreter library.'
  s.homepage = 'http://ifrotz.org'
  s.rubyforge_project = 'silence stupid WARNINGS'
  s.has_rdoc = true
  s.files = Dir[
    'lib/**/*',
    'bin/dorothy',
    'test/**/*',
    'ext/**/*',
    'doc/**/*',
    'Rakefile',
    'README',
    'COPYING']
  s.extensions = "ext/dorothy/machine/extconf.rb"
  s.executables = ['dorothy']
  s.require_paths << "ext"
  s.author = 'Eric K Idema'
  s.email = 'eki@vying.org'
end
