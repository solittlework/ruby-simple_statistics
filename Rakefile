require "bundler/gem_tasks"
require 'rake/clean'
require 'rake/extensiontask'

desc 'Build the sstat C extension'
task :build_ext do
    Dir.chdir("ext/sstat") do
    sh "ruby extconf.rb --enable-static"
    sh "make"
    end
end

task :install do
    Dir.chdir("ext/sstat") do
    Dir.chdir("gsl-2.1/") do
        sh "./configure && make && sudo make install" # 
    end
    end
end

# Define test task
desc 'sstat test'
task :test do
    sh "rspec spec"
end

# Define clean task
task :clean do
    rm "ext/sstat/sstat.so" if File.exists? "ext/sstat/sstat.so"
end

spec = Gem::Specification.new do |s|
    s.name = "sstat"
    s.version = '0.0.12'
    s.files = Dir['bin/**/*', 'ext/**/*', 'lib/**/*']
    s.platform = Gem::Platform::RUBY
    s.license = "MIT"
    s.extensions = FileList["ext/**/extconf.rb"]
    s.authors = ['Haipeng Li', 'Bret Hoehn', 'Russ Greiner']
    s.email = 'haipeng3@ualberta.ca'
    s.summary = 'This gem includes native ruby statistics functions. For speed purpose, functions here are all created from C.'
end

# Use the 'rake package' commend to package your gem
Gem::PackageTask.new(spec) do |pkg|
    cp "ext/sstat/sstat.so", "tmp/x86_64-linux/sstat/2.1.1"
end

Rake::ExtensionTask.new('sstat',spec)
CLEAN.include('ext/**/*{.o,.log,.so,.bundle}')
CLEAN.include('ext/**/Makefile')
CLOBBER.include('lib/*{.so,.bundle}')

task :run => [ :clean, :build_ext, :test ]