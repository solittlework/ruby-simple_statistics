require "bundler/gem_tasks"
require 'rake/clean'
require 'rake/extensiontask'

desc 'Build the sstat C extension'
task :build_ext do
  Dir.chdir("ext/sstat") do
    #sh "swig -ruby -c++ sstat.i"
    sh "ruby extconf.rb --enable-static"
    sh "make"
  end
  cp "ext/sstat/sstat.so", "lib/"
end

task :install do
  Dir.chdir("ext/sstat") do
  	Dir.chdir("gsl-2.1/") do
  		sh "./configure && make && sudo make install" # 
  	end
  end
end

desc 'sstat test'
task :test do
	sh "rspec spec"
end

spec = Gem::Specification.new do |s|
  s.name = "sstat"
  s.version = '0.0.5'
  s.files = Dir['bin/**/*', 'ext/**/*', 'lib/**/*']
  s.platform = Gem::Platform::RUBY
  s.extensions = FileList["ext/**/extconf.rb"]
  s.authors = ['Haipeng Li', 'Bret Hoehn', 'Russ Greiner']
  s.email = 'haipeng3@ualberta.ca'
  s.summary = 'This gem includes native ruby statistics functions. For speed purpose, the functions here are build from C. Not all funcstions are home made. Some of the functions are from GSL (GNU Scentific Libaray). This gem is still not ready for general usage. So far, please do not download the gem for your project.'
end

# use the 'rake package' commend to package your gem
# add your default gem packing task
Gem::PackageTask.new(spec) do |pkg|
end
Rake::ExtensionTask.new('sstat',spec)
CLEAN.include('ext/**/*{.o,.log,.so,.bundle}')
CLEAN.include('ext/**/Makefile')
CLOBBER.include('lib/*{.so,.bundle}')

task :go => [ :clean, :build_ext, :test ]