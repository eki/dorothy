require 'rake'
require 'rake/testtask'
require 'rake/rdoctask'
require 'rake/clean'
require 'fileutils'
include FileUtils

###
### cleanup tasks
###

CLEAN.include( 'ext/**/*.o', 'ext/**/*.so' )

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

task :default => [:clean, :compile]

