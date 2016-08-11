# SimpleStatistics

Simple Statistics aims to provide basic statistic functions. To speed up the gem, all of the functions here are written by C. Some of the functions are transferred from the GNU Scientific Library. This gem is still under active development. Please only use the functions demonstrated in the Usage Part. All of the functions presented are passed the memory test of Valgrind. If you have found any bugs, please report to haipeng3@ualberta.ca

## Installation

Add this line to your application's Gemfile:

```ruby
gem 'sstat'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install 

## Usage

```ruby
require 'sstat'
``````
#### Basic Histogram Analysis
```ruby
#init simple statistics histogram
sstat_instance = SStat::Hist.new
bin = [2, 3, 5, 2, 1]
range = [0, 1, 2, 3, 4, 5]
hist_mean = sstat_instance.hist_mean(bin, range)
# => hist_mean.round(3) => 2.269
hist_median = sstat_instance.hist_median(bin, range)
hist_median.round(3) => 2.500
``````

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).