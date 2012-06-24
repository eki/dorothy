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
  ruby = ($0 =~ /rake(.+)/) ? "ruby#{$1}" : "ruby"
  sh %{cd ext/dorothy/machine && #{ruby} ./extconf.rb && make}
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
