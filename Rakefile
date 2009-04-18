require 'rake'
require 'rake/testtask'
require 'rake/rdoctask'
require 'rake/clean'
require 'fileutils'
include FileUtils

###
### cleanup tasks
###

CLEAN.include( 'ext/**/*.o', 'ext/**/*.so', 'test/**/*.z?' )
CLOBBER.include( 'pkg', 'doc/api', 'lib/dorothy/version.rb' )

###
### rdoc task
###

Rake::RDocTask.new do |rd|
  rd.main = "README"
  rd.rdoc_dir = "doc/api"
  rd.rdoc_files.include( "README", "COPYING", "lib/**/*.rb", 
                         "ext/**/*.h", "ext/**/*.c" )
end

###
### task to compile the extension
###

desc "compile the C extension part of the dorothy library"
task :compile do
  sh %{cd ext/dorothy/machine && ruby ./extconf.rb && make}
end

###
### the default task
###

task :default => [:clean, :compile, :compile_test_programs, :test]

###
### compile the test programs
###

desc "compile the test programs"
task :compile_test_programs do
  Dir['test/dorothy/programs/*.inf'].each do |f|
    sh %{cd test/dorothy/programs && inform #{File.basename( f )}}
  end

  Dir['test/dorothy/programs/v3/*.inf'].each do |f|
    sh %{cd test/dorothy/programs/v3 && inform -v3 #{File.basename( f )}}
  end
end

Rake::TestTask.new do |t|
  t.libs << "test" << "ext"
  t.test_files = FileList['test/**/*_test.rb']
end

# Try to load the version number -- it's okay if it's not available

begin
  require 'lib/dorothy/version.rb'
rescue Exception
  module Dorothy; end
end

desc "Appends the value in VERSION to lib/dorothy/version.rb"
task :version do
  v = ENV['VERSION']
  raise "provide a VERSION via the environment variable" unless v
  sh %{echo 'module Dorothy; VERSION = "#{v}"; end' >> lib/dorothy/version.rb}
end

begin
  require 'rubygems'
  require 'rake/gempackagetask'
rescue Exception
  nil
end

if defined?( Gem ) && Dorothy.const_defined?( 'VERSION' )
  task :gem => [:clean, :compile, :compile_test_programs, :test]

  PKG_FILES = FileList[
    'lib/**/*',
    'bin/dorothy',
    'test/**/*',
    'ext/**/*',
    'doc/**/*',
    'Rakefile',
    'README',
    'COPYING']

  spec = Gem::Specification.new do |s|
    s.name = 'dorothy'
    s.version = Dorothy::VERSION
    s.summary = 'A Z-Machine interpreter library.'
    s.description = 'A Z-Machine interpreter library.'
    s.homepage = 'http://ifrotz.org'
    s.rubyforge_project = 'silence stupid WARNINGS'
    s.has_rdoc = true
    s.files = PKG_FILES.to_a
    s.extensions = "ext/dorothy/machine/extconf.rb"
    s.executables = ['dorothy']
    s.require_paths << "ext"
    s.author = 'Eric K Idema'
    s.email = 'eki@vying.org'
  end

  package_task = Rake::GemPackageTask.new( spec ) do |pkg|
    pkg.need_tar_gz = true
    pkg.need_zip = true
  end
end

