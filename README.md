# SimpleStatistics

Simple Statistics aims to provide basic statistic functions for Ruby. To speed up, all of the functions here are written by C, and then wrapped using Ruby. Some of the functions are transferred from the GNU Scientific Library. This gem is still under active development. Therefore, please only use the functions shown in the Usage Part. If you have found any bugs or specifc requirements, please email to haipeng3@ualberta.ca
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
sstat_hist = SStat::Hist.new
bin = [2, 3, 5, 2, 1]
range = [0, 1, 2, 3, 4, 5]
hist_mean = sstat_hist.hist_mean(bin, range)
# => hist_mean.round(3) => 2.269
hist_median = sstat_hist.hist_median(bin, range)
hist_median.round(3) => 2.500
``````
#### Kaplan Meier estimator for survival prediction
sstat_surv = SStat::Surv.new
#event time for samples
time = [1, 2, 3, 4, 5, 6, 7]
# cesnroed information
# 1 -> censored. The event time is the patient's last visit time.
# 0 -> uncensored. The event time is the patient's actual dead time.
censored = [1, 1, 0, 0, 0, 1, 0]
sstat_surv.kaplan_meier(time, censored)
# output {"time" => [3.0, 4.0, 5.0, 7.0], "prob"=>[0.8, 0.6000000000000001, 0.4, 0.0]}

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## License

The gem is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).