require "bundler/gem_tasks"
require 'rake/clean'

desc 'Build the sstat C extension'
task :build_ext do
  Dir.chdir("ext/") do
    sh "swig -ruby -c++ sstat.i"
    ruby "extconf.rb"
    sh "make"
  end
  cp "ext/sstat.so", "lib/"
end

desc 'sstat test'
task :test do
	sh "rspec spec"
end

CLEAN.include('ext/**/*{.o,.log,.so,.bundle}')
CLEAN.include('ext/**/Makefile')
CLOBBER.include('lib/*{.so,.bundle}')