require "bundler/gem_tasks"
require 'rake/clean'

desc 'Build the sstat C extension'
task :build_ext do
  Dir.chdir("ext/") do
    #sh "swig -ruby -c++ sstat.i"
    sh "ruby extconf.rb --enable-static"
    sh "make"
  end
  cp "ext/sstat.so", "lib/"
end

task :install do
  Dir.chdir("ext/") do
  	Dir.chdir("gsl-2.1/") do
  		sh "./configure && make && sudo make install" # 
  	end
  end
end

desc 'sstat test'
task :test do
	sh "rspec spec"
end

CLEAN.include('ext/**/*{.o,.log,.so,.bundle}')
CLEAN.include('ext/**/Makefile')
CLOBBER.include('lib/*{.so,.bundle}')